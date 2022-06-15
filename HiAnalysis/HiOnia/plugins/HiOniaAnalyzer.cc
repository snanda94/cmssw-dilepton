// -*- C++ -*-
//
// Package:    HiOniaAnalyzer
// Class:      HiOniaAnalyzer
//
/**\class HiOniaAnalyzer HiOniaAnalyzer.cc UserCode/tdahms/HiAnalysis/HiOnia/plugins/HiOniaAnalyzer.cc
 Description: [one line class summary]
 Implementation:
     [Notes on implementation]
*/

// system include files
#include <memory>
#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <utility>

#include <TTree.h>
#include <TLorentzVector.h>
#include <TVector2.h>
#include <TClonesArray.h>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/one/EDAnalyzer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "DataFormats/PatCandidates/interface/Muon.h"
#include "DataFormats/PatCandidates/interface/CompositeCandidate.h"
#include "DataFormats/Candidate/interface/CompositeCandidate.h"
#include <DataFormats/RecoCandidate/interface/RecoChargedCandidate.h>
#include "DataFormats/Common/interface/TriggerResults.h"
#include "DataFormats/VertexReco/interface/Vertex.h"
#include "DataFormats/VertexReco/interface/VertexFwd.h"
#include "DataFormats/BeamSpot/interface/BeamSpot.h"
#include "DataFormats/TrackReco/interface/Track.h"
#include "DataFormats/TrackReco/interface/TrackFwd.h"

#include "HLTrigger/HLTcore/interface/HLTPrescaleProvider.h"
#include "FWCore/Framework/interface/ConsumesCollector.h"
#include "HLTrigger/HLTcore/interface/HLTConfigProvider.h"

#include "DataFormats/HeavyIonEvent/interface/Centrality.h"
#include "DataFormats/HeavyIonEvent/interface/EvtPlane.h"
#include "SimDataFormats/GeneratorProducts/interface/GenEventInfoProduct.h"

#include "HiAnalysis/HiOnia/interface/MyCommonHistoManager.h"
#include "DataFormats/Math/interface/deltaR.h"

#include "FWCore/ServiceRegistry/interface/Service.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"

//
// class declaration
//

class HiOniaAnalyzer : public edm::one::EDAnalyzer<edm::one::SharedResources, edm::one::WatchRuns> {
public:
  explicit HiOniaAnalyzer(const edm::ParameterSet&);
  ~HiOniaAnalyzer();

private:
  virtual void beginJob();
  virtual void analyze(const edm::Event&, const edm::EventSetup&);
  virtual void endJob();

  void InitEvent();
  void InitTree();

  void makeCuts(bool keepSameSign);
  void makeBcCuts(bool keepWrongSign);
  void makeDimutrkCuts(bool keepWrongSign);
  bool checkCuts(const pat::CompositeCandidate* cand,
                 const pat::Muon* muon1,
                 const pat::Muon* muon2,
                 bool (HiOniaAnalyzer::*callFunc1)(const pat::Muon*),
                 bool (HiOniaAnalyzer::*callFunc2)(const pat::Muon*));
  bool checkBcCuts(const pat::CompositeCandidate* cand,
                   const pat::Muon* muon1,
                   const pat::Muon* muon2,
                   const pat::Muon* muon3,
                   bool (HiOniaAnalyzer::*callFunc1)(const pat::Muon*),
                   bool (HiOniaAnalyzer::*callFunc2)(const pat::Muon*),
                   bool (HiOniaAnalyzer::*callFunc3)(const pat::Muon*));
  bool checkDimuTrkCuts(const pat::CompositeCandidate* cand,
                        const pat::Muon* muon1,
                        const pat::Muon* muon2,
                        const reco::RecoChargedCandidate* trk,
                        bool (HiOniaAnalyzer::*callFunc1)(const pat::Muon*),
                        bool (HiOniaAnalyzer::*callFunc2)(const pat::Muon*),
                        bool (HiOniaAnalyzer::*callFunc3)(const reco::TrackRef));

  reco::GenParticleRef findDaughterRef(reco::GenParticleRef GenParticleDaughter, int GenParticlePDG);
  bool isSameLorentzV(TLorentzVector* v1, TLorentzVector* v2);
  int IndexOfThisMuon(TLorentzVector* v1, bool isGen = false);
  int IndexOfThisTrack(TLorentzVector* v1, bool isGen = false);
  int IndexOfThisJpsi(int mu1_idx, int mu2_idx, int flipJpsi = 0);
  void fillGenInfo();
  void fillMuMatchingInfo();
  void fillQQMatchingInfo();
  void fillBcMatchingInfo();
  bool isAbHadron(int pdgID);
  bool isNeutrino(int pdgID);
  bool isAMixedbHadron(int pdgID, int momPdgID);
  std::pair<bool, reco::GenParticleRef> findBcMotherRef(reco::GenParticleRef GenParticleMother, int GenParticlePDG);
  bool isChargedTrack(int pdgId);
  std::vector<reco::GenParticleRef> GenBrothers(reco::GenParticleRef GenParticleMother, int GenJpsiPDG);
  reco::GenParticleRef findMotherRef(reco::GenParticleRef GenParticleMother, int GenParticlePDG);
  std::pair<std::vector<reco::GenParticleRef>, std::pair<float, float> > findGenMCInfo(const reco::GenParticle* genJpsi);
  std::pair<int, std::pair<float, float> > findGenBcInfo(reco::GenParticleRef genBc, const reco::GenParticle* genJpsi);

  void fillRecoMuons(int theCentralityBin);
  bool isMuonInAccept(const pat::Muon* aMuon, std::string muonType);
  bool isTrkInMuonAccept(TLorentzVector trk4mom, std::string muonType);

  bool isSoftMuonBase(const pat::Muon* aMuon);
  bool isSoftMuon(const pat::Muon* aMuon);
  bool isHybridSoftMuon(const pat::Muon* aMuon);
  bool isTightMuon(const pat::Muon* aMuon);
  Short_t MuInSV(TLorentzVector v1, TLorentzVector v2, TLorentzVector v3);

  void fillRecoTracks();

  pair<unsigned int, const pat::CompositeCandidate*> theBestQQ();
  double CorrectMass(const reco::Muon& mu1, const reco::Muon& mu2, int mode);

  bool selGlobalMuon(const pat::Muon* aMuon);
  bool selTrackerMuon(const pat::Muon* aMuon);
  bool selGlobalOrTrackerMuon(const pat::Muon* aMuon);
  bool selAllMuon(const pat::Muon* aMuon);
  bool selTrk(const reco::TrackRef aTrk);
  bool PassMiniAODcut(const pat::Muon* aMuon);

  void fillRecoHistos();
  void fillRecoJpsi(int count, std::string trigName, std::string centName);
  void fillHistosAndDS(unsigned int theCat, const pat::CompositeCandidate* aJpsiCand);

  void fillTreeMuon(const pat::Muon* muon, int iType, ULong64_t trigBits);
  void fillTreeJpsi(int count);
  void fillTreeBc(int count);
  void fillTreeDimuTrk(int count);

  void checkTriggers(const pat::CompositeCandidate* aJpsiCand);
  void hltReport(const edm::Event& iEvent, const edm::EventSetup& iSetup);

  long int FloatToIntkey(float v);
  void beginRun(const edm::Run&, const edm::EventSetup&);
  void endRun(const edm::Run&, const edm::EventSetup&){};

  TLorentzVector lorentzMomentum(const reco::Candidate::LorentzVector& p);
  int muonIDmask(const pat::Muon* muon);

  // ----------member data ---------------------------
  enum StatBins { BIN_nEvents = 0 };

  enum dimuonCategories {
    GlbTrk_GlbTrk = 0,
    Glb_Glb = 1,
    Trk_Trk = 2,
    GlbOrTrk_GlbOrTrk = 3,
    TwoGlbAmongThree = 4,
    All_All = 5
  };

  enum muonCategories { GlbTrk = 0, Trk = 1, Glb = 2, GlbOrTrk = 3, All = 4 };

  std::vector<std::string> theRegions;
  std::vector<std::string> theCentralities;
  std::vector<std::string> theTriggerNames;
  std::vector<std::string> theSign;

  HLTConfigProvider hltConfig;
  bool hltConfigInit;

  float etaMin;
  float etaMax;

  // TFileService
  edm::Service<TFileService> fs;

  // // TFile
  // TFile* fOut;

  // TTree
  TTree* myTree;

  TClonesArray* Reco_mu_4mom;
  TClonesArray* Reco_mu_L1_4mom;
  TClonesArray* Reco_QQ_4mom;
  TClonesArray* Reco_QQ_mumi_4mom;
  TClonesArray* Reco_QQ_mupl_4mom;
  TClonesArray* Reco_3mu_4mom;
  TClonesArray* Reco_QQ_vtx;
  TClonesArray* Reco_3mu_vtx;
  TClonesArray* Reco_trk_4mom;
  TClonesArray* Reco_trk_vtx;
  TClonesArray* Gen_Bc_4mom;
  TClonesArray* Gen_Bc_nuW_4mom;
  TClonesArray* Gen_3mu_4mom;

  TClonesArray* Gen_mu_4mom;
  TClonesArray* Gen_QQ_4mom;

  static const int Max_QQ_size = 10000;
  static const int Max_Bc_size = 10000;
  static const int Max_mu_size = 1000;
  static const int Max_trk_size = 10000;

  float Gen_weight;  // generator weight
  float Gen_pthat;   // ptHat scale of generated hard scattering

  Short_t Gen_QQ_size;               // number of generated Onia
  Short_t Gen_QQ_type[Max_QQ_size];  // Onia type: prompt, non-prompt, unmatched
  float Gen_QQ_ctau[Max_QQ_size];    // ctau: flight time
  float Gen_QQ_ctau3D[Max_QQ_size];  // ctau3D: 3D flight time
  int Gen_QQ_momId
      [Max_QQ_size];  // PDG ID of the generated mother of the Gen QQ, going back far enough in the geneaology to find a potential B mother
  float Gen_QQ_momPt[Max_QQ_size];       // Pt of mother particle of 2 muons
  Short_t Gen_QQ_mupl_idx[Max_QQ_size];  // index of the muon plus from Jpsi, in the full list of muons
  Short_t Gen_QQ_mumi_idx[Max_QQ_size];  // index of the muon minus from Jpsi, in the full list of muons
  Short_t Gen_QQ_whichRec
      [Max_QQ_size];  // index of the reconstructed Jpsi that was matched with this gen Jpsi. Is -1 if one of the 2 muons from Jpsi was not reconstructed. Is -2 if the two muons were reconstructed, but the dimuon was not selected
  Short_t Gen_QQ_Bc_idx[Max_QQ_size];  //Index of the Bc gen mother. -1 if there is no Bc mother

  Short_t Gen_Bc_size;                  // number of generated Bc
  float Gen_Bc_ctau[Max_Bc_size];       // ctau: flight time
  int Gen_Bc_pdgId[Max_Bc_size];        // pdgId of Bc
  Short_t Gen_Bc_QQ_idx[Max_Bc_size];   //Points to the number of the associated Jpsi daughter
  Short_t Gen_Bc_muW_idx[Max_Bc_size];  // index of the muon from W from Bc, in the full list of muons
  Short_t Gen_3mu_whichRec
      [Max_Bc_size];  // index of the reconstructed trimuon that was matched with this gen visible Bc. Is -1 if one muon of the Bc was not reconstructed

  Short_t Gen_mu_size;                 // number of generated muons
  Short_t Gen_mu_charge[Max_mu_size];  // muon charge
  Short_t Gen_mu_type[Max_mu_size];    // muon type: prompt, non-prompt, unmatched
  Short_t Gen_mu_whichRec
      [Max_mu_size];  // index of the reconstructed muon that was matched with this gen muon. Is -1 if the muon was not reconstructed
  float Gen_mu_MatchDeltaR[Max_mu_size];  // deltaR between reco and gen matched muons

  Short_t Reco_3mu_size;  // Number of reconstructed trimuons
  Short_t Reco_3mu_charge[Max_Bc_size];
  Short_t Reco_3mu_mupl_idx[Max_Bc_size];  // index of the muon plus from Jpsi, in the full list of muons
  Short_t Reco_3mu_mumi_idx[Max_Bc_size];  // index of the muon minus from Jpsi, in the full list of muons
  Short_t Reco_3mu_muW_idx[Max_Bc_size];   // index of the muon from W, in the full list of muons
  Short_t Reco_3mu_muW2_idx[Max_Bc_size];  // index of the muon from W, for the second possible OS dimuon
  //  Short_t Reco_3mu_muW_trkIdx[Max_Bc_size];    // index of the generated track closest to the reco muon from W, when the Jpsi is true and the muon is fake
  Short_t Reco_3mu_QQ1_idx[Max_Bc_size];   // index of a possible Jpsi from Bc, 1st of the two OS muon pairs
  Short_t Reco_3mu_QQ2_idx[Max_Bc_size];   // index of a possible Jpsi from Bc, 2nd of the two OS muon pairs
  Short_t Reco_3mu_QQss_idx[Max_Bc_size];  // index of a the same-sign muon pair from Bc
  Short_t Reco_3mu_whichGen
      [Max_Bc_size];  // index of the generated Bc that was matched with this rec Bc. Is -1 if one or more of the 3 muons from Bc was not reconstructed
  bool Reco_3mu_muW_isGenJpsiBro
      [Max_Bc_size];  // is this true or fake muon matched to a generated particle that is brother/nephew to the gen Jpsi (daughter of the gen B)
  int Reco_3mu_muW_trueId[Max_Bc_size];   //pdgId of the generated particle that the reco_muW is matched to
  float Reco_3mu_VtxProb[Max_Bc_size];    // chi2 probability of vertex fitting
  float Reco_3mu_KCVtxProb[Max_Bc_size];  // chi2 probability of kinematic constrained vertex fitting
  float Reco_3mu_ctau[Max_Bc_size];       // ctau: flight time
  float Reco_3mu_ctauErr[Max_Bc_size];    // error on ctau
  float Reco_3mu_cosAlpha
      [Max_QQ_size];  // cosine of angle between momentum of Bc and direction of PV--displaced vertex segment (in XY plane)
  float Reco_3mu_ctau3D[Max_Bc_size];     // ctau: flight time in 3D
  float Reco_3mu_ctauErr3D[Max_Bc_size];  // error on ctau in 3D
  float Reco_3mu_cosAlpha3D
      [Max_QQ_size];  // cosine of angle between momentum of Bc and direction of PV--displaced vertex segment (3D)
  float Reco_3mu_KCctau[Max_Bc_size];     // ctau: flight time
  float Reco_3mu_KCctauErr[Max_Bc_size];  // error on ctau
  float Reco_3mu_KCcosAlpha
      [Max_QQ_size];  // cosine of angle between momentum of Bc and direction of PV--displaced vertex segment (in XY plane)
  float Reco_3mu_KCctau3D[Max_Bc_size];     // ctau: flight time in 3D
  float Reco_3mu_KCctauErr3D[Max_Bc_size];  // error on ctau in 3D
  float Reco_3mu_KCcosAlpha3D
      [Max_QQ_size];  // cosine of angle between momentum of Bc and direction of PV--displaced vertex segment (3D)
  float Reco_3mu_MassErr[Max_Bc_size];
  float Reco_3mu_CorrM[Max_Bc_size];
  Short_t Reco_3mu_NbMuInSameSV[Max_Bc_size];
  float Reco_3mu_muW_dxy[Max_Bc_size];
  float Reco_3mu_muW_dz[Max_Bc_size];
  float Reco_3mu_mumi_dxy[Max_Bc_size];
  float Reco_3mu_mumi_dz[Max_Bc_size];
  float Reco_3mu_mupl_dxy[Max_Bc_size];
  float Reco_3mu_mupl_dz[Max_Bc_size];

  Short_t Reco_QQ_size;                   // Number of reconstructed Onia
  Short_t Reco_QQ_type[Max_QQ_size];      // Onia category: GG, GT, TT
  Short_t Reco_QQ_sign[Max_QQ_size];      /* Mu Mu combinations sign:
                             0 = +/- (signal)
                             1 = +/+
                             2 = -/-
                          */
  Short_t Reco_QQ_mupl_idx[Max_QQ_size];  // index of the muon plus from Jpsi, in the full list of muons
  Short_t Reco_QQ_mumi_idx[Max_QQ_size];  // index of the muon minus from Jpsi, in the full list of muons
  Short_t Reco_QQ_whichGen
      [Max_QQ_size];  // index of the generated Jpsi that was matched with this rec Jpsi. Is -1 if one of the 2 muons from Jpsi was not reconstructed
  ULong64_t Reco_QQ_trig[Max_QQ_size];  // Vector of trigger bits matched to the Onia
  bool Reco_QQ_isCowboy[Max_mu_size];   // Cowboy/Sailor Flag
  float Reco_QQ_VtxProb[Max_QQ_size];   // chi2 probability of vertex fitting
  float Reco_QQ_ctau[Max_QQ_size];      // ctau: flight time
  float Reco_QQ_ctauErr[Max_QQ_size];   // error on ctau
  float Reco_QQ_cosAlpha
      [Max_QQ_size];  // cosine of angle between momentum of Jpsi and direction of PV--displaced vertex segment (in XY plane)
  float Reco_QQ_ctau3D[Max_QQ_size];     // ctau: flight time in 3D
  float Reco_QQ_ctauErr3D[Max_QQ_size];  // error on ctau in 3D
  float Reco_QQ_cosAlpha3D
      [Max_QQ_size];  // cosine of angle between momentum of Jpsi and direction of PV--displaced vertex segment (3D)
  float Reco_QQ_dca[Max_QQ_size];
  float Reco_QQ_MassErr[Max_QQ_size];

  int Reco_QQ_NtrkPt02[Max_QQ_size];
  int Reco_QQ_NtrkPt03[Max_QQ_size];
  int Reco_QQ_NtrkPt04[Max_QQ_size];

  int Reco_QQ_NtrkDeltaR03[Max_QQ_size];
  int Reco_QQ_NtrkDeltaR04[Max_QQ_size];
  int Reco_QQ_NtrkDeltaR05[Max_QQ_size];

  float Reco_QQ_mupl_dxy[Max_QQ_size];  // dxy for plus inner track muons
  float Reco_QQ_mumi_dxy[Max_QQ_size];  // dxy for minus inner track muons
  float Reco_QQ_mupl_dz[Max_QQ_size];   // dz for plus inner track muons
  float Reco_QQ_mumi_dz[Max_QQ_size];   // dz for minus inner track muons
  Short_t Reco_QQ_flipJpsi[Max_QQ_size];

  Short_t Reco_mu_size;  // Number of reconstructed muons
  int Reco_mu_SelectionType[Max_mu_size];
  ULong64_t Reco_mu_trig[Max_mu_size];  // Vector of trigger bits matched to the muon
  Short_t Reco_mu_charge[Max_mu_size];  // Vector of charge of muons
  Short_t Reco_mu_type[Max_mu_size];    // Vector of type of muon (global=0, tracker=1, calo=2)
  Short_t Reco_mu_whichGen
      [Max_mu_size];  // index of the generated muon that was matched with this reco muon. Is -1 if the muon is not associated with a generated muon (fake, or very bad resolution)

  bool Reco_mu_highPurity[Max_mu_size];     // Vector of high purity flag
  bool Reco_mu_TrkMuArb[Max_mu_size];       // Vector of TrackerMuonArbitrated
  bool Reco_mu_TMOneStaTight[Max_mu_size];  // Vector of TMOneStationTight
  bool Reco_mu_isPF[Max_mu_size];           // Vector of isParticleFlow muon
  bool Reco_mu_isTracker[Max_mu_size];
  bool Reco_mu_isGlobal[Max_mu_size];
  bool Reco_mu_isSoft[Max_mu_size];
  bool Reco_mu_isHybridSoft[Max_mu_size];
  bool Reco_mu_isMedium[Max_mu_size];
  Short_t Reco_mu_candType
      [Max_mu_size];  // candidate type of muon. 0 (or not present): muon collection, 1: packedPFCandidate, 2: lostTrack collection
  bool Reco_mu_InTightAcc[Max_mu_size];  // Is in the tight acceptance for global muons
  bool Reco_mu_InLooseAcc[Max_mu_size];  // Is in the loose acceptance for global muons

  int Reco_mu_nPixValHits[Max_mu_size];      // Number of valid pixel hits in sta muons
  int Reco_mu_nMuValHits[Max_mu_size];       // Number of valid muon hits in sta muons
  int Reco_mu_nMuValHits_inner[Max_mu_size];       // Number of valid muon hits in Inner sta muons
  int Reco_mu_nMuValHits_bestTracker[Max_mu_size];       // Number of valid muon hits in best trcaker
  int Reco_mu_nTrkHits[Max_mu_size];         // track hits global muons
  int Reco_mu_nPixWMea[Max_mu_size];         // pixel layers with measurement for inner track muons
  int Reco_mu_nTrkWMea[Max_mu_size];         // track layers with measurement for inner track muons
  int Reco_mu_StationsMatched[Max_mu_size];  // number of stations matched for inner track muons
  float Reco_mu_segmentComp[Max_mu_size];
  float Reco_mu_kink[Max_mu_size];
  float Reco_mu_localChi2[Max_mu_size];
  float Reco_mu_normChi2_bestTracker[Max_mu_size];
  float Reco_mu_normChi2_inner[Max_mu_size];   // chi2/ndof for inner track muons
  float Reco_mu_normChi2_global[Max_mu_size];  // chi2/ndof for global muons
  float Reco_mu_dxy[Max_mu_size];              // dxy for inner track muons
  float Reco_mu_dxyErr[Max_mu_size];           // dxy error for inner track muons
  float Reco_mu_dz[Max_mu_size];               // dz for inner track muons
  float Reco_mu_dzErr[Max_mu_size];            // dz error for inner track muons
  float Reco_mu_pt_inner[Max_mu_size];         // pT for inner track muons
  float Reco_mu_pt_global[Max_mu_size];        // pT for global muons
  float Reco_mu_ptErr_inner[Max_mu_size];      // pT error for inner track muons
  float Reco_mu_ptErr_global[Max_mu_size];     // pT error for global muons
  float Reco_mu_pTrue[Max_mu_size];     // P of the associated generated muon, used to match the Reco_mu with the Gen_mu
  float Reco_mu_validFraction[Max_mu_size];
  float Reco_mu_pT[Max_mu_size];
  float Reco_mu_Eta[Max_mu_size];
  float Reco_mu_Phi[Max_mu_size];
  int Reco_mu_simExtType[Max_Bc_size];  //

  Short_t muType;  // type of muon (GlbTrk=0, Trk=1, Glb=2, none=-1)
  std::vector<float>
      EtaOfWantedMuons;  //To know which single muons to fill, when fillSingleMuons=false and we want only the muons from selected dimuons
  std::vector<float>
      EtaOfWantedTracks;  //To know which tracks to fill (only tracks that are part of a selected dimuon-track candidate)

  Short_t Reco_trk_size;                  // Number of reconstructed tracks
  Short_t Reco_trk_charge[Max_trk_size];  // Vector of charge of tracks
  Short_t Reco_trk_whichGenmu[Max_trk_size];
  bool Reco_trk_InLooseAcc[Max_trk_size];
  bool Reco_trk_InTightAcc[Max_trk_size];
  float Reco_trk_dxyError[Max_trk_size];
  float Reco_trk_dzError[Max_trk_size];
  float Reco_trk_dxy[Max_trk_size];
  float Reco_trk_dz[Max_trk_size];
  float Reco_trk_ptErr[Max_trk_size];
  int Reco_trk_originalAlgo[Max_trk_size];
  int Reco_trk_nPixWMea[Max_trk_size];
  int Reco_trk_nTrkWMea[Max_trk_size];

  // histos
  TH1F* hGoodMuonsNoTrig = NULL;
  TH1F* hGoodMuons = NULL;
  TH1F* hL1DoubleMu0 = NULL;

  MyCommonHistoManager* myRecoMuonHistos = NULL;
  MyCommonHistoManager* myRecoGlbMuonHistos = NULL;
  MyCommonHistoManager* myRecoTrkMuonHistos = NULL;

  MyCommonHistoManager* myRecoJpsiHistos = NULL;
  MyCommonHistoManager* myRecoJpsiGlbGlbHistos = NULL;
  MyCommonHistoManager* myRecoJpsiGlbTrkHistos = NULL;
  MyCommonHistoManager* myRecoJpsiTrkTrkHistos = NULL;

  // event counters
  TH1F* hStats = NULL;

  // centrality
  TH1F* hCent = NULL;

  // number of primary vertices
  TH1F* hPileUp = NULL;

  // z vertex distribution
  TH1F* hZVtx = NULL;

  // centrality
  int centBin;
  int theCentralityBin;

  Short_t Npix, NpixelTracks, Ntracks;
  int NtracksPtCut, NtracksEtaCut, NtracksEtaPtCut;
  float SumET_HF, SumET_HFplus, SumET_HFminus, SumET_HFplusEta4, SumET_HFminusEta4, SumET_HFhit, SumET_HFhitPlus,
      SumET_HFhitMinus, SumET_EB, SumET_ET, SumET_EE, SumET_EEplus, SumET_EEminus, SumET_ZDC, SumET_ZDCplus,
      SumET_ZDCminus;

  // Event Plane variables
  int nEP;  // number of event planes
  //float *hiEvtPlane;
  float rpAng[50];
  float rpCos[50];
  float rpSin[50];

  // handles
  edm::Handle<pat::CompositeCandidateCollection> collJpsi;
  edm::Handle<pat::CompositeCandidateCollection> collTrimuon;
  edm::Handle<pat::CompositeCandidateCollection> collDimutrk;
  edm::Handle<pat::MuonCollection> collMuon;
  edm::Handle<pat::MuonCollection> collMuonNoTrig;
  edm::Handle<reco::TrackCollection> collTracks;
  edm::Handle<reco::VertexCollection> SVs;

  edm::Handle<reco::GenParticleCollection> collGenParticles;
  edm::Handle<GenEventInfoProduct> genInfo;

  edm::Handle<edm::TriggerResults> collTriggerResults;

  // data members
  edm::EDGetTokenT<pat::MuonCollection> _patMuonToken;
  edm::EDGetTokenT<pat::MuonCollection> _patMuonNoTrigToken;
  edm::EDGetTokenT<pat::CompositeCandidateCollection> _patJpsiToken;
  edm::EDGetTokenT<pat::CompositeCandidateCollection> _patTrimuonToken;
  edm::EDGetTokenT<pat::CompositeCandidateCollection> _patDimuTrkToken;
  edm::EDGetTokenT<reco::TrackCollection> _recoTracksToken;
  edm::EDGetTokenT<reco::GenParticleCollection> _genParticleToken;
  edm::EDGetTokenT<GenEventInfoProduct> _genInfoToken;
  edm::EDGetTokenT<reco::VertexCollection> _thePVsToken;
  edm::EDGetTokenT<reco::VertexCollection> _SVToken;
  edm::EDGetTokenT<edm::TriggerResults> _tagTriggerResultsToken;
  edm::EDGetTokenT<reco::Centrality> _centralityTagToken;
  edm::EDGetTokenT<int> _centralityBinTagToken;
  edm::EDGetTokenT<reco::EvtPlaneCollection> _evtPlaneTagToken;
  std::string _histfilename;
  std::string _datasetname;
  std::string _muonSel;

  std::vector<double> _centralityranges;
  std::vector<double> _ptbinranges;
  std::vector<double> _etabinranges;
  std::vector<string> _dblTriggerPathNames;
  std::vector<string> _dblTriggerFilterNames;
  std::vector<string> _sglTriggerPathNames;
  std::vector<string> _sglTriggerFilterNames;

  bool _onlythebest;
  bool _applycuts;
  bool _SofterSgMuAcceptance;
  bool _SumETvariables;
  bool _selTightGlobalMuon;
  bool _storeefficiency;
  bool _muonLessPrimaryVertex;
  bool _useSVfinder;
  bool _useBS;
  bool _useRapidity;
  bool _removeSignal;
  bool _removeMuons;
  bool _storeSs;
  bool _AtLeastOneCand;
  bool _combineCategories;
  bool _fillRooDataSet;
  bool _fillTree;
  bool _fillHistos;
  bool _theMinimumFlag;
  bool _fillSingleMuons;
  bool _onlySingleMuons;
  bool _fillRecoTracks;
  bool _isHI;
  bool _isPA;
  bool _isMC;
  bool _isPromptMC;
  bool _useEvtPlane;
  bool _useGeTracks;
  bool _doTrimuons;
  bool _doDimuTrk;
  int _flipJpsiDirection;
  bool _genealogyInfo;
  bool _miniAODcut;

  int _oniaPDG;
  int _BcPDG;
  int _OneMatchedHLTMu;
  bool _checkTrigNames;

  std::vector<unsigned int> _thePassedCats;
  std::vector<const pat::CompositeCandidate*> _thePassedCands;
  std::vector<unsigned int> _thePassedBcCats;
  std::vector<const pat::CompositeCandidate*> _thePassedBcCands;

  std::vector<reco::GenParticleRef> _Gen_QQ_MomAndTrkBro[Max_QQ_size];

  // number of events
  unsigned int nEvents;
  unsigned int passedCandidates;

  unsigned int runNb;
  unsigned int eventNb;
  unsigned int lumiSection;

  // limits
  float JpsiMassMin;
  float JpsiMassMax;
  float JpsiPtMin;   // SET BY
  float JpsiPtMax;   // DEFINITION
  float JpsiRapMin;  // OF BIN
  float JpsiRapMax;  // LIMITS
  float JpsiPDGMass = 3.096916;
  float BcPDGMass = 6.276;

  math::XYZPoint RefVtx;
  float RefVtx_xError;
  float RefVtx_yError;
  float RefVtx_zError;
  float zVtx;
  Short_t nPV;

