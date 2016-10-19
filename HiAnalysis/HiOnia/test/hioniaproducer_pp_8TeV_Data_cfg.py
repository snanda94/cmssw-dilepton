import FWCore.ParameterSet.Config as cms
import FWCore.ParameterSet.VarParsing as VarParsing


#----------------------------------------------------------------------------

# Setup Settings for ONIA SKIM:

ispPb         = False     # if PbPb data/MC: True or if pp data/MC: False    
isMC           = False     # if input is MONTECARLO: True or if it's DATA: False
isPromptDATA   = True      # if input is Prompt RECO DATA: True or if it's Express Stream DATA: False
isPromptMC     = False     # if MC is Prompt Quarkonia: True or if it's Non Prompt Quarkonia: False
useExtraColl   = False     # General Tracks + Stand Alone Muons + Converted Photon collections
applyEventSel  = False     # Only apply Event Selection if the required collections are present 
applyMuonCuts  = True     # Apply muon ID quality cuts
muonSelection  = "GlbTrk"  # Single muon selection: Glb(isGlobal), GlbTrk(isGlobal&&isTracker), Trk(isTracker) are availale
genPDG         = 443       # Generated Particle PDG ID (only needed for MC), Jpsi: 443 , Psi(2S): 100443, Upsilon(1S): 553 , Upsilon(2S): 100553 , Upsilon(2S): 200553

#----------------------------------------------------------------------------


# Print Onia Skim settings:
if (isPromptDATA and isMC): raise SystemExit("[ERROR] isMC and isPromptDATA can not be true at the same time, please fix your settings!.")
print( " " ) 
print( "[INFO] Settings used for ONIA TREE: " )  
print( "[INFO] ispPb        = " + ("True" if ispPb else "False") )  
print( "[INFO] isMC          = " + ("True" if isMC else "False") )  
print( "[INFO] isPromptDATA  = " + ("True" if isPromptDATA else "False") )  
print( "[INFO] isPromptMC    = " + ("True" if isPromptMC else "False") ) 
print( "[INFO] useExtraColl  = " + ("True" if useExtraColl else "False") ) 
print( "[INFO] applyEventSel = " + ("True" if applyEventSel else "False") )  
print( "[INFO] applyMuonCuts = " + ("True" if applyMuonCuts else "False") ) 
print( "[INFO] muonSelection = " + muonSelection )  
print( " " ) 

# set up process
process = cms.Process("HIOnia")

# setup 'analysis'  options
options = VarParsing.VarParsing ('analysis')

# Input and Output File Names
options.outputFile = "OniaTree.root"
options.secondaryOutputFile = "Jpsi_DataSet.root"
options.inputFiles =  '/store/data/Run2012A/MuOnia/AOD/22Jan2013-v1/30000/000D2FF5-EE82-E211-BEBA-0026189438A5.root'
options.maxEvents = -1 # -1 means all events

# Get and parse the command line arguments
options.parseArguments()
process.load("FWCore.MessageService.MessageLogger_cfi")
process.MessageLogger.categories.extend(["GetManyWithoutRegistration","GetByLabelWithoutRegistration"])
process.MessageLogger.destinations = ['cout', 'cerr']
process.MessageLogger.cerr.FwkReport.reportEvery = 1000
process.MessageLogger.categories.extend(["HiOnia2MuMuPAT_muonLessSizeORpvTrkSize"])
process.MessageLogger.cerr.HiOnia2MuMuPAT_muonLessSizeORpvTrkSize = cms.untracked.PSet( limit = cms.untracked.int32(5) )

# load the Geometry and Magnetic Field for the TransientTrackBuilder

process.load('Configuration.StandardSequences.Services_cff')
process.load('Configuration.StandardSequences.GeometryRecoDB_cff')
process.load('Configuration.StandardSequences.MagneticField_AutoFromDBCurrent_cff')
process.load('Configuration.StandardSequences.Reconstruction_Data_cff')
process.load("TrackingTools/TransientTrack/TransientTrackBuilder_cfi")

# Global Tag:
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')
from Configuration.AlCa.GlobalTag import GlobalTag
process.GlobalTag = GlobalTag(process.GlobalTag, 'auto:run1_data', '')
process.GlobalTag.snapshotTime = cms.string("9999-12-31 23:59:59.000")


HLTProName = "HLT"

###################### Onia Skim Producer #################################################

