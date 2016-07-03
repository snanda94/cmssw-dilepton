#include <iostream>
#include <sstream>
#include <istream>
#include <fstream>
#include <iomanip>
#include <string>
#include <cmath>
#include <functional>
#include <stdlib.h>
#include <string.h>

#include "HLTrigger/HLTanalyzers/interface/HLTInfo.h"
#include "FWCore/Common/interface/TriggerNames.h"

// L1 related
#include "HLTrigger/HLTcore/interface/HLTConfigProvider.h"
#include "L1Trigger/GlobalTriggerAnalyzer/interface/L1GtUtils.h"
#include "CondFormats/L1TObjects/interface/L1GtTriggerMenu.h"
#include "CondFormats/DataRecord/interface/L1GtTriggerMenuRcd.h"
#include "DataFormats/L1GlobalTrigger/interface/L1GlobalTriggerReadoutSetupFwd.h"
#include "DataFormats/L1GlobalTrigger/interface/L1GlobalTriggerReadoutSetup.h"
#include "DataFormats/L1GlobalTrigger/interface/L1GlobalTriggerReadoutRecord.h"

HLTInfo::HLTInfo() {

  //set parameter defaults 
  _Debug=false;
  _OR_BXes=false;
  UnpackBxInEvent=1;
}

void HLTInfo::beginRun(const edm::Run& run, const edm::EventSetup& c){ 


  bool changed(true);
  if (hltPrescaleProvider_->init(run,c,processName_,changed)) {
    // if init returns TRUE, initialisation has succeeded!
    if (changed) {
      // The HLT config has actually changed wrt the previous Run, hence rebook your
      // histograms or do anything else dependent on the revised HLT config
      std::cout << "Initalizing HLTConfigProvider"  << std::endl;
    }
  } else {
    // if init returns FALSE, initialisation has NOT succeeded, which indicates a problem
    // with the file and/or code and needs to be investigated!
    std::cout << " HLT config extraction failure with process name " << processName_ << std::endl;
    // In this case, all access methods will return empty values!
  }

}

/*  Setup the analysis to put the branch-variables into the tree. */
void HLTInfo::setup(const edm::ParameterSet& pSet, TTree* HltTree) {

  processName_ = pSet.getParameter<std::string>("HLTProcessName") ;

  edm::ParameterSet myHltParams = pSet.getParameter<edm::ParameterSet>("RunParameters") ;
  std::vector<std::string> parameterNames = myHltParams.getParameterNames() ;
  
  for ( std::vector<std::string>::iterator iParam = parameterNames.begin();
        iParam != parameterNames.end(); iParam++ ){
    if ( (*iParam) == "Debug" ) _Debug =  myHltParams.getParameter<bool>( *iParam );
  }

  dummyBranches_ = pSet.getUntrackedParameter<std::vector<std::string> >("dummyBranches",std::vector<std::string>(0));

  HltEvtCnt = 0;
  const int kMaxTrigFlag = 10000;
  trigflag = new int[kMaxTrigFlag];
  trigPrescl = new int[kMaxTrigFlag];
  L1EvtCnt = 0;
  const int kMaxL1Flag = 10000;
  l1flag = new int[kMaxL1Flag];
  l1flag5Bx = new int[kMaxTrigFlag];
  l1Prescl = new int[kMaxL1Flag];
  l1techflag = new int[kMaxL1Flag];
  l1techflag5Bx = new int[kMaxTrigFlag];
  l1techPrescl = new int[kMaxTrigFlag];
  const int kMaxHLTPart = 10000;
  hltppt = new float[kMaxHLTPart];
  hltpeta = new float[kMaxHLTPart];
  const int kMaxL1ExtMu = 10000;
  l1extmupt = new float[kMaxL1ExtMu];
  l1extmue = new float[kMaxL1ExtMu];
  l1extmueta = new float[kMaxL1ExtMu];
  l1extmuphi = new float[kMaxL1ExtMu];
  l1extmuiso = new int[kMaxL1ExtMu];
  l1extmumip = new int[kMaxL1ExtMu];
  l1extmufor = new int[kMaxL1ExtMu];
  l1extmurpc = new int[kMaxL1ExtMu];
  l1extmuqul = new int[kMaxL1ExtMu];
  l1extmuchg = new int[kMaxL1ExtMu];

  algoBitToName = new TString[128];
  techBitToName = new TString[128];

  HltTree->Branch("NL1Mu",&nl1extmu,"NL1Mu/I");
  HltTree->Branch("L1MuPt",l1extmupt,"L1MuPt[NL1Mu]/F");
  HltTree->Branch("L1MuE",l1extmue,"L1MuE[NL1Mu]/F");
  HltTree->Branch("L1MuEta",l1extmueta,"L1MuEta[NL1Mu]/F");
  HltTree->Branch("L1MuPhi",l1extmuphi,"L1MuPhi[NL1Mu]/F");
  HltTree->Branch("L1MuIsol",l1extmuiso,"L1MuIsol[NL1Mu]/I");
  HltTree->Branch("L1MuMip",l1extmumip,"L1MuMip[NL1Mu]/I");
  HltTree->Branch("L1MuFor",l1extmufor,"L1MuFor[NL1Mu]/I");
  HltTree->Branch("L1MuRPC",l1extmurpc,"L1MuRPC[NL1Mu]/I");
  HltTree->Branch("L1MuQal",l1extmuqul,"L1MuQal[NL1Mu]/I");
  HltTree->Branch("L1MuChg",l1extmuchg,"L1MuChg[NL1Mu]/I");

}