  // Trigger stuff
  // PUT HERE THE *LAST FILTERS* OF THE BITS YOU LIKE
  static const unsigned int sNTRIGGERS = 65;
  unsigned int NTRIGGERS;
  unsigned int NTRIGGERS_DBL;
  unsigned int nTrig;

  // MC 8E29
  bool isTriggerMatched[sNTRIGGERS];
  std::string HLTLastFilters[sNTRIGGERS];
  bool alreadyFilled[sNTRIGGERS];
  ULong64_t HLTriggers;
  int trigPrescale[sNTRIGGERS];

  std::map<std::string, int> mapTriggerNameToIntFired_;
  std::map<std::string, int> mapTriggerNameToPrescaleFac_;
  std::map<long int, int> mapMuonMomToIndex_;
  std::map<long int, int> mapGenMuonMomToIndex_;
  std::map<long int, int> mapTrkMomToIndex_;

  HLTPrescaleProvider hltPrescaleProvider;
  bool hltPrescaleInit;

  const edm::ParameterSet _iConfig;
};

//
// constants, enums and typedefs
//

//
// static data member definitions
//

//
// constructors and destructor
//
HiOniaAnalyzer::HiOniaAnalyzer(const edm::ParameterSet& iConfig)
    : _patMuonToken(consumes<pat::MuonCollection>(iConfig.getParameter<edm::InputTag>("srcMuon"))),
      _patMuonNoTrigToken(consumes<pat::MuonCollection>(iConfig.getParameter<edm::InputTag>("srcMuonNoTrig"))),
      _patJpsiToken(consumes<pat::CompositeCandidateCollection>(iConfig.getParameter<edm::InputTag>("srcDimuon"))),
      _patTrimuonToken(consumes<pat::CompositeCandidateCollection>(iConfig.getParameter<edm::InputTag>(
          "srcTrimuon"))),  //the names of userData are the same as for dimuons, but with 'trimuon' product instance name. Ignored if the collection does not exist
      _patDimuTrkToken(consumes<pat::CompositeCandidateCollection>(iConfig.getParameter<edm::InputTag>(
          "srcDimuTrk"))),  //the names of userData are the same as for dimuons, but with 'dimutrk' product instance name. Ignored if the collection does not exist
      _recoTracksToken(consumes<reco::TrackCollection>(iConfig.getParameter<edm::InputTag>("srcTracks"))),
      _genParticleToken(consumes<reco::GenParticleCollection>(iConfig.getParameter<edm::InputTag>("genParticles"))),
      _genInfoToken(consumes<GenEventInfoProduct>(edm::InputTag("generator"))),
      _thePVsToken(consumes<reco::VertexCollection>(iConfig.getParameter<edm::InputTag>("primaryVertexTag"))),
      _SVToken(
          consumes<reco::VertexCollection>(iConfig.getParameter<edm::InputTag>("srcSV"))),  //consumes<edm::View<VTX>>
      _tagTriggerResultsToken(
          consumes<edm::TriggerResults>(iConfig.getParameter<edm::InputTag>("triggerResultsLabel"))),
      _centralityTagToken(consumes<reco::Centrality>(iConfig.getParameter<edm::InputTag>("CentralitySrc"))),
      _centralityBinTagToken(consumes<int>(iConfig.getParameter<edm::InputTag>("CentralityBinSrc"))),
      _evtPlaneTagToken(consumes<reco::EvtPlaneCollection>(iConfig.getParameter<edm::InputTag>("EvtPlane"))),
      _histfilename(iConfig.getParameter<std::string>("histFileName")),
      _datasetname(iConfig.getParameter<std::string>("dataSetName")),
      _muonSel(iConfig.getParameter<std::string>("muonSel")),
      _centralityranges(iConfig.getParameter<std::vector<double> >("centralityRanges")),
      _ptbinranges(iConfig.getParameter<std::vector<double> >("pTBinRanges")),
      _etabinranges(iConfig.getParameter<std::vector<double> >("etaBinRanges")),
      _dblTriggerPathNames(iConfig.getParameter<std::vector<string> >("dblTriggerPathNames")),
      _dblTriggerFilterNames(iConfig.getParameter<std::vector<string> >("dblTriggerFilterNames")),
      _sglTriggerPathNames(iConfig.getParameter<std::vector<string> >("sglTriggerPathNames")),
      _sglTriggerFilterNames(iConfig.getParameter<std::vector<string> >("sglTriggerFilterNames")),
      _onlythebest(iConfig.getParameter<bool>("onlyTheBest")),
      _applycuts(iConfig.getParameter<bool>("applyCuts")),
      _SofterSgMuAcceptance(iConfig.getParameter<bool>("SofterSgMuAcceptance")),
      _SumETvariables(iConfig.getParameter<bool>("SumETvariables")),
      _selTightGlobalMuon(iConfig.getParameter<bool>("selTightGlobalMuon")),
      _storeefficiency(iConfig.getParameter<bool>("storeEfficiency")),
      _muonLessPrimaryVertex(iConfig.getParameter<bool>("muonLessPV")),
      _useSVfinder(iConfig.getParameter<bool>("useSVfinder")),
      _useBS(iConfig.getParameter<bool>("useBeamSpot")),
      _useRapidity(iConfig.getParameter<bool>("useRapidity")),
      _removeSignal(iConfig.getUntrackedParameter<bool>("removeSignalEvents", false)),
      _removeMuons(iConfig.getUntrackedParameter<bool>("removeTrueMuons", false)),
      _storeSs(iConfig.getParameter<bool>("storeSameSign")),
      _AtLeastOneCand(iConfig.getParameter<bool>("AtLeastOneCand")),
      _combineCategories(iConfig.getParameter<bool>("combineCategories")),
      _fillRooDataSet(iConfig.getParameter<bool>("fillRooDataSet")),
      _fillTree(iConfig.getParameter<bool>("fillTree")),
      _fillHistos(iConfig.getParameter<bool>("fillHistos")),
      _theMinimumFlag(iConfig.getParameter<bool>("minimumFlag")),
      _fillSingleMuons(iConfig.getParameter<bool>("fillSingleMuons")),
      _onlySingleMuons(iConfig.getParameter<bool>("onlySingleMuons")),
      _fillRecoTracks(iConfig.getParameter<bool>("fillRecoTracks")),
      _isHI(iConfig.getUntrackedParameter<bool>("isHI", false)),
      _isPA(iConfig.getUntrackedParameter<bool>("isPA", true)),
      _isMC(iConfig.getUntrackedParameter<bool>("isMC", false)),
      _isPromptMC(iConfig.getUntrackedParameter<bool>("isPromptMC", true)),
      _useEvtPlane(iConfig.getUntrackedParameter<bool>("useEvtPlane", false)),
      _useGeTracks(iConfig.getUntrackedParameter<bool>("useGeTracks", false)),
      _doTrimuons(iConfig.getParameter<bool>("doTrimuons")),
      _doDimuTrk(iConfig.getParameter<bool>("DimuonTrk")),
      _flipJpsiDirection(iConfig.getParameter<int>("flipJpsiDirection")),
      _genealogyInfo(iConfig.getParameter<bool>("genealogyInfo")),
      _miniAODcut(iConfig.getParameter<bool>("miniAODcut")),
      _oniaPDG(iConfig.getParameter<int>("oniaPDG")),
      _BcPDG(iConfig.getParameter<int>("BcPDG")),
      _OneMatchedHLTMu(iConfig.getParameter<int>("OneMatchedHLTMu")),
      _checkTrigNames(iConfig.getParameter<bool>("checkTrigNames")),
      hltPrescaleProvider(iConfig, consumesCollector(), *this),
      _iConfig(iConfig) {
  usesResource(TFileService::kSharedResource);

  if (_doTrimuons && _doDimuTrk) {
    cout << "FATAL ERROR: _doTrimuons and _doDimuTrk cannot be both true! Code not designed to do both at a time; "
            "Return now."
         << endl;
    return;
  }
  if (_doDimuTrk) {
    if (!_useGeTracks) {
      cout << "Have to use generalTracks if doDimuonTrk==true. _useGeTracks = true is forced." << endl;
      _useGeTracks = true;
    }
    if (!_fillRecoTracks) {
      cout << "Have to use generalTracks if doDimuonTrk==true. _fillRecoTracks = true is forced." << endl;
      _fillRecoTracks = true;
    }
  }

  //now do whatever initialization is needed
  nEvents = 0;
  passedCandidates = 0;

  theRegions.push_back("All");
  theRegions.push_back("Barrel");
  theRegions.push_back("EndCap");

  std::stringstream centLabel;
  for (unsigned int iCent = 0; iCent < _centralityranges.size(); ++iCent) {
    if (iCent == 0)
      centLabel << "00" << _centralityranges.at(iCent);
    else
      centLabel << _centralityranges.at(iCent - 1) << _centralityranges.at(iCent);

    theCentralities.push_back(centLabel.str());
    centLabel.str("");
  }
  theCentralities.push_back("MinBias");

  theSign.push_back("pm");
  if (_storeSs) {
    theSign.push_back("pp");
    theSign.push_back("mm");
  }

  NTRIGGERS_DBL = _dblTriggerPathNames.size();
  NTRIGGERS = NTRIGGERS_DBL + _sglTriggerPathNames.size() + 1;  // + 1 for "NoTrigger"
  std::cout << "NTRIGGERS_DBL = " << NTRIGGERS_DBL << "\t NTRIGGERS_SGL = " << _sglTriggerPathNames.size()
            << "\t NTRIGGERS = " << NTRIGGERS << std::endl;
  nTrig = NTRIGGERS - 1;

  isTriggerMatched[0] = true;  // first entry 'hardcoded' true to accept "all" events
  HLTLastFilters[0] = "";
  theTriggerNames.push_back("NoTrigger");

  for (unsigned int iTr = 1; iTr < NTRIGGERS; ++iTr) {
    isTriggerMatched[iTr] = false;

    if (iTr <= NTRIGGERS_DBL) {
      HLTLastFilters[iTr] = _dblTriggerFilterNames.at(iTr - 1);
      theTriggerNames.push_back(_dblTriggerPathNames.at(iTr - 1));
    } else {
      HLTLastFilters[iTr] = _sglTriggerFilterNames.at(iTr - NTRIGGERS_DBL - 1);
      theTriggerNames.push_back(_sglTriggerPathNames.at(iTr - NTRIGGERS_DBL - 1));
    }
    std::cout << " Trigger " << iTr << "\t" << HLTLastFilters[iTr] << std::endl;
  }

  if (_OneMatchedHLTMu >= (int)NTRIGGERS) {
    std::cout
        << "WARNING: the _OneMatchedHLTMu parameter is asking for a wrong trigger number. No matching will be done."
        << std::endl;
    _OneMatchedHLTMu = -1;
  }
  if (_OneMatchedHLTMu > -1)
    std::cout << " Will keep only dimuons (trimuons) that have one (two) daughters matched to "
              << HLTLastFilters[_OneMatchedHLTMu] << " filter." << std::endl;

  etaMax = 2.5;

  JpsiMassMin = 2.6;
  JpsiMassMax = 3.5;

  JpsiPtMin = _ptbinranges[0];
  //std::cout << "Pt min = " << JpsiPtMin << std::endl;
  JpsiPtMax = _ptbinranges[_ptbinranges.size() - 1];
  //std::cout << "Pt max = " << JpsiPtMax << std::endl;

  JpsiRapMin = _etabinranges[0];
  //std::cout << "Rap min = " << JpsiRapMin << std::endl;
  JpsiRapMax = _etabinranges[_etabinranges.size() - 1];
  //std::cout << "Rap max = " << JpsiRapMax << std::endl;

  for (std::vector<std::string>::iterator it = theTriggerNames.begin(); it != theTriggerNames.end(); ++it) {
    mapTriggerNameToIntFired_[*it] = -9999;
    mapTriggerNameToPrescaleFac_[*it] = -1;
  }
}

HiOniaAnalyzer::~HiOniaAnalyzer() {
  // do anything here that needs to be done at desctruction time
  // (e.g. close files, deallocate resources etc.)
  Reco_mu_4mom->Delete();
  Reco_mu_L1_4mom->Delete();
  Reco_QQ_4mom->Delete();
  Reco_QQ_mumi_4mom->Delete();
  Reco_QQ_mupl_4mom->Delete();
  Reco_QQ_vtx->Delete();
  if (_useGeTracks && _fillRecoTracks) {
    Reco_trk_4mom->Delete();
    Reco_trk_vtx->Delete();
  }
  if (_doTrimuons || _doDimuTrk) {
    Reco_3mu_4mom->Delete();
    Reco_3mu_vtx->Delete();
    if (_isMC) {
      Gen_Bc_4mom->Delete();
      Gen_Bc_nuW_4mom->Delete();
      Gen_3mu_4mom->Delete();
    }
  }
  if (_isMC) {
    Gen_mu_4mom->Delete();
    Gen_QQ_4mom->Delete();
  }
}

//
// member functions
//

// ------------ method called to for each event  ------------
void HiOniaAnalyzer::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup) {
  //   using namespace edm;
  InitEvent();
  nEvents++;
  hStats->Fill(BIN_nEvents);

  runNb = iEvent.id().run();
  eventNb = iEvent.id().event();
  lumiSection = iEvent.luminosityBlock();

  edm::Handle<reco::VertexCollection> privtxs;
  iEvent.getByToken(_thePVsToken, privtxs);
  reco::VertexCollection::const_iterator privtx;

  if (privtxs.isValid()) {
    nPV = privtxs->size();

    if (privtxs->begin() != privtxs->end()) {
      privtx = privtxs->begin();
      RefVtx = privtx->position();
      RefVtx_xError = privtx->xError();
      RefVtx_yError = privtx->yError();
      RefVtx_zError = privtx->zError();
    } else {
      RefVtx.SetXYZ(0., 0., 0.);
      RefVtx_xError = 0.0;
      RefVtx_yError = 0.0;
      RefVtx_zError = 0.0;
    }

    zVtx = RefVtx.Z();

    hZVtx->Fill(zVtx);
    hPileUp->Fill(nPV);
  } else {
    std::cout << "ERROR: privtxs is NULL or not isValid ! Return now" << std::endl;
    return;
  }

  this->hltReport(iEvent, iSetup);

  for (unsigned int iTr = 1; iTr < theTriggerNames.size(); iTr++) {
    if (mapTriggerNameToIntFired_[theTriggerNames.at(iTr)] == 3) {
      HLTriggers += pow(2, iTr - 1);
      hStats->Fill(iTr);  // event info
    }
    trigPrescale[iTr - 1] = mapTriggerNameToPrescaleFac_[theTriggerNames.at(iTr)];
  }

  edm::Handle<reco::Centrality> centrality;
  edm::Handle<int> cbin_;
  if (_isHI || _isPA) {
    iEvent.getByToken(_centralityTagToken, centrality);
    iEvent.getByToken(_centralityBinTagToken, cbin_);
  }
  if (centrality.isValid() && cbin_.isValid()) {
    centBin = *cbin_;
    hCent->Fill(centBin);

    for (unsigned int iCent = 0; iCent < _centralityranges.size(); ++iCent) {
      if ((_isHI && centBin < _centralityranges.at(iCent) / 0.5) || (_isPA && centBin < _centralityranges.at(iCent))) {
        theCentralityBin = iCent;
        break;
      }
    }

    Npix = (Short_t)centrality->multiplicityPixel();
    NpixelTracks = (Short_t)centrality->NpixelTracks();
    Ntracks = (Short_t)centrality->Ntracks();
    NtracksPtCut = centrality->NtracksPtCut();
    NtracksEtaCut = centrality->NtracksEtaCut();
    NtracksEtaPtCut = centrality->NtracksEtaPtCut();

    if (_SumETvariables) {
      SumET_HF = centrality->EtHFtowerSum();
      SumET_HFplus = centrality->EtHFtowerSumPlus();
      SumET_HFminus = centrality->EtHFtowerSumMinus();
      SumET_HFplusEta4 = centrality->EtHFtruncatedPlus();
      SumET_HFminusEta4 = centrality->EtHFtruncatedMinus();

      SumET_HFhit = centrality->EtHFhitSum();
      SumET_HFhitPlus = centrality->EtHFhitSumPlus();
      SumET_HFhitMinus = centrality->EtHFhitSumMinus();

      SumET_ZDC = centrality->zdcSum();
      SumET_ZDCplus = centrality->zdcSumPlus();
      SumET_ZDCminus = centrality->zdcSumMinus();

      SumET_EEplus = centrality->EtEESumPlus();
      SumET_EEminus = centrality->EtEESumMinus();
      SumET_EE = centrality->EtEESum();
      SumET_EB = centrality->EtEBSum();
      SumET_ET = centrality->EtMidRapiditySum();
    }
  } else {
    centBin = 0;
    theCentralityBin = 0;

    Npix = 0;
    NpixelTracks = 0;
    Ntracks = 0;
    NtracksPtCut = 0;
    NtracksEtaCut = 0;
    NtracksEtaPtCut = 0;

    SumET_HF = 0;
    SumET_HFplus = 0;
    SumET_HFminus = 0;
    SumET_HFplusEta4 = 0;
    SumET_HFminusEta4 = 0;

    SumET_HFhit = 0;
    SumET_HFhitPlus = 0;
    SumET_HFhitMinus = 0;

    SumET_ZDC = 0;
    SumET_ZDCplus = 0;
    SumET_ZDCminus = 0;
    SumET_EEplus = 0;
    SumET_EEminus = 0;
    SumET_EE = 0;
    SumET_EB = 0;
    SumET_ET = 0;
  }

  if ((_isHI || _isPA) && _useEvtPlane) {
    nEP = 0;
    edm::Handle<reco::EvtPlaneCollection> flatEvtPlanes;
    iEvent.getByToken(_evtPlaneTagToken, flatEvtPlanes);
    if (flatEvtPlanes.isValid()) {
      for (reco::EvtPlaneCollection::const_iterator rp = flatEvtPlanes->begin(); rp != flatEvtPlanes->end(); rp++) {
        rpAng[nEP] = rp->angle(2);   // Using Event Plane Level 2 -> Includes recentering and flattening.
        rpSin[nEP] = rp->sumSin(2);  // Using Event Plane Level 2 -> Includes recentering and flattening.
        rpCos[nEP] = rp->sumCos(2);  // Using Event Plane Level 2 -> Includes recentering and flattening.
        nEP++;
      }
    } else if (!_isMC) {
      std::cout << "Warning! Can't get flattened hiEvtPlane product!" << std::endl;
    }
  }

  iEvent.getByToken(_patJpsiToken, collJpsi);
  if (_doTrimuons)
    iEvent.getByToken(_patTrimuonToken, collTrimuon);
  if (_doDimuTrk)
    iEvent.getByToken(_patDimuTrkToken, collDimutrk);
  iEvent.getByToken(_patMuonToken, collMuon);
  iEvent.getByToken(_patMuonNoTrigToken, collMuonNoTrig);

  if (_useSVfinder)
    iEvent.getByToken(_SVToken, SVs);

  if (!_onlySingleMuons) {
    // APPLY CUTS
    this->makeCuts(_storeSs);

    // APPLY CUTS for Bc (trimuon)
    if (_doTrimuons)
      this->makeBcCuts(_storeSs);

    // APPLY CUTS for Bc (dimuon+track)
    if (_doDimuTrk)
      this->makeDimutrkCuts(_storeSs);
  }

  if (_fillSingleMuons || !_AtLeastOneCand || !_doTrimuons || !_isMC ||
      _thePassedBcCands.size() >
          0) {  //not storing the mu reconstructed info if we do a trimuon MC and there is no reco trimuon
    //_fillSingleMuons is checked within the fillRecoMuons function: the info on the wanted muons was stored in the makeCuts function
    this->fillRecoMuons(theCentralityBin);

    if (_useGeTracks) {
      iEvent.getByToken(_recoTracksToken, collTracks);
      if (_fillRecoTracks) {
        if (!collTracks.isValid()) {
          cout << " collTrack is not valid !!!! Abandoning fillRecoTracks()" << endl;
        } else
          this->fillRecoTracks();
      }
    }
  }

  if (!_onlySingleMuons)
    this->fillRecoHistos();

  //for pp, record Ntracks as well
  if (!(_isHI) && !(_isPA)) {
    iEvent.getByToken(_recoTracksToken, collTracks);
    if (collTracks.isValid()) {
      for (unsigned int tidx = 0; tidx < collTracks->size(); tidx++) {
        const reco::TrackRef track(collTracks, tidx);
        if (track->qualityByName("highPurity") && track->eta() < 2.4 &&
            fabs(track->dxy(RefVtx) / track->dxyError()) < 3 && fabs(track->dz(RefVtx) / track->dzError()) < 3 &&
            track->dz(RefVtx) < 0.5 && fabs(track->ptError() / track->pt()) < 0.1) {
          Ntracks++;
        }
      }
    }
  }

  if (_isMC) {
    //GEN info
    iEvent.getByToken(_genParticleToken, collGenParticles);
    iEvent.getByToken(_genInfoToken, genInfo);
    this->fillGenInfo();

    //MC MATCHING info
    this->fillMuMatchingInfo();  //Needs to be done after fillGenInfo, and the filling of reco muons collections
    if (!_onlySingleMuons)
      this->fillQQMatchingInfo();  //Needs to be done after fillMuMatchingInfo
    if (_doTrimuons || _doDimuTrk) {
      if (!_onlySingleMuons)
        this->fillBcMatchingInfo();  //Needs to be done after fillQQMatchingInfo
    }
  }

  //keeping events with at least ONE CANDIDATE when asked
  bool oneGoodCand = !_AtLeastOneCand;  //if !_AtLeastOneCand, pass in all cases
  if (_AtLeastOneCand) {
    if (_doTrimuons || _doDimuTrk) {
      if (Reco_3mu_size > 0)
        oneGoodCand = true;
    } else if (Reco_QQ_size > 0)
      oneGoodCand = true;
  }

  // ---- Fill the tree with this event only if AtLeastOneCand=false OR if there is at least one dimuon candidate in the event (or at least one trimuon cand if doTrimuons=true) ----
  if (_fillTree && oneGoodCand)
    myTree->Fill();

  return;
}

void HiOniaAnalyzer::fillRecoHistos() {
  if (!_doTrimuons || !_isMC ||
      _thePassedBcCands.size() >
          0) {  //not storing the mu and QQ reconstructed info if we do a trimuon MC and there is no reco trimuon
    // BEST J/PSI?
    if (_onlythebest) {  // yes, fill simply the best (possibly same-sign)

      pair<unsigned int, const pat::CompositeCandidate*> theBest = theBestQQ();
      if (theBest.first < 10)
        this->fillHistosAndDS(theBest.first, theBest.second);

    } else {  // no, fill all candidates passing cuts (possibly same-sign)

      for (unsigned int count = 0; count < _thePassedCands.size(); count++) {
        const pat::CompositeCandidate* aJpsiCand = _thePassedCands.at(count);

        this->checkTriggers(aJpsiCand);
        if (_fillTree)
          this->fillTreeJpsi(count);

        for (unsigned int iTr = 0; iTr < NTRIGGERS; ++iTr) {
          if (isTriggerMatched[iTr]) {
            this->fillRecoJpsi(count, theTriggerNames.at(iTr), theCentralities.at(theCentralityBin));
          }
        }
      }
    }
  }

  //Fill Bc (trimuon)
  if (_fillTree && _doTrimuons) {
    for (unsigned int count = 0; count < _thePassedBcCands.size(); count++) {
      this->fillTreeBc(count);
    }
  }
  //Fill Bc (dimuon+track)
  if (_fillTree && _doDimuTrk) {
    for (unsigned int count = 0; count < _thePassedBcCands.size(); count++) {
      this->fillTreeDimuTrk(count);
    }
  }

  return;
}

void HiOniaAnalyzer::fillTreeMuon(const pat::Muon* muon, int iType, ULong64_t trigBits) {
  if (Reco_mu_size >= Max_mu_size) {
    std::cout << "Too many muons: " << Reco_mu_size << std::endl;
    std::cout << "Maximum allowed: " << Max_mu_size << std::endl;
    return;
  }

  if (muon != NULL) {
    Reco_mu_charge[Reco_mu_size] = muon->charge();
    Reco_mu_type[Reco_mu_size] = iType;

    TLorentzVector vMuon = lorentzMomentum(muon->p4());
    new ((*Reco_mu_4mom)[Reco_mu_size]) TLorentzVector(vMuon);

    TLorentzVector vMuonL1;
    if (muon->hasUserFloat("l1Eta") && muon->hasUserFloat("l1Phi")) {
      vMuonL1.SetPtEtaPhiM(vMuon.Pt(), muon->userFloat("l1Eta"), muon->userFloat("l1Phi"), vMuon.M());
    } else {
      vMuonL1.SetPtEtaPhiM(0, 0, 0, 0);
    }
    new ((*Reco_mu_L1_4mom)[Reco_mu_size]) TLorentzVector(vMuonL1);

    //Fill map of the muon indices. Use long int keys, to avoid rounding errors on a float key. Implies a precision of 10^-6
    mapMuonMomToIndex_[FloatToIntkey(vMuon.Pt())] = Reco_mu_size;

    Reco_mu_trig[Reco_mu_size] = trigBits;

    reco::TrackRef iTrack = muon->innerTrack();
    reco::TrackRef bestTrack = muon->muonBestTrack();
  
    TLorentzVector Mu_probe;
  
    if (!_theMinimumFlag) {
      Reco_mu_InTightAcc[Reco_mu_size] = isMuonInAccept(muon, "GLB");
      Reco_mu_InLooseAcc[Reco_mu_size] = isMuonInAccept(muon, "GLBSOFT");
      Reco_mu_SelectionType[Reco_mu_size] = muonIDmask(muon);
      Reco_mu_StationsMatched[Reco_mu_size] = muon->numberOfMatchedStations();
      Reco_mu_isTracker[Reco_mu_size] = muon->isTrackerMuon();
      Reco_mu_isGlobal[Reco_mu_size] = muon->isGlobalMuon();
      Reco_mu_isPF[Reco_mu_size] = muon->isPFMuon();
      Reco_mu_isSoft[Reco_mu_size] = isSoftMuon(muon);
      Reco_mu_isHybridSoft[Reco_mu_size] = isHybridSoftMuon(muon);
      Reco_mu_isMedium[Reco_mu_size] = muon->isMediumMuon();
      Reco_mu_candType[Reco_mu_size] = (Short_t)(muon->hasUserInt("candType")) ? (muon->userInt("candType")) : (-1);

      Reco_mu_TMOneStaTight[Reco_mu_size] = muon::isGoodMuon(*muon, muon::TMOneStationTight);

      Reco_mu_localChi2[Reco_mu_size] = muon->combinedQuality().chi2LocalPosition;
      Reco_mu_kink[Reco_mu_size] = muon->combinedQuality().trkKink;
      Reco_mu_segmentComp[Reco_mu_size] = muon->segmentCompatibility(reco::Muon::SegmentAndTrackArbitration);

      Reco_mu_normChi2_bestTracker[Reco_mu_size] = bestTrack->normalizedChi2();
      Reco_mu_nMuValHits_bestTracker[Reco_mu_size] = bestTrack->hitPattern().numberOfValidMuonHits();

      Mu_probe.SetPxPyPzE(bestTrack->px(), bestTrack->py(), bestTrack->pz(), bestTrack->p());
      Reco_mu_pT[Reco_mu_size] = Mu_probe.Pt();
      Reco_mu_Eta[Reco_mu_size] = Mu_probe.Eta();
      Reco_mu_Phi[Reco_mu_size] = Mu_probe.Phi();

      if (iTrack.isNonnull()) {
        Reco_mu_highPurity[Reco_mu_size] = iTrack->quality(reco::TrackBase::highPurity);
        Reco_mu_nTrkHits[Reco_mu_size] = iTrack->found();
        Reco_mu_normChi2_inner[Reco_mu_size] =
            (muon->hasUserFloat("trackChi2") ? muon->userFloat("trackChi2") : iTrack->normalizedChi2());
        Reco_mu_nPixValHits[Reco_mu_size] = iTrack->hitPattern().numberOfValidPixelHits();
        Reco_mu_nPixWMea[Reco_mu_size] = iTrack->hitPattern().pixelLayersWithMeasurement();
        Reco_mu_nTrkWMea[Reco_mu_size] = iTrack->hitPattern().trackerLayersWithMeasurement();
        Reco_mu_dxy[Reco_mu_size] = iTrack->dxy(RefVtx);
        Reco_mu_dxyErr[Reco_mu_size] = iTrack->dxyError();
        Reco_mu_dz[Reco_mu_size] = iTrack->dz(RefVtx);
        Reco_mu_dzErr[Reco_mu_size] = iTrack->dzError();
        Reco_mu_nMuValHits_inner[Reco_mu_size] = iTrack->hitPattern().numberOfValidMuonHits();
        Reco_mu_validFraction[Reco_mu_size] = iTrack->validFraction();
        //Reco_mu_pt_inner[Reco_mu_size] = iTrack->pt();
        Reco_mu_ptErr_inner[Reco_mu_size] = iTrack->ptError();
      } else if (_muonSel != (std::string)("All")) {
        std::cout << "ERROR: 'iTrack' pointer in fillTreeMuon is NULL ! Return now" << std::endl;
        return;
      }

      if (muon->isGlobalMuon()) {
        reco::TrackRef gTrack = muon->globalTrack();
        Reco_mu_nMuValHits[Reco_mu_size] = gTrack->hitPattern().numberOfValidMuonHits();
        Reco_mu_normChi2_global[Reco_mu_size] = gTrack->normalizedChi2();
        //Reco_mu_pt_global[Reco_mu_size] = gTrack->pt();
        //Reco_mu_ptErr_global[Reco_mu_size] = gTrack->ptError();

      } else {
        Reco_mu_nMuValHits[Reco_mu_size] = -1;
        Reco_mu_normChi2_global[Reco_mu_size] = 999;
        //Reco_mu_pt_global[Reco_mu_size] = -1;
        //Reco_mu_ptErr_global[Reco_mu_size] = -1;
      }
    }

    if (_isMC) {
      Reco_mu_pTrue[Reco_mu_size] =
          ((muon->genParticleRef()).isNonnull()) ? ((float)(muon->genParticleRef())->p()) : (-1);
      if (_genealogyInfo) {
        Reco_mu_simExtType[Reco_mu_size] = muon->simExtType();
      }
    }

  } else {
    std::cout << "ERROR: 'muon' pointer in fillTreeMuon is NULL ! Return now" << std::endl;
    return;
  }

  Reco_mu_size++;
  return;
}