# HLT Dimuon Triggers
import HLTrigger.HLTfilters.hltHighLevel_cfi
process.hltOniaHI = HLTrigger.HLTfilters.hltHighLevel_cfi.hltHighLevel.clone()
# HLT pPb TEST MENU:  /users/anstahll/PA2016/HIDileptonPA2016/V12
process.hltOniaHI.HLTPaths =  [
      # from https://espace.cern.ch/cms-quarkonia/trigger-bph/SitePages/2012-InclusiveJPsi.aspx
      "HLT_Dimuon0_Jpsi_v*",
      "HLT_Dimuon8_Jpsi_v*",
      "HLT_Dimuon10_Jpsi_v*",
      "HLT_Dimuon0_Jpsi_NoVertexing_v*",
      # from https://espace.cern.ch/cms-quarkonia/trigger-bph/SitePages/2012-DisplacedJPsi.aspx
      "HLT_DoubleMu4_Jpsi_Displaced_v*",
      # from https://espace.cern.ch/cms-quarkonia/trigger-bph/SitePages/2012-PsiPrime.aspx
      "HLT_Dimuon0_PsiPrime_v*",
      "HLT_Dimuon5_PsiPrime_v*",
      "HLT_Dimuon7_PsiPrime_v*",
      "HLT_Dimuon9_PsiPrime_v*",
      # from https://espace.cern.ch/cms-quarkonia/trigger-bph/SitePages/2012-Upsilon.aspx
      "HLT_Dimuon0_Upsilon_v*",
      "HLT_Dimuon5_Upsilon_v*",
      "HLT_Dimuon7_Upsilon_v*",
      "HLT_Dimuon8_Upsilon_v*",
      "HLT_Dimuon11_Upsilon_v*",
      # for Z
      "HLT_Mu17_TkMu8_v*"
      "HLT_Mu17_Mu8_v*"
      "HLT_Mu5_v*",
      # https://espace.cern.ch/cms-quarkonia/trigger-bph/SitePages/2012-EfficiencyTriggers.aspx
      "HLT_Mu5_Track2_Jpsi_v*",
      "HLT_Mu5_Track3p5_Jpsi_v*",
      "HLT_Mu7_Track7_Jpsi_v*",
      "HLT_Mu5_L2Mu3_Jpsi_v*",
      # for W
      "HLT_Mu17_v*",
      "HLT_Mu24_v*",
      "HLT_Mu24_eta2p1_v*",
      "HLT_IsoMu24_v*",
      "HLT_IsoMu24_eta2p1_v*",
      "HLT_Mu40_v*",
      "HLT_Mu40_eta2p1_v*",
      "HLT_IsoMu40_eta2p1_v*",
  ]

process.hltOniaHI.throw = False
process.hltOniaHI.andOr = True
process.hltOniaHI.TriggerResultsTag = cms.InputTag("TriggerResults","",HLTProName)

from HiSkim.HiOnia2MuMu.onia2MuMuPAT_cff import *
onia2MuMuPAT(process, GlobalTag=process.GlobalTag.globaltag, MC=isMC, HLT=HLTProName, Filter=isMC)

### Temporal fix for the PAT Trigger prescale warnings.
process.patTriggerFull.l1GtReadoutRecordInputTag = cms.InputTag("gtDigis","","RECO")
process.patTriggerFull.l1tAlgBlkInputTag = cms.InputTag("gtStage2Digis","","RECO")
process.patTriggerFull.l1tExtBlkInputTag = cms.InputTag("gtStage2Digis","","RECO")
###

##### Onia2MuMuPAT input collections/options
process.onia2MuMuPatGlbGlb.dimuonSelection          = cms.string("mass > 0")
process.onia2MuMuPatGlbGlb.resolvePileUpAmbiguity   = True
process.onia2MuMuPatGlbGlb.srcTracks                = cms.InputTag("generalTracks")
process.onia2MuMuPatGlbGlb.primaryVertexTag         = cms.InputTag("offlinePrimaryVertices")
process.patMuonsWithoutTrigger.pvSrc                = cms.InputTag("offlinePrimaryVertices")
# Adding muonLessPV gives you lifetime values wrt. muonLessPV only
process.onia2MuMuPatGlbGlb.addMuonlessPrimaryVertex = True
if isMC:
  process.genMuons.src = "genParticles"
  process.onia2MuMuPatGlbGlb.genParticles = "genParticles"

##### Dimuon pair selection
commonP1 = "|| (innerTrack.isNonnull && genParticleRef(0).isNonnull)"
commonP2 = " && abs(innerTrack.dxy)<4 && abs(innerTrack.dz)<35"
if muonSelection == "Glb":
  highP = "isGlobalMuon"; # At least one muon must pass this selection
  process.onia2MuMuPatGlbGlb.higherPuritySelection = cms.string("("+highP+commonP1+")"+commonP2)
  lowP = "isGlobalMuon"; # BOTH muons must pass this selection
  process.onia2MuMuPatGlbGlb.lowerPuritySelection = cms.string("("+lowP+commonP1+")"+commonP2)