/* **Analyze the event** */
void HLTInfo::analyze(const edm::Handle<edm::TriggerResults>                 & hltresults,
                      const edm::Handle< BXVector<l1t::Muon> >               & L1ExtMu,
                      const edm::Handle<L1GlobalTriggerReadoutRecord>        & L1GTRR,
		      const edm::Handle<L1GctHFBitCountsCollection>          & gctBitCounts,
		      const edm::Handle<L1GctHFRingEtSumsCollection>         & gctRingSums,
		      edm::EventSetup const& eventSetup,
		      edm::Event const& iEvent,
                      TTree* HltTree) {

//   std::cout << " Beginning HLTInfo " << std::endl;


  /////////// Analyzing HLT Trigger Results (TriggerResults) //////////
  if (hltresults.isValid()) {
    int ntrigs = hltresults->size();
    if (ntrigs==0){std::cout << "%HLTInfo -- No trigger name given in TriggerResults of the input " << std::endl;}

    edm::TriggerNames const& triggerNames = iEvent.triggerNames(*hltresults);

    // 1st event : Book as many branches as trigger paths provided in the input...
    if (HltEvtCnt==0){
      for (int itrig = 0; itrig != ntrigs; ++itrig) {
        TString trigName = triggerNames.triggerName(itrig);
        HltTree->Branch(trigName,trigflag+itrig,trigName+"/I");
        HltTree->Branch(trigName+"_Prescl",trigPrescl+itrig,trigName+"_Prescl/I");
      }

      int itdum = ntrigs;
      for (unsigned int idum = 0; idum < dummyBranches_.size(); ++idum) {
	TString trigName(dummyBranches_[idum].data());
	bool addThisBranch = 1;
	for (int itrig = 0; itrig != ntrigs; ++itrig) {
	  TString realTrigName = triggerNames.triggerName(itrig);
	  if(trigName == realTrigName) addThisBranch = 0;
	}
	if(addThisBranch){
	  HltTree->Branch(trigName,trigflag+itdum,trigName+"/I");
	  HltTree->Branch(trigName+"_Prescl",trigPrescl+itdum,trigName+"_Prescl/I");
	  trigflag[itdum] = 0;
	  trigPrescl[itdum] = 0;
	  ++itdum;
	}
      }

      HltEvtCnt++;
    }
    // ...Fill the corresponding accepts in branch-variables
    //HLTConfigProvider const&  hltConfig = hltPrescaleProvider_->hltConfigProvider();
    //std::cout << "Number of prescale sets: " << hltConfig.prescaleSize() << std::endl;
    //std::cout << "Number of HLT paths: " << hltConfig.size() << std::endl;
    //int presclSet = hltPrescaleProvider_->prescaleSet(iEvent, eventSetup);
    //std::cout<<"\tPrescale set number: "<< presclSet <<std::endl; 

    for (int itrig = 0; itrig != ntrigs; ++itrig){

      std::string trigName=triggerNames.triggerName(itrig);
      bool accept = hltresults->accept(itrig);

      trigPrescl[itrig] = hltPrescaleProvider_->prescaleValue(iEvent, eventSetup, trigName);


      if (accept){trigflag[itrig] = 1;}
      else {trigflag[itrig] = 0;}

      if (_Debug){
        if (_Debug) std::cout << "%HLTInfo --  Number of HLT Triggers: " << ntrigs << std::endl;
        std::cout << "%HLTInfo --  HLTTrigger(" << itrig << "): " << trigName << " = " << accept << std::endl;
      }
    }
  }
  else { if (_Debug) std::cout << "%HLTInfo -- No Trigger Result" << std::endl;}

  /////////// Analyzing L1Extra objects //////////

  const int maxL1Mu = 1;
  for (int i=0; i!=maxL1Mu; ++i){
    l1extmupt[i] = -999.;
    l1extmue[i] = -999.;
    l1extmueta[i] = -999.;
    l1extmuphi[i] = -999.;
    l1extmuiso[i] = -999;
    l1extmumip[i] = -999;
    l1extmufor[i] = -999;
    l1extmurpc[i] = -999;
    l1extmuqul[i] = -999;
    l1extmuchg[i] = -999;
  }
  if (L1ExtMu.isValid()) {
    nl1extmu = maxL1Mu;
    int il1exmu = 0;
    for(int i = L1ExtMu->getFirstBX(); i <= L1ExtMu->getLastBX(); ++i) {      
      for (std::vector<l1t::Muon>::const_iterator muItr = L1ExtMu->begin(i); muItr != L1ExtMu->end(i); ++muItr) {
        l1extmupt[il1exmu]  = muItr->pt();
        l1extmue[il1exmu]   = muItr->energy();
        l1extmueta[il1exmu] = muItr->eta();
        l1extmuphi[il1exmu] = muItr->phi();
        
        l1extmuiso[il1exmu] = -999.;//muItr->isIsolated(); // = 1 for Isolated ?
        l1extmumip[il1exmu] = -999.;//muItr->isMip(); // = 1 for Mip ?
        l1extmufor[il1exmu] = -999.;//muItr->isForward();
        l1extmurpc[il1exmu] = -999.;//muItr->isRPC();
        
        l1extmuchg[il1exmu] = muItr->charge();
        //L1MuGMTExtendedCand gmtCand = muItr->gmtMuonCand();
        l1extmuqul[il1exmu] = -999.;//gmtCand.quality(); // Muon quality as defined in the GT
        il1exmu++;
      }
    }
    nl1extmu = il1exmu;
  }
  else {
    nl1extmu = 0;
    if (_Debug) std::cout << "%HLTInfo -- No L1 MU object" << std::endl;
  }

  //==============L1 information=======================================

  // L1 Triggers from Menu
  L1GtUtils const& l1GtUtils = hltPrescaleProvider_->l1GtUtils();

  edm::ESHandle<L1GtTriggerMenu> menuRcd;
  eventSetup.get<L1GtTriggerMenuRcd>().get(menuRcd) ;
  const L1GtTriggerMenu* menu = menuRcd.product();

  int iErrorCode = -1;
  L1GtUtils::TriggerCategory trigCategory = L1GtUtils::AlgorithmTrigger;
  const int pfSetIndexAlgorithmTrigger = l1GtUtils.prescaleFactorSetIndex(
             iEvent, trigCategory, iErrorCode);
  if (iErrorCode == 0) {
    if (_Debug) std::cout << "%Prescale set index: " << pfSetIndexAlgorithmTrigger  << std::endl;
  }else{
    //    std::cout << "%Could not extract Prescale set index from event record. Error code: " << iErrorCode << std::endl;
  }

  // 1st event : Book as many branches as trigger paths provided in the input...
  if (L1GTRR.isValid()) {  

    DecisionWord gtDecisionWord = L1GTRR->decisionWord();
    const unsigned int numberTriggerBits(gtDecisionWord.size());
    const TechnicalTriggerWord&  technicalTriggerWordBeforeMask = L1GTRR->technicalTriggerWord();
    const unsigned int numberTechnicalTriggerBits(technicalTriggerWordBeforeMask.size());

    // 1st event : Book as many branches as trigger paths provided in the input...
    if (L1EvtCnt==0){

 
      //ccla determine if more than 1 bx was unpacked in event; add OR all bx's if so
      const edm::Provenance& prov = iEvent.getProvenance(L1GTRR.id());
      //const string& procName = prov.processName();
      //std::cout << "procName:" << procName << std::endl;
      //std::cout << "provinfo:" << prov << std::endl;
      //std::cout << "setid:" << setId << std::endl;
      edm::ParameterSet pSet=parameterSet(prov);
      //std::cout << "pset:" << pSet << std::endl;
      if (pSet.exists("UnpackBxInEvent")){
	UnpackBxInEvent = pSet.getParameter<int>("UnpackBxInEvent");
      }
      if (_Debug) std::cout << "Number of beam crossings unpacked by GT: " << UnpackBxInEvent << std::endl;
      if (UnpackBxInEvent == 5) _OR_BXes = true;

      // get L1 menu from event setup
      for (CItAlgo algo = menu->gtAlgorithmMap().begin(); algo!=menu->gtAlgorithmMap().end(); ++algo) {
	if (_Debug) std::cout << "Name: " << (algo->second).algoName() << " Alias: " << (algo->second).algoAlias() << std::endl;
        int itrig = (algo->second).algoBitNumber();
	//        algoBitToName[itrig] = TString( (algo->second).algoName() );
	algoBitToName[itrig] = TString( (algo->second).algoAlias() );
        HltTree->Branch(algoBitToName[itrig],l1flag+itrig,algoBitToName[itrig]+"/I");
        HltTree->Branch(algoBitToName[itrig]+"_Prescl",l1Prescl+itrig,algoBitToName[itrig]+"_Prescl/I");
	if (_OR_BXes)
	  HltTree->Branch(algoBitToName[itrig]+"_5bx",l1flag5Bx+itrig,algoBitToName[itrig]+"_5bx/I");
      }

      // Book branches for tech bits
      for (CItAlgo techTrig = menu->gtTechnicalTriggerMap().begin(); techTrig != menu->gtTechnicalTriggerMap().end(); ++techTrig) {
        int itrig = (techTrig->second).algoBitNumber();
	techBitToName[itrig] = TString( (techTrig->second).algoName() );
	if (_Debug) std::cout << "tech bit " << itrig << ": " << techBitToName[itrig] << " " << std::endl;
	HltTree->Branch(techBitToName[itrig],l1techflag+itrig,techBitToName[itrig]+"/I");
        HltTree->Branch(techBitToName[itrig]+"_Prescl",l1techPrescl+itrig,techBitToName[itrig]+"_Prescl/I");
	if (_OR_BXes)
	  HltTree->Branch(techBitToName[itrig]+"_5bx",l1techflag5Bx+itrig,techBitToName[itrig]+"_5bx/I");
      }
    }

    std::string triggerAlgTechTrig = "PhysicsAlgorithms";
    for (unsigned int iBit = 0; iBit < numberTriggerBits; ++iBit) {     
      // ...Fill the corresponding accepts in branch-variables
      l1flag[iBit] = gtDecisionWord[iBit];

      std::string l1triggername= std::string (algoBitToName[iBit]);
      l1Prescl[iBit] = l1GtUtils.prescaleFactor(iEvent, 
					       l1triggername,
					       iErrorCode);
      
      if (_Debug) std::cout << "L1 TD: "<<iBit<<" "<<algoBitToName[iBit]<<" "
			    << gtDecisionWord[iBit]<<" "
			    << l1Prescl[iBit] << std::endl;

    }

    triggerAlgTechTrig = "TechnicalTriggers";
    for (unsigned int iBit = 0; iBit < numberTechnicalTriggerBits; ++iBit) {
      l1techflag[iBit] = (int) technicalTriggerWordBeforeMask.at(iBit);

      std::string l1triggername= std::string (techBitToName[iBit]);
      l1techPrescl[iBit] = l1GtUtils.prescaleFactor(iEvent, 
					       l1triggername,
					       iErrorCode);

      if (_Debug) std::cout << "L1 TD: "<<iBit<<" "<<techBitToName[iBit]<<" "
			    << l1techflag[iBit]<<" "
			    << l1Prescl[iBit] << std::endl;

    }

    if (_OR_BXes){
      // look at all 5 bx window in case gt timing is off
      // get Field Decision Logic
      std::vector<DecisionWord> m_gtDecisionWord5Bx;
      std::vector<TechnicalTriggerWord> m_gtTechDecisionWord5Bx;
      std::vector<int> m_ibxn;

      const std::vector<L1GtFdlWord> &m_gtFdlWord(L1GTRR->gtFdlVector());
      for (std::vector<L1GtFdlWord>::const_iterator itBx = m_gtFdlWord.begin();
	   itBx != m_gtFdlWord.end(); ++itBx) {
	if (_Debug && L1EvtCnt==0) std::cout << "bx: " << (*itBx).bxInEvent() << " ";
	m_gtDecisionWord5Bx.push_back((*itBx).gtDecisionWord());
	m_gtTechDecisionWord5Bx.push_back((*itBx).gtTechnicalTriggerWord());
      }
      // --- Fill algo bits ---
      for (unsigned int iBit = 0; iBit < numberTriggerBits; ++iBit) {     
	// ...Fill the corresponding accepts in branch-variables
	if (_Debug) std::cout << std::endl << " L1 TD: "<<iBit<<" "<<algoBitToName[iBit]<<" ";
	int result=0;
	int bitword=0; 
	for (unsigned int jbx=0; jbx<m_gtDecisionWord5Bx.size(); ++jbx) {
	  if (_Debug) std::cout << m_gtDecisionWord5Bx[jbx][iBit]<< " ";
	  result += m_gtDecisionWord5Bx[jbx][iBit];
	  if (m_gtDecisionWord5Bx[jbx][iBit]>0) bitword |= 1 << jbx;
	}
	if (_Debug && result>1) {std::cout << "5BxOr=" << result << "  Bitword= "<< bitword <<std::endl;
	  std::cout << "Unpacking: " ;
	  for (int i = 0; i<UnpackBxInEvent ; ++i){
	    bool bitOn=bitword & (1 << i);
	    std::cout << bitOn << " ";
	  }
	  std::cout << "\n";
	}
	l1flag5Bx[iBit] = bitword;
      }
      // --- Fill tech bits ---
      for (unsigned int iBit = 0; iBit < m_gtTechDecisionWord5Bx[2].size(); ++iBit) {     
	// ...Fill the corresponding accepts in branch-variables
	if (_Debug) std::cout << std::endl << " L1 TD: "<<iBit<<" "<<techBitToName[iBit]<<" ";
	int result=0;
	int bitword=0;       
	for (unsigned int jbx=0; jbx<m_gtTechDecisionWord5Bx.size(); ++jbx) {
	  if (_Debug) std::cout << m_gtTechDecisionWord5Bx[jbx][iBit]<< " ";
	  result += m_gtTechDecisionWord5Bx[jbx][iBit];
	  if (m_gtTechDecisionWord5Bx[jbx][iBit]>0) bitword |= 1 << jbx;
	}
	if (_Debug && result>1) {std::cout << "5BxOr=" << result << "  Bitword= "<< bitword  << std::endl;
	  std::cout << "Unpacking: " ;
	  for (int i = 0; i<UnpackBxInEvent ; ++i){
	    bool bitOn=bitword & (1 << i);
	    std::cout << bitOn << " ";
	  }
	  std::cout << "\n";
	}
	l1techflag5Bx[iBit] = bitword;
      }
    } // end of OR_BX

    L1EvtCnt++;
  }
  else {
    if (_Debug) std::cout << "%HLTInfo -- No L1 GT ReadoutRecord " << std::endl;
  }

  if (_Debug) std::cout << "%HLTInfo -- Done with routine" << std::endl;
}