void HiOniaAnalyzer::fillTreeJpsi(int count) {
  if (Reco_QQ_size >= Max_QQ_size) {
    std::cout << "Too many dimuons: " << Reco_QQ_size << std::endl;
    std::cout << "Maximum allowed: " << Max_QQ_size << std::endl;
    return;
  }

  const pat::CompositeCandidate* aJpsiCand = _thePassedCands.at(count);

  if (aJpsiCand != NULL) {
    const pat::Muon* muon1 = dynamic_cast<const pat::Muon*>(aJpsiCand->daughter("muon1"));
    const pat::Muon* muon2 = dynamic_cast<const pat::Muon*>(aJpsiCand->daughter("muon2"));

    ULong64_t trigBits = 0;
    for (unsigned int iTr = 1; iTr < NTRIGGERS; ++iTr) {
      if (isTriggerMatched[iTr]) {
        trigBits += pow(2, iTr - 1);
      }
    }

    if (muon1 == NULL || muon2 == NULL) {
      std::cout << "ERROR: 'muon1' or 'muon2' pointer in fillTreeJpsi is NULL ! Return now" << std::endl;
      return;
    } else {
      Reco_QQ_sign[Reco_QQ_size] = muon1->charge() + muon2->charge();
      Reco_QQ_type[Reco_QQ_size] = _thePassedCats.at(count);

      Reco_QQ_trig[Reco_QQ_size] = trigBits;

      if (!(_isHI) && _muonLessPrimaryVertex && aJpsiCand->hasUserData("muonlessPV")) {
        RefVtx = (*aJpsiCand->userData<reco::Vertex>("muonlessPV")).position();
        RefVtx_xError = (*aJpsiCand->userData<reco::Vertex>("muonlessPV")).xError();
        RefVtx_yError = (*aJpsiCand->userData<reco::Vertex>("muonlessPV")).yError();
        RefVtx_zError = (*aJpsiCand->userData<reco::Vertex>("muonlessPV")).zError();
      } else if (!_muonLessPrimaryVertex && aJpsiCand->hasUserData("PVwithmuons")) {
        RefVtx = (*aJpsiCand->userData<reco::Vertex>("PVwithmuons")).position();
        RefVtx_xError = (*aJpsiCand->userData<reco::Vertex>("PVwithmuons")).xError();
        RefVtx_yError = (*aJpsiCand->userData<reco::Vertex>("PVwithmuons")).yError();
        RefVtx_zError = (*aJpsiCand->userData<reco::Vertex>("PVwithmuons")).zError();
      } else {
        cout << "HiOniaAnalyzer::fillTreeJpsi: no PVfor muon pair stored" << endl;
        return;
      }

      new ((*Reco_QQ_vtx)[Reco_QQ_size]) TVector3(RefVtx.X(), RefVtx.Y(), RefVtx.Z());

      TLorentzVector vMuon1 = lorentzMomentum(muon1->p4());
      TLorentzVector vMuon2 = lorentzMomentum(muon2->p4());

      reco::Track iTrack_mupl, iTrack_mumi, mu1Trk, mu2Trk;
      if (_flipJpsiDirection > 0 && aJpsiCand->hasUserData("muon1Track") && aJpsiCand->hasUserData("muon2Track")) {
        mu1Trk = *(aJpsiCand->userData<reco::Track>("muon1Track"));
        mu2Trk = *(aJpsiCand->userData<reco::Track>("muon2Track"));
      }

      Reco_QQ_flipJpsi[Reco_QQ_size] = _flipJpsiDirection;
      if (aJpsiCand->hasUserInt("flipJpsi"))
        Reco_QQ_flipJpsi[Reco_QQ_size] = aJpsiCand->userInt("flipJpsi");

      if ((muon1->innerTrack()).isNull() || (muon2->innerTrack()).isNull()) {
        std::cout << "ERROR: 'iTrack_mupl' or 'iTrack_mumi' pointer in fillTreeJpsi is NULL ! Return now" << std::endl;
        return;
      }

      if (muon1->charge() > muon2->charge()) {
        Reco_QQ_mupl_idx[Reco_QQ_size] = IndexOfThisMuon(&vMuon1);  //needs the non-flipped muon momentum
        Reco_QQ_mumi_idx[Reco_QQ_size] = IndexOfThisMuon(&vMuon2);

        if (TVector2::Phi_mpi_pi(vMuon1.Phi() - vMuon2.Phi()) > 0)
          Reco_QQ_isCowboy[Reco_QQ_size] = true;
        else
          Reco_QQ_isCowboy[Reco_QQ_size] = false;

        if (_flipJpsiDirection > 0) {
          iTrack_mupl = mu1Trk;
          iTrack_mumi = mu2Trk;
          new ((*Reco_QQ_mupl_4mom)[Reco_QQ_size]) TLorentzVector(
              mu1Trk.px(), mu1Trk.py(), mu1Trk.pz(), vMuon1.E());  //only the direction of the 3-momentum changes
          new ((*Reco_QQ_mumi_4mom)[Reco_QQ_size]) TLorentzVector(mu2Trk.px(), mu2Trk.py(), mu2Trk.pz(), vMuon2.E());
        } else if (_muonLessPrimaryVertex || _useGeTracks) {
          iTrack_mupl = *(muon1->innerTrack());
          iTrack_mumi = *(muon2->innerTrack());
        }

      } else {
        Reco_QQ_mupl_idx[Reco_QQ_size] = IndexOfThisMuon(&vMuon2);  //needs the non-flipped muon momentum
        Reco_QQ_mumi_idx[Reco_QQ_size] = IndexOfThisMuon(&vMuon1);

        if (TVector2::Phi_mpi_pi(vMuon2.Phi() - vMuon1.Phi()) > 0)
          Reco_QQ_isCowboy[Reco_QQ_size] = true;
        else
          Reco_QQ_isCowboy[Reco_QQ_size] = false;

        if (_flipJpsiDirection > 0) {
          iTrack_mupl = mu2Trk;
          iTrack_mumi = mu1Trk;
          new ((*Reco_QQ_mumi_4mom)[Reco_QQ_size]) TLorentzVector(
              mu1Trk.px(), mu1Trk.py(), mu1Trk.pz(), vMuon1.E());  //only the direction of the 3-momentum changes
          new ((*Reco_QQ_mupl_4mom)[Reco_QQ_size]) TLorentzVector(mu2Trk.px(), mu2Trk.py(), mu2Trk.pz(), vMuon2.E());
        } else if (_muonLessPrimaryVertex || _useGeTracks) {
          iTrack_mupl = *(muon2->innerTrack());
          iTrack_mumi = *(muon1->innerTrack());
        }
      }

      if ((!_theMinimumFlag && _muonLessPrimaryVertex) || (_flipJpsiDirection > 0)) {
        Reco_QQ_mupl_dxy[Reco_QQ_size] = iTrack_mupl.dxy(RefVtx);
        Reco_QQ_mumi_dxy[Reco_QQ_size] = iTrack_mumi.dxy(RefVtx);
        Reco_QQ_mupl_dz[Reco_QQ_size] = iTrack_mupl.dz(RefVtx);
        Reco_QQ_mumi_dz[Reco_QQ_size] = iTrack_mumi.dz(RefVtx);
      }

      TLorentzVector vJpsi = lorentzMomentum(aJpsiCand->p4());
      new ((*Reco_QQ_4mom)[Reco_QQ_size]) TLorentzVector(vJpsi);

      if (_useBS) {
        if (aJpsiCand->hasUserFloat("ppdlBS")) {
          Reco_QQ_ctau[Reco_QQ_size] = 10.0 * aJpsiCand->userFloat("ppdlBS");
        } else {
          Reco_QQ_ctau[Reco_QQ_size] = -100;
          std::cout << "Warning: User Float ppdlBS was not found" << std::endl;
        }
        if (aJpsiCand->hasUserFloat("ppdlErrBS")) {
          Reco_QQ_ctauErr[Reco_QQ_size] = 10.0 * aJpsiCand->userFloat("ppdlErrBS");
        } else {
          Reco_QQ_ctauErr[Reco_QQ_size] = -100;
          std::cout << "Warning: User Float ppdlErrBS was not found" << std::endl;
        }
        if (aJpsiCand->hasUserFloat("ppdlBS3D")) {
          Reco_QQ_ctau3D[Reco_QQ_size] = 10.0 * aJpsiCand->userFloat("ppdlBS3D");
        } else {
          Reco_QQ_ctau3D[Reco_QQ_size] = -100;
          std::cout << "Warning: User Float ppdlBS3D was not found" << std::endl;
        }
        if (aJpsiCand->hasUserFloat("ppdlErrBS3D")) {
          Reco_QQ_ctauErr3D[Reco_QQ_size] = 10.0 * aJpsiCand->userFloat("ppdlErrBS3D");
        } else {
          Reco_QQ_ctauErr3D[Reco_QQ_size] = -100;
          std::cout << "Warning: User Float ppdlErrBS3D was not found" << std::endl;
        }
      } else {
        if (aJpsiCand->hasUserFloat("ppdlPV")) {
          Reco_QQ_ctau[Reco_QQ_size] = 10.0 * aJpsiCand->userFloat("ppdlPV");
        } else {
          Reco_QQ_ctau[Reco_QQ_size] = -100;
          std::cout << "Warning: User Float ppdlPV was not found" << std::endl;
        }
        if (aJpsiCand->hasUserFloat("ppdlErrPV")) {
          Reco_QQ_ctauErr[Reco_QQ_size] = 10.0 * aJpsiCand->userFloat("ppdlErrPV");
        } else {
          Reco_QQ_ctauErr[Reco_QQ_size] = -100;
          std::cout << "Warning: User Float ppdlErrPV was not found" << std::endl;
        }
        if (aJpsiCand->hasUserFloat("ppdlPV3D")) {
          Reco_QQ_ctau3D[Reco_QQ_size] = 10.0 * aJpsiCand->userFloat("ppdlPV3D");
        } else {
          Reco_QQ_ctau3D[Reco_QQ_size] = -100;
          std::cout << "Warning: User Float ppdlPV3D was not found" << std::endl;
        }
        if (aJpsiCand->hasUserFloat("ppdlErrPV3D")) {
          Reco_QQ_ctauErr3D[Reco_QQ_size] = 10.0 * aJpsiCand->userFloat("ppdlErrPV3D");
        } else {
          Reco_QQ_ctau3D[Reco_QQ_size] = -100;
          std::cout << "Warning: User Float ppdlErrPV3D was not found" << std::endl;
        }
        if (aJpsiCand->hasUserFloat("cosAlpha")) {
          Reco_QQ_cosAlpha[Reco_QQ_size] = aJpsiCand->userFloat("cosAlpha");
        } else {
          Reco_QQ_cosAlpha[Reco_QQ_size] = -10;
          std::cout << "Warning: User Float cosAlpha was not found" << std::endl;
        }
        if (aJpsiCand->hasUserFloat("cosAlpha3D")) {
          Reco_QQ_cosAlpha3D[Reco_QQ_size] = aJpsiCand->userFloat("cosAlpha3D");
        } else {
          Reco_QQ_cosAlpha3D[Reco_QQ_size] = -10;
          std::cout << "Warning: User Float cosAlpha3D was not found" << std::endl;
        }
      }
      if (aJpsiCand->hasUserFloat("vProb")) {
        Reco_QQ_VtxProb[Reco_QQ_size] = aJpsiCand->userFloat("vProb");
      } else {
        Reco_QQ_VtxProb[Reco_QQ_size] = -1;
        std::cout << "Warning: User Float vProb was not found" << std::endl;
      }
      if (aJpsiCand->hasUserFloat("DCA")) {
        Reco_QQ_dca[Reco_QQ_size] = aJpsiCand->userFloat("DCA");
      } else {
        Reco_QQ_dca[Reco_QQ_size] = -10;
        std::cout << "Warning: User Float DCA was not found" << std::endl;
      }
      if (aJpsiCand->hasUserFloat("MassErr")) {
        Reco_QQ_MassErr[Reco_QQ_size] = aJpsiCand->userFloat("MassErr");
      } else {
        Reco_QQ_MassErr[Reco_QQ_size] = -10;
        std::cout << "Warning: User Float MassErr was not found" << std::endl;
      }

      Reco_QQ_NtrkDeltaR03[Reco_QQ_size] = 0;
      Reco_QQ_NtrkDeltaR04[Reco_QQ_size] = 0;
      Reco_QQ_NtrkDeltaR05[Reco_QQ_size] = 0;

      Reco_QQ_NtrkPt02[Reco_QQ_size] = 0;
      Reco_QQ_NtrkPt03[Reco_QQ_size] = 0;
      Reco_QQ_NtrkPt04[Reco_QQ_size] = 0;

      //--- counting tracks around Jpsi direction ---
      if (_useGeTracks && !_doDimuTrk && collTracks.isValid()) {
        for (std::vector<reco::Track>::const_iterator it = collTracks->begin(); it != collTracks->end(); ++it) {
          const reco::Track* track = &(*it);

          if (track == NULL) {
            std::cout << "ERROR: 'track' pointer in fillTreeJpsi is NULL ! Return now" << std::endl;
            return;
          } else {
            double dz = track->dz(RefVtx);
            double dzsigma = sqrt(track->dzError() * track->dzError() + RefVtx_zError * RefVtx_zError);
            double dxy = track->dxy(RefVtx);
            double dxysigma = sqrt(track->dxyError() * track->dxyError() + RefVtx_xError * RefVtx_yError);

            if (track->qualityByName("highPurity") && track->pt() > 0.2 && fabs(track->eta()) < 2.4 &&
                track->ptError() / track->pt() < 0.1 && fabs(dz / dzsigma) < 3.0 && fabs(dxy / dxysigma) < 3.0) {
              Reco_QQ_NtrkPt02[Reco_QQ_size]++;
              if (track->pt() > 0.3)
                Reco_QQ_NtrkPt03[Reco_QQ_size]++;
              if (track->pt() > 0.4) {
                Reco_QQ_NtrkPt04[Reco_QQ_size]++;

                if (iTrack_mupl.charge() == track->charge()) {
                  double Reco_QQ_mupl_NtrkDeltaR =
                      deltaR(iTrack_mupl.eta(), iTrack_mupl.phi(), track->eta(), track->phi());
                  double Reco_QQ_mupl_RelDelPt = abs(1.0 - iTrack_mupl.pt() / track->pt());

                  if (Reco_QQ_mupl_NtrkDeltaR < 0.001 && Reco_QQ_mupl_RelDelPt < 0.001)
                    continue;
                } else {
                  double Reco_QQ_mumi_NtrkDeltaR =
                      deltaR(iTrack_mumi.eta(), iTrack_mumi.phi(), track->eta(), track->phi());
                  double Reco_QQ_mumi_RelDelPt = abs(1.0 - iTrack_mumi.pt() / track->pt());
                  if (Reco_QQ_mumi_NtrkDeltaR < 0.001 && Reco_QQ_mumi_RelDelPt < 0.001)
                    continue;
                }

                double Reco_QQ_NtrkDeltaR = deltaR(aJpsiCand->eta(), aJpsiCand->phi(), track->eta(), track->phi());
                if (Reco_QQ_NtrkDeltaR < 0.3)
                  Reco_QQ_NtrkDeltaR03[Reco_QQ_size]++;
                if (Reco_QQ_NtrkDeltaR < 0.4)
                  Reco_QQ_NtrkDeltaR04[Reco_QQ_size]++;
                if (Reco_QQ_NtrkDeltaR < 0.5)
                  Reco_QQ_NtrkDeltaR05[Reco_QQ_size]++;
              }
            }
          }
        }
      }
    }
  } else {
    std::cout << "ERROR: 'aJpsiCand' pointer in fillTreeJpsi is NULL ! Return now" << std::endl;
    return;
  }

  Reco_QQ_size++;
  return;
}

void HiOniaAnalyzer::fillTreeBc(int count) {
  if (Reco_3mu_size >= Max_Bc_size) {
    std::cout << "Too many trimuons: " << Reco_3mu_size << std::endl;
    std::cout << "Maximum allowed: " << Max_Bc_size << std::endl;
    return;
  }

  const pat::CompositeCandidate* aBcCand = _thePassedBcCands.at(count);

  if (aBcCand == NULL) {
    std::cout << "ERROR: 'aBcCand' pointer in fillTreeBc is NULL ! Return now" << std::endl;
    return;
  } else {
    const pat::Muon* muon1 = dynamic_cast<const pat::Muon*>(aBcCand->daughter("muon1"));
    const pat::Muon* muon2 = dynamic_cast<const pat::Muon*>(aBcCand->daughter("muon2"));
    const pat::Muon* muon3 = dynamic_cast<const pat::Muon*>(aBcCand->daughter("muon3"));

    if (muon1 == NULL || muon2 == NULL || muon3 == NULL) {
      std::cout << "ERROR: 'muon1' or 'muon2' or 'muon3' pointer in fillTreeBc is NULL ! Return now" << std::endl;
      return;
    } else {
      int charge = muon1->charge() + muon2->charge() + muon3->charge();

      TLorentzVector vMuon1 = lorentzMomentum(muon1->p4());
      TLorentzVector vMuon2 = lorentzMomentum(muon2->p4());
      TLorentzVector vMuon3 = lorentzMomentum(muon3->p4());

      int mu1_idx = IndexOfThisMuon(&vMuon1);  //the muon list contains unchanged muons (even in jpsiFlipping case)
      int mu2_idx = IndexOfThisMuon(&vMuon2);
      int mu3_idx = IndexOfThisMuon(&vMuon3);

      int flipJ = 0;
      if (aBcCand->hasUserInt("flipJpsi"))
        flipJ = aBcCand->userInt("flipJpsi");

      //One dimuon combination has to pass the Jpsi kinematic cuts
      if (IndexOfThisJpsi(mu1_idx, mu2_idx, flipJ) == -1 && IndexOfThisJpsi(mu2_idx, mu3_idx, flipJ) == -1 &&
          IndexOfThisJpsi(mu1_idx, mu3_idx, flipJ) == -1) {
        return;
      }

      if (_flipJpsiDirection > 0) {  //in case of Jpsi flipping, we know that mu1-mu2 is the chosen Jpsi OS dimuon
        Reco_3mu_QQ1_idx[Reco_3mu_size] = IndexOfThisJpsi(mu1_idx, mu2_idx, flipJ);
        Reco_3mu_muW_idx[Reco_3mu_size] = mu3_idx;
        Reco_3mu_mumi_idx[Reco_3mu_size] = (Reco_mu_charge[mu1_idx] == -1) ? mu1_idx : mu2_idx;
        Reco_3mu_mupl_idx[Reco_3mu_size] = (Reco_mu_charge[mu1_idx] == 1) ? mu1_idx : mu2_idx;

        if (Reco_mu_charge[mu1_idx] ==
            Reco_mu_charge[mu3_idx]) {  //supposing mu1 and mu2 are precedently chosen as opposite-sign
          Reco_3mu_QQ2_idx[Reco_3mu_size] = IndexOfThisJpsi(mu3_idx, mu2_idx, flipJ);
          Reco_3mu_QQss_idx[Reco_3mu_size] = IndexOfThisJpsi(mu3_idx, mu1_idx, flipJ);
        } else {
          Reco_3mu_QQ2_idx[Reco_3mu_size] = IndexOfThisJpsi(mu3_idx, mu1_idx, flipJ);
          Reco_3mu_QQss_idx[Reco_3mu_size] = IndexOfThisJpsi(mu3_idx, mu2_idx, flipJ);
        }
      }  // end jpsi flipping case

      else {
        //If Bc charge is OK, write out the QQ indices for the two opposite-sign pairs
        if (fabs(charge) == 1) {
          int mu_loneCharge = mu1_idx;
          int mu_SameCharge1 = mu2_idx;
          int mu_SameCharge2 = mu3_idx;
          //Look for the muon that has a different charge than the two others
          if (Reco_mu_charge[mu1_idx] == Reco_mu_charge[mu2_idx]) {
            mu_loneCharge = mu3_idx;
            mu_SameCharge2 = mu1_idx;
          } else if (Reco_mu_charge[mu1_idx] == Reco_mu_charge[mu3_idx]) {
            mu_loneCharge = mu2_idx;
            mu_SameCharge1 = mu1_idx;
          }

          //Look for the Jpsi indices corresponding to two muons
          Reco_3mu_QQ1_idx[Reco_3mu_size] = IndexOfThisJpsi(mu_loneCharge, mu_SameCharge1);
          Reco_3mu_QQ2_idx[Reco_3mu_size] = IndexOfThisJpsi(mu_loneCharge, mu_SameCharge2);
          if (Reco_3mu_QQ1_idx[Reco_3mu_size] == -1 && Reco_3mu_QQ2_idx[Reco_3mu_size] == -1)
            return;  //need one OS pair to pass the dimuon selection
          Reco_3mu_QQss_idx[Reco_3mu_size] = IndexOfThisJpsi(mu_SameCharge1, mu_SameCharge2);

          //Split the muon types according to one valid QQ hypothesis among the two OS pairs
          if (Reco_3mu_QQ1_idx[Reco_3mu_size] > -1) {  //The second os pair could also pass
            Reco_3mu_mumi_idx[Reco_3mu_size] = (Reco_mu_charge[mu_loneCharge] == -1) ? mu_loneCharge : mu_SameCharge1;
            Reco_3mu_mupl_idx[Reco_3mu_size] = (Reco_mu_charge[mu_SameCharge1] == 1) ? mu_SameCharge1 : mu_loneCharge;
            Reco_3mu_muW_idx[Reco_3mu_size] = mu_SameCharge2;
          } else {  //Only the second pair is valid
            Reco_3mu_mumi_idx[Reco_3mu_size] = (Reco_mu_charge[mu_loneCharge] == -1) ? mu_loneCharge : mu_SameCharge2;
            Reco_3mu_mupl_idx[Reco_3mu_size] = (Reco_mu_charge[mu_SameCharge2] == 1) ? mu_SameCharge2 : mu_loneCharge;
            Reco_3mu_muW_idx[Reco_3mu_size] = mu_SameCharge1;
          }
          //pointing to the muW corresponding to the 2nd OS pair
          Reco_3mu_muW2_idx[Reco_3mu_size] = mu_SameCharge1;

        }  //end Jpsi attribution for good Bc charge

        //If charge of the Bc is wrong, simpler procedure : random attribution of the three same-sign pairs
        else {
          Reco_3mu_QQ1_idx[Reco_3mu_size] = IndexOfThisJpsi(mu1_idx, mu2_idx);
          Reco_3mu_QQ2_idx[Reco_3mu_size] = IndexOfThisJpsi(mu1_idx, mu3_idx);
          Reco_3mu_QQss_idx[Reco_3mu_size] = IndexOfThisJpsi(mu2_idx, mu3_idx);

          Reco_3mu_mumi_idx[Reco_3mu_size] = mu1_idx;  //Which muon is mumi or mupl is random
          Reco_3mu_mupl_idx[Reco_3mu_size] = mu2_idx;
          Reco_3mu_muW_idx[Reco_3mu_size] = mu3_idx;
          Reco_3mu_muW2_idx[Reco_3mu_size] = mu2_idx;
        }
      }  // end "no Jpsi flipping"

      //*********
      //Fill all remaining Bc variables
      Reco_3mu_charge[Reco_3mu_size] = charge;
      TLorentzVector vBc = lorentzMomentum(aBcCand->p4());
      new ((*Reco_3mu_4mom)[Reco_3mu_size]) TLorentzVector(vBc);
      // if(_flipJpsiDirection>0){
      // 	cout<<"Bc mass, old vs new = "<< (vMuon1+vMuon2+vMuon3).M()<<" "<<vBc.M()<<endl;
      // }

      if (!(_isHI) && _muonLessPrimaryVertex && aBcCand->hasUserData("muonlessPV")) {
        RefVtx = (*aBcCand->userData<reco::Vertex>("muonlessPV")).position();
        RefVtx_xError = (*aBcCand->userData<reco::Vertex>("muonlessPV")).xError();
        RefVtx_yError = (*aBcCand->userData<reco::Vertex>("muonlessPV")).yError();
        RefVtx_zError = (*aBcCand->userData<reco::Vertex>("muonlessPV")).zError();
      } else if (aBcCand->hasUserData("PVwithmuons")) {
        RefVtx = (*aBcCand->userData<reco::Vertex>("PVwithmuons")).position();
        RefVtx_xError = (*aBcCand->userData<reco::Vertex>("PVwithmuons")).xError();
        RefVtx_yError = (*aBcCand->userData<reco::Vertex>("PVwithmuons")).yError();
        RefVtx_zError = (*aBcCand->userData<reco::Vertex>("PVwithmuons")).zError();
      } else {
        cout << "HiOniaAnalyzer::fillTreeBc: no PVfor muon pair stored" << endl;
        return;
      }

      new ((*Reco_3mu_vtx)[Reco_3mu_size]) TVector3(RefVtx.X(), RefVtx.Y(), RefVtx.Z());

      reco::Track iTrack_mupl, iTrack_mumi, iTrack_muW, mu1Trk, mu2Trk;
      if (_flipJpsiDirection > 0 && aBcCand->hasUserData("muon1Track") && aBcCand->hasUserData("muon2Track")) {
        mu1Trk = *(aBcCand->userData<reco::Track>("muon1Track"));
        mu2Trk = *(aBcCand->userData<reco::Track>("muon2Track"));
      }

      //Long here but nothing much is happening -- just have to find back the mumi-pl-W indices
      if ((!_theMinimumFlag && _muonLessPrimaryVertex) || (_flipJpsiDirection > 0)) {
        if (_flipJpsiDirection > 0) {
          iTrack_mupl = (muon1->charge() > 0) ? mu1Trk : mu2Trk;
          iTrack_mumi = (muon1->charge() > 0) ? mu2Trk : mu1Trk;
          iTrack_muW = *(muon3->innerTrack());
        }

        else {  //find indices of mumi-pl-W
          if (Reco_3mu_muW_idx[Reco_3mu_size] == mu1_idx) {
            iTrack_muW = *(muon1->innerTrack());
            if (Reco_3mu_mumi_idx[Reco_3mu_size] == mu2_idx) {
              iTrack_mumi = *(muon2->innerTrack());
              iTrack_mupl = *(muon3->innerTrack());
            } else {
              iTrack_mumi = *(muon3->innerTrack());
              iTrack_mupl = *(muon2->innerTrack());
            }
          } else if (Reco_3mu_muW_idx[Reco_3mu_size] == mu2_idx) {
            iTrack_muW = *(muon2->innerTrack());
            if (Reco_3mu_mumi_idx[Reco_3mu_size] == mu1_idx) {
              iTrack_mumi = *(muon1->innerTrack());
              iTrack_mupl = *(muon3->innerTrack());
            } else {
              iTrack_mumi = *(muon3->innerTrack());
              iTrack_mupl = *(muon1->innerTrack());
            }
          } else {
            iTrack_muW = *(muon3->innerTrack());
            if (Reco_3mu_mumi_idx[Reco_3mu_size] == mu2_idx) {
              iTrack_mumi = *(muon2->innerTrack());
              iTrack_mupl = *(muon1->innerTrack());
            } else {
              iTrack_mumi = *(muon1->innerTrack());
              iTrack_mupl = *(muon2->innerTrack());
            }
          }
        }

        Reco_3mu_muW_dxy[Reco_3mu_size] = iTrack_muW.dxy(RefVtx);
        Reco_3mu_muW_dz[Reco_3mu_size] = iTrack_muW.dz(RefVtx);
        Reco_3mu_mumi_dxy[Reco_3mu_size] = iTrack_mumi.dxy(RefVtx);
        Reco_3mu_mumi_dz[Reco_3mu_size] = iTrack_mumi.dz(RefVtx);
        Reco_3mu_mupl_dxy[Reco_3mu_size] = iTrack_mupl.dxy(RefVtx);
        Reco_3mu_mupl_dz[Reco_3mu_size] = iTrack_mupl.dz(RefVtx);
      }

      //*********
      //Lifetime related variables

      if (_useBS) {
        if (aBcCand->hasUserFloat("ppdlBS")) {
          Reco_3mu_ctau[Reco_3mu_size] = 10.0 * aBcCand->userFloat("ppdlBS");
        } else {
          Reco_3mu_ctau[Reco_3mu_size] = -10;
          std::cout << "Warning: User Float ppdlBS was not found" << std::endl;
        }
        if (aBcCand->hasUserFloat("ppdlErrBS")) {
          Reco_3mu_ctauErr[Reco_3mu_size] = 10.0 * aBcCand->userFloat("ppdlErrBS");
        } else {
          Reco_3mu_ctauErr[Reco_3mu_size] = -10;
          std::cout << "Warning: User Float ppdlErrBS was not found" << std::endl;
        }
        if (aBcCand->hasUserFloat("ppdlBS3D")) {
          Reco_3mu_ctau3D[Reco_3mu_size] = 10.0 * aBcCand->userFloat("ppdlBS3D");
        } else {
          Reco_3mu_ctau3D[Reco_3mu_size] = -10;
          std::cout << "Warning: User Float ppdlBS3D was not found" << std::endl;
        }
        if (aBcCand->hasUserFloat("ppdlErrBS3D")) {
          Reco_3mu_ctauErr3D[Reco_3mu_size] = 10.0 * aBcCand->userFloat("ppdlErrBS3D");
        } else {
          Reco_3mu_ctauErr3D[Reco_3mu_size] = -10;
          std::cout << "Warning: User Float ppdlErrBS3D was not found" << std::endl;
        }
      } else {
        if (aBcCand->hasUserFloat("ppdlPV")) {
          Reco_3mu_ctau[Reco_3mu_size] = 10.0 * aBcCand->userFloat("ppdlPV");
        } else {
          Reco_3mu_ctau[Reco_3mu_size] = -10;
          std::cout << "Warning: User Float ppdlPV was not found" << std::endl;
        }
        if (aBcCand->hasUserFloat("ppdlErrPV")) {
          Reco_3mu_ctauErr[Reco_3mu_size] = 10.0 * aBcCand->userFloat("ppdlErrPV");
        } else {
          Reco_3mu_ctauErr[Reco_3mu_size] = -10;
          std::cout << "Warning: User Float ppdlErrPV was not found" << std::endl;
        }
        if (aBcCand->hasUserFloat("ppdlPV3D")) {
          Reco_3mu_ctau3D[Reco_3mu_size] = 10.0 * aBcCand->userFloat("ppdlPV3D");
        } else {
          Reco_3mu_ctau3D[Reco_3mu_size] = -10;
          std::cout << "Warning: User Float ppdlPV3D was not found" << std::endl;
        }
        if (aBcCand->hasUserFloat("ppdlErrPV3D")) {
          Reco_3mu_ctauErr3D[Reco_3mu_size] = 10.0 * aBcCand->userFloat("ppdlErrPV3D");
        } else {
          Reco_3mu_ctau3D[Reco_3mu_size] = -10;
          std::cout << "Warning: User Float ppdlErrPV3D was not found" << std::endl;
        }
        if (aBcCand->hasUserFloat("cosAlpha")) {
          Reco_3mu_cosAlpha[Reco_3mu_size] = aBcCand->userFloat("cosAlpha");
        } else {
          Reco_3mu_cosAlpha[Reco_3mu_size] = -10;
          std::cout << "Warning: User Float cosAlpha was not found" << std::endl;
        }
        if (aBcCand->hasUserFloat("cosAlpha3D")) {
          Reco_3mu_cosAlpha3D[Reco_3mu_size] = aBcCand->userFloat("cosAlpha3D");
        } else {
          Reco_3mu_cosAlpha3D[Reco_3mu_size] = -10;
          std::cout << "Warning: User Float cosAlpha3D was not found" << std::endl;
        }
      }

      if (aBcCand->hasUserFloat("vProb")) {
        Reco_3mu_VtxProb[Reco_3mu_size] = aBcCand->userFloat("vProb");
      } else {
        Reco_3mu_VtxProb[Reco_3mu_size] = -1;
        std::cout << "Warning: User Float vProb was not found" << std::endl;
      }
      if (aBcCand->hasUserFloat("MassErr")) {
        Reco_3mu_MassErr[Reco_3mu_size] = aBcCand->userFloat("MassErr");
      } else {
        Reco_3mu_MassErr[Reco_3mu_size] = -10;
        std::cout << "Warning: User Float MassErr was not found" << std::endl;
      }

      //Correct the Bc mass for the momentum of the neutrino, transverse to the Bc flight direction
      float Mtrimu = vBc.M();
      float Ptrimu = vBc.P();
      float sinalpha = sin(acos(Reco_3mu_cosAlpha3D[Reco_3mu_size]));
      float PperpTrimu = sinalpha * Ptrimu;
      Reco_3mu_CorrM[Reco_3mu_size] = sqrt(Mtrimu * Mtrimu + PperpTrimu * PperpTrimu) + PperpTrimu;

      if (_useSVfinder && SVs.isValid() && SVs->size() > 0) {
        Reco_3mu_NbMuInSameSV[Reco_3mu_size] = MuInSV(vMuon1, vMuon2, vMuon3);
      }

      Reco_3mu_size++;
    }
  }

  return;
}