elif muonSelection == "GlbTrk":
  highP = "(isGlobalMuon && isTrackerMuon)";
  process.onia2MuMuPatGlbGlb.higherPuritySelection = cms.string("("+highP+commonP1+")"+commonP2)
  lowP = "(isGlobalMuon && isTrackerMuon)";
  process.onia2MuMuPatGlbGlb.lowerPuritySelection = cms.string("("+lowP+commonP1+")"+commonP2)
elif muonSelection == "Trk":
  highP = "isTrackerMuon";
  process.onia2MuMuPatGlbGlb.higherPuritySelection = cms.string("("+highP+commonP1+")"+commonP2)
  lowP = "isTrackerMuon";
  process.onia2MuMuPatGlbGlb.lowerPuritySelection = cms.string("("+lowP+commonP1+")"+commonP2)
else:
  print "ERROR: Incorrect muon selection " + muonSelection + " . Valid options are: Glb, Trk, GlbTrk"

##### Remove few paths for MC
if isMC:
  process.patMuonSequence.remove(process.hltOniaHI)


###################### HiOnia Analyzer #################################################

process.hionia = cms.EDAnalyzer('HiOniaAnalyzer',
                                #-- Collections
                                # l1GtReadoutRecordInputTag = cms.InputTag("gtDigis","","RECO"), # Only use if prescale warnings are shown
                                srcMuon             = cms.InputTag("patMuonsWithTrigger"),     # Name of PAT Muon Collection
                                srcMuonNoTrig       = cms.InputTag("patMuonsWithoutTrigger"),  # Name of PAT Muon Without Trigger Collection
                                src                 = cms.InputTag("onia2MuMuPatGlbGlb"),      # Name of Onia Skim Collection
                                EvtPlane            = cms.InputTag("hiEvtPlane",""),           # Name of Event Plane Collection. For RECO use: hiEventPlane,recoLevel

                                triggerResultsLabel = cms.InputTag("TriggerResults","",HLTProName), # Label of Trigger Results

                                #-- Reco Details
                                useBeamSpot = cms.bool(False),  
                                useRapidity = cms.bool(True),
                                
                                #--
                                maxAbsZ = cms.double(24.0),
                                
                                pTBinRanges      = cms.vdouble(0.0, 6.0, 8.0, 9.0, 10.0, 12.0, 15.0, 40.0),
                                etaBinRanges     = cms.vdouble(0.0, 2.5),
                                centralityRanges = cms.vdouble(20,40,100),

                                onlyTheBest        = cms.bool(False),		
                                applyCuts          = cms.bool(applyMuonCuts),
                                selTightGlobalMuon = cms.bool(False),
                                storeEfficiency    = cms.bool(False),
                      
                                removeSignalEvents = cms.untracked.bool(False),  # Remove/Keep signal events
                                removeTrueMuons    = cms.untracked.bool(False),  # Remove/Keep gen Muons
                                storeSameSign      = cms.untracked.bool(True),   # Store/Drop same sign dimuons
                                
                                #-- Gen Details
                                oniaPDG = cms.int32(genPDG),
                                muonSel = cms.string(muonSelection),
                                isHI = cms.untracked.bool(False),
                                isPA = cms.untracked.bool(ispPb),
                                isMC = cms.untracked.bool(isMC),
                                isPromptMC = cms.untracked.bool(isPromptMC),
                                useEvtPlane = cms.untracked.bool(False),
                                useGeTracks = cms.untracked.bool(useExtraColl),
                                runVersionChange = cms.untracked.uint32(182133),

                                #-- Histogram configuration
                                combineCategories = cms.bool(False),
                                fillRooDataSet    = cms.bool(False),
                                fillTree          = cms.bool(True),
                                fillHistos        = cms.bool(False),
                                minimumFlag       = cms.bool(False),
                                fillSingleMuons   = cms.bool(True),
                                fillRecoTracks    = cms.bool(useExtraColl),
                                histFileName      = cms.string(options.outputFile),		
                                dataSetName       = cms.string(options.secondaryOutputFile),
       
                                # HLT pPb TEST MENU:  /users/anstahll/PA2016/HIDileptonPA2016/V12
                                
                                dblTriggerPathNames = cms.vstring(
                                      # from https://espace.cern.ch/cms-quarkonia/trigger-bph/SitePages/2012-InclusiveJPsi.aspx
                                      "HLT_Dimuon0_Jpsi_v*",
                                      "HLT_Dimuon8_Jpsi_v*",
                                      "HLT_Dimuon10_Jpsi_v*",
                                      "HLT_Dimuon0_Jpsi_NoVertexing_v*",
                                      # from https://espace.cern.ch/cms-quarkonia/trigger-bph/SitePages/2012-DisplacedJPsi.aspx
                                      "HLT_DoubleMu4_Jpsi_Displaced_v*",
                                      # from https://espace.cern.ch/cms-quarkonia/trigger-bph/SitePages/2012-PsiPrime.aspx
                                      "HLT_Dimuon0_PsiPrime_v*",
                                      "HLT_Dimuon5_PsiPrime_v*",
                                      "HLT_Dimuon7_PsiPrime_v*",
                                      "HLT_Dimuon9_PsiPrime_v*",
                                      # from https://espace.cern.ch/cms-quarkonia/trigger-bph/SitePages/2012-Upsilon.aspx
                                      "HLT_Dimuon0_Upsilon_v*",
                                      "HLT_Dimuon5_Upsilon_v*",
                                      "HLT_Dimuon7_Upsilon_v*",
                                      "HLT_Dimuon8_Upsilon_v*",
                                      "HLT_Dimuon11_Upsilon_v*",
                                      # for Z
                                      "HLT_Mu17_TkMu8_v*",
                                      "HLT_Mu17_Mu8_v*"
                                      ),
                                
                                dblTriggerFilterNames = cms.vstring(
                                      "hltVertexmumuFilterJpsi",
                                      "hltVertexmumuFilterDimuon8Jpsi",
                                      "hltVertexmumuFilterDimuon10Jpsi",
                                      "hltJpsiNoVertexingL3Filtered",
                                      "hltDisplacedmumuFilterDoubleMu4Jpsi",
                                      "hltVertexmumuFilterDimuon0PsiPrime",
                                      "hltVertexmumuFilterDimuon5PsiPrime",
                                      "hltVertexmumuFilterDimuon7PsiPrime",
                                      "hltVertexmumuFilterDimuon9PsiPrime",
                                      "hltVertexmumuFilterUpsilon",
                                      "hltVertexmumuFilterDimuon5Upsilon",
                                      "hltVertexmumuFilterDimuon7Upsilon",
                                      "hltVertexmumuFilterDimuon8Upsilon",
                                      "hltVertexmumuFilterDimuon11Upsilon",
                                      "hltDiMuonGlb17Trk8DzFiltered0p2",
                                      "hltDiMuonGlb17Glb8DzFiltered0p2"
                                      ),
                                
                                sglTriggerPathNames = cms.vstring(
                                      "HLT_Mu5_v*",
                                      # https://espace.cern.ch/cms-quarkonia/trigger-bph/SitePages/2012-EfficiencyTriggers.aspx
                                      "HLT_Mu5_Track2_Jpsi_v*",
                                      "HLT_Mu5_Track3p5_Jpsi_v*",
                                      "HLT_Mu7_Track7_Jpsi_v*",
                                      "HLT_Mu5_L2Mu3_Jpsi_v*",
                                      # for W
                                      "HLT_Mu17_v*",
                                      "HLT_Mu24_v*",
                                      "HLT_Mu24_eta2p1_v*",
                                      "HLT_IsoMu24_v*",
                                      "HLT_IsoMu24_eta2p1_v*",
                                      "HLT_Mu40_v*",
                                      "HLT_Mu40_eta2p1_v*",
                                      "HLT_IsoMu40_eta2p1_v*",
                                      ),
                                
                                sglTriggerFilterNames = cms.vstring(
                                      "hltL3fL1sMu3L3Filtered5",
                                      "hltMu5Track2JpsiTrackMassFiltered",
                                      "hltMu5Track3p5JpsiTrackMassFiltered",
                                      "hltMu7Track7JpsiTrackMassFiltered",
                                      "hltMu5L2Mu3JpsiTrackMassFiltered",
                                      "hltL3fL1sMu12L3Filtered17",
                                      "hltL3fL1sMu16L1f0L2f16QL3Filtered24Q",
                                      "hltL3fL1sMu16Eta2p1L1f0L2f16QL3Filtered24Q",
                                      "hltL3crIsoL1sMu16L1f0L2f16QL3f24QL3crIsoRhoFiltered0p15",
                                      "hltL3crIsoL1sMu16Eta2p1L1f0L2f16QL3f24QL3crIsoRhoFiltered0p15",
                                      "hltL3fL1sMu16L1f0L2f16QL3Filtered40Q",
                                      "hltL3fL1sMu16Eta2p1L1f0L2f16QL3Filtered40Q",
                                      "hltL3crIsoL1sMu16Eta2p1L1f0L2f16QL3f40QL3crIsoRhoFiltered0p15",
                                      ),
                                )

