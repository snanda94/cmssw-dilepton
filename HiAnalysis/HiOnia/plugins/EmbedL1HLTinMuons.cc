#include "FWCore/Framework/interface/global/EDProducer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/ParameterSet/interface/ConfigurationDescriptions.h"
#include "FWCore/Common/interface/TriggerNames.h"
#include "DataFormats/PatCandidates/interface/Muon.h"
#include "Geometry/CommonTopologies/interface/GlobalTrackingGeometry.h"
#include "Geometry/Records/interface/GlobalTrackingGeometryRecord.h"
#include "DataFormats/MuonDetId/interface/RPCDetId.h"
#include "DataFormats/MuonDetId/interface/GEMDetId.h"
#include "DataFormats/MuonDetId/interface/ME0DetId.h"

namespace pat {

  class EmbedL1HLTinMuons : public edm::global::EDProducer<> {
  public:
    explicit EmbedL1HLTinMuons(const edm::ParameterSet& iConfig)
        : muonToken_(consumes<pat::MuonCollection>(iConfig.getParameter<edm::InputTag>("muons"))),
          triggerResultsToken_(consumes<edm::TriggerResults>(iConfig.getParameter<edm::InputTag>("triggerResults"))),
          triggerObjectsToken_(
              consumes<pat::TriggerObjectStandAloneCollection>(iConfig.getParameter<edm::InputTag>("triggerObjects"))),
          geometryToken_(esConsumes()) {
      produces<pat::MuonCollection>();
    }
    ~EmbedL1HLTinMuons() override{};

    void produce(edm::StreamID, edm::Event& iEvent, const edm::EventSetup& iSetup) const override;

    static void fillDescriptions(edm::ConfigurationDescriptions&);

  private:
    const edm::EDGetTokenT<pat::MuonCollection> muonToken_;
    const edm::EDGetTokenT<edm::TriggerResults> triggerResultsToken_;
    const edm::EDGetTokenT<pat::TriggerObjectStandAloneCollection> triggerObjectsToken_;
    const edm::ESGetToken<GlobalTrackingGeometry, GlobalTrackingGeometryRecord> geometryToken_;

    std::optional<GlobalPoint> getMuonDirection(const reco::MuonChamberMatch&,
                                                const DetId&,
                                                const GlobalTrackingGeometry&) const;

    void fillL1TriggerInfo(pat::Muon&,
                           const pat::TriggerObjectStandAloneCollection&,
                           const edm::TriggerNames&,
                           const GlobalTrackingGeometry&) const;

    void fillHLTriggerInfo(pat::Muon&, const pat::TriggerObjectStandAloneCollection&, const edm::TriggerNames&) const;
  };

}  // namespace pat

void pat::EmbedL1HLTinMuons::produce(edm::StreamID, edm::Event& iEvent, const edm::EventSetup& iSetup) const {
  // extract input information
  const auto& muons = iEvent.get(muonToken_);
  const auto& triggerObjects = iEvent.get(triggerObjectsToken_);
  const auto& triggerResults = iEvent.get(triggerResultsToken_);
  const auto& triggerNames = iEvent.triggerNames(triggerResults);
  const auto& geometry = iSetup.getHandle(geometryToken_);

  // initialize output muon collection
  auto output = std::make_unique<pat::MuonCollection>(muons);

  // add trigger information to muons
  for (auto& muon : *output) {
    const_cast<TriggerObjectStandAloneCollection&>(muon.triggerObjectMatches()).clear();
    fillL1TriggerInfo(muon, triggerObjects, triggerNames, *geometry);
    fillHLTriggerInfo(muon, triggerObjects, triggerNames);
  }

  iEvent.put(std::move(output));
}

std::optional<GlobalPoint> pat::EmbedL1HLTinMuons::getMuonDirection(const reco::MuonChamberMatch& chamberMatch,
                                                                    const DetId& chamberId,
                                                                    const GlobalTrackingGeometry& geometry) const {
  const auto& chamberGeometry = geometry.idToDet(chamberId);
  if (chamberGeometry) {
    LocalPoint localPosition(chamberMatch.x, chamberMatch.y, 0);
    return std::optional<GlobalPoint>(std::in_place, chamberGeometry->toGlobal(localPosition));
  }
  return std::optional<GlobalPoint>();
}