void HiOniaAnalyzer::fillTreeDimuTrk(int count) {
  if (Reco_3mu_size >= Max_Bc_size) {
    std::cout << "Too many dimuon+track candidates: " << Reco_3mu_size << std::endl;
    std::cout << "Maximum allowed: " << Max_Bc_size << std::endl;
    return;
  }

  const pat::CompositeCandidate* aBcCand = _thePassedBcCands.at(count);

  if (aBcCand == NULL) {
    std::cout << "ERROR: 'aBcCand' pointer in fillTreeDimuTrk is NULL ! Return now" << std::endl;
    return;
  } else {
    const pat::Muon* muon1 = dynamic_cast<const pat::Muon*>(aBcCand->daughter("muon1"));
    const pat::Muon* muon2 = dynamic_cast<const pat::Muon*>(aBcCand->daughter("muon2"));
    const reco::RecoChargedCandidate* trk3 =
        dynamic_cast<const reco::RecoChargedCandidate*>(aBcCand->daughter("track"));

    if (muon1 == NULL || muon2 == NULL || trk3 == NULL) {
      std::cout << "ERROR: 'muon1' or 'muon2' or 'trk3' pointer in fillTreeDimuTrk is NULL ! Return now" << std::endl;
      return;
    } else {
      Reco_3mu_charge[Reco_3mu_size] = muon1->charge() + muon2->charge() + trk3->charge();

      TLorentzVector vMuon1 = lorentzMomentum(muon1->p4());
      TLorentzVector vMuon2 = lorentzMomentum(muon2->p4());
      TLorentzVector vTrk3 = lorentzMomentum(trk3->p4());

      int mu1_idx = IndexOfThisMuon(&vMuon1);
      int mu2_idx = IndexOfThisMuon(&vMuon2);
      int trk3_idx = IndexOfThisTrack(&vTrk3);

      //The dimuon has to pass the Jpsi kinematic cuts
      Reco_3mu_QQ1_idx[Reco_3mu_size] = IndexOfThisJpsi(mu1_idx, mu2_idx);
      if (Reco_3mu_QQ1_idx[Reco_3mu_size] == -1) {
        return;
      }

      TLorentzVector vBc = lorentzMomentum(aBcCand->p4());
      new ((*Reco_3mu_4mom)[Reco_3mu_size]) TLorentzVector(vBc);

      Reco_3mu_mumi_idx[Reco_3mu_size] = (Reco_mu_charge[mu1_idx] == -1) ? mu1_idx : mu2_idx;
      Reco_3mu_mupl_idx[Reco_3mu_size] = (Reco_mu_charge[mu1_idx] == -1) ? mu2_idx : mu1_idx;
      Reco_3mu_muW_idx[Reco_3mu_size] =
          trk3_idx;  //let's keep the same variables names as for the trimuon (for internal use...)

      //*********
      //Fill all remaining Bc variables
      if (!(_isHI) && _muonLessPrimaryVertex && aBcCand->hasUserData("muonlessPV")) {
        RefVtx = (*aBcCand->userData<reco::Vertex>("muonlessPV")).position();
        RefVtx_xError = (*aBcCand->userData<reco::Vertex>("muonlessPV")).xError();
        RefVtx_yError = (*aBcCand->userData<reco::Vertex>("muonlessPV")).yError();
        RefVtx_zError = (*aBcCand->userData<reco::Vertex>("muonlessPV")).zError();
      } else if (aBcCand->hasUserData("PVwithmuons")) {
        RefVtx = (*aBcCand->userData<reco::Vertex>("PVwithmuons")).position();
        RefVtx_xError = (*aBcCand->userData<reco::Vertex>("PVwithmuons")).xError();
        RefVtx_yError = (*aBcCand->userData<reco::Vertex>("PVwithmuons")).yError();
        RefVtx_zError = (*aBcCand->userData<reco::Vertex>("PVwithmuons")).zError();
      } else {
        cout << "HiOniaAnalyzer::fillTreeDimuTrk: no PVfor muon pair stored" << endl;
        return;
      }

      new ((*Reco_3mu_vtx)[Reco_3mu_size]) TVector3(RefVtx.X(), RefVtx.Y(), RefVtx.Z());

      Reco_3mu_muW_dxy[Reco_3mu_size] = trk3->track()->dxy(RefVtx);
      Reco_3mu_muW_dz[Reco_3mu_size] = trk3->track()->dz(RefVtx);

      //*********
      //Lifetime related variables

      if (_useBS) {
        if (aBcCand->hasUserFloat("ppdlBS")) {
          Reco_3mu_ctau[Reco_3mu_size] = 10.0 * aBcCand->userFloat("ppdlBS");
        } else {
          Reco_3mu_ctau[Reco_3mu_size] = -10;
          std::cout << "Warning: User Float ppdlBS was not found" << std::endl;
        }
        if (aBcCand->hasUserFloat("ppdlErrBS")) {
          Reco_3mu_ctauErr[Reco_3mu_size] = 10.0 * aBcCand->userFloat("ppdlErrBS");
        } else {
          Reco_3mu_ctauErr[Reco_3mu_size] = -10;
          std::cout << "Warning: User Float ppdlErrBS was not found" << std::endl;
        }
        if (aBcCand->hasUserFloat("ppdlBS3D")) {
          Reco_3mu_ctau3D[Reco_3mu_size] = 10.0 * aBcCand->userFloat("ppdlBS3D");
        } else {
          Reco_3mu_ctau3D[Reco_3mu_size] = -10;
          std::cout << "Warning: User Float ppdlBS3D was not found" << std::endl;
        }
        if (aBcCand->hasUserFloat("ppdlErrBS3D")) {
          Reco_3mu_ctauErr3D[Reco_3mu_size] = 10.0 * aBcCand->userFloat("ppdlErrBS3D");
        } else {
          Reco_3mu_ctauErr3D[Reco_3mu_size] = -10;
          std::cout << "Warning: User Float ppdlErrBS3D was not found" << std::endl;
        }
      } else {
        if (aBcCand->hasUserFloat("ppdlPV")) {
          Reco_3mu_ctau[Reco_3mu_size] = 10.0 * aBcCand->userFloat("ppdlPV");
        } else {
          Reco_3mu_ctau[Reco_3mu_size] = -10;
          std::cout << "Warning: User Float ppdlPV was not found" << std::endl;
        }
        if (aBcCand->hasUserFloat("ppdlErrPV")) {
          Reco_3mu_ctauErr[Reco_3mu_size] = 10.0 * aBcCand->userFloat("ppdlErrPV");
        } else {
          Reco_3mu_ctauErr[Reco_3mu_size] = -10;
          std::cout << "Warning: User Float ppdlErrPV was not found" << std::endl;
        }
        if (aBcCand->hasUserFloat("ppdlPV3D")) {
          Reco_3mu_ctau3D[Reco_3mu_size] = 10.0 * aBcCand->userFloat("ppdlPV3D");
        } else {
          Reco_3mu_ctau3D[Reco_3mu_size] = -10;
          std::cout << "Warning: User Float ppdlPV3D was not found" << std::endl;
        }
        if (aBcCand->hasUserFloat("ppdlErrPV3D")) {
          Reco_3mu_ctauErr3D[Reco_3mu_size] = 10.0 * aBcCand->userFloat("ppdlErrPV3D");
        } else {
          Reco_3mu_ctau3D[Reco_3mu_size] = -10;
          std::cout << "Warning: User Float ppdlErrPV3D was not found" << std::endl;
        }
        if (aBcCand->hasUserFloat("cosAlpha")) {
          Reco_3mu_cosAlpha[Reco_3mu_size] = aBcCand->userFloat("cosAlpha");
        } else {
          Reco_3mu_cosAlpha[Reco_3mu_size] = -10;
          std::cout << "Warning: User Float cosAlpha was not found" << std::endl;
        }
        if (aBcCand->hasUserFloat("cosAlpha3D")) {
          Reco_3mu_cosAlpha3D[Reco_3mu_size] = aBcCand->userFloat("cosAlpha3D");
        } else {
          Reco_3mu_cosAlpha3D[Reco_3mu_size] = -10;
          std::cout << "Warning: User Float cosAlpha3D was not found" << std::endl;
        }

        if (_doDimuTrk) {
          if (aBcCand->hasUserFloat("KCppdlPV")) {
            Reco_3mu_KCctau[Reco_3mu_size] = 10.0 * aBcCand->userFloat("KCppdlPV");
          } else {
            Reco_3mu_KCctau[Reco_3mu_size] = -10;
            std::cout << "Warning: User Float KCppdlPV was not found" << std::endl;
          }
          if (aBcCand->hasUserFloat("KCppdlErrPV")) {
            Reco_3mu_KCctauErr[Reco_3mu_size] = 10.0 * aBcCand->userFloat("KCppdlErrPV");
          } else {
            Reco_3mu_KCctauErr[Reco_3mu_size] = -10;
            std::cout << "Warning: User Float KCppdlErrPV was not found" << std::endl;
          }
          if (aBcCand->hasUserFloat("KCppdlPV3D")) {
            Reco_3mu_KCctau3D[Reco_3mu_size] = 10.0 * aBcCand->userFloat("KCppdlPV3D");
          } else {
            Reco_3mu_KCctau3D[Reco_3mu_size] = -10;
            std::cout << "Warning: User Float KCppdlPV3D was not found" << std::endl;
          }
          if (aBcCand->hasUserFloat("KCppdlErrPV3D")) {
            Reco_3mu_KCctauErr3D[Reco_3mu_size] = 10.0 * aBcCand->userFloat("KCppdlErrPV3D");
          } else {
            Reco_3mu_KCctau3D[Reco_3mu_size] = -10;
            std::cout << "Warning: User Float KCppdlErrPV3D was not found" << std::endl;
          }
          if (aBcCand->hasUserFloat("KCcosAlpha")) {
            Reco_3mu_KCcosAlpha[Reco_3mu_size] = aBcCand->userFloat("KCcosAlpha");
          } else {
            Reco_3mu_KCcosAlpha[Reco_3mu_size] = -10;
            std::cout << "Warning: User Float KCcosAlpha was not found" << std::endl;
          }
          if (aBcCand->hasUserFloat("KCcosAlpha3D")) {
            Reco_3mu_KCcosAlpha3D[Reco_3mu_size] = aBcCand->userFloat("KCcosAlpha3D");
          } else {
            Reco_3mu_KCcosAlpha3D[Reco_3mu_size] = -10;
            std::cout << "Warning: User Float KCcosAlpha3D was not found" << std::endl;
          }
        }
      }

      if (aBcCand->hasUserFloat("vProb")) {
        Reco_3mu_VtxProb[Reco_3mu_size] = aBcCand->userFloat("vProb");
      } else {
        Reco_3mu_VtxProb[Reco_3mu_size] = -1;
        std::cout << "Warning: User Float vProb was not found" << std::endl;
      }
      if (aBcCand->hasUserFloat("KinConstrainedVtxProb")) {
        Reco_3mu_KCVtxProb[Reco_3mu_size] = aBcCand->userFloat("KinConstrainedVtxProb");
      } else {
        Reco_3mu_KCVtxProb[Reco_3mu_size] = -10;
        std::cout << "Warning: User Float KinConstrainedVtxProb was not found" << std::endl;
      }
      if (aBcCand->hasUserFloat("MassErr")) {
        Reco_3mu_MassErr[Reco_3mu_size] = aBcCand->userFloat("MassErr");
      } else {
        Reco_3mu_MassErr[Reco_3mu_size] = -10;
        std::cout << "Warning: User Float MassErr was not found" << std::endl;
      }

      //Correct the Bc mass for the momentum of the neutrino, transverse to the Bc flight direction
      float Mtrimu = vBc.M();
      float Ptrimu = vBc.P();
      float sinalpha = sin(acos(Reco_3mu_cosAlpha3D[Reco_3mu_size]));
      float PperpTrimu = sinalpha * Ptrimu;
      Reco_3mu_CorrM[Reco_3mu_size] = sqrt(Mtrimu * Mtrimu + PperpTrimu * PperpTrimu) + PperpTrimu;

      if (_useSVfinder && SVs.isValid() && SVs->size() > 0) {
        Reco_3mu_NbMuInSameSV[Reco_3mu_size] = MuInSV(vMuon1, vMuon2, vTrk3);
      }

      Reco_3mu_size++;
    }
  }

  return;
}

void HiOniaAnalyzer::fillRecoJpsi(int count, std::string trigName, std::string centName) {
  pat::CompositeCandidate* aJpsiCand = _thePassedCands.at(count)->clone();

  if (aJpsiCand == NULL) {
    std::cout << "ERROR: 'aJpsiCand' pointer in fillTreeJpsi is NULL ! Return now" << std::endl;
    return;
  } else {
    aJpsiCand->addUserInt("centBin", centBin);
    const pat::Muon* muon1 = dynamic_cast<const pat::Muon*>(aJpsiCand->daughter("muon1"));
    const pat::Muon* muon2 = dynamic_cast<const pat::Muon*>(aJpsiCand->daughter("muon2"));

    if (muon1 == NULL || muon2 == NULL) {
      std::cout << "ERROR: 'muon1' or 'muon2' pointer in fillTreeJpsi is NULL ! Return now" << std::endl;
      return;
    } else {
      int iSign = muon1->charge() + muon2->charge();
      if (iSign != 0) {
        (iSign == 2) ? (iSign = 1) : (iSign = 2);
      }

      std::string theLabel = trigName + "_" + centName + "_" + theSign.at(iSign);

      bool isBarrel = false;
      if (fabs(aJpsiCand->rapidity()) < 1.2)
        isBarrel = true;

      if (iSign == 0 && aJpsiCand->mass() >= JpsiMassMin && aJpsiCand->mass() < JpsiMassMax &&
          aJpsiCand->pt() >= JpsiPtMin && aJpsiCand->pt() < JpsiPtMax && fabs(aJpsiCand->rapidity()) >= JpsiRapMin &&
          fabs(aJpsiCand->rapidity()) < JpsiRapMax) {
        passedCandidates++;
      }

      if (_fillHistos) {
        if (_combineCategories &&
            _thePassedCats.at(count) <= Trk_Trk) {  // for the moment consider Glb+Glb, GlbTrk+GlbTrk, Trk+Trk
          myRecoJpsiHistos->Fill(aJpsiCand, "All_" + theLabel);
          if (isBarrel)
            myRecoJpsiHistos->Fill(aJpsiCand, "Barrel_" + theLabel);
          else
            myRecoJpsiHistos->Fill(aJpsiCand, "EndCap_" + theLabel);
        } else {
          switch (_thePassedCats.at(count)) {
            case Glb_Glb:
              myRecoJpsiGlbGlbHistos->Fill(aJpsiCand, "All_" + theLabel);
              if (isBarrel)
                myRecoJpsiGlbGlbHistos->Fill(aJpsiCand, "Barrel_" + theLabel);
              else
                myRecoJpsiGlbGlbHistos->Fill(aJpsiCand, "EndCap_" + theLabel);
              break;
            case GlbTrk_GlbTrk:
              myRecoJpsiGlbTrkHistos->Fill(aJpsiCand, "All_" + theLabel);
              if (isBarrel)
                myRecoJpsiGlbTrkHistos->Fill(aJpsiCand, "Barrel_" + theLabel);
              else
                myRecoJpsiGlbTrkHistos->Fill(aJpsiCand, "EndCap_" + theLabel);
              break;
            case Trk_Trk:
              myRecoJpsiTrkTrkHistos->Fill(aJpsiCand, "All_" + theLabel);
              if (isBarrel)
                myRecoJpsiTrkTrkHistos->Fill(aJpsiCand, "Barrel_" + theLabel);
              else
                myRecoJpsiTrkTrkHistos->Fill(aJpsiCand, "EndCap_" + theLabel);
              break;
            default:
              break;
          }
        }
      }
    }
  }
  this->fillHistosAndDS(_thePassedCats.at(count), aJpsiCand);

  delete aJpsiCand;
  return;
}

void HiOniaAnalyzer::fillHistosAndDS(unsigned int theCat, const pat::CompositeCandidate* aJpsiCand) { return; }

void HiOniaAnalyzer::checkTriggers(const pat::CompositeCandidate* aJpsiCand) {
  if (aJpsiCand == NULL) {
    std::cout << "ERROR: 'aJpsiCand' pointer in checkTriggers is NULL ! Return now" << std::endl;
    return;
  } else {
    const pat::Muon* muon1 = dynamic_cast<const pat::Muon*>(aJpsiCand->daughter("muon1"));
    const pat::Muon* muon2 = dynamic_cast<const pat::Muon*>(aJpsiCand->daughter("muon2"));

    if (muon1 == NULL || muon2 == NULL) {
      std::cout << "ERROR: 'muon1' or 'muon2' pointer in checkTriggers is NULL ! Return now" << std::endl;
      return;
    } else {
      // Trigger passed
      for (unsigned int iTr = 1; iTr < NTRIGGERS; ++iTr) {
        const auto& mu1HLTMatchesFilter = muon1->triggerObjectMatchesByFilter(HLTLastFilters[iTr]);
        const auto& mu2HLTMatchesFilter = muon2->triggerObjectMatchesByFilter(HLTLastFilters[iTr]);

        // const pat::TriggerObjectStandAloneCollection mu1HLTMatchesPath = muon1->triggerObjectMatchesByPath( theTriggerNames.at(iTr), true, false );
        // const pat::TriggerObjectStandAloneCollection mu2HLTMatchesPath = muon2->triggerObjectMatchesByPath( theTriggerNames.at(iTr), true, false );

        bool pass1 = false;
        bool pass2 = false;

        pass1 = mu1HLTMatchesFilter.size() > 0;
        pass2 = mu2HLTMatchesFilter.size() > 0;

        //    pass1 = mu1HLTMatchesPath.size() > 0;
        //    pass2 = mu2HLTMatchesPath.size() > 0;

        if (iTr > NTRIGGERS_DBL) {  // single triggers here
          isTriggerMatched[iTr] = pass1 || pass2;
        } else {  // double triggers here
          isTriggerMatched[iTr] = pass1 && pass2;
        }
      }

      for (unsigned int iTr = 1; iTr < NTRIGGERS; ++iTr) {
        if (isTriggerMatched[iTr]) {
          // since we have bins for event info, let's try to fill here the trigger info for each pair
          // also if there are several pairs matched to the same kind of trigger
          hStats->Fill(iTr + NTRIGGERS);  // pair info
        }
      }
    }
  }

  return;
}

void HiOniaAnalyzer::makeCuts(bool keepSameSign) {
  math::XYZPoint RefVtx_tmp = RefVtx;

  if (collJpsi.isValid()) {
    for (std::vector<pat::CompositeCandidate>::const_iterator it = collJpsi->begin(); it != collJpsi->end(); ++it) {
      const pat::CompositeCandidate* cand = &(*it);

      if (cand == NULL) {
        std::cout << "ERROR: 'cand' pointer in makeCuts is NULL ! Return now" << std::endl;
        return;
      } else {
        const pat::Muon* muon1 = dynamic_cast<const pat::Muon*>(cand->daughter("muon1"));
        const pat::Muon* muon2 = dynamic_cast<const pat::Muon*>(cand->daughter("muon2"));

        if (muon1 == NULL || muon2 == NULL) {
          std::cout << "ERROR: 'muon1' or 'muon2' pointer in makeCuts is NULL ! Return now" << std::endl;
          return;
        } else {
          if (!keepSameSign && (muon1->charge() + muon2->charge() != 0))
            continue;

          if (!(_isHI) && _muonLessPrimaryVertex && cand->hasUserData("muonlessPV"))
            RefVtx = (*cand->userData<reco::Vertex>("muonlessPV")).position();
          else if (!_muonLessPrimaryVertex && cand->hasUserData("PVwithmuons"))
            RefVtx = (*cand->userData<reco::Vertex>("PVwithmuons")).position();
          else {
            std::cout << "HiOniaAnalyzer::makeCuts: no PV for muon pair stored ! Go to next candidate." << std::endl;
            continue;
          }

          if (fabs(RefVtx.Z()) > _iConfig.getParameter<double>("maxAbsZ"))
            continue;

          if (fabs(muon1->eta()) >= etaMax || fabs(muon2->eta()) >= etaMax)
            continue;

          //Pass muon selection?
          bool muonSelFound = false;
          if (_muonSel == (std::string)("Glb")) {
            if (checkCuts(cand, muon1, muon2, &HiOniaAnalyzer::selGlobalMuon, &HiOniaAnalyzer::selGlobalMuon)) {
              _thePassedCats.push_back(Glb_Glb);
              _thePassedCands.push_back(cand);
              if (!_fillSingleMuons) {
                EtaOfWantedMuons.push_back(muon1->eta());
                EtaOfWantedMuons.push_back(muon2->eta());
              }
            }
            muonSelFound = true;
          }
          if (_muonSel == (std::string)("TwoGlbAmongThree")) {
            if (checkCuts(cand, muon1, muon2, &HiOniaAnalyzer::selGlobalMuon, &HiOniaAnalyzer::selTrackerMuon)) {
              _thePassedCats.push_back(TwoGlbAmongThree);
              _thePassedCands.push_back(cand);
              if (!_fillSingleMuons) {
                EtaOfWantedMuons.push_back(muon1->eta());
                EtaOfWantedMuons.push_back(muon2->eta());
              }
            }
            muonSelFound = true;
          }
          if (_muonSel == (std::string)("GlbTrk")) {
            if (checkCuts(cand, muon1, muon2, &HiOniaAnalyzer::selGlobalMuon, &HiOniaAnalyzer::selGlobalMuon)) {
              _thePassedCats.push_back(GlbTrk_GlbTrk);
              _thePassedCands.push_back(cand);
              if (!_fillSingleMuons) {
                EtaOfWantedMuons.push_back(muon1->eta());
                EtaOfWantedMuons.push_back(muon2->eta());
              }
            }
            muonSelFound = true;
          }
          if (_muonSel == (std::string)("Trk")) {
            if (checkCuts(cand, muon1, muon2, &HiOniaAnalyzer::selTrackerMuon, &HiOniaAnalyzer::selTrackerMuon)) {
              _thePassedCats.push_back(Trk_Trk);
              _thePassedCands.push_back(cand);
              if (!_fillSingleMuons) {
                EtaOfWantedMuons.push_back(muon1->eta());
                EtaOfWantedMuons.push_back(muon2->eta());
              }
            }
            muonSelFound = true;
          }
          if (_muonSel == (std::string)("GlbOrTrk")) {
            if (checkCuts(cand,
                          muon1,
                          muon2,
                          &HiOniaAnalyzer::selGlobalOrTrackerMuon,
                          &HiOniaAnalyzer::selGlobalOrTrackerMuon)) {
              _thePassedCats.push_back(GlbOrTrk_GlbOrTrk);
              _thePassedCands.push_back(cand);
              if (!_fillSingleMuons) {
                EtaOfWantedMuons.push_back(muon1->eta());
                EtaOfWantedMuons.push_back(muon2->eta());
              }
            }
            muonSelFound = true;
          }
          if (_muonSel == (std::string)("All")) {
            if (checkCuts(cand, muon1, muon2, &HiOniaAnalyzer::selAllMuon, &HiOniaAnalyzer::selAllMuon)) {
              _thePassedCats.push_back(All_All);
              _thePassedCands.push_back(cand);
              if (!_fillSingleMuons) {
                EtaOfWantedMuons.push_back(muon1->eta());
                EtaOfWantedMuons.push_back(muon2->eta());
              }
            }
            muonSelFound = true;
          }
          if (!muonSelFound) {
            std::cout << "[HiOniaAnalyzer::makeCuts] --- The muon selection: " << _muonSel
                      << " is invalid. The supported options are: All, Glb, GlbTrk, GlbOrTrk, Trk, and TwoGlbAmongThree"
                      << std::endl;
          }
        }
      }
    }
  }
  RefVtx = RefVtx_tmp;
  return;
}

