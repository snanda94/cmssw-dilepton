#ifndef HLTINFO_H
#define HLTINFO_H

#include "TH1.h"
#include "TH2.h"
#include "TFile.h"
#include "TNamed.h"
#include <memory>
#include <vector>
#include <map>
#include "TROOT.h"
#include "TChain.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "DataFormats/Common/interface/TriggerResults.h"
#include "DataFormats/L1TCalorimeter/interface/CaloTower.h"
#include "DataFormats/L1Trigger/interface/EGamma.h"
#include "DataFormats/L1Trigger/interface/Muon.h"
#include "DataFormats/L1Trigger/interface/Jet.h"
#include "DataFormats/L1Trigger/interface/Tau.h"
#include "DataFormats/L1Trigger/interface/EtSum.h"
#include "DataFormats/Candidate/interface/Candidate.h"

//ccla
#include "FWCore/Framework/interface/EventPrincipal.h"
#include "FWCore/Common/interface/Provenance.h"

#include "HLTrigger/HLTcore/interface/HLTPrescaleProvider.h"

#include "L1Trigger/RegionalCaloTrigger/interface/L1RCTProducer.h" 
#include "DataFormats/L1GlobalTrigger/interface/L1GlobalTriggerReadoutRecord.h"
#include "DataFormats/L1GlobalTrigger/interface/L1GlobalTriggerReadoutSetupFwd.h"

namespace edm {
  class ConsumesCollector;
  class ParameterSet;
}

typedef std::vector<std::string> MyStrings;

/** \class HLTInfo
  *  
  * $Date: November 2006
  * $Revision: 
  * \author P. Bargassa - Rice U.
  * $Date: July 2016
  * $Revision: Updated for L1 Stage 2 
  * \author A. Stahl - Ecole Polytechnique
  */
class HLTInfo {
public:

  template <typename T>
  HLTInfo(edm::ParameterSet const& pset,
          edm::ConsumesCollector&& iC,
          T& module);

  template <typename T>
  HLTInfo(edm::ParameterSet const& pset,
          edm::ConsumesCollector& iC,
          T& module);

  void setup(const edm::ParameterSet& pSet, TTree* tree);
  void beginRun(const edm::Run& , const edm::EventSetup& );

  /** Analyze the Data */
  void analyze(const edm::Handle<edm::TriggerResults>                 & hltresults,
               const edm::Handle<L1GlobalTriggerReadoutRecord>        & L1GTRR,
               const edm::Handle< BXVector<l1t::EGamma> >             & L1Stage2EGamma,
               const edm::Handle< BXVector<l1t::Muon> >               & L1Stage2Muon,
               const edm::Handle< BXVector<l1t::Jet> >                & L1Stage2Jet,
               const edm::Handle< BXVector<l1t::Tau> >                & L1Stage2Tau,
               const edm::Handle< BXVector<l1t::EtSum> >              & L1Stage2EtSum,
               const edm::Handle< BXVector<l1t::CaloTower> >          & L1Stage2CaloTower,  
	       edm::EventSetup const& eventSetup,
	       edm::Event const& iEvent,
	       TTree* tree);

private:
 
  HLTInfo();

  // Tree variables
  float *hltppt, *hltpeta;
  float *l1stage2eget, *l1stage2ege, *l1stage2egeta, *l1stage2egphi;
  float *l1stage2mupt, *l1stage2mue, *l1stage2mueta, *l1stage2muphi;
  int   *l1stage2muchg, *l1stage2muiso, *l1stage2muqul;
  float *l1stage2jtet, *l1stage2jte, *l1stage2jteta, *l1stage2jtphi;
  float *l1stage2tauet, *l1stage2taue, *l1stage2taueta, *l1stage2tauphi;
  int   *l1stage2etset, *l1stage2etsphi;
  int   *l1stage2ctetem, *l1stage2cteth, *l1stage2ctetc, *l1stage2cteta, *l1stage2ctphi;
  int   *l1stage2etstype;
  int   *l1stage2egbx, *l1stage2mubx, *l1stage2jtbx, *l1stage2taubx, *l1stage2etsbx, *l1stage2ctbx;
  int   L1TEvtCnt,L1EvtCnt,HltEvtCnt,nhltpart,nl1stage2eg,nl1stage2mu,nl1stage2jet,nl1stage2tau,nl1stage2ets,nl1stage2ct;
  int   *trigflag, *l1TFinalFlag, *l1flag, *l1techflag;
  int   *trigPrescl, *l1TPrescl, *l1Prescl, *l1techPrescl; 

  std::map<int,TString> techBitToName;
  std::map<int,TString> algoBitToName;
  std::vector<std::string> dummyBranches_;

  std::unique_ptr<HLTPrescaleProvider> hltPrescaleProvider_;
  std::string processName_;

  // input variables
  bool _Debug;
};

template <typename T>
HLTInfo::HLTInfo(edm::ParameterSet const& pset,
                 edm::ConsumesCollector&& iC,
                 T& module) :
  HLTInfo(pset, iC, module) {
}

template <typename T>
HLTInfo::HLTInfo(edm::ParameterSet const& pset,
                 edm::ConsumesCollector& iC,
                 T& module) :
    HLTInfo() {
    hltPrescaleProvider_.reset(new HLTPrescaleProvider(pset, iC, module));
}

#endif