void pat::EmbedL1HLTinMuons::fillL1TriggerInfo(pat::Muon& muon,
                                               const pat::TriggerObjectStandAloneCollection& triggerObjects,
                                               const edm::TriggerNames& triggerNames,
                                               const GlobalTrackingGeometry& geometry) const {
  // L1 trigger object parameters are defined at MB2/ME2. Use the muon
  // chamber matching information to get the local direction of the
  // muon trajectory to match the trigger objects
  const auto absEta = std::abs(muon.eta());

  std::optional<GlobalPoint> muonPosition;
  std::tuple<short, short, float> key({0, 0, 0});
  for (const auto& match : muon.matches()) {
    const auto detector = match.detector();
    if (detector > ((absEta > 0.8 && absEta < 1.4) ? MuonSubdetId::RPC : MuonSubdetId::CSC))
      continue;
    const auto station = std::abs(match.station());
    if (station > 3)
      continue;
    const auto stIdx = station == 2 ? 10 : station;
    if ((stIdx <  std::get<0>(key)) ||
        (stIdx == std::get<0>(key) && detector < std::get<1>(key)))
      continue;
    const auto xSig = (match.x-muon.vertex().x())/(match.xErr!=0 ? match.xErr : 1E-6);
    const auto ySig = (match.y-muon.vertex().y())/(match.yErr!=0 ? match.yErr : 1E-6);
    const auto dSig = std::sqrt(xSig*xSig + ySig*ySig);
    if (stIdx == std::get<0>(key) && detector == std::get<1>(key) && dSig <= std::get<2>(key))
      continue;
    const auto pos = getMuonDirection(match, match.id, geometry);
    if (!pos)
      continue;
    muonPosition = pos;
    key = std::make_tuple(stIdx, detector, dSig);
  }
    
  /*
  float maxSig(0);
  std::optional<GlobalPoint> muonPosition;
  const auto absEta = std::abs(muon.eta());
  for (const auto& match : muon.matches()) {
    const auto station = std::abs(match.station());
    if (station > 2)
      continue;
    if (match.id.subdetId()==MuonSubdetId::RPC && (absEta<0.8 || absEta>1.4))
      continue;
    const auto pos = getMuonDirection(match, match.id, geometry);
    if (!pos)
      continue;
    if (false && station < 2) {
      const auto xSig = (match.x-muon.vertex().x())/(match.xErr!=0 ? match.xErr : 1E-6);
      const auto ySig = (match.y-muon.vertex().y())/(match.yErr!=0 ? match.yErr : 1E-6);
      const auto dSig = std::sqrt(xSig*xSig + ySig*ySig);
      if (dSig <= maxSig)
        continue;
      maxSig = dSig;
    }
    muonPosition = pos;
    if (station == 2)
      break;
  }
  */
  if (!muonPosition)
    return;

  bool isGoodMuon = muon.innerTrack().isNonnull() && muon.innerTrack()->hitPattern().trackerLayersWithMeasurement() > 5 && (
                    (muon.isTrackerMuon() && muon.innerTrack()->quality(reco::Track::highPurity) && muon.isGood("TMOneStationTight") && muon.innerTrack()->hitPattern().pixelLayersWithMeasurement() > 0) ||
                    (muon.isGlobalMuon() && muon.isPFMuon() && muon.numberOfMatchedStations() > 1 && muon.globalTrack()->hitPattern().numberOfValidMuonHits() > 0 && muon.globalTrack()->normalizedChi2() < 10. && muon.innerTrack()->hitPattern().numberOfValidPixelHits() > 0) ||
                    (muon.isGlobalMuon() && muon.isTrackerMuon() && muon.innerTrack()->hitPattern().pixelLayersWithMeasurement() > 0));

  if (muon.hasUserFloat("l1Eta") && isGoodMuon) {
    if (abs(muon.userFloat("l1Eta")-muonPosition->eta()) > 0.2 || abs(reco::deltaPhi(muon.userFloat("l1Phi"), muonPosition->phi())) > 0.2) {
      std::cout << "EXTRA : " << muon.userFloat("l1Eta") << " , " << muonPosition->eta() << " << >> " << muon.userFloat("l1Phi") << " , " << muonPosition->phi() << " >> " << muon.pt() << " , " << muon.eta() << " , "  << muon.phi() << " >> " << muon.isTrackerMuon() << " , " << muon.isGlobalMuon() << " , " << muon.isStandAloneMuon() << " , " << muon.isPFMuon() << " , " << muon.isGood("TMOneStationTight") << " <> " << muon.innerTrack()->hitPattern().trackerLayersWithMeasurement() << " , " << muon.innerTrack()->hitPattern().pixelLayersWithMeasurement() << " , " << muon.innerTrack()->quality(reco::Track::highPurity) << std::endl;
      for (const auto& chamberMatch : muon.matches()) {
        const auto nMatches = chamberMatch.segmentMatches.size() + chamberMatch.gemMatches.size() + chamberMatch.rpcMatches.size() + chamberMatch.me0Matches.size();
        if (chamberMatch.id.subdetId() == MuonSubdetId::DT) {
          DTChamberId detId(chamberMatch.id.rawId());
          const auto muonPosition = getMuonDirection(chamberMatch, detId, geometry);
          std::cout << "DT: " << detId.station() << " >> " << (muonPosition ? muonPosition->eta() : -99) << " , " << (muonPosition ? float(muonPosition->phi()) : -99.) << " >> " << chamberMatch.dist() << " , " << chamberMatch.distErr() << " , " << chamberMatch.segmentMatches.size() << " <> " << nMatches << " >> " << chamberMatch.x << " , " << chamberMatch.y << " , " << chamberMatch.xErr << " , " << chamberMatch.yErr << " <> " << sqrt(pow(chamberMatch.x-muon.vertex().x(), 2) + pow(chamberMatch.y-muon.vertex().y(), 2)) << " >> " << sqrt(pow((chamberMatch.x-muon.vertex(). x())/chamberMatch.xErr, 2) + pow((chamberMatch.y-muon.vertex().y())/chamberMatch.yErr, 2)) << std::endl;
        }
        else if (chamberMatch.id.subdetId() == MuonSubdetId::CSC) {
          CSCDetId detId(chamberMatch.id.rawId());
          const auto muonPosition = getMuonDirection(chamberMatch, detId, geometry);
          std::cout << "CSC: " << detId.station() << " >> " << (muonPosition ? muonPosition->eta() : -99) << " , " << (muonPosition ? float(muonPosition->phi()) : -99.) << " >> " << chamberMatch.dist() << " , " << chamberMatch.distErr() << " , " << chamberMatch.segmentMatches.size() << " <> " << nMatches << " >> " << chamberMatch.x << " , " << chamberMatch.y << " , " << chamberMatch.xErr << " , " << chamberMatch.yErr << " <> " << sqrt(pow(chamberMatch.x-muon.vertex(). x(), 2) + pow(chamberMatch.y-muon.vertex().y(), 2)) << " >> " << sqrt(pow((chamberMatch.x-muon.vertex(). x())/chamberMatch.xErr, 2) + pow((chamberMatch.y-muon.vertex().y())/chamberMatch.yErr, 2)) << std::endl;
        }
        else if (chamberMatch.id.subdetId() == MuonSubdetId::RPC) {
          RPCDetId detId(chamberMatch.id.rawId());
          const auto muonPosition = getMuonDirection(chamberMatch, detId, geometry);
          std::cout << "RPC: " << detId.station() << " >> " << (muonPosition ? muonPosition->eta() : -99) << " , " << (muonPosition ? float(muonPosition->phi()) : -99.) << " >> " << chamberMatch.dist() << " , " << chamberMatch.distErr() << " , " << chamberMatch.segmentMatches.size() << " <> " << nMatches << " >> " << chamberMatch.x << " , " << chamberMatch.y << " , " << chamberMatch.xErr << " , " << chamberMatch.yErr << " <> " << sqrt(pow(chamberMatch.x-muon.vertex(). x(), 2) + pow(chamberMatch.y-muon.vertex().y(), 2)) << " >> " << sqrt(pow((chamberMatch.x-muon.vertex(). x())/chamberMatch.xErr, 2) + pow((chamberMatch.y-muon.vertex().y())/chamberMatch.yErr, 2)) << std::endl;
        }
        else if (chamberMatch.id.subdetId() == MuonSubdetId::GEM) {
          GEMDetId detId(chamberMatch.id.rawId());
          const auto muonPosition = getMuonDirection(chamberMatch, detId, geometry);
          std::cout << "GEM: " << detId.station() << " >> " << (muonPosition ? muonPosition->eta() : -99) << " , " << (muonPosition ? float(muonPosition->phi()) : -99.) << " >> " << chamberMatch.dist() << " , " << chamberMatch.distErr() << std::endl;
        }
        else if (chamberMatch.id.subdetId() == MuonSubdetId::ME0) {
          ME0DetId detId(chamberMatch.id.rawId());
          const auto muonPosition = getMuonDirection(chamberMatch, detId, geometry);
          std::cout << "ME0: " << detId.station() << " >> " << (muonPosition ? muonPosition->eta() : -99) << " , " << (muonPosition ? float(muonPosition->phi()) : -99.) << " >> " << chamberMatch.dist() << " , " << chamberMatch.distErr() << std::endl;
        }
        else {
          std::cout << "WTF: " << chamberMatch.id.subdetId() << std::endl;
        }
      }
      //if (abs(muon.userFloat("l1Eta")-muonPosition->eta()) > 0.15 || abs(muon.userFloat("l1Phi")-muonPosition->phi()) > 0.15)
      //  throw std::logic_error("STOP");
    }
  }

  // add L1 trigger object to muon
  for (const auto& triggerObject : triggerObjects) {
    if (!triggerObject.hasTriggerObjectType(trigger::TriggerL1Mu))
      continue;
    if (std::abs(triggerObject.eta()) < 0.001) {
      if (std::abs(deltaPhi(triggerObject.phi(), muonPosition->phi())) > 0.1)
        continue;
    } else if (deltaR(triggerObject.p4(), *muonPosition) > 0.15)
      continue;
    muon.addTriggerObjectMatch(triggerObject);
  }
}