void HiOniaAnalyzer::makeBcCuts(bool keepWrongSign) {
  math::XYZPoint RefVtx_tmp = RefVtx;

  if (collTrimuon.isValid()) {
    for (std::vector<pat::CompositeCandidate>::const_iterator it = collTrimuon->begin(); it != collTrimuon->end();
         ++it) {
      const pat::CompositeCandidate* cand = &(*it);

      if (cand == NULL) {
        std::cout << "ERROR: 'cand' pointer in makeBcCuts is NULL ! Return now" << std::endl;
        return;
      } else {
        const pat::Muon* muon1 = dynamic_cast<const pat::Muon*>(cand->daughter("muon1"));
        const pat::Muon* muon2 = dynamic_cast<const pat::Muon*>(cand->daughter("muon2"));
        const pat::Muon* muon3 = dynamic_cast<const pat::Muon*>(cand->daughter("muon3"));

        if (muon1 == NULL || muon2 == NULL || muon3 == NULL) {
          std::cout << "ERROR: 'muon1' or 'muon2' or 'muon3' pointer in makeBcCuts is NULL ! Return now" << std::endl;
          return;
        } else {
          if (!keepWrongSign && (muon1->charge() + muon2->charge() + muon3->charge() != 1) &&
              (muon1->charge() + muon2->charge() + muon3->charge() != -1))
            continue;

          if (!(_isHI) && _muonLessPrimaryVertex && cand->hasUserData("muonlessPV"))
            RefVtx = (*cand->userData<reco::Vertex>("muonlessPV")).position();
          else if (!_muonLessPrimaryVertex && cand->hasUserData("PVwithmuons"))
            RefVtx = (*cand->userData<reco::Vertex>("PVwithmuons")).position();
          else {
            std::cout << "HiOniaAnalyzer::makeCuts: no PV for muon pair stored ! Go to next candidate." << std::endl;
            continue;
          }

          if (fabs(RefVtx.Z()) > _iConfig.getParameter<double>("maxAbsZ"))
            continue;

          if (fabs(muon1->eta()) >= etaMax || fabs(muon2->eta()) >= etaMax || fabs(muon3->eta()) >= etaMax)
            continue;

          //Pass muon selection?
          if ((_muonSel == (std::string)("GlbOrTrk")) && checkBcCuts(cand,
                                                                     muon1,
                                                                     muon2,
                                                                     muon3,
                                                                     &HiOniaAnalyzer::selGlobalOrTrackerMuon,
                                                                     &HiOniaAnalyzer::selGlobalOrTrackerMuon,
                                                                     &HiOniaAnalyzer::selGlobalOrTrackerMuon)) {
            _thePassedBcCats.push_back(Glb_Glb);
            _thePassedBcCands.push_back(cand);
            if (!_fillSingleMuons) {
              EtaOfWantedMuons.push_back(muon1->eta());
              EtaOfWantedMuons.push_back(muon2->eta());
              EtaOfWantedMuons.push_back(muon3->eta());
            }
          } else if ((_muonSel ==
                      (std::string)(
                          "TwoGlbAmongThree")) &&  //argument functions 2 and 3 have to be the same for good symmetrization
                     checkBcCuts(cand,
                                 muon1,
                                 muon2,
                                 muon3,
                                 &HiOniaAnalyzer::selTrackerMuon,
                                 &HiOniaAnalyzer::selGlobalMuon,
                                 &HiOniaAnalyzer::selGlobalMuon)) {
            _thePassedBcCats.push_back(TwoGlbAmongThree);
            _thePassedBcCands.push_back(cand);
            if (!_fillSingleMuons) {
              EtaOfWantedMuons.push_back(muon1->eta());
              EtaOfWantedMuons.push_back(muon2->eta());
              EtaOfWantedMuons.push_back(muon3->eta());
            }
          } else if ((_muonSel == (std::string)("Glb")) && checkBcCuts(cand,
                                                                       muon1,
                                                                       muon2,
                                                                       muon3,
                                                                       &HiOniaAnalyzer::selGlobalMuon,
                                                                       &HiOniaAnalyzer::selGlobalMuon,
                                                                       &HiOniaAnalyzer::selGlobalMuon)) {
            _thePassedBcCats.push_back(Glb_Glb);
            _thePassedBcCands.push_back(cand);
            if (!_fillSingleMuons) {
              EtaOfWantedMuons.push_back(muon1->eta());
              EtaOfWantedMuons.push_back(muon2->eta());
              EtaOfWantedMuons.push_back(muon3->eta());
            }
          } else if ((_muonSel == (std::string)("GlbTrk")) && checkBcCuts(cand,
                                                                          muon1,
                                                                          muon2,
                                                                          muon3,
                                                                          &HiOniaAnalyzer::selGlobalMuon,
                                                                          &HiOniaAnalyzer::selGlobalMuon,
                                                                          &HiOniaAnalyzer::selGlobalMuon)) {
            _thePassedBcCats.push_back(GlbTrk_GlbTrk);
            _thePassedBcCands.push_back(cand);
            if (!_fillSingleMuons) {
              EtaOfWantedMuons.push_back(muon1->eta());
              EtaOfWantedMuons.push_back(muon2->eta());
              EtaOfWantedMuons.push_back(muon3->eta());
            }
          } else if ((_muonSel == (std::string)("Trk")) && checkBcCuts(cand,
                                                                       muon1,
                                                                       muon2,
                                                                       muon3,
                                                                       &HiOniaAnalyzer::selTrackerMuon,
                                                                       &HiOniaAnalyzer::selTrackerMuon,
                                                                       &HiOniaAnalyzer::selTrackerMuon)) {
            _thePassedBcCats.push_back(Trk_Trk);
            _thePassedBcCands.push_back(cand);
            if (!_fillSingleMuons) {
              EtaOfWantedMuons.push_back(muon1->eta());
              EtaOfWantedMuons.push_back(muon2->eta());
              EtaOfWantedMuons.push_back(muon3->eta());
            }
          } else if ((_muonSel == (std::string)("All")) && checkBcCuts(cand,
                                                                       muon1,
                                                                       muon2,
                                                                       muon3,
                                                                       &HiOniaAnalyzer::selAllMuon,
                                                                       &HiOniaAnalyzer::selAllMuon,
                                                                       &HiOniaAnalyzer::selAllMuon)) {
            _thePassedBcCats.push_back(All_All);
            _thePassedBcCands.push_back(cand);
            if (!_fillSingleMuons) {
              EtaOfWantedMuons.push_back(muon1->eta());
              EtaOfWantedMuons.push_back(muon2->eta());
              EtaOfWantedMuons.push_back(muon3->eta());
            }
          } else {
            //std::cout << "[HiOniaAnalyzer::makeCuts] trimuon --- muon did not pass selection: " << _muonSel << std::endl;
          }
        }
      }
    }
  }

  RefVtx = RefVtx_tmp;
  return;
}

void HiOniaAnalyzer::makeDimutrkCuts(bool keepWrongSign) {
  math::XYZPoint RefVtx_tmp = RefVtx;

  if (collDimutrk.isValid()) {
    for (std::vector<pat::CompositeCandidate>::const_iterator it = collDimutrk->begin(); it != collDimutrk->end();
         ++it) {
      const pat::CompositeCandidate* cand = &(*it);

      if (cand == NULL) {
        std::cout << "ERROR: 'cand' pointer in makeDimutrkCuts is NULL ! Return now" << std::endl;
        return;
      } else {
        const pat::Muon* muon1 = dynamic_cast<const pat::Muon*>(cand->daughter("muon1"));
        const pat::Muon* muon2 = dynamic_cast<const pat::Muon*>(cand->daughter("muon2"));
        const reco::RecoChargedCandidate* trk =
            dynamic_cast<const reco::RecoChargedCandidate*>(cand->daughter("track"));

        if (muon1 == NULL || muon2 == NULL || trk == NULL) {
          std::cout << "ERROR: 'muon1' or 'muon2' or 'trk' pointer in makeDimutrkCuts is NULL ! Return now"
                    << std::endl;
          return;
        } else {
          if (!keepWrongSign && (muon1->charge() + muon2->charge() + trk->charge() != 1) &&
              (muon1->charge() + muon2->charge() + trk->charge() != -1))
            continue;

          if (!(_isHI) && _muonLessPrimaryVertex && cand->hasUserData("muonlessPV"))
            RefVtx = (*cand->userData<reco::Vertex>("muonlessPV")).position();
          else if (!_muonLessPrimaryVertex && cand->hasUserData("PVwithmuons"))
            RefVtx = (*cand->userData<reco::Vertex>("PVwithmuons")).position();
          else {
            std::cout << "HiOniaAnalyzer::makeCuts: no PV for muon pair stored ! Go to next candidate." << std::endl;
            continue;
          }

          if (fabs(RefVtx.Z()) > _iConfig.getParameter<double>("maxAbsZ"))
            continue;

          if (fabs(muon1->eta()) >= etaMax || fabs(muon2->eta()) >= etaMax || fabs(trk->eta()) >= etaMax)
            continue;

          //Pass muon selection?
          if ((_muonSel == (std::string)("GlbOrTrk")) && checkDimuTrkCuts(cand,
                                                                          muon1,
                                                                          muon2,
                                                                          trk,
                                                                          &HiOniaAnalyzer::selGlobalOrTrackerMuon,
                                                                          &HiOniaAnalyzer::selGlobalOrTrackerMuon,
                                                                          &HiOniaAnalyzer::selTrk)) {
            _thePassedBcCats.push_back(Glb_Glb);
            _thePassedBcCands.push_back(cand);
            if (!_fillSingleMuons) {
              EtaOfWantedMuons.push_back(muon1->eta());
              EtaOfWantedMuons.push_back(muon2->eta());
            }
            EtaOfWantedTracks.push_back(trk->eta());
          } else if ((_muonSel ==
                      (std::string)(
                          "TwoGlbAmongThree")) &&  //argument functions 2 and 3 have to be the same for good symmetrization
                     checkDimuTrkCuts(cand,
                                      muon1,
                                      muon2,
                                      trk,
                                      &HiOniaAnalyzer::selTrackerMuon,
                                      &HiOniaAnalyzer::selGlobalMuon,
                                      &HiOniaAnalyzer::selTrk)) {
            _thePassedBcCats.push_back(TwoGlbAmongThree);
            _thePassedBcCands.push_back(cand);
            if (!_fillSingleMuons) {
              EtaOfWantedMuons.push_back(muon1->eta());
              EtaOfWantedMuons.push_back(muon2->eta());
            }
            EtaOfWantedTracks.push_back(trk->eta());
          } else if ((_muonSel == (std::string)("Glb")) && checkDimuTrkCuts(cand,
                                                                            muon1,
                                                                            muon2,
                                                                            trk,
                                                                            &HiOniaAnalyzer::selGlobalMuon,
                                                                            &HiOniaAnalyzer::selGlobalMuon,
                                                                            &HiOniaAnalyzer::selTrk)) {
            _thePassedBcCats.push_back(Glb_Glb);
            _thePassedBcCands.push_back(cand);
            if (!_fillSingleMuons) {
              EtaOfWantedMuons.push_back(muon1->eta());
              EtaOfWantedMuons.push_back(muon2->eta());
            }
            EtaOfWantedTracks.push_back(trk->eta());
          } else if ((_muonSel == (std::string)("GlbTrk")) && checkDimuTrkCuts(cand,
                                                                               muon1,
                                                                               muon2,
                                                                               trk,
                                                                               &HiOniaAnalyzer::selGlobalMuon,
                                                                               &HiOniaAnalyzer::selGlobalMuon,
                                                                               &HiOniaAnalyzer::selTrk)) {
            _thePassedBcCats.push_back(GlbTrk_GlbTrk);
            _thePassedBcCands.push_back(cand);
            if (!_fillSingleMuons) {
              EtaOfWantedMuons.push_back(muon1->eta());
              EtaOfWantedMuons.push_back(muon2->eta());
            }
            EtaOfWantedTracks.push_back(trk->eta());
          } else if ((_muonSel == (std::string)("Trk")) && checkDimuTrkCuts(cand,
                                                                            muon1,
                                                                            muon2,
                                                                            trk,
                                                                            &HiOniaAnalyzer::selTrackerMuon,
                                                                            &HiOniaAnalyzer::selTrackerMuon,
                                                                            &HiOniaAnalyzer::selTrk)) {
            _thePassedBcCats.push_back(Trk_Trk);
            _thePassedBcCands.push_back(cand);
            if (!_fillSingleMuons) {
              EtaOfWantedMuons.push_back(muon1->eta());
              EtaOfWantedMuons.push_back(muon2->eta());
            }
            EtaOfWantedTracks.push_back(trk->eta());
          } else if ((_muonSel == (std::string)("All")) && checkDimuTrkCuts(cand,
                                                                            muon1,
                                                                            muon2,
                                                                            trk,
                                                                            &HiOniaAnalyzer::selAllMuon,
                                                                            &HiOniaAnalyzer::selAllMuon,
                                                                            &HiOniaAnalyzer::selTrk)) {
            _thePassedBcCats.push_back(All_All);
            _thePassedBcCands.push_back(cand);
            if (!_fillSingleMuons) {
              EtaOfWantedMuons.push_back(muon1->eta());
              EtaOfWantedMuons.push_back(muon2->eta());
            }
            EtaOfWantedTracks.push_back(trk->eta());
          } else {
            //std::cout << "[HiOniaAnalyzer::makeCuts] trimuon --- muon did not pass selection: " << _muonSel << std::endl;
          }
        }
      }
    }
  }

  RefVtx = RefVtx_tmp;
  return;
}

bool HiOniaAnalyzer::checkCuts(const pat::CompositeCandidate* cand,
                               const pat::Muon* muon1,
                               const pat::Muon* muon2,
                               bool (HiOniaAnalyzer::*callFunc1)(const pat::Muon*),
                               bool (HiOniaAnalyzer::*callFunc2)(const pat::Muon*)) {
  if ((((this->*callFunc1)(muon1) && (this->*callFunc2)(muon2)) ||
       ((this->*callFunc1)(muon2) && (this->*callFunc2)(muon1))) &&
      (!_applycuts || true) &&  //Add hard-coded cuts here if desired
      ((_OneMatchedHLTMu == -1) || (muon1->triggerObjectMatchesByFilter(HLTLastFilters[_OneMatchedHLTMu])).size() > 0 ||
       (muon2->triggerObjectMatchesByFilter(HLTLastFilters[_OneMatchedHLTMu])).size() > 0))
    return true;
  else
    return false;
}

bool HiOniaAnalyzer::checkBcCuts(const pat::CompositeCandidate* cand,
                                 const pat::Muon* muon1,
                                 const pat::Muon* muon2,
                                 const pat::Muon* muon3,
                                 bool (HiOniaAnalyzer::*callFunc1)(const pat::Muon*),
                                 bool (HiOniaAnalyzer::*callFunc2)(const pat::Muon*),
                                 bool (HiOniaAnalyzer::*callFunc3)(const pat::Muon*)) {
  const auto& mu1HLTMatchesFilter =
      muon1->triggerObjectMatchesByFilter(HLTLastFilters[(_OneMatchedHLTMu < 0) ? 0 : _OneMatchedHLTMu]);
  const auto& mu2HLTMatchesFilter =
      muon2->triggerObjectMatchesByFilter(HLTLastFilters[(_OneMatchedHLTMu < 0) ? 0 : _OneMatchedHLTMu]);
  const auto& mu3HLTMatchesFilter =
      muon3->triggerObjectMatchesByFilter(HLTLastFilters[(_OneMatchedHLTMu < 0) ? 0 : _OneMatchedHLTMu]);

  if ((((this->*callFunc1)(muon1) && (this->*callFunc2)(muon2) && (this->*callFunc3)(muon3))
       //symmetrize, assuming arguments functions 2 and 3 are THE SAME !
       || ((this->*callFunc1)(muon2) && (this->*callFunc2)(muon1) && (this->*callFunc3)(muon3)) ||
       ((this->*callFunc1)(muon3) && (this->*callFunc2)(muon1) && (this->*callFunc3)(muon2))) &&
      (!_applycuts || true) &&  //Add hard-coded cuts here if desired
      ((_OneMatchedHLTMu == -1) || (mu1HLTMatchesFilter.size() > 0 && mu2HLTMatchesFilter.size() > 0) ||
       (mu1HLTMatchesFilter.size() > 0 && mu3HLTMatchesFilter.size() > 0) ||
       (mu2HLTMatchesFilter.size() > 0 && mu3HLTMatchesFilter.size() > 0)))
    return true;
  else
    return false;
}

bool HiOniaAnalyzer::checkDimuTrkCuts(const pat::CompositeCandidate* cand,
                                      const pat::Muon* muon1,
                                      const pat::Muon* muon2,
                                      const reco::RecoChargedCandidate* trk,
                                      bool (HiOniaAnalyzer::*callFunc1)(const pat::Muon*),
                                      bool (HiOniaAnalyzer::*callFunc2)(const pat::Muon*),
                                      bool (HiOniaAnalyzer::*callFunc3)(const reco::TrackRef)) {
  const auto& mu1HLTMatchesFilter =
      muon1->triggerObjectMatchesByFilter(HLTLastFilters[(_OneMatchedHLTMu < 0) ? 0 : _OneMatchedHLTMu]);
  const auto& mu2HLTMatchesFilter =
      muon2->triggerObjectMatchesByFilter(HLTLastFilters[(_OneMatchedHLTMu < 0) ? 0 : _OneMatchedHLTMu]);

  if ((((this->*callFunc1)(muon1) && (this->*callFunc2)(muon2) && (this->*callFunc3)(trk->track())) ||
       ((this->*callFunc1)(muon2) && (this->*callFunc2)(muon1) && (this->*callFunc3)(trk->track()))) &&
      (!_applycuts || true) &&  //Add hard-coded cuts here if desired
      ((_OneMatchedHLTMu == -1) || (mu1HLTMatchesFilter.size() > 0 && mu2HLTMatchesFilter.size() > 0)))
    return true;
  else
    return false;
}

pair<unsigned int, const pat::CompositeCandidate*> HiOniaAnalyzer::theBestQQ() {
  unsigned int theBestCat = 99;
  const pat::CompositeCandidate* theBestCand = new pat::CompositeCandidate();

  for (unsigned int i = 0; i < _thePassedCands.size(); i++) {
    if (_thePassedCats.at(i) < theBestCat) {
      theBestCat = _thePassedCats.at(i);
      theBestCand = _thePassedCands.at(i);
    }
  }

  pair<unsigned int, const pat::CompositeCandidate*> result = make_pair(theBestCat, theBestCand);
  return result;
}

bool HiOniaAnalyzer::isTrkInMuonAccept(TLorentzVector trk4mom, std::string muonType) {
  if (muonType == (std::string)("GLB")) {
    return (fabs(trk4mom.Eta()) < 2.4 && ((fabs(trk4mom.Eta()) < 1.2 && trk4mom.Pt() >= 3.5) ||
                                          (1.2 <= fabs(trk4mom.Eta()) && fabs(trk4mom.Eta()) < 2.1 &&
                                           trk4mom.Pt() >= 5.47 - 1.89 * fabs(trk4mom.Eta())) ||
                                          (2.1 <= fabs(trk4mom.Eta()) && trk4mom.Pt() >= 1.5)));
  } else if (muonType == (std::string)("TRK") || muonType == (std::string)("TRKSOFT")) {
    return (fabs(trk4mom.Eta()) < 2.4 &&
            ((fabs(trk4mom.Eta()) < 1.1 && trk4mom.Pt() >= 3.3) ||
             (1.1 <= fabs(trk4mom.Eta()) && fabs(trk4mom.Eta()) < 1.3 &&
              trk4mom.Pt() >= 13.2 - 9.0 * fabs(trk4mom.Eta())) ||
             (1.3 <= fabs(trk4mom.Eta()) && trk4mom.Pt() >= 0.8 && trk4mom.Pt() >= 3.02 - 1.17 * fabs(trk4mom.Eta()))));
  } else if (muonType == (std::string)("GLBSOFT")) {
    return (
        fabs(trk4mom.Eta()) < 2.4 &&
        ((fabs(trk4mom.Eta()) < 0.3 && trk4mom.Pt() >= 3.4) ||
         (fabs(trk4mom.Eta()) > 0.3 && fabs(trk4mom.Eta()) < 1.1 && trk4mom.Pt() >= 3.3) ||
         (fabs(trk4mom.Eta()) > 1.1 && fabs(trk4mom.Eta()) < 1.4 && trk4mom.Pt() >= 7.7 - 4.0 * fabs(trk4mom.Eta())) ||
         (fabs(trk4mom.Eta()) > 1.4 && fabs(trk4mom.Eta()) < 1.55 && trk4mom.Pt() >= 2.1) ||
         (fabs(trk4mom.Eta()) > 1.55 && fabs(trk4mom.Eta()) < 2.2 &&
          trk4mom.Pt() >= 4.25 - 1.39 * fabs(trk4mom.Eta())) ||
         (fabs(trk4mom.Eta()) > 2.2 && trk4mom.Pt() >= 1.2)));
  } else
    std::cout << "ERROR: Incorrect Muon Type" << std::endl;

  return false;
}

bool HiOniaAnalyzer::isMuonInAccept(const pat::Muon* aMuon, const std::string muonType) {
  if (muonType == (std::string)("GLB")) {
    return (fabs(aMuon->eta()) < 2.4 && ((fabs(aMuon->eta()) < 1.2 && aMuon->pt() >= 3.5) ||
                                         (1.2 <= fabs(aMuon->eta()) && fabs(aMuon->eta()) < 2.1 &&
                                          aMuon->pt() >= 5.47 - 1.89 * fabs(aMuon->eta())) ||
                                         (2.1 <= fabs(aMuon->eta()) && aMuon->pt() >= 1.5)));
  } else if (muonType == (std::string)("Acceptance2015")) {
    return (fabs(aMuon->eta()) < 2.4 && ((fabs(aMuon->eta()) < 1.2 && aMuon->pt() >= 3.5) ||
                                         (1.2 <= fabs(aMuon->eta()) && fabs(aMuon->eta()) < 2.1 &&
                                          aMuon->pt() >= 5.77 - 1.89 * fabs(aMuon->eta())) ||
                                         (2.1 <= fabs(aMuon->eta()) && aMuon->pt() >= 1.8)));
  } else if (muonType == (std::string)("TRK")) {  //This is actually softer than the "TRKSOFT" acceptance
    return (fabs(aMuon->eta()) < 2.4 && ((fabs(aMuon->eta()) < 0.8 && aMuon->pt() >= 3.3) ||
                                         (0.8 <= fabs(aMuon->eta()) && fabs(aMuon->eta()) < 2. && aMuon->p() >= 2.9) ||
                                         (2. <= fabs(aMuon->eta()) && aMuon->pt() >= 0.8)));
  } else if (muonType == (std::string)("GLBSOFT")) {
    return (
        fabs(aMuon->eta()) < 2.4 &&
        ((fabs(aMuon->eta()) < 0.3 && aMuon->pt() >= 3.4) ||
         (fabs(aMuon->eta()) > 0.3 && fabs(aMuon->eta()) < 1.1 && aMuon->pt() >= 3.3) ||
         (fabs(aMuon->eta()) > 1.1 && fabs(aMuon->eta()) < 1.4 && aMuon->pt() >= 7.7 - 4.0 * fabs(aMuon->eta())) ||
         (fabs(aMuon->eta()) > 1.4 && fabs(aMuon->eta()) < 1.55 && aMuon->pt() >= 2.1) ||
         (fabs(aMuon->eta()) > 1.55 && fabs(aMuon->eta()) < 2.2 && aMuon->pt() >= 4.25 - 1.39 * fabs(aMuon->eta())) ||
         (fabs(aMuon->eta()) > 2.2 && aMuon->pt() >= 1.2)));
  } else if (muonType == (std::string)("TRKSOFT")) {
    return (
        fabs(aMuon->eta()) < 2.4 &&
        ((fabs(aMuon->eta()) < 1.1 && aMuon->pt() >= 3.3) ||
         (1.1 <= fabs(aMuon->eta()) && fabs(aMuon->eta()) < 1.3 && aMuon->pt() >= 13.2 - 9.0 * fabs(aMuon->eta())) ||
         (1.3 <= fabs(aMuon->eta()) && aMuon->pt() >= 0.8 && aMuon->pt() >= 3.02 - 1.17 * fabs(aMuon->eta()))));
  } else
    std::cout << "ERROR: Incorrect Muon Type" << std::endl;

  return false;
}

bool HiOniaAnalyzer::isSoftMuonBase(const pat::Muon* aMuon) {
  return (aMuon->isTrackerMuon() && aMuon->innerTrack()->hitPattern().trackerLayersWithMeasurement() > 5 &&
          aMuon->innerTrack()->hitPattern().pixelLayersWithMeasurement() > 0 &&
          fabs(aMuon->innerTrack()->dxy(RefVtx)) < 0.3 && fabs(aMuon->innerTrack()->dz(RefVtx)) < 20.);
}

bool HiOniaAnalyzer::isSoftMuon(const pat::Muon* aMuon) {
  return (isSoftMuonBase(aMuon) && muon::isGoodMuon(*aMuon, muon::TMOneStationTight) &&
          aMuon->innerTrack()->quality(reco::TrackBase::highPurity));
}

bool HiOniaAnalyzer::isHybridSoftMuon(const pat::Muon* aMuon) {
  return (isSoftMuonBase(aMuon) && aMuon->isGlobalMuon());
}

bool HiOniaAnalyzer::isTightMuon(const pat::Muon* aMuon) {
  return (aMuon->isGlobalMuon() &&
          //aMuon->isPFMuon()   &&
          aMuon->globalTrack()->normalizedChi2() < 10. &&
          aMuon->globalTrack()->hitPattern().numberOfValidMuonHits() > 0 && aMuon->numberOfMatchedStations() > 1 &&
          fabs(aMuon->muonBestTrack()->dxy(RefVtx)) < 0.2 && fabs(aMuon->muonBestTrack()->dz(RefVtx)) < 0.5 &&
          aMuon->innerTrack()->hitPattern().numberOfValidPixelHits() > 0 &&
          aMuon->innerTrack()->hitPattern().trackerLayersWithMeasurement() > 5);
}

bool HiOniaAnalyzer::selGlobalMuon(const pat::Muon* aMuon) {
  if (!aMuon->isGlobalMuon())
    return false;

  if (_muonSel == (std::string)("GlbTrk") && !aMuon->isTrackerMuon())
    return false;

  if (!_applycuts)
    return true;

  bool isInAcc = isMuonInAccept(aMuon, (std::string)(_SofterSgMuAcceptance ? "GLBSOFT" : "GLB"));
  bool isGood = (_selTightGlobalMuon ? isTightMuon(aMuon) : isSoftMuonBase(aMuon));

  return (isInAcc && isGood && (!_miniAODcut || PassMiniAODcut(aMuon)));
}

bool HiOniaAnalyzer::selTrackerMuon(const pat::Muon* aMuon) {
  if (!aMuon->isTrackerMuon())
    return false;

  if (!_applycuts)
    return true;

  bool isInAcc = isMuonInAccept(aMuon, (std::string)(_SofterSgMuAcceptance ? "TRKSOFT" : "TRK"));
  bool isGood = isSoftMuonBase(aMuon);

  return (isInAcc && isGood && (!_miniAODcut || PassMiniAODcut(aMuon)));
}

bool HiOniaAnalyzer::selGlobalOrTrackerMuon(const pat::Muon* aMuon) {
  if (!aMuon->isGlobalMuon() && !aMuon->isTrackerMuon())
    return false;

  if (!_applycuts)
    return true;

  bool isInAcc = isMuonInAccept(aMuon, (std::string)(_SofterSgMuAcceptance ? "TRKSOFT" : "TRK"));
  bool isGood = isSoftMuonBase(aMuon);

  return (isInAcc && isGood && (!_miniAODcut || PassMiniAODcut(aMuon)));
}

bool HiOniaAnalyzer::selAllMuon(const pat::Muon* aMuon) { return !_miniAODcut || PassMiniAODcut(aMuon); }

bool HiOniaAnalyzer::PassMiniAODcut(const pat::Muon* aMuon) {
  return aMuon->pt() > 5 || aMuon->isPFMuon() ||
         (aMuon->pt() > 1.2 && (aMuon->isGlobalMuon() || aMuon->isStandAloneMuon())) ||
         (aMuon->isTrackerMuon() && aMuon->innerTrack()->quality(reco::TrackBase::highPurity));
}

bool HiOniaAnalyzer::selTrk(const reco::TrackRef aTrk) {
  if (!(aTrk->qualityByName("highPurity") && aTrk->ptError() / aTrk->pt() < 0.1))
    return false;

  if (!_applycuts)
    return true;

  bool isInAcc =
      aTrk->pt() > 1.2 &&
      fabs(aTrk->eta()) <
          2.4;  //(aTrk->pt())>0.2 && fabs(aTrk->eta())<2.4 && aTrk->ptError()/aTrk->pt()<0.1 && fabs(aTrk->dxy(RefVtx))<0.35 && fabs(aTrk->dz(RefVtx))<20; //keep margin in dxy and dz, if the RefVtx is not the good one due to muonlessPV

  return (isInAcc);
}

void HiOniaAnalyzer::InitEvent() {
  for (unsigned int iTr = 1; iTr < NTRIGGERS; ++iTr) {
    alreadyFilled[iTr] = false;
  }
  HLTriggers = 0;
  nEP = 0;

  _thePassedCats.clear();
  _thePassedCands.clear();

  Reco_QQ_size = 0;
  Reco_mu_size = 0;
  Reco_trk_size = 0;

  Reco_QQ_4mom->Clear();
  Reco_QQ_mupl_4mom->Clear();
  Reco_QQ_mumi_4mom->Clear();
  Reco_QQ_vtx->Clear();
  Reco_mu_4mom->Clear();
  Reco_mu_L1_4mom->Clear();

  if (_useGeTracks && _fillRecoTracks) {
    Reco_trk_4mom->Clear();
    Reco_trk_vtx->Clear();
  }

  if (_isMC) {
    Gen_QQ_4mom->Clear();
    Gen_mu_4mom->Clear();

    Gen_QQ_size = 0;
    Gen_mu_size = 0;

    Gen_weight = -1.;
    Gen_pthat = -1.;

    mapGenMuonMomToIndex_.clear();
  }

  if (_doTrimuons || _doDimuTrk) {
    _thePassedBcCats.clear();
    _thePassedBcCands.clear();

    Reco_3mu_size = 0;
    Reco_3mu_vtx->Clear();
    Reco_3mu_4mom->Clear();

    if (_isMC) {
      Gen_Bc_size = 0;
      Gen_Bc_4mom->Clear();
      Gen_Bc_nuW_4mom->Clear();
      Gen_3mu_4mom->Clear();
    }
  }

  mapMuonMomToIndex_.clear();
  mapTrkMomToIndex_.clear();
  for (std::map<std::string, int>::iterator clearIt = mapTriggerNameToIntFired_.begin();
       clearIt != mapTriggerNameToIntFired_.end();
       clearIt++) {
    clearIt->second = 0;
  }
  for (std::map<std::string, int>::iterator clearIt = mapTriggerNameToPrescaleFac_.begin();
       clearIt != mapTriggerNameToPrescaleFac_.end();
       clearIt++) {
    clearIt->second = -1;
  }

  return;
}