process.hionia.primaryVertexTag = cms.InputTag("offlinePrimaryVertices")
process.hionia.genParticles     = cms.InputTag("genParticles")
process.hionia.muonLessPV       = cms.bool(False)
process.hionia.CentralitySrc    = cms.InputTag("")
process.hionia.CentralityBinSrc = cms.InputTag("")
process.hionia.srcTracks        = cms.InputTag("generalTracks")       

process.Onia2MuMuPAT = cms.Path(
        process.patMuonSequence *
        process.onia2MuMuPatGlbGlb *
        process.onia2MuMuPatGlbGlbFilter *
        process.hionia
    )


##### Event Selection
if applyEventSel:
  if ispPb:
     # dummy (from 5TeV PbPb)
    process.load('HeavyIonsAnalysis.Configuration.collisionEventSelection_cff')
    process.load('HeavyIonsAnalysis.EventAnalysis.HIClusterCompatibilityFilter_cfi')
    process.clusterCompatibilityFilter.clusterPars = cms.vdouble(0.0,0.006)
    process.oniaSequence.replace(process.hionia , process.hfCoincFilter3 * process.primaryVertexFilter * process.clusterCompatibilityFilter * process.hionia )
  else:
     # from 5TeV pp
    process.PAprimaryVertexFilter = cms.EDFilter("VertexSelector",
                                                 src = cms.InputTag("offlinePrimaryVertices"),
                                                 cut = cms.string("!isFake && abs(z) <= 25 && position.Rho <= 2 && tracksSize >= 2"),
                                                 filter = cms.bool(True),
                                                 )
    process.NoScraping = cms.EDFilter("FilterOutScraping",
                                      applyfilter = cms.untracked.bool(True),
                                      debugOn = cms.untracked.bool(False),
                                      numtrack = cms.untracked.uint32(10),
                                      thresh = cms.untracked.double(0.25),
                                      )
    process.oniaSequence.replace(process.hionia , process.PAprimaryVertexFilter * process.NoScraping * process.hionia )