void pat::EmbedL1HLTinMuons::fillHLTriggerInfo(pat::Muon& muon,
                                               const pat::TriggerObjectStandAloneCollection& triggerObjects,
                                               const edm::TriggerNames& triggerNames) const {
  // add HLT object to muon
  for (const auto& triggerObject : triggerObjects) {
    if (!triggerObject.hasTriggerObjectType(trigger::TriggerMuon))
      continue;
    if (deltaR(triggerObject.p4(), muon) > 0.1)
      continue;
    muon.addTriggerObjectMatch(triggerObject);
  }
}

// ------------ method fills 'descriptions' with the allowed parameters for the module  ------------
void pat::EmbedL1HLTinMuons::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  edm::ParameterSetDescription desc;
  desc.add<edm::InputTag>("muons", edm::InputTag("unpackedMuons"))->setComment("muon input collection");
  desc.add<edm::InputTag>("triggerResults", edm::InputTag("TriggerResults::HLT"))
      ->setComment("trigger results collection");
  desc.add<edm::InputTag>("triggerObjects", edm::InputTag("slimmedPatTrigger::PAT"))
      ->setComment("trigger objects collection");
  descriptions.add("unpackedMuonsWithTrigger", desc);
}

#include "FWCore/Framework/interface/MakerMacros.h"
using namespace pat;
DEFINE_FWK_MODULE(EmbedL1HLTinMuons);