bool HiOniaAnalyzer::isSameLorentzV(TLorentzVector* v1, TLorentzVector* v2) {
  return (fabs(v1->Pt() - v2->Pt()) < 1e-5 && fabs(v1->Phi() - v2->Phi()) < 1e-6);
}

int HiOniaAnalyzer::IndexOfThisMuon(TLorentzVector* v1, bool isGen) {
  const auto& mapMuIdx = (isGen ? mapGenMuonMomToIndex_ : mapMuonMomToIndex_);
  const long int& muPt = FloatToIntkey(v1->Pt());

  if (mapMuIdx.count(muPt) == 0)
    return -1;
  else
    return mapMuIdx.at(muPt);
}

int HiOniaAnalyzer::IndexOfThisTrack(TLorentzVector* v1, bool isGen) {
  const auto& mapTrkIdx = (isGen ? mapTrkMomToIndex_ : mapTrkMomToIndex_);
  const long int& trkPt = FloatToIntkey(v1->Pt());

  if (mapTrkIdx.count(trkPt) == 0)
    return -1;
  else
    return mapTrkIdx.at(trkPt);
}

int HiOniaAnalyzer::IndexOfThisJpsi(int mu1_idx, int mu2_idx, int flipJpsi) {
  int GoodIndex = -1;
  for (int iJpsi = 0; iJpsi < Reco_QQ_size; iJpsi++) {
    if (((Reco_QQ_mumi_idx[iJpsi] == mu1_idx && Reco_QQ_mupl_idx[iJpsi] == mu2_idx) ||
         (Reco_QQ_mumi_idx[iJpsi] == mu2_idx && Reco_QQ_mupl_idx[iJpsi] == mu1_idx)) &&
        flipJpsi == Reco_QQ_flipJpsi[iJpsi]) {
      GoodIndex = iJpsi;
      break;
    }
  }
  return GoodIndex;
}

Short_t HiOniaAnalyzer::MuInSV(TLorentzVector v1, TLorentzVector v2, TLorentzVector v3) {
  //	cout<<"SV collection size : "<<SVs->size()<<endl;
  int nMuInSV = 0;
  for (std::vector<reco::Vertex>::const_iterator vt = SVs->begin(); vt != SVs->end(); ++vt) {
    const reco::Vertex* vtx = &(*vt);
    int nTrksInSV = 0;
    for (reco::Vertex::trackRef_iterator it = vtx->tracks_begin(); it != vtx->tracks_end(); ++it) {
      if ((fabs((*it)->pt() - v1.Pt()) < 1e-3 && fabs((*it)->eta() - v1.Eta()) < 1e-4) ||
          (fabs((*it)->pt() - v2.Pt()) < 1e-3 && fabs((*it)->eta() - v2.Eta()) < 1e-4) ||
          (fabs((*it)->pt() - v3.Pt()) < 1e-3 && fabs((*it)->eta() - v3.Eta()) < 1e-4)) {
        nTrksInSV += 1;
      }
    }
    if (nTrksInSV > nMuInSV)
      nMuInSV = nTrksInSV;
  }
  //cout<<"Number of muons from Bc that are in a given SV = "<<nMuInSV<<endl;

  return nMuInSV;
}

reco::GenParticleRef HiOniaAnalyzer::findDaughterRef(reco::GenParticleRef GenParticleDaughter, int GenParticlePDG) {
  reco::GenParticleRef GenParticleTmp = GenParticleDaughter;
  bool foundFirstDaughter = false;

  for (int j = 0; j < 1000; ++j) {
    //cout<<""; // trick to prevent rare segfault errors with the GenParticleTmp

    if (GenParticleTmp.isNonnull() && GenParticleTmp->status() > 0 && GenParticleTmp->status() < 1000 &&
        GenParticleTmp->numberOfDaughters() > 0) {
      if (GenParticleTmp->pdgId() == GenParticlePDG ||
          GenParticleTmp->daughterRef(0)->pdgId() ==
              GenParticlePDG)  //if oscillating B, can take two decays to return to pdgID(B parent)
      {
        GenParticleTmp = GenParticleTmp->daughterRef(0);
      } else if (!foundFirstDaughter)  //if Tmp is not a Bc, it means Tmp is a true daughter
      {
        foundFirstDaughter = true;
        GenParticlePDG = GenParticleTmp->pdgId();
      }
    } else
      break;
  }
  if (GenParticleTmp.isNonnull() && GenParticleTmp->status() > 0 && GenParticleTmp->status() < 1000 &&
      foundFirstDaughter) {  //(GenParticleTmp->pdgId()==GenParticlePDG)) {
    GenParticleDaughter = GenParticleTmp;
  }

  return GenParticleDaughter;
}

std::pair<bool, reco::GenParticleRef> HiOniaAnalyzer::findBcMotherRef(reco::GenParticleRef GenParticleMother,
                                                                      int BcPDG) {
  bool FoundBc = false;
  for (int i = 0; i < 1000; ++i) {
    if ((fabs(GenParticleMother->pdgId()) == BcPDG) && (GenParticleMother->status() == 2) &&
        (GenParticleMother->numberOfDaughters() >= 2)) {
      FoundBc = true;
      break;
    } else if (GenParticleMother.isNonnull() && GenParticleMother->numberOfMothers() > 0) {
      GenParticleMother = GenParticleMother->motherRef();
    } else {
      break;
    }
  }

  std::pair<bool, reco::GenParticleRef> res;
  res.first = FoundBc;
  res.second = GenParticleMother;
  return res;
}

void HiOniaAnalyzer::fillGenInfo() {
  if (Gen_QQ_size >= Max_QQ_size) {
    std::cout << "Too many dimuons: " << Gen_QQ_size << std::endl;
    std::cout << "Maximum allowed: " << Max_QQ_size << std::endl;
    return;
  }

  if (Gen_Bc_size >= Max_Bc_size) {
    std::cout << "Too many Bc's: " << Gen_Bc_size << std::endl;
    std::cout << "Maximum allowed: " << Max_Bc_size << std::endl;
    return;
  }

  if (Gen_mu_size >= Max_mu_size) {
    std::cout << "Too many muons: " << Gen_mu_size << std::endl;
    std::cout << "Maximum allowed: " << Max_mu_size << std::endl;
    return;
  }

  if (genInfo.isValid()) {
    if (genInfo->hasBinningValues())
      Gen_pthat = genInfo->binningValues()[0];
    Gen_weight = genInfo->weight();
  }

  if (collGenParticles.isValid()) {
    //Fill the single muons, before the dimuons (important)
    for (std::vector<reco::GenParticle>::const_iterator it = collGenParticles->begin(); it != collGenParticles->end();
         ++it) {
      const reco::GenParticle* gen = &(*it);

      if (abs(gen->pdgId()) == 13 && (gen->status() == 1)) {
        Gen_mu_type[Gen_mu_size] = _isPromptMC ? 0 : 1;  // prompt: 0, non-prompt: 1
        Gen_mu_charge[Gen_mu_size] = gen->charge();

        TLorentzVector vMuon = lorentzMomentum(gen->p4());
        new ((*Gen_mu_4mom)[Gen_mu_size]) TLorentzVector(vMuon);

        //Fill map of the muon indices. Use long int keys, to avoid rounding errors on a float key. Implies a precision of 10^-6
        mapGenMuonMomToIndex_[FloatToIntkey(vMuon.Pt())] = Gen_mu_size;

        Gen_mu_size++;
      }
    }

    for (std::vector<reco::GenParticle>::const_iterator it = collGenParticles->begin(); it != collGenParticles->end();
         ++it) {
      const reco::GenParticle* gen = &(*it);

      if (abs(gen->pdgId()) == _oniaPDG && (gen->status() == 2 || (abs(gen->pdgId()) == 23 && gen->status() == 62)) &&
          gen->numberOfDaughters() >= 2) {
        reco::GenParticleRef genMuon1 = findDaughterRef(gen->daughterRef(0), gen->pdgId());
        reco::GenParticleRef genMuon2 = findDaughterRef(gen->daughterRef(1), gen->pdgId());

        if (abs(genMuon1->pdgId()) == 13 && abs(genMuon2->pdgId()) == 13 && (genMuon1->status() == 1) &&
            (genMuon2->status() == 1)) {
          Gen_QQ_Bc_idx[Gen_QQ_size] = -1;
          Gen_QQ_type[Gen_QQ_size] = _isPromptMC ? 0 : 1;  // prompt: 0, non-prompt: 1
          std::pair<std::vector<reco::GenParticleRef>, std::pair<float, float> > MCinfo = findGenMCInfo(gen);
          Gen_QQ_ctau[Gen_QQ_size] = 10.0 * MCinfo.second.first;
          Gen_QQ_ctau3D[Gen_QQ_size] = 10.0 * MCinfo.second.second;

          if (_genealogyInfo) {
            _Gen_QQ_MomAndTrkBro[Gen_QQ_size] = MCinfo.first;
            Gen_QQ_momId[Gen_QQ_size] = _Gen_QQ_MomAndTrkBro[Gen_QQ_size][0]->pdgId();
          }

          TLorentzVector vJpsi = lorentzMomentum(gen->p4());
          new ((*Gen_QQ_4mom)[Gen_QQ_size]) TLorentzVector(vJpsi);

          TLorentzVector vMuon1 = lorentzMomentum(genMuon1->p4());
          TLorentzVector vMuon2 = lorentzMomentum(genMuon2->p4());

          if (genMuon1->charge() > genMuon2->charge()) {
            Gen_QQ_mupl_idx[Gen_QQ_size] = IndexOfThisMuon(&vMuon1, true);
            Gen_QQ_mumi_idx[Gen_QQ_size] = IndexOfThisMuon(&vMuon2, true);
          } else {
            Gen_QQ_mupl_idx[Gen_QQ_size] = IndexOfThisMuon(&vMuon2, true);
            Gen_QQ_mumi_idx[Gen_QQ_size] = IndexOfThisMuon(&vMuon1, true);
          }

          if (_doTrimuons) {
            //GenInfo for the Bc and the daughter muon from the W daughter of the Bc. Beware, this is designed for generated Bc's having QQ as a daughter!!
            std::pair<bool, reco::GenParticleRef> findBcMom = findBcMotherRef(
                findMotherRef(gen->motherRef(), gen->pdgId()), _BcPDG);  //the boolean says if the Bc mother was found

            if (findBcMom.first) {
              if (Gen_QQ_Bc_idx[Gen_QQ_size] > -1) {
                std::cout << "WARNING : Jpsi seems to have more than one Bc mother" << std::endl;
              }

              reco::GenParticleRef genBc = findBcMom.second;
              if (genBc->numberOfDaughters() >= 3) {
                reco::GenParticleRef genDau1 = findDaughterRef(genBc->daughterRef(0), genBc->pdgId());
                reco::GenParticleRef genDau2 = findDaughterRef(genBc->daughterRef(1), genBc->pdgId());
                reco::GenParticleRef genDau3 = findDaughterRef(genBc->daughterRef(2), genBc->pdgId());

                //Which daughter is the mu or nu from the W?
                bool goodDaughters = true;
                reco::GenParticleRef gennuW = genDau1;
                reco::GenParticleRef genmuW = genDau2;

                if (isNeutrino(genDau1->pdgId()) && (abs(genDau2->pdgId()) == 13)) {
                } else if (isNeutrino(genDau2->pdgId()) && (abs(genDau1->pdgId()) == 13)) {
                  reco::GenParticleRef gennuW = genDau2;
                  reco::GenParticleRef genmuW = genDau1;
                } else if (isNeutrino(genDau1->pdgId()) && (abs(genDau3->pdgId()) == 13)) {
                  reco::GenParticleRef gennuW = genDau1;
                  reco::GenParticleRef genmuW = genDau3;
                } else if (isNeutrino(genDau3->pdgId()) && (abs(genDau1->pdgId()) == 13)) {
                  reco::GenParticleRef gennuW = genDau3;
                  reco::GenParticleRef genmuW = genDau1;
                } else if (isNeutrino(genDau2->pdgId()) && (abs(genDau3->pdgId()) == 13)) {
                  reco::GenParticleRef gennuW = genDau2;
                  reco::GenParticleRef genmuW = genDau3;
                } else if (isNeutrino(genDau3->pdgId()) && (abs(genDau2->pdgId()) == 13)) {
                  reco::GenParticleRef gennuW = genDau3;
                  reco::GenParticleRef genmuW = genDau2;
                } else {
                  goodDaughters = false;
                }

                //Fill info for Bc and its mu,nu daughters
                if (goodDaughters && (genmuW->charge() == genBc->charge()) && (genmuW->status() == 1)) {
                  Gen_QQ_Bc_idx[Gen_QQ_size] = Gen_Bc_size;
                  Gen_Bc_QQ_idx[Gen_Bc_size] = Gen_QQ_size;

                  Gen_Bc_pdgId[Gen_Bc_size] = genBc->pdgId();
                  std::pair<int, std::pair<float, float> > MCinfo = findGenBcInfo(genBc, gen);
                  Gen_Bc_ctau[Gen_Bc_size] = 10.0 * MCinfo.second.first;

                  TLorentzVector vBc = lorentzMomentum(genBc->p4());
                  new ((*Gen_Bc_4mom)[Gen_Bc_size]) TLorentzVector(vBc);

                  TLorentzVector vmuW = lorentzMomentum(genmuW->p4());
                  Gen_Bc_muW_idx[Gen_Bc_size] = IndexOfThisMuon(&vmuW, true);

                  TLorentzVector vnuW = lorentzMomentum(gennuW->p4());
                  new ((*Gen_Bc_nuW_4mom)[Gen_Bc_size]) TLorentzVector(vnuW);

                  Gen_Bc_size++;
                } else {
                  std::cout << "WARNING : Problem with daughters of the gen Bc, hence Bc and its daughters are not "
                               "written out"
                            << std::endl;
                }
              }
            }
          }
          Gen_QQ_size++;
        }
      }
    }
  }
  return;
}

//Find the indices of the reconstructed muon matching each generated muon, and vice versa
void HiOniaAnalyzer::fillMuMatchingInfo() {
  //initialize Gen_mu_whichRec
  for (int igen = 0; igen < Gen_mu_size; igen++) {
    Gen_mu_whichRec[igen] = -1;
  }

  //Find the index of generated muon associated to a reco muon, txs to Reco_mu_pTrue
  for (int irec = 0; irec < Reco_mu_size; irec++) {
    int foundGen = -1;
    if (Reco_mu_pTrue[irec] >= 0) {  //if pTrue=-1, then the reco muon is a fake

      for (int igen = 0; igen < Gen_mu_size; igen++) {
        TLorentzVector* genmuMom = (TLorentzVector*)Gen_mu_4mom->ConstructedAt(igen);
        if (fabs(genmuMom->P() - Reco_mu_pTrue[irec]) / Reco_mu_pTrue[irec] < 1e-6 &&
            Gen_mu_charge[igen] == Reco_mu_charge[irec]) {
          foundGen = igen;
          break;
        }
      }
    }

    Reco_mu_whichGen[irec] = foundGen;
    if (foundGen > -1)
      Gen_mu_whichRec[foundGen] = irec;
  }
}

//Find the indices of the reconstructed J/psi matching each generated J/psi (when the two daughter muons are reconstructed), and vice versa
void HiOniaAnalyzer::fillQQMatchingInfo() {
  for (int igen = 0; igen < Gen_QQ_size; igen++) {
    Gen_QQ_whichRec[igen] = -1;
    int Reco_mupl_idx =
        Gen_mu_whichRec[Gen_QQ_mupl_idx[igen]];  //index of the reconstructed mupl associated to the generated mupl of Jpsi
    int Reco_mumi_idx =
        Gen_mu_whichRec[Gen_QQ_mumi_idx[igen]];  //index of the reconstructed mumi associated to the generated mumi of Jpsi

    if ((Reco_mupl_idx >= 0) && (Reco_mumi_idx >= 0)) {  //Search for Reco_QQ only if both muons are reco
      for (int irec = 0; irec < Reco_QQ_size; irec++) {
        if (((Reco_mupl_idx == Reco_QQ_mupl_idx[irec]) &&
             (Reco_mumi_idx == Reco_QQ_mumi_idx[irec])) ||  //the charges might be wrong in reco
            ((Reco_mupl_idx == Reco_QQ_mumi_idx[irec]) && (Reco_mumi_idx == Reco_QQ_mupl_idx[irec]))) {
          Gen_QQ_whichRec[igen] = irec;
          break;
        }
      }

      if (Gen_QQ_whichRec[igen] == -1)
        Gen_QQ_whichRec[igen] = -2;  //Means the two muons were reconstructed, but the dimuon was not selected
    }
  }

  //Find the index of generated J/psi associated to a reco QQ
  for (int irec = 0; irec < Reco_QQ_size; irec++) {
    Reco_QQ_whichGen[irec] = -1;

    for (int igen = 0; igen < Gen_QQ_size; igen++) {
      if ((Gen_QQ_whichRec[igen] == irec)) {
        Reco_QQ_whichGen[irec] = igen;
        break;
      }
    }
  }
}

//Build the visible Bc when the three daugther muons are reconstructed. Record indices between Gen and Rec
void HiOniaAnalyzer::fillBcMatchingInfo() {
  for (int igen = 0; igen < Gen_Bc_size; igen++) {
    Gen_3mu_whichRec[igen] = -1;

    //Build the visible Gen Bc (sum of lorentzvectors of the three gen muons)
    TLorentzVector gen_3mu_4mom = *((TLorentzVector*)Gen_QQ_4mom->ConstructedAt(Gen_Bc_QQ_idx[igen])) +
                                  *((TLorentzVector*)Gen_mu_4mom->ConstructedAt(Gen_Bc_muW_idx[igen]));
    new ((*Gen_3mu_4mom)[igen]) TLorentzVector(gen_3mu_4mom);

    //Look for index of reconstructed muon
    int Reco_muW_idx =
        Gen_mu_whichRec[Gen_Bc_muW_idx[igen]];  //index of the reconstructed muW associated to the generated muW of Bc
    int Reco_mupl_idx = Gen_mu_whichRec
        [Gen_QQ_mupl_idx[Gen_Bc_QQ_idx[igen]]];  //index of the reconstructed mupl associated to the generated mupl of Jpsi
    int Reco_mumi_idx = Gen_mu_whichRec
        [Gen_QQ_mumi_idx[Gen_Bc_QQ_idx[igen]]];  //index of the reconstructed mumi associated to the generated mumi of Jpsi

    if ((Reco_mupl_idx >= 0) && (Reco_mumi_idx >= 0) &&
        (Reco_muW_idx >= 0)) {  //Search for Reco_Bc only if the three muons are reco

      for (int irec = 0; irec < Reco_3mu_size; irec++) {
        if (((Reco_mupl_idx == Reco_3mu_mupl_idx[irec]) && (Reco_mumi_idx == Reco_3mu_mumi_idx[irec]) &&
             (Reco_muW_idx == Reco_3mu_muW_idx[irec])) ||  //the charges might be wrong in reco
            ((Reco_mupl_idx == Reco_3mu_mupl_idx[irec]) && (Reco_mumi_idx == Reco_3mu_muW_idx[irec]) &&
             (Reco_muW_idx == Reco_3mu_mumi_idx[irec])) ||
            ((Reco_mupl_idx == Reco_3mu_muW_idx[irec]) && (Reco_mumi_idx == Reco_3mu_mumi_idx[irec]) &&
             (Reco_muW_idx == Reco_3mu_mupl_idx[irec])) ||
            ((Reco_mupl_idx == Reco_3mu_muW_idx[irec]) && (Reco_mumi_idx == Reco_3mu_mupl_idx[irec]) &&
             (Reco_muW_idx == Reco_3mu_mumi_idx[irec])) ||
            ((Reco_mupl_idx == Reco_3mu_mumi_idx[irec]) && (Reco_mumi_idx == Reco_3mu_muW_idx[irec]) &&
             (Reco_muW_idx == Reco_3mu_mupl_idx[irec])) ||
            ((Reco_mupl_idx == Reco_3mu_mumi_idx[irec]) && (Reco_mumi_idx == Reco_3mu_mupl_idx[irec]) &&
             (Reco_muW_idx == Reco_3mu_muW_idx[irec]))) {
          Gen_3mu_whichRec[igen] = irec;
          break;
        }
      }

      if ((Gen_3mu_whichRec[igen] == -1)) {
        Gen_3mu_whichRec[igen] = -2;  //The three muons were reconstructed, but the associated trimuon was not selected
      }
    }
  }

  for (int irec = 0; irec < Reco_3mu_size; irec++) {
    //Find the index of generated Bc associated to a reco trimuon
    Reco_3mu_whichGen[irec] = -1;
    for (int igen = 0; igen < Gen_Bc_size; igen++) {
      if ((Gen_3mu_whichRec[igen] == irec)) {
        Reco_3mu_whichGen[irec] = igen;
        break;
      }
    }

    //Match the muW to a gen particle, and find out if it comes from the B meson hard process
    if (_genealogyInfo) {
      //cout<<"Checking genealogy for Reco_3mu #"<<irec<<endl;
      //cout<<"Reco_muW simExtType,simPdgId,simMotherPdgId = "<<Reco_mu_simExtType[Reco_3mu_muW_idx[irec]]<<" "<<Reco_mu_simPdgId[Reco_3mu_muW_idx[irec]]<<" "<<Reco_mu_simMotherPdgId[Reco_3mu_muW_idx[irec]]<<endl;
      Reco_3mu_muW_isGenJpsiBro[irec] = false;
      Reco_3mu_muW_trueId[irec] = 0;

      int goodRecQQidx = (Reco_3mu_QQ1_idx[irec] > -1) ? (Reco_3mu_QQ1_idx[irec]) : (Reco_3mu_QQ2_idx[irec]);
      if (Reco_3mu_QQ1_idx[irec] > -1 &&
          Reco_3mu_QQ2_idx[irec] >
              -1) {  //If both OS pairs are reconstructed, choose the reco QQ that is matched to a gen
        goodRecQQidx =
            (Reco_QQ_whichGen[Reco_3mu_QQ1_idx[irec]] > -1) ? (Reco_3mu_QQ1_idx[irec]) : (Reco_3mu_QQ2_idx[irec]);
      }
      int genQQidx = Reco_QQ_whichGen[goodRecQQidx];
      int newmuWidx = (goodRecQQidx == Reco_3mu_QQ1_idx[irec])
                          ? (Reco_3mu_muW_idx[irec])
                          : (Reco_3mu_muW2_idx[irec]);  //use the muW_idx corresponding to the chosen QQ dimuon
      int genMuWidx = Reco_mu_whichGen[newmuWidx];

      //If the muon is fake, match it to whatever generated particles
      if (genMuWidx == -1) {
        TLorentzVector* recmuW = (TLorentzVector*)Reco_mu_4mom->ConstructedAt(newmuWidx);
        bool SureDecayInFlight = (fabs(Reco_mu_simExtType[newmuWidx]) == 4);
        bool Unmatched = (fabs(Reco_mu_simExtType[newmuWidx]) == 0);
        float dRmax = SureDecayInFlight ? 0.3 : (Unmatched ? 0.15 : 0.1);
        float dRmin = dRmax;
        float matchedPhi = 5;

        //Loop over gen particles
        //cout<<"Looking for a (charged track) gen particle matching the fake Reco_muW of pt,eta = "<<recmuW->Pt()<<" "<<recmuW->Eta()<<endl;
        for (std::vector<reco::GenParticle>::const_iterator it = collGenParticles->begin();
             it != collGenParticles->end();
             ++it) {
          const reco::GenParticle* gen = &(*it);

          if (isChargedTrack(gen->pdgId())  //&& gen->status() == 1
          ) {
            TLorentzVector genP = lorentzMomentum(gen->p4());
            float dR = recmuW->DeltaR(genP);
            if (dR < dRmin &&
                (!Unmatched ||
                 (genP.Pt() >
                  0.6 *
                      recmuW->Pt()))  // if unmatched, probably a ghost, so at least have a decent Pt agreement (leaving possibility for energy loss of a decayInFlight) for the particle causing the ghost
            ) {
              dRmin = dR;
              Reco_3mu_muW_trueId[irec] = gen->pdgId();
              matchedPhi = genP.Phi();
              //cout<<"   ........Found matched gen particle for fake reco muon: ID, pt, eta, deltaR = "<<Reco_3mu_muW_trueId[irec]<<" "<<genP.Pt()<<" "<<genP.Eta()<<" "<<dRmin<<endl;
            }
          }
        }
        //end loop over gen particles

        //If the Jpsi is true, check if the matched gen particle was part of the B-parent process
        if (genQQidx > -1) {
          for (auto&& bro : _Gen_QQ_MomAndTrkBro[genQQidx]) {
            if (fabs(bro->phi() - matchedPhi) < 1e-6 && isChargedTrack(bro->pdgId())) {
              Reco_3mu_muW_isGenJpsiBro[irec] = true;
              //cout<<"   !!!!!!!!!!!!    FAKE muW is from B parent"<<endl;
              break;
            }
          }
        }
      }

      //If the muon is true, take the associated gen muon, and find if it comes from the B parent of Jpsi
      if (genMuWidx > -1) {
        Reco_3mu_muW_trueId[irec] = (Gen_mu_charge[genMuWidx] == -1) ? 13 : (-13);

        if (genQQidx > -1) {
          //cout<<"Checking if this gen muon comes from B parent: pt,eta = "<<((TLorentzVector*)Gen_mu_4mom->ConstructedAt(genMuWidx))->Pt()<<" "<<((TLorentzVector*)Gen_mu_4mom->ConstructedAt(genMuWidx))->Eta()<<endl;
          for (auto&& bro : _Gen_QQ_MomAndTrkBro[genQQidx]) {
            if (fabs(bro->pdgId()) == 13 &&
                fabs(bro->phi() - ((TLorentzVector*)Gen_mu_4mom->ConstructedAt(genMuWidx))->Phi()) < 1e-6) {
              Reco_3mu_muW_isGenJpsiBro[irec] = true;
              //cout<<"   !!!!!!!!!!!!    True muW is from B parent: pt, eta = "<<((TLorentzVector*)Gen_mu_4mom->ConstructedAt(genMuWidx))->Pt()<<" "<<((TLorentzVector*)Gen_mu_4mom->ConstructedAt(genMuWidx))->Eta()<<endl;
              break;
            }
          }
        } else {  //cout<<"True Muon but fake Jpsi"<<endl;
        }
      }

      //cout<<endl;
    }
  }
}

void HiOniaAnalyzer::fillRecoTracks() {
  if (collTracks.isValid()) {
    for (unsigned int tidx = 0; tidx < collTracks->size(); tidx++) {
      const reco::TrackRef track(collTracks, tidx);

      if (!track.isNonnull()) {
        std::cout << "ERROR: 'track' pointer in fillRecoTracks is NULL ! Go to next track." << endl;
      } else {
        bool WantedTrack = false;
        for (int k = 0; k < (int)EtaOfWantedTracks.size(); k++) {
          if (fabs(track->eta() - EtaOfWantedTracks[k]) < 1e-5) {
            WantedTrack = true;
            break;
          }
        }
        if (_doDimuTrk && !WantedTrack)
          continue;

        if (selTrk(track)) {
          if (Reco_trk_size >= Max_trk_size) {
            std::cout << "Too many tracks: " << Reco_trk_size << std::endl;
            std::cout << "Maximum allowed: " << Max_trk_size << std::endl;
            break;
          }

          TLorentzVector vTrack;
          vTrack.SetPtEtaPhiM(track->pt(), track->eta(), track->phi(), 0.10566);  //0.13957018 for the pion

          Reco_trk_whichGenmu[Reco_trk_size] = -1;
          if (_isMC) {
            float dRmax = 0.05;  //dR max of the matching to gen muons//same than for reco-gen muon matching
            float dR;
            float dPtmax = 0.5;
            for (int igen = 0; igen < Gen_mu_size; igen++) {
              TLorentzVector* genmu = (TLorentzVector*)Gen_mu_4mom->ConstructedAt(igen);
              dR = genmu->DeltaR(vTrack);
              if (dR <= dRmax && track->charge() == Gen_mu_charge[igen] &&
                  fabs(genmu->Pt() - vTrack.Pt()) / genmu->Pt() < dPtmax) {
                dRmax = dR;
                Reco_trk_whichGenmu[Reco_trk_size] = igen;
              }
            }
          }

          if (!_doDimuTrk && Reco_trk_whichGenmu[Reco_trk_size] == -1)
            continue;

          Reco_trk_charge[Reco_trk_size] = track->charge();

          //new((*Reco_trk_vtx)[Reco_trk_size])TVector3(track->vx(),track->vy(),track->vz());
          //cout<<"Just found and filled Reco_trk_vtx"<<endl;

          Reco_trk_originalAlgo[Reco_mu_size] = track->originalAlgo();
          Reco_trk_nPixWMea[Reco_mu_size] = track->hitPattern().pixelLayersWithMeasurement();
          Reco_trk_nTrkWMea[Reco_mu_size] = track->hitPattern().trackerLayersWithMeasurement();
          Reco_trk_dxyError[Reco_trk_size] = track->dxyError();
          Reco_trk_dzError[Reco_trk_size] = track->dzError();
          Reco_trk_dxy[Reco_trk_size] = track->dxy(RefVtx);
          Reco_trk_dz[Reco_trk_size] = track->dz(RefVtx);
          Reco_trk_ptErr[Reco_trk_size] = track->ptError();

          mapTrkMomToIndex_[FloatToIntkey(vTrack.Pt())] = Reco_trk_size;

          Reco_trk_InLooseAcc[Reco_trk_size] = isTrkInMuonAccept(vTrack, "GLBSOFT");
          Reco_trk_InTightAcc[Reco_trk_size] = isTrkInMuonAccept(vTrack, "GLB");

          new ((*Reco_trk_4mom)[Reco_trk_size]) TLorentzVector(vTrack);
          Reco_trk_size++;
        }
      }
    }
  }

  return;
}

