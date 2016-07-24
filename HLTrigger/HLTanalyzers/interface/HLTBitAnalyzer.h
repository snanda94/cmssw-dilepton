#include <iostream>

#include "HLTrigger/HLTanalyzers/interface/EventHeader.h"
#include "HLTrigger/HLTanalyzers/interface/HLTInfo.h"

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Framework/interface/ESHandle.h"

#include "DataFormats/Common/interface/Handle.h"

#include "DataFormats/L1GlobalTrigger/interface/L1GlobalTriggerReadoutRecord.h"
#include "DataFormats/L1GlobalTrigger/interface/L1GlobalTriggerReadoutSetupFwd.h"

/** \class HLTBitAnalyzer
  *  
  * $Date: November 2006
  * $Revision: 
  * \author P. Bargassa - Rice U.
  * $Date: July 2016
  * $Revision: Updated for L1 Stage 2 
  * \author A. Stahl - Ecole Polytechnique
  */

class HLTBitAnalyzer : public edm::EDAnalyzer {
public:
  explicit HLTBitAnalyzer(edm::ParameterSet const& conf);
  virtual void analyze(edm::Event const& e, edm::EventSetup const& iSetup);
  virtual void endJob();
  virtual void beginRun(edm::Run const&, edm::EventSetup const&);

  //  static void fillDescriptions(edm::ConfigurationDescriptions & descriptions); 

  // Analysis tree to be filled
  TTree *HltTree;

private:
  // variables persistent across events should be declared here.
  //
  ///Default analyses

  EventHeader evt_header_;
  HLTInfo     hlt_analysis_;

  edm::InputTag hltresults_;
  edm::InputTag gtReadoutRecord_;
  edm::InputTag m_l1stage2mu;
  edm::InputTag m_l1stage2eg;
  edm::InputTag m_l1stage2jet;
  edm::InputTag m_l1stage2tau;
  edm::InputTag m_l1stage2ets;
  edm::InputTag m_l1stage2ct;

  edm::EDGetTokenT<edm::TriggerResults>                  hltresultsToken_;
  edm::EDGetTokenT<L1GlobalTriggerReadoutRecord>         gtReadoutRecordToken_;
  edm::EDGetTokenT< BXVector<l1t::Muon> >                l1stage2muToken_;
  edm::EDGetTokenT< BXVector<l1t::EGamma> >              l1stage2egToken_;
  edm::EDGetTokenT< BXVector<l1t::Jet> >                 l1stage2jetToken_; 
  edm::EDGetTokenT< BXVector<l1t::Tau> >                 l1stage2tauToken_;
  edm::EDGetTokenT< BXVector<l1t::EtSum> >               l1stage2etsToken_;
  edm::EDGetTokenT< BXVector<l1t::CaloTower> >           l1stage2ctToken_;

  int errCnt;
  static int errMax() { return 5; }

  std::string _HistName; // Name of histogram file
  double _EtaMin,_EtaMax;
  TFile* m_file; // pointer to Histogram file
  bool _UseTFileService;

};