process.options = cms.untracked.PSet(wantSummary = cms.untracked.bool(True))
process.source.fileNames      = cms.untracked.vstring(options.inputFiles)        
process.maxEvents             = cms.untracked.PSet( input = cms.untracked.int32(options.maxEvents) )
process.TFileService = cms.Service("TFileService", fileName = cms.string( options.outputFile ) )
process.schedule              = cms.Schedule(
    process.Onia2MuMuPAT
    )

from Configuration.Applications.ConfigBuilder import MassReplaceInputTag
MassReplaceInputTag(process)



'''

###################### HLTAnalyzer #################################################
process.load("HLTrigger.HLTanalyzers.HLTAnalyser_cfi")

process.hltanalysis.RunParameters.HistogramFile = cms.untracked.string(options.outputFile)
process.hltanalysis.xSection=1.0
process.hltanalysis.filterEff=1.0
process.hltanalysis.l1GtReadoutRecord = cms.InputTag( 'hltGtDigis','',HLTProName ) 
process.hltanalysis.l1GtObjectMapRecord = cms.InputTag( 'hltL1GtObjectMap','',HLTProName )
process.hltanalysis.hltresults = cms.InputTag( 'TriggerResults','',HLTProName)
process.hltanalysis.HLTProcessName = cms.string(HLTProName)

process.hltanalysis.muonFilters = cms.VInputTag(
    cms.InputTag("hltL1fL1DoubleMu0L1Filtered0MassGT1",""),    # 0
    );

process.hltanalysis.muon = cms.InputTag("muons")
process.hltanalysis.l1extramu = cms.string("hltGmtStage2Digis")
process.hltanalysis.MuCandTag2 = cms.InputTag("hltL2MuonCandidates")
process.hltanalysis.MuCandTag3 = cms.InputTag("hltL3MuonCandidates")
process.hltanalysis.L3TkTracksFromL2OIStateTag = cms.InputTag("hltL3TkTracksFromL2OIState")
process.hltanalysis.L3TkTracksFromL2OIHitTag = cms.InputTag("hltL3TkTracksFromL2OIHit")
process.hltanalysis.OfflinePrimaryVertices0 = cms.InputTag('offlinePrimaryVertices')
process.hltanalysis.PrimaryVertices = cms.InputTag('hltPixelVertices')

process.hltanalysis.UseTFileService = cms.untracked.bool(True)

process.hltAnalysis = cms.Path( process.hltanalysis )

'''