void HiOniaAnalyzer::fillRecoMuons(int iCent) {
  int nL1DoubleMu0Muons = 0;
  int nGoodMuons = 0;
  int nGoodMuonsNoTrig = 0;

  if (collMuonNoTrig.isValid()) {
    for (std::vector<pat::Muon>::const_iterator it = collMuonNoTrig->begin(); it != collMuonNoTrig->end(); ++it) {
      const pat::Muon* muon = &(*it);

      if (muon == NULL) {
        std::cout << "ERROR: 'muon' pointer in fillRecoMuons is NULL ! Return now" << std::endl;
        return;
      } else {
        if (selGlobalMuon(muon))
          nGoodMuonsNoTrig++;
      }
    }
  }

  if (collMuon.isValid()) {
    for (vector<pat::Muon>::const_iterator it = collMuon->begin(); it != collMuon->end(); ++it) {
      const pat::Muon* muon = &(*it);

      if (muon == NULL) {
        std::cout << "ERROR: 'muon' pointer in fillRecoMuons is NULL ! Return now" << std::endl;
        return;
      } else {
        //Trick to recover feature of filling only muons from selected dimuons
        if (!_fillSingleMuons) {
          bool WantedMuon = false;
          for (int k = 0; k < (int)EtaOfWantedMuons.size(); k++) {
            if (fabs(muon->eta() - EtaOfWantedMuons[k]) < 1e-5) {
              WantedMuon = true;
              break;
            }
          }
          if (!WantedMuon)
            continue;
        }

        bool isBarrel = false;
        if (fabs(muon->eta() < 1.2))
          isBarrel = true;
        std::string theLabel = theTriggerNames.at(0) + "_" + theCentralities.at(iCent);

        if (_fillHistos) {
          if (_combineCategories) {
            if (selGlobalMuon(muon) || selTrackerMuon(muon)) {
              myRecoMuonHistos->Fill(muon, "All_" + theLabel);
              if (isBarrel)
                myRecoMuonHistos->Fill(muon, "Barrel_" + theLabel);
              else
                myRecoMuonHistos->Fill(muon, "EndCap_" + theLabel);
            }
          } else {
            if (selGlobalMuon(muon)) {
              myRecoGlbMuonHistos->Fill(muon, "All_" + theLabel);
              if (isBarrel)
                myRecoGlbMuonHistos->Fill(muon, "Barrel_" + theLabel);
              else
                myRecoGlbMuonHistos->Fill(muon, "EndCap_" + theLabel);
            } else if (selTrackerMuon(muon)) {
              myRecoTrkMuonHistos->Fill(muon, "All_" + theLabel);
              if (isBarrel)
                myRecoTrkMuonHistos->Fill(muon, "Barrel_" + theLabel);
              else
                myRecoTrkMuonHistos->Fill(muon, "EndCap_" + theLabel);
            }
          }
        }

        muType = -99;
        if (_muonSel == (std::string)("Glb") && selGlobalMuon(muon))
          muType = Glb;
        if (_muonSel == (std::string)("GlbTrk") && selGlobalMuon(muon))
          muType = GlbTrk;
        if (_muonSel == (std::string)("Trk") && selTrackerMuon(muon))
          muType = Trk;
        if (_muonSel == (std::string)("TwoGlbAmongThree") && selGlobalOrTrackerMuon(muon))
          muType = GlbOrTrk;
        if (_muonSel == (std::string)("GlbOrTrk") && selGlobalOrTrackerMuon(muon))
          muType = GlbOrTrk;
        if (_muonSel == (std::string)("All") && selAllMuon(muon))
          muType = All;

        if (muType == GlbOrTrk || muType == GlbTrk || muType == Trk || muType == Glb || muType == All) {
          nGoodMuons++;

          ULong64_t trigBits = 0;
          for (unsigned int iTr = 1; iTr < NTRIGGERS; ++iTr) {
            const pat::TriggerObjectStandAloneCollection muHLTMatchesFilter =
                muon->triggerObjectMatchesByFilter(HLTLastFilters[iTr]);

            // apparently matching by path gives false positives so we use matching by filter for all triggers for which we know the filter name
            if (muHLTMatchesFilter.size() > 0) {
              std::string theLabel = theTriggerNames.at(iTr) + "_" + theCentralities.at(iCent);

              if (_fillHistos) {
                if (_combineCategories) {
                  myRecoMuonHistos->Fill(muon, "All_" + theLabel);
                  if (isBarrel)
                    myRecoMuonHistos->Fill(muon, "Barrel_" + theLabel);
                  else
                    myRecoMuonHistos->Fill(muon, "EndCap_" + theLabel);
                } else if (muType == Glb || muType == GlbTrk) {
                  myRecoGlbMuonHistos->Fill(muon, "All_" + theLabel);
                  if (isBarrel)
                    myRecoGlbMuonHistos->Fill(muon, "Barrel_" + theLabel);
                  else
                    myRecoGlbMuonHistos->Fill(muon, "EndCap_" + theLabel);
                } else if (muType == Trk || muType == GlbOrTrk || muType == All) {
                  myRecoTrkMuonHistos->Fill(muon, "All_" + theLabel);
                  if (isBarrel)
                    myRecoTrkMuonHistos->Fill(muon, "Barrel_" + theLabel);
                  else
                    myRecoTrkMuonHistos->Fill(muon, "EndCap_" + theLabel);
                }
              }

              trigBits += pow(2, iTr - 1);

              if (iTr == 1)
                nL1DoubleMu0Muons++;
            }
          }
          if (_fillTree)
            this->fillTreeMuon(muon, muType, trigBits);
        }
      }
    }
  }

  hGoodMuonsNoTrig->Fill(nGoodMuonsNoTrig);
  hGoodMuons->Fill(nGoodMuons);
  hL1DoubleMu0->Fill(nL1DoubleMu0Muons);

  return;
}

void HiOniaAnalyzer::InitTree() {
  Reco_mu_4mom = new TClonesArray("TLorentzVector", Max_mu_size);
  Reco_mu_L1_4mom = new TClonesArray("TLorentzVector", Max_mu_size);
  Reco_QQ_4mom = new TClonesArray("TLorentzVector", Max_QQ_size);
  Reco_QQ_mumi_4mom = new TClonesArray("TLorentzVector", Max_QQ_size);
  Reco_QQ_mupl_4mom = new TClonesArray("TLorentzVector", Max_QQ_size);
  Reco_QQ_vtx = new TClonesArray("TVector3", Max_QQ_size);

  if (_useGeTracks && _fillRecoTracks) {
    Reco_trk_4mom = new TClonesArray("TLorentzVector", Max_trk_size);
    Reco_trk_vtx = new TClonesArray("TVector3", Max_trk_size);
  }

  if (_isMC) {
    Gen_mu_4mom = new TClonesArray("TLorentzVector", 10);
    Gen_QQ_4mom = new TClonesArray("TLorentzVector", 10);
  }

  if (_doTrimuons || _doDimuTrk) {
    Reco_3mu_4mom = new TClonesArray("TLorentzVector", Max_Bc_size);
    Reco_3mu_vtx = new TClonesArray("TVector3", Max_Bc_size);

    if (_isMC) {
      Gen_Bc_4mom = new TClonesArray("TLorentzVector", 10);
      Gen_Bc_nuW_4mom = new TClonesArray("TLorentzVector", 10);
      Gen_3mu_4mom = new TClonesArray("TLorentzVector", 10);
    }
  }

  //myTree = new TTree("myTree","My TTree of dimuons");
  myTree = fs->make<TTree>("myTree", "My TTree of dimuons");

  myTree->Branch("eventNb", &eventNb, "eventNb/i");
  if (!_isMC) {
    myTree->Branch("runNb", &runNb, "runNb/i");
    myTree->Branch("LS", &lumiSection, "LS/i");
  }
  myTree->Branch("zVtx", &zVtx, "zVtx/F");
  myTree->Branch("nPV", &nPV, "nPV/S");
  if (_isHI || _isPA) {
    myTree->Branch("Centrality", &centBin, "Centrality/I");
    myTree->Branch("Npix", &Npix, "Npix/S");
    myTree->Branch("NpixelTracks", &NpixelTracks, "NpixelTracks/S");
  }
  myTree->Branch("Ntracks", &Ntracks, "Ntracks/S");

  //myTree->Branch("nTrig", &nTrig, "nTrig/I");
  myTree->Branch("trigPrescale", trigPrescale, Form("trigPrescale[%d]/I", nTrig));
  myTree->Branch("HLTriggers", &HLTriggers, "HLTriggers/l");

  if ((_isHI || _isPA) && _SumETvariables) {
    myTree->Branch("SumET_HF", &SumET_HF, "SumET_HF/F");
    myTree->Branch("SumET_HFplus", &SumET_HFplus, "SumET_HFplus/F");
    myTree->Branch("SumET_HFminus", &SumET_HFminus, "SumET_HFminus/F");
    myTree->Branch("SumET_HFplusEta4", &SumET_HFplusEta4, "SumET_HFplusEta4/F");
    myTree->Branch("SumET_HFminusEta4", &SumET_HFminusEta4, "SumET_HFminusEta4/F");
    myTree->Branch("SumET_ET", &SumET_ET, "SumET_ET/F");
    myTree->Branch("SumET_EE", &SumET_EE, "SumET_EE/F");
    myTree->Branch("SumET_EB", &SumET_EB, "SumET_EB/F");
    myTree->Branch("SumET_EEplus", &SumET_EEplus, "SumET_EEplus/F");
    myTree->Branch("SumET_EEminus", &SumET_EEminus, "SumET_EEminus/F");
    myTree->Branch("SumET_ZDC", &SumET_ZDC, "SumET_ZDC/F");
    myTree->Branch("SumET_ZDCplus", &SumET_ZDCplus, "SumET_ZDCplus/F");
    myTree->Branch("SumET_ZDCminus", &SumET_ZDCminus, "SumET_ZDCminus/F");
  }

  if ((_isHI || _isPA) && _useEvtPlane) {
    myTree->Branch("nEP", &nEP, "nEP/I");
    myTree->Branch("rpAng", &rpAng, "rpAng[nEP]/F");
    myTree->Branch("rpSin", &rpSin, "rpSin[nEP]/F");
    myTree->Branch("rpCos", &rpCos, "rpCos[nEP]/F");
  }

  if (!_onlySingleMuons) {
    if (_doTrimuons || _doDimuTrk) {
      myTree->Branch("Reco_3mu_size", &Reco_3mu_size, "Reco_3mu_size/S");
      myTree->Branch("Reco_3mu_charge", Reco_3mu_charge, "Reco_3mu_charge[Reco_3mu_size]/S");
      myTree->Branch("Reco_3mu_4mom", "TClonesArray", &Reco_3mu_4mom, 32000, 0);
      myTree->Branch("Reco_3mu_mupl_idx", Reco_3mu_mupl_idx, "Reco_3mu_mupl_idx[Reco_3mu_size]/S");
      myTree->Branch("Reco_3mu_mumi_idx", Reco_3mu_mumi_idx, "Reco_3mu_mumi_idx[Reco_3mu_size]/S");
      myTree->Branch("Reco_3mu_muW_idx", Reco_3mu_muW_idx, "Reco_3mu_muW_idx[Reco_3mu_size]/S");
      if (!_doDimuTrk)
        myTree->Branch("Reco_3mu_muW2_idx", Reco_3mu_muW2_idx, "Reco_3mu_muW2_idx[Reco_3mu_size]/S");
      myTree->Branch("Reco_3mu_QQ1_idx", Reco_3mu_QQ1_idx, "Reco_3mu_QQ1_idx[Reco_3mu_size]/S");
      if (!_doDimuTrk) {
        myTree->Branch("Reco_3mu_QQ2_idx", Reco_3mu_QQ2_idx, "Reco_3mu_QQ2_idx[Reco_3mu_size]/S");
        myTree->Branch("Reco_3mu_QQss_idx", Reco_3mu_QQss_idx, "Reco_3mu_QQss_idx[Reco_3mu_size]/S");
      }
      if (_isMC && _genealogyInfo) {
        myTree->Branch(
            "Reco_3mu_muW_isGenJpsiBro", Reco_3mu_muW_isGenJpsiBro, "Reco_3mu_muW_isGenJpsiBro[Reco_3mu_size]/O");
        myTree->Branch("Reco_3mu_muW_trueId", Reco_3mu_muW_trueId, "Reco_3mu_muW_trueId[Reco_3mu_size]/I");
      }

      myTree->Branch("Reco_3mu_ctau", Reco_3mu_ctau, "Reco_3mu_ctau[Reco_3mu_size]/F");
      myTree->Branch("Reco_3mu_ctauErr", Reco_3mu_ctauErr, "Reco_3mu_ctauErr[Reco_3mu_size]/F");
      myTree->Branch("Reco_3mu_cosAlpha", Reco_3mu_cosAlpha, "Reco_3mu_cosAlpha[Reco_3mu_size]/F");
      myTree->Branch("Reco_3mu_ctau3D", Reco_3mu_ctau3D, "Reco_3mu_ctau3D[Reco_3mu_size]/F");
      myTree->Branch("Reco_3mu_ctauErr3D", Reco_3mu_ctauErr3D, "Reco_3mu_ctauErr3D[Reco_3mu_size]/F");
      myTree->Branch("Reco_3mu_cosAlpha3D", Reco_3mu_cosAlpha3D, "Reco_3mu_cosAlpha3D[Reco_3mu_size]/F");

      if (_isMC) {
        myTree->Branch("Reco_3mu_whichGen", Reco_3mu_whichGen, "Reco_3mu_whichGen[Reco_3mu_size]/S");
      }
      myTree->Branch("Reco_3mu_VtxProb", Reco_3mu_VtxProb, "Reco_3mu_VtxProb[Reco_3mu_size]/F");

      if (_doDimuTrk) {
        myTree->Branch("Reco_3mu_KCVtxProb", Reco_3mu_KCVtxProb, "Reco_3mu_KCVtxProb[Reco_3mu_size]/F");
        myTree->Branch("Reco_3mu_KCctau", Reco_3mu_KCctau, "Reco_3mu_KCctau[Reco_3mu_size]/F");
        myTree->Branch("Reco_3mu_KCctauErr", Reco_3mu_KCctauErr, "Reco_3mu_KCctauErr[Reco_3mu_size]/F");
        myTree->Branch("Reco_3mu_KCcosAlpha", Reco_3mu_KCcosAlpha, "Reco_3mu_KCcosAlpha[Reco_3mu_size]/F");
        myTree->Branch("Reco_3mu_KCctau3D", Reco_3mu_KCctau3D, "Reco_3mu_KCctau3D[Reco_3mu_size]/F");
        myTree->Branch("Reco_3mu_KCctauErr3D", Reco_3mu_KCctauErr3D, "Reco_3mu_KCctauErr3D[Reco_3mu_size]/F");
        myTree->Branch("Reco_3mu_KCcosAlpha3D", Reco_3mu_KCcosAlpha3D, "Reco_3mu_KCcosAlpha3D[Reco_3mu_size]/F");
      }
      if ((!_theMinimumFlag && _muonLessPrimaryVertex) || (_flipJpsiDirection > 0)) {
        myTree->Branch(
            "Reco_3mu_muW_dxy_muonlessVtx", Reco_3mu_muW_dxy, "Reco_3mu_muW_dxy_muonlessVtx[Reco_3mu_size]/F");
        myTree->Branch("Reco_3mu_muW_dz_muonlessVtx", Reco_3mu_muW_dz, "Reco_3mu_muW_dz_muonlessVtx[Reco_3mu_size]/F");
        myTree->Branch(
            "Reco_3mu_mumi_dxy_muonlessVtx", Reco_3mu_mumi_dxy, "Reco_3mu_mumi_dxy_muonlessVtx[Reco_3mu_size]/F");
        myTree->Branch(
            "Reco_3mu_mumi_dz_muonlessVtx", Reco_3mu_mumi_dz, "Reco_3mu_mumi_dz_muonlessVtx[Reco_3mu_size]/F");
        myTree->Branch(
            "Reco_3mu_mupl_dxy_muonlessVtx", Reco_3mu_mupl_dxy, "Reco_3mu_mupl_dxy_muonlessVtx[Reco_3mu_size]/F");
        myTree->Branch(
            "Reco_3mu_mupl_dz_muonlessVtx", Reco_3mu_mupl_dz, "Reco_3mu_mupl_dz_muonlessVtx[Reco_3mu_size]/F");
      }

      myTree->Branch("Reco_3mu_MassErr", Reco_3mu_MassErr, "Reco_3mu_MassErr[Reco_3mu_size]/F");
      myTree->Branch("Reco_3mu_CorrM", Reco_3mu_CorrM, "Reco_3mu_CorrM[Reco_3mu_size]/F");
      if (_useSVfinder && SVs.isValid() && SVs->size() > 0) {
        myTree->Branch("Reco_3mu_NbMuInSameSV", Reco_3mu_NbMuInSameSV, "Reco_3mu_NbMuInSameSV[Reco_3mu_size]/S");
      }
      myTree->Branch("Reco_3mu_vtx", "TClonesArray", &Reco_3mu_vtx, 32000, 0);
    }

    myTree->Branch("Reco_QQ_size", &Reco_QQ_size, "Reco_QQ_size/S");
    myTree->Branch("Reco_QQ_type", Reco_QQ_type, "Reco_QQ_type[Reco_QQ_size]/S");
    myTree->Branch("Reco_QQ_sign", Reco_QQ_sign, "Reco_QQ_sign[Reco_QQ_size]/S");
    myTree->Branch("Reco_QQ_4mom", "TClonesArray", &Reco_QQ_4mom, 32000, 0);
    myTree->Branch("Reco_QQ_mupl_idx", Reco_QQ_mupl_idx, "Reco_QQ_mupl_idx[Reco_QQ_size]/S");
    myTree->Branch("Reco_QQ_mumi_idx", Reco_QQ_mumi_idx, "Reco_QQ_mumi_idx[Reco_QQ_size]/S");

    myTree->Branch("Reco_QQ_trig", Reco_QQ_trig, "Reco_QQ_trig[Reco_QQ_size]/l");
    myTree->Branch("Reco_QQ_isCowboy", Reco_QQ_isCowboy, "Reco_QQ_isCowboy[Reco_QQ_size]/O");
    myTree->Branch("Reco_QQ_ctau", Reco_QQ_ctau, "Reco_QQ_ctau[Reco_QQ_size]/F");
    myTree->Branch("Reco_QQ_ctauErr", Reco_QQ_ctauErr, "Reco_QQ_ctauErr[Reco_QQ_size]/F");
    myTree->Branch("Reco_QQ_cosAlpha", Reco_QQ_cosAlpha, "Reco_QQ_cosAlpha[Reco_QQ_size]/F");
    myTree->Branch("Reco_QQ_ctau3D", Reco_QQ_ctau3D, "Reco_QQ_ctau3D[Reco_QQ_size]/F");
    myTree->Branch("Reco_QQ_ctauErr3D", Reco_QQ_ctauErr3D, "Reco_QQ_ctauErr3D[Reco_QQ_size]/F");
    myTree->Branch("Reco_QQ_cosAlpha3D", Reco_QQ_cosAlpha3D, "Reco_QQ_cosAlpha3D[Reco_QQ_size]/F");

    if (_isMC) {
      myTree->Branch("Reco_QQ_whichGen", Reco_QQ_whichGen, "Reco_QQ_whichGen[Reco_QQ_size]/S");
    }
    myTree->Branch("Reco_QQ_VtxProb", Reco_QQ_VtxProb, "Reco_QQ_VtxProb[Reco_QQ_size]/F");
    myTree->Branch("Reco_QQ_dca", Reco_QQ_dca, "Reco_QQ_dca[Reco_QQ_size]/F");
    myTree->Branch("Reco_QQ_MassErr", Reco_QQ_MassErr, "Reco_QQ_MassErr[Reco_QQ_size]/F");
    myTree->Branch("Reco_QQ_vtx", "TClonesArray", &Reco_QQ_vtx, 32000, 0);

    if ((!_theMinimumFlag && _muonLessPrimaryVertex) || (_flipJpsiDirection > 0)) {
      myTree->Branch("Reco_QQ_mupl_dxy_muonlessVtx", Reco_QQ_mupl_dxy, "Reco_QQ_mupl_dxy_muonlessVtx[Reco_QQ_size]/F");
      myTree->Branch("Reco_QQ_mumi_dxy_muonlessVtx", Reco_QQ_mumi_dxy, "Reco_QQ_mumi_dxy_muonlessVtx[Reco_QQ_size]/F");
      myTree->Branch("Reco_QQ_mupl_dz_muonlessVtx", Reco_QQ_mupl_dz, "Reco_QQ_mupl_dz_muonlessVtx[Reco_QQ_size]/F");
      myTree->Branch("Reco_QQ_mumi_dz_muonlessVtx", Reco_QQ_mumi_dz, "Reco_QQ_mumi_dz_muonlessVtx[Reco_QQ_size]/F");
    }
    if (_flipJpsiDirection > 0) {
      myTree->Branch("Reco_QQ_flipJpsi", Reco_QQ_flipJpsi, "Reco_QQ_flipJpsi[Reco_QQ_size]/S");
      myTree->Branch("Reco_QQ_mumi_4mom", "TClonesArray", &Reco_QQ_mumi_4mom, 32000, 0);
      myTree->Branch("Reco_QQ_mupl_4mom", "TClonesArray", &Reco_QQ_mupl_4mom, 32000, 0);
    }
  }

  myTree->Branch("Reco_mu_size", &Reco_mu_size, "Reco_mu_size/S");
  myTree->Branch("Reco_mu_type", Reco_mu_type, "Reco_mu_type[Reco_mu_size]/S");
  if (_isMC) {
    myTree->Branch("Reco_mu_whichGen", Reco_mu_whichGen, "Reco_mu_whichGen[Reco_mu_size]/S");
  }
  myTree->Branch("Reco_mu_SelectionType", Reco_mu_SelectionType, "Reco_mu_SelectionType[Reco_mu_size]/I");
  myTree->Branch("Reco_mu_charge", Reco_mu_charge, "Reco_mu_charge[Reco_mu_size]/S");
  myTree->Branch("Reco_mu_4mom", "TClonesArray", &Reco_mu_4mom, 32000, 0);
  myTree->Branch("Reco_mu_L1_4mom", "TClonesArray", &Reco_mu_L1_4mom, 32000, 0);
  myTree->Branch("Reco_mu_trig", Reco_mu_trig, "Reco_mu_trig[Reco_mu_size]/l");

  if (!_theMinimumFlag) {
    myTree->Branch("Reco_mu_InTightAcc", Reco_mu_InTightAcc, "Reco_mu_InTightAcc[Reco_mu_size]/O");
    myTree->Branch("Reco_mu_InLooseAcc", Reco_mu_InLooseAcc, "Reco_mu_InLooseAcc[Reco_mu_size]/O");
    myTree->Branch("Reco_mu_highPurity", Reco_mu_highPurity, "Reco_mu_highPurity[Reco_mu_size]/O");
    // myTree->Branch("Reco_mu_TrkMuArb", Reco_mu_TrkMuArb,   "Reco_mu_TrkMuArb[Reco_mu_size]/O");
    myTree->Branch("Reco_mu_TMOneStaTight", Reco_mu_TMOneStaTight, "Reco_mu_TMOneStaTight[Reco_mu_size]/O");

    myTree->Branch("Reco_mu_isPF", Reco_mu_isPF, "Reco_mu_isPF[Reco_mu_size]/O");
    myTree->Branch("Reco_mu_isTracker", Reco_mu_isTracker, "Reco_mu_isTracker[Reco_mu_size]/O");
    myTree->Branch("Reco_mu_isGlobal", Reco_mu_isGlobal, "Reco_mu_isGlobal[Reco_mu_size]/O");
    myTree->Branch("Reco_mu_isSoft", Reco_mu_isSoft, "Reco_mu_isSoft[Reco_mu_size]/O");
    myTree->Branch("Reco_mu_isHybridSoft", Reco_mu_isHybridSoft, "Reco_mu_isHybridSoft[Reco_mu_size]/O");
    myTree->Branch("Reco_mu_isMedium", Reco_mu_isMedium, "Reco_mu_isMedium[Reco_mu_size]/O");

    myTree->Branch("Reco_mu_candType", Reco_mu_candType, "Reco_mu_candType[Reco_mu_size]/S");
    myTree->Branch("Reco_mu_nPixValHits", Reco_mu_nPixValHits, "Reco_mu_nPixValHits[Reco_mu_size]/I");
    myTree->Branch("Reco_mu_nMuValHits", Reco_mu_nMuValHits, "Reco_mu_nMuValHits[Reco_mu_size]/I");
    myTree->Branch("Reco_mu_nMuValHits_inner", Reco_mu_nMuValHits_inner, "Reco_mu_nMuValHits_inner[Reco_mu_size]/I");
    myTree->Branch("Reco_mu_nMuValHits_bestTracker", Reco_mu_nMuValHits_bestTracker, "Reco_mu_nMuValHits_bestTracker[Reco_mu_size]/I");
    myTree->Branch("Reco_mu_nTrkHits", Reco_mu_nTrkHits, "Reco_mu_nTrkHits[Reco_mu_size]/I");
    myTree->Branch("Reco_mu_segmentComp", Reco_mu_segmentComp, "Reco_mu_segmentComp[Reco_mu_size]/F");
    myTree->Branch("Reco_mu_kink", Reco_mu_kink, "Reco_mu_kink[Reco_mu_size]/F");
    myTree->Branch("Reco_mu_localChi2", Reco_mu_localChi2, "Reco_mu_localChi2[Reco_mu_size]/F");
     myTree->Branch("Reco_mu_validFraction", Reco_mu_validFraction, "Reco_mu_validFraction[Reco_mu_size]/F");
    myTree->Branch(
        "Reco_mu_normChi2_bestTracker", Reco_mu_normChi2_bestTracker, "Reco_mu_normChi2_bestTracker[Reco_mu_size]/F");
    myTree->Branch("Reco_mu_normChi2_inner", Reco_mu_normChi2_inner, "Reco_mu_normChi2_inner[Reco_mu_size]/F");
    myTree->Branch("Reco_mu_normChi2_global", Reco_mu_normChi2_global, "Reco_mu_normChi2_global[Reco_mu_size]/F");
    myTree->Branch("Reco_mu_nPixWMea", Reco_mu_nPixWMea, "Reco_mu_nPixWMea[Reco_mu_size]/I");
    myTree->Branch("Reco_mu_nTrkWMea", Reco_mu_nTrkWMea, "Reco_mu_nTrkWMea[Reco_mu_size]/I");
    myTree->Branch("Reco_mu_StationsMatched", Reco_mu_StationsMatched, "Reco_mu_StationsMatched[Reco_mu_size]/I");
    myTree->Branch("Reco_mu_dxy", Reco_mu_dxy, "Reco_mu_dxy[Reco_mu_size]/F");
    myTree->Branch("Reco_mu_dxyErr", Reco_mu_dxyErr, "Reco_mu_dxyErr[Reco_mu_size]/F");
    myTree->Branch("Reco_mu_dz", Reco_mu_dz, "Reco_mu_dz[Reco_mu_size]/F");
    myTree->Branch("Reco_mu_dzErr", Reco_mu_dzErr, "Reco_mu_dzErr[Reco_mu_size]/F");
    myTree->Branch("Reco_mu_pT", Reco_mu_pT, "Reco_mu_pT[Reco_mu_size]/F");
    myTree->Branch("Reco_mu_Eta", Reco_mu_Eta, "Reco_mu_Eta[Reco_mu_size]/F");
    myTree->Branch("Reco_mu_Phi", Reco_mu_Phi, "Reco_mu_Phi[Reco_mu_size]/F");
    // myTree->Branch("Reco_mu_pt_inner",Reco_mu_pt_inner, "Reco_mu_pt_inner[Reco_mu_size]/F");
    // myTree->Branch("Reco_mu_pt_global",Reco_mu_pt_global, "Reco_mu_pt_global[Reco_mu_size]/F");
    myTree->Branch("Reco_mu_ptErr_inner", Reco_mu_ptErr_inner, "Reco_mu_ptErr_inner[Reco_mu_size]/F");
    // myTree->Branch("Reco_mu_ptErr_global",Reco_mu_ptErr_global, "Reco_mu_ptErr_global[Reco_mu_size]/F");
  }

  if (_useGeTracks && _fillRecoTracks) {
    if (!_doDimuTrk) {
      myTree->Branch("Reco_QQ_NtrkPt02", Reco_QQ_NtrkPt02, "Reco_QQ_NtrkPt02[Reco_QQ_size]/I");
      myTree->Branch("Reco_QQ_NtrkPt03", Reco_QQ_NtrkPt03, "Reco_QQ_NtrkPt03[Reco_QQ_size]/I");
      myTree->Branch("Reco_QQ_NtrkPt04", Reco_QQ_NtrkPt04, "Reco_QQ_NtrkPt04[Reco_QQ_size]/I");
      myTree->Branch("Reco_QQ_NtrkDeltaR03", Reco_QQ_NtrkDeltaR03, "Reco_QQ_NtrkDeltaR03[Reco_QQ_size]/I");
      myTree->Branch("Reco_QQ_NtrkDeltaR04", Reco_QQ_NtrkDeltaR04, "Reco_QQ_NtrkDeltaR04[Reco_QQ_size]/I");
      myTree->Branch("Reco_QQ_NtrkDeltaR05", Reco_QQ_NtrkDeltaR05, "Reco_QQ_NtrkDeltaR05[Reco_QQ_size]/I");
    }

    myTree->Branch("Reco_trk_size", &Reco_trk_size, "Reco_trk_size/S");
    myTree->Branch("Reco_trk_charge", Reco_trk_charge, "Reco_trk_charge[Reco_trk_size]/S");
    myTree->Branch("Reco_trk_InLooseAcc", Reco_trk_InLooseAcc, "Reco_trk_InLooseAcc[Reco_trk_size]/O");
    myTree->Branch("Reco_trk_InTightAcc", Reco_trk_InTightAcc, "Reco_trk_InTightAcc[Reco_trk_size]/O");
    myTree->Branch("Reco_trk_4mom", "TClonesArray", &Reco_trk_4mom, 32000, 0);
    myTree->Branch("Reco_trk_dxyError", Reco_trk_dxyError, "Reco_trk_dxyError[Reco_trk_size]/F");
    myTree->Branch("Reco_trk_dzError", Reco_trk_dzError, "Reco_trk_dzError[Reco_trk_size]/F");
    myTree->Branch("Reco_trk_dxy", Reco_trk_dxy, "Reco_trk_dxy[Reco_trk_size]/F");
    myTree->Branch("Reco_trk_dz", Reco_trk_dz, "Reco_trk_dz[Reco_trk_size]/F");
    myTree->Branch("Reco_trk_ptErr", Reco_trk_ptErr, "Reco_trk_ptErr[Reco_trk_size]/F");
    myTree->Branch("Reco_trk_originalAlgo", Reco_trk_originalAlgo, "Reco_trk_originalAlgo[Reco_trk_size]/I");
    myTree->Branch("Reco_trk_nPixWMea", Reco_trk_nPixWMea, "Reco_trk_nPixWMea[Reco_trk_size]/I");
    myTree->Branch("Reco_trk_nTrkWMea", Reco_trk_nTrkWMea, "Reco_trk_nTrkWMea[Reco_trk_size]/I");
    if (_isMC) {
      myTree->Branch("Reco_trk_whichGenmu", Reco_trk_whichGenmu, "Reco_trk_whichGenmu[Reco_trk_size]/S");
    }
  }

  if (_isMC) {
    if (_genealogyInfo) {
      myTree->Branch("Reco_mu_simExtType", Reco_mu_simExtType, "Reco_mu_simExtType[Reco_mu_size]/I");
    }
    myTree->Branch("Gen_weight", &Gen_weight, "Gen_weight/F");
    myTree->Branch("Gen_pthat", &Gen_pthat, "Gen_pthat/F");

    if (!_onlySingleMuons) {
      myTree->Branch("Gen_QQ_size", &Gen_QQ_size, "Gen_QQ_size/S");
      //myTree->Branch("Gen_QQ_type",      Gen_QQ_type,    "Gen_QQ_type[Gen_QQ_size]/S");
      myTree->Branch("Gen_QQ_4mom", "TClonesArray", &Gen_QQ_4mom, 32000, 0);
      myTree->Branch("Gen_QQ_ctau", Gen_QQ_ctau, "Gen_QQ_ctau[Gen_QQ_size]/F");
      myTree->Branch("Gen_QQ_ctau3D", Gen_QQ_ctau3D, "Gen_QQ_ctau3D[Gen_QQ_size]/F");
      myTree->Branch("Gen_QQ_mupl_idx", Gen_QQ_mupl_idx, "Gen_QQ_mupl_idx[Gen_QQ_size]/S");
      myTree->Branch("Gen_QQ_mumi_idx", Gen_QQ_mumi_idx, "Gen_QQ_mumi_idx[Gen_QQ_size]/S");
      myTree->Branch("Gen_QQ_whichRec", Gen_QQ_whichRec, "Gen_QQ_whichRec[Gen_QQ_size]/S");
      if (_genealogyInfo) {
        myTree->Branch("Gen_QQ_momId", Gen_QQ_momId, "Gen_QQ_momId[Gen_QQ_size]/I");
      }

      if (_doTrimuons || _doDimuTrk) {
        myTree->Branch("Gen_QQ_Bc_idx", Gen_QQ_Bc_idx, "Gen_QQ_Bc_idx[Gen_QQ_size]/S");
        myTree->Branch("Gen_Bc_size", &Gen_Bc_size, "Gen_Bc_size/S");
        myTree->Branch("Gen_Bc_4mom", "TClonesArray", &Gen_Bc_4mom, 32000, 0);
        myTree->Branch("Gen_Bc_nuW_4mom", "TClonesArray", &Gen_Bc_nuW_4mom, 32000, 0);
        myTree->Branch("Gen_Bc_QQ_idx", Gen_Bc_QQ_idx, "Gen_Bc_QQ_idx[Gen_Bc_size]/S");
        myTree->Branch("Gen_Bc_muW_idx", Gen_Bc_muW_idx, "Gen_Bc_muW_idx[Gen_Bc_size]/S");
        myTree->Branch("Gen_Bc_pdgId", Gen_Bc_pdgId, "Gen_Bc_pdgId[Gen_Bc_size]/I");
        myTree->Branch("Gen_Bc_ctau", Gen_Bc_ctau, "Gen_Bc_ctau[Gen_Bc_size]/F");

        myTree->Branch("Gen_3mu_4mom", "TClonesArray", &Gen_3mu_4mom, 32000, 0);
        myTree->Branch("Gen_3mu_whichRec", Gen_3mu_whichRec, "Gen_3mu_whichRec[Gen_Bc_size]/S");
      }
    }

    myTree->Branch("Gen_mu_size", &Gen_mu_size, "Gen_mu_size/S");
    //myTree->Branch("Gen_mu_type",   Gen_mu_type,   "Gen_mu_type[Gen_mu_size]/S");
    myTree->Branch("Gen_mu_charge", Gen_mu_charge, "Gen_mu_charge[Gen_mu_size]/S");
    myTree->Branch("Gen_mu_4mom", "TClonesArray", &Gen_mu_4mom, 32000, 0);
    myTree->Branch("Gen_mu_whichRec", Gen_mu_whichRec, "Gen_mu_whichRec[Gen_mu_size]/S");
  }

  return;
}

// ------------ method called once each job just before starting event loop  ------------
void HiOniaAnalyzer::beginJob() {
  //fOut = new TFile(_histfilename.c_str(), "RECREATE");
  InitTree();

  // book histos
  //hGoodMuonsNoTrig = new TH1F("hGoodMuonsNoTrig","hGoodMuonsNoTrig",10,0,10);
  hGoodMuonsNoTrig = fs->make<TH1F>("hGoodMuonsNoTrig", "hGoodMuonsNoTrig", 10, 0, 10);
  //hGoodMuons = new TH1F("hGoodMuons","hGoodMuons",10,0,10);
  hGoodMuons = fs->make<TH1F>("hGoodMuons", "hGoodMuons", 10, 0, 10);
  //hL1DoubleMu0 = new TH1F("hL1DoubleMu0","hL1DoubleMu0",10,0,10);
  hL1DoubleMu0 = fs->make<TH1F>("hL1DoubleMu0", "hL1DoubleMu0", 10, 0, 10);

  hGoodMuonsNoTrig->Sumw2();
  hGoodMuons->Sumw2();
  hL1DoubleMu0->Sumw2();

  // muons
  if (_combineCategories)
    myRecoMuonHistos = new MyCommonHistoManager("RecoMuon");
  else {
    myRecoGlbMuonHistos = new MyCommonHistoManager("GlobalMuon");
    myRecoTrkMuonHistos = new MyCommonHistoManager("TrackerMuon");
  }

  // J/psi
  if (_combineCategories)
    myRecoJpsiHistos = new MyCommonHistoManager("RecoJpsi");
  else {
    myRecoJpsiGlbGlbHistos = new MyCommonHistoManager("GlbGlbJpsi");
    myRecoJpsiGlbTrkHistos = new MyCommonHistoManager("GlbTrkJpsi");
    myRecoJpsiTrkTrkHistos = new MyCommonHistoManager("TrkTrkJpsi");
  }

  for (unsigned int i = 0; i < theRegions.size(); ++i) {
    for (unsigned int j = 0; j < NTRIGGERS; ++j) {
      for (unsigned int k = 0; k < theCentralities.size(); ++k) {
        std::string theAppendix = theRegions.at(i);
        theAppendix += "_" + theTriggerNames.at(j);
        theAppendix += "_" + theCentralities.at(k);

        // muons
        if (_combineCategories) {
          myRecoMuonHistos->Add(theAppendix);
          myRecoMuonHistos->GetHistograms(theAppendix)->SetMassBinning(1, 0.10, 0.11);
          myRecoMuonHistos->GetHistograms(theAppendix)->SetPtBinning(200, 0.0, 100.0);
        } else {
          myRecoGlbMuonHistos->Add(theAppendix);
          myRecoTrkMuonHistos->Add(theAppendix);

          myRecoGlbMuonHistos->GetHistograms(theAppendix)->SetMassBinning(1, 0.10, 0.11);
          myRecoGlbMuonHistos->GetHistograms(theAppendix)->SetPtBinning(200, 0.0, 100.0);

          myRecoTrkMuonHistos->GetHistograms(theAppendix)->SetMassBinning(1, 0.10, 0.11);
          myRecoTrkMuonHistos->GetHistograms(theAppendix)->SetPtBinning(200, 0.0, 100.0);
        }

        for (unsigned int l = 0; l < theSign.size(); ++l) {
          // J/psi
          if (_combineCategories)
            myRecoJpsiHistos->Add(theAppendix + "_" + theSign.at(l));
          else {
            myRecoJpsiGlbGlbHistos->Add(theAppendix + "_" + theSign.at(l));
            myRecoJpsiGlbTrkHistos->Add(theAppendix + "_" + theSign.at(l));
            myRecoJpsiTrkTrkHistos->Add(theAppendix + "_" + theSign.at(l));
          }
        }
      }
    }
  }
  /*
  if (_combineCategories)
    myRecoMuonHistos->Print();
  else
    myRecoGlbMuonHistos->Print();
  */
  //hStats = new TH1F("hStats","hStats;;Number of Events",2*NTRIGGERS+1,0,2*NTRIGGERS+1);
  hStats = fs->make<TH1F>("hStats", "hStats;;Number of Events", 2 * NTRIGGERS + 1, 0, 2 * NTRIGGERS + 1);
  hStats->GetXaxis()->SetBinLabel(1, "All");
  for (int i = 2; i < (int)theTriggerNames.size() + 1; ++i) {
    hStats->GetXaxis()->SetBinLabel(i, theTriggerNames.at(i - 1).c_str());              // event info
    hStats->GetXaxis()->SetBinLabel(i + NTRIGGERS, theTriggerNames.at(i - 1).c_str());  // muon pair info
  }
  hStats->Sumw2();

  //hCent = new TH1F("hCent","hCent;centrality bin;Number of Events",200,0,200);
  hCent = fs->make<TH1F>("hCent", "hCent;centrality bin;Number of Events", 200, 0, 200);
  hCent->Sumw2();

  //hPileUp = new TH1F("hPileUp","Number of Primary Vertices;n_{PV};counts", 50, 0, 50);
  hPileUp = fs->make<TH1F>("hPileUp", "Number of Primary Vertices;n_{PV};counts", 50, 0, 50);
  hPileUp->Sumw2();

  //hZVtx = new TH1F("hZVtx","Primary z-vertex distribution;z_{vtx} [cm];counts", 120, -30, 30);
  hZVtx = fs->make<TH1F>("hZVtx", "Primary z-vertex distribution;z_{vtx} [cm];counts", 120, -30, 30);
  hZVtx->Sumw2();

  return;
}

void HiOniaAnalyzer::beginRun(const edm::Run& iRun, const edm::EventSetup& iSetup) {
  //init HLTConfigProvider

  EDConsumerBase::Labels labelTriggerResults;
  EDConsumerBase::labelsForToken(_tagTriggerResultsToken, labelTriggerResults);
  const std::string pro = labelTriggerResults.process;

  //bool init(const edm::Run& iRun, const edm::EventSetup& iSetup, const std::string& processName, bool& changed);
  bool changed = true;
  hltConfigInit = hltConfig.init(iRun, iSetup, pro, changed);

  changed = true;
  hltPrescaleInit = hltPrescaleProvider.init(iRun, iSetup, pro, changed);

  return;
}

// ------------ method called once each job just after ending the event loop  ------------
void HiOniaAnalyzer::endJob() {
  std::cout << "Total number of events = " << nEvents << std::endl;
  std::cout << "Total number of passed candidates = " << passedCandidates << std::endl;
  /*
  fOut->cd();
  hStats->Write();
  hCent->Write();
  hPileUp->Write();
  hZVtx->Write();
  if (_fillTree)
    myTree->Write();
  hGoodMuonsNoTrig->Write();
  hGoodMuons->Write();
  hL1DoubleMu0->Write();
  if (_fillHistos) {
    // muons
    if (_combineCategories)
      myRecoGlbMuonHistos->Write(fOut);
    else {
      myRecoGlbMuonHistos->Write(fOut);
      myRecoTrkMuonHistos->Write(fOut);
    }
    // J/psi
    if (_combineCategories)
      myRecoJpsiHistos->Write(fOut);
    else {
      myRecoJpsiGlbGlbHistos->Write(fOut);
      myRecoJpsiGlbTrkHistos->Write(fOut);
      myRecoJpsiTrkTrkHistos->Write(fOut);
    }
  }
  */
  return;
}

TLorentzVector HiOniaAnalyzer::lorentzMomentum(const reco::Candidate::LorentzVector& p) {
  TLorentzVector res;
  res.SetPtEtaPhiM(p.pt(), p.eta(), p.phi(), p.mass());

  return res;
}

void HiOniaAnalyzer::hltReport(const edm::Event& iEvent, const edm::EventSetup& iSetup) {
  std::map<std::string, bool> mapTriggernameToTriggerFired;
  std::map<std::string, unsigned int> mapTriggernameToHLTbit;

  for (std::vector<std::string>::const_iterator it = theTriggerNames.begin(); it != theTriggerNames.end(); ++it) {
    mapTriggernameToTriggerFired[*it] = false;
    mapTriggernameToHLTbit[*it] = 1000;
  }

  // HLTConfigProvider
  if (hltConfigInit) {
    //! Use HLTConfigProvider
    const unsigned int n = hltConfig.size();
    for (std::map<std::string, unsigned int>::iterator it = mapTriggernameToHLTbit.begin();
         it != mapTriggernameToHLTbit.end();
         it++) {
      unsigned int triggerIndex = hltConfig.triggerIndex(it->first);
      if (it->first == "NoTrigger")
        continue;
      if (triggerIndex >= n) {
        if (_checkTrigNames)
          std::cout << "[HiOniaAnalyzer::hltReport] --- TriggerName " << it->first << " not available in config!"
                    << std::endl;
      } else {
        it->second = triggerIndex;
        //      std::cout << "[HiOniaAnalyzer::hltReport] --- TriggerName " << it->first << " available in config!" << std::endl;
      }
    }
  }

  // Get Trigger Results
  iEvent.getByToken(_tagTriggerResultsToken, collTriggerResults);
  if (collTriggerResults.isValid() && (collTriggerResults->size() == hltConfig.size())) {
    //    std::cout << "[HiOniaAnalyzer::hltReport] --- J/psi TriggerResults IS valid in current event" << std::endl;

    // loop over Trigger Results to check if paths was fired
    for (std::vector<std::string>::iterator itHLTNames = theTriggerNames.begin(); itHLTNames != theTriggerNames.end();
         itHLTNames++) {
      const std::string triggerPathName = *itHLTNames;
      if (mapTriggernameToHLTbit[triggerPathName] < 1000) {
        if (collTriggerResults->accept(mapTriggernameToHLTbit[triggerPathName])) {
          mapTriggerNameToIntFired_[triggerPathName] = 3;
        }
        if (_isMC) {
          mapTriggerNameToPrescaleFac_[triggerPathName] = 1;
        } else {
          //-------prescale factor------------
          if (hltPrescaleInit && hltPrescaleProvider.prescaleSet(iEvent, iSetup) >= 0) {
            std::pair<std::vector<std::pair<std::string, int> >, int> detailedPrescaleInfo =
                hltPrescaleProvider.prescaleValuesInDetail(iEvent, iSetup, triggerPathName);
            //get HLT prescale info from hltPrescaleProvider
            const int hltPrescale = detailedPrescaleInfo.second;
            //get L1 prescale info from hltPrescaleProvider
            int l1Prescale = -1;
            if (detailedPrescaleInfo.first.size() == 1) {
              l1Prescale = detailedPrescaleInfo.first.at(0).second;
            } else if (detailedPrescaleInfo.first.size() > 1) {
              l1Prescale =
                  1;  // Means it is a complex l1 seed, and for us it is only Mu3 OR Mu5, both of them unprescaled at L1
            } else if (_checkTrigNames) {
              std::cout << "[HiOniaAnalyzer::hltReport] --- L1 prescale was NOT found for TriggerName "
                        << triggerPathName << " , default L1 prescale value set to 1 " << std::endl;
            }
            //compute the total prescale = HLT prescale * L1 prescale
            mapTriggerNameToPrescaleFac_[triggerPathName] = hltPrescale * l1Prescale;
          }
        }
      }
    }
  } else
    std::cout << "[HiOniaAnalyzer::hltReport] --- TriggerResults NOT valid in current event" << std::endl;

  return;
}

bool HiOniaAnalyzer::isAbHadron(int pdgID) {
  if (abs(pdgID) == 511 || abs(pdgID) == 521 || abs(pdgID) == 531 || abs(pdgID) == 5122 || abs(pdgID) == 541)
    return true;
  return false;
}

bool HiOniaAnalyzer::isNeutrino(int pdgID) {
  if (abs(pdgID) == 14 || abs(pdgID) == 16 || abs(pdgID) == 18)
    return true;
  return false;
}

bool HiOniaAnalyzer::isAMixedbHadron(int pdgID, int momPdgID) {
  if ((abs(pdgID) == 511 && abs(momPdgID) == 511 && pdgID * momPdgID < 0) ||
      (abs(pdgID) == 531 && abs(momPdgID) == 531 && pdgID * momPdgID < 0))
    return true;
  return false;
}

reco::GenParticleRef HiOniaAnalyzer::findMotherRef(reco::GenParticleRef GenParticleMother, int GenParticlePDG) {
  for (int i = 0; i < 1000; ++i) {
    if (GenParticleMother.isNonnull() && (GenParticleMother->pdgId() == GenParticlePDG) &&
        GenParticleMother->numberOfMothers() > 0) {
      GenParticleMother = GenParticleMother->motherRef();
    } else
      break;
  }
  return GenParticleMother;
}

bool HiOniaAnalyzer::isChargedTrack(int pdgId) {
  return ((fabs(pdgId) == 211) || (fabs(pdgId) == 321) || (fabs(pdgId) == 2212) || (fabs(pdgId) == 11) ||
          (fabs(pdgId) == 13));
}

std::vector<reco::GenParticleRef> HiOniaAnalyzer::GenBrothers(reco::GenParticleRef GenParticleMother, int GenJpsiPDG) {
  bool foundJpsi = false;
  std::vector<reco::GenParticleRef> res;

  if (!GenParticleMother.isNonnull())
    return res;
  //if(Reco_3mu_size>0) cout<<"\nScanning daughters of Jpsi "<<GenJpsiPDG<<" mother, pdg = "<<GenParticleMother->pdgId()<<endl;
  for (int i = 0; i < (int)GenParticleMother->numberOfDaughters(); i++) {
    reco::GenParticleRef dau = findDaughterRef(GenParticleMother->daughterRef(i), GenParticleMother->pdgId());
    for (int l = 0; l < 100; l++) {  //avoid having a daughter of same pdgId
      if (!(dau.isNonnull() && dau->status() > 0 && dau->status() < 1000))
        break;
      if (dau->pdgId() == GenParticleMother->pdgId() && dau->numberOfDaughters() == 1)
        dau = findDaughterRef(dau->daughterRef(0), dau->pdgId());
      else
        break;
    }

    if (!(dau.isNonnull() && dau->status() > 0 && dau->status() < 1000))
      continue;
    //if(Reco_3mu_size>0) cout<<"Daughter #"<<i<<" pdg = "<< dau->pdgId()<<" pt,eta = "<<dau->pt()<<" "<<dau->eta()<<endl;
    if (isChargedTrack(dau->pdgId())) {
      res.push_back(dau);
    }
    if (dau->pdgId() == GenJpsiPDG) {
      foundJpsi = true;  //continue;
    }

    for (int j = 0; j < (int)dau->numberOfDaughters(); j++) {
      reco::GenParticleRef grandDau = findDaughterRef(dau->daughterRef(j), dau->pdgId());
      for (int l = 0; l < 100; l++) {  //avoid having a daughter of same pdgId
        if (!(grandDau.isNonnull() && grandDau->status() > 0 && grandDau->status() < 1000))
          break;
        if (grandDau->pdgId() == dau->pdgId() && grandDau->numberOfDaughters() == 1)
          grandDau = findDaughterRef(grandDau->daughterRef(0), grandDau->pdgId());
        else
          break;
      }

      if (!(grandDau.isNonnull() && grandDau->status() > 0 && grandDau->status() < 1000))
        continue;
      //if(Reco_3mu_size>0) cout<<"    grand-daughter #"<<j<<" pdg = "<< grandDau->pdgId()<<" pt,eta = "<<grandDau->pt()<<" "<<grandDau->eta()<<endl;
      if (isChargedTrack(grandDau->pdgId())) {
        res.push_back(grandDau);
      }
      if (grandDau->pdgId() == GenJpsiPDG) {
        foundJpsi = true;  //continue;
      }

      for (int k = 0; k < (int)grandDau->numberOfDaughters(); k++) {
        reco::GenParticleRef ggrandDau = findDaughterRef(grandDau->daughterRef(k), grandDau->pdgId());
        for (int l = 0; l < 100; l++) {  //avoid having a daughter of same pdgId
          if (!(ggrandDau.isNonnull() && ggrandDau->status() > 0 && ggrandDau->status() < 1000))
            break;
          if (ggrandDau->pdgId() == grandDau->pdgId() && ggrandDau->numberOfDaughters() == 1)
            ggrandDau = findDaughterRef(ggrandDau->daughterRef(0), ggrandDau->pdgId());
          else
            break;
        }

        if (!(ggrandDau.isNonnull() && ggrandDau->status() > 0 && ggrandDau->status() < 1000))
          continue;
        //if(Reco_3mu_size>0) cout<<"        grand-grand-daughter #"<<k<<" pdg = "<< ggrandDau->pdgId()<<" pt,eta = "<<ggrandDau->pt()<<" "<<ggrandDau->eta()<<endl;
        if (isChargedTrack(ggrandDau->pdgId())) {
          res.push_back(ggrandDau);
        }
        if (ggrandDau->pdgId() == GenJpsiPDG) {
          foundJpsi = true;  //continue;
        }
      }
    }
  }

  if (!foundJpsi) {
    cout << "!!!!!!!!!!!!!!!!!!!!!!!!!!!! Incoherence in genealogy: Jpsi not found in the daughters!\n" << endl;
  }
  // if(!isAbHadron(GenParticleMother->pdgId())){
  //   cout<<"\n!!!!!!!!!!!!!!!!!!!!!!!!!!!! Jpsi ancestor is not a b-hadron! pdgID(mother of this ancestor) = "<<findMotherRef(GenParticleMother->motherRef() , GenParticleMother->pdgId())->pdgId()<<endl;
  // }

  return res;
}

std::pair<std::vector<reco::GenParticleRef>, std::pair<float, float> > HiOniaAnalyzer::findGenMCInfo(
    const reco::GenParticle* genJpsi) {
  float trueLife = -99.;
  float trueLife3D = -99.;
  std::vector<reco::GenParticleRef> JpsiBrothers;

  if (genJpsi->numberOfMothers() > 0) {
    TVector3 trueVtx(0.0, 0.0, 0.0);
    TVector3 trueP(0.0, 0.0, 0.0);
    TVector3 trueVtxMom(0.0, 0.0, 0.0);

    trueVtx.SetXYZ(genJpsi->vertex().x(), genJpsi->vertex().y(), genJpsi->vertex().z());
    trueP.SetXYZ(genJpsi->momentum().x(), genJpsi->momentum().y(), genJpsi->momentum().z());

    bool aBhadron = false;
    reco::GenParticleRef Jpsimom_final;
    reco::GenParticleRef Jpsimom = findMotherRef(genJpsi->motherRef(), genJpsi->pdgId());

    if (Jpsimom.isNull()) {
      std::pair<float, float> trueLifePair = std::make_pair(trueLife, trueLife3D);
      std::pair<std::vector<reco::GenParticleRef>, std::pair<float, float> > result =
          std::make_pair(JpsiBrothers, trueLifePair);
      return result;
    } else if (Jpsimom->numberOfMothers() <= 0) {
      if (isAbHadron(Jpsimom->pdgId())) {
        Jpsimom_final = Jpsimom;
        aBhadron = true;
      }
    }

    else {
      reco::GenParticleRef Jpsigrandmom = findMotherRef(Jpsimom->motherRef(), Jpsimom->pdgId());
      if (isAbHadron(Jpsimom->pdgId())) {
        if (Jpsigrandmom.isNonnull() && isAMixedbHadron(Jpsimom->pdgId(), Jpsigrandmom->pdgId())) {
          Jpsimom_final = Jpsigrandmom;
        } else {
          Jpsimom_final = Jpsimom;
        }
        aBhadron = true;
      }

      else if (Jpsigrandmom.isNonnull() && isAbHadron(Jpsigrandmom->pdgId())) {
        if (Jpsigrandmom->numberOfMothers() <= 0) {
          Jpsimom_final = Jpsigrandmom;
        } else {
          reco::GenParticleRef JpsiGrandgrandmom = findMotherRef(Jpsigrandmom->motherRef(), Jpsigrandmom->pdgId());
          if (JpsiGrandgrandmom.isNonnull() && isAMixedbHadron(Jpsigrandmom->pdgId(), JpsiGrandgrandmom->pdgId())) {
            Jpsimom_final = JpsiGrandgrandmom;
          } else {
            Jpsimom_final = Jpsigrandmom;
          }
        }
        aBhadron = true;
      }

      //This is to forcefully find the b-like mother of Jpsi
      else if (Jpsigrandmom.isNonnull() && Jpsigrandmom->numberOfMothers() > 0) {
        reco::GenParticleRef JpsiGrandgrandmom = findMotherRef(Jpsigrandmom->motherRef(), Jpsigrandmom->pdgId());
        if (JpsiGrandgrandmom.isNonnull() && isAbHadron(JpsiGrandgrandmom->pdgId())) {
          Jpsimom_final = JpsiGrandgrandmom;
          aBhadron = true;
        }
      }
    }
    if (!aBhadron) {
      Jpsimom_final = Jpsimom;
    }

    if (Jpsimom_final.isNonnull()) {
      trueVtxMom.SetXYZ(Jpsimom_final->vertex().x(), Jpsimom_final->vertex().y(), Jpsimom_final->vertex().z());
      if (_genealogyInfo && Reco_3mu_size > 0) {
        JpsiBrothers = GenBrothers(Jpsimom_final, genJpsi->pdgId());
      }
      JpsiBrothers.insert(JpsiBrothers.begin(), Jpsimom_final);
    }

    TVector3 vdiff = trueVtx - trueVtxMom;
    trueLife = vdiff.Perp() * JpsiPDGMass / trueP.Perp();
    trueLife3D = vdiff.Mag() * JpsiPDGMass / trueP.Mag();
  }

  std::pair<float, float> trueLifePair = std::make_pair(trueLife, trueLife3D);
  std::pair<std::vector<reco::GenParticleRef>, std::pair<float, float> > result =
      std::make_pair(JpsiBrothers, trueLifePair);
  return result;
}

std::pair<int, std::pair<float, float> > HiOniaAnalyzer::findGenBcInfo(reco::GenParticleRef genBc,
                                                                       const reco::GenParticle* genJpsi) {
  int momBcID = 0;
  float trueLife = -99.;

  TVector3 trueVtx(0.0, 0.0, 0.0);
  TVector3 trueP(0.0, 0.0, 0.0);
  TVector3 trueVtxMom(0.0, 0.0, 0.0);

  trueVtx.SetXYZ(genJpsi->vertex().x(), genJpsi->vertex().y(), genJpsi->vertex().z());
  trueVtxMom.SetXYZ(genBc->vertex().x(), genBc->vertex().y(), genBc->vertex().z());
  trueP.SetXYZ(genBc->momentum().x(), genBc->momentum().y(), genBc->momentum().z());

  TVector3 vdiff = trueVtx - trueVtxMom;
  trueLife = vdiff.Perp() * BcPDGMass / trueP.Perp();

  std::pair<float, float> trueLifePair = std::make_pair(trueLife, trueLife);
  std::pair<int, std::pair<float, float> > result = std::make_pair(momBcID, trueLifePair);
  return result;
}

int HiOniaAnalyzer::muonIDmask(const pat::Muon* muon) {
  int mask = 0;
  int type;
  for (type = muon::All; type <= muon::RPCMuLoose; type++)
    if (muon->hasUserInt(Form("muonID_%d", type)) ? muon->userInt(Form("muonID_%d", type))
                                                  : muon::isGoodMuon(*muon, muon::SelectionType(type)))
      mask = mask | (int)pow(2, type);

  return mask;
}

long int HiOniaAnalyzer::FloatToIntkey(float v) {
  float vres = fabs(v);
  while (vres > 0.1)
    vres = vres / 10;                  //Assume argument v is always above 0.1, true for abs(Pt)
  return (long int)(10000000 * vres);  // Precision 10^-6 (i.e. 7-1) on the comparison
}

//define this as a plug-in
DEFINE_FWK_MODULE(HiOniaAnalyzer);
