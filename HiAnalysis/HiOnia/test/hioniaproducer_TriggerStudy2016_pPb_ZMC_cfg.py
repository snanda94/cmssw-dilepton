import FWCore.ParameterSet.Config as cms
import FWCore.ParameterSet.VarParsing as VarParsing


#----------------------------------------------------------------------------

# Setup Settings for ONIA SKIM:

isMC           = True     # if input is MONTECARLO: True or if it's DATA: False
muonSelection  = "Trk"    # Single muon selection: Glb(isGlobal), GlbTrk(isGlobal&&isTracker), Trk(isTracker) are availale

#----------------------------------------------------------------------------

# set up process
process = cms.Process("HIOnia")

# setup 'analysis'  options
options = VarParsing.VarParsing ('analysis')

# Input and Output File Names
options.outputFile = "OniaTree.root"
options.secondaryOutputFile = "Jpsi_DataSet.root"
options.inputFiles =  '/store/user/anstahll/TriggerStudy2016/MC/ZMuMuPt040_pythia6Gun_RECO_20160701/ZMuMuGun/ZMuMuPt040_pythia6Gun_RECO_20160701/160702_070700/0000/ZMuMuPt40_RAW2DIGI_L1Reco_RECO_1.root'
options.maxEvents = -1 # -1 means all events

# Get and parse the command line arguments
options.parseArguments()
process.load("FWCore.MessageService.MessageLogger_cfi")
process.MessageLogger.categories.extend(["GetManyWithoutRegistration","GetByLabelWithoutRegistration"])
process.MessageLogger.destinations = ['cout', 'cerr']
process.MessageLogger.cerr.FwkReport.reportEvery = 1000

# load the Geometry and Magnetic Field for the TransientTrackBuilder

process.load('Configuration.StandardSequences.Services_cff')
process.load('Configuration.Geometry.GeometryExtended2016Reco_cff')
process.load('Configuration.StandardSequences.MagneticField_cff')
process.load('Configuration.StandardSequences.Reconstruction_cff')
process.load("TrackingTools/TransientTrack/TransientTrackBuilder_cfi")

# Global Tag:
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')
from Configuration.AlCa.GlobalTag import GlobalTag
process.GlobalTag = GlobalTag(process.GlobalTag, 'auto:run2_mc_GRun', '')
process.GlobalTag.snapshotTime = cms.string("9999-12-31 23:59:59.000")


HLTProName = "TEST"

###################### Onia Skim Producer #################################################

# HLT Dimuon Triggers
import HLTrigger.HLTfilters.hltHighLevel_cfi
process.hltOniaHI = HLTrigger.HLTfilters.hltHighLevel_cfi.hltHighLevel.clone()
# HLT pPb TEST MENU:  /users/anstahll/PA2016/HIDileptonPA2016/V10
process.hltOniaHI.HLTPaths =  [
  "HLT_PAL1DoubleMu0_MassGT1_v1",
  "HLT_PAL1DoubleMuOpen_MassGT1_v1",
  "HLT_PAL1DoubleMuOpen_OS_v1",
  "HLT_PAL1DoubleMuOpen_SS_v1",
  "HLT_PAL1DoubleMuOpen_v1",
  "HLT_PAL1DoubleMu0_v1",
  "HLT_PAL1DoubleMu0_QGTE8_v1",
  "HLT_PAL1DoubleMu0_QGTE9_v1",
  "HLT_PAL1DoubleMu0_QGTE10_v1",
  "HLT_PAL1DoubleMu0_QGTE11_v1",
  "HLT_PAL1DoubleMu0_QGTE12_v1",
  "HLT_PAL1DoubleMu0_QGTE13_v1",
  "HLT_PAL1DoubleMu0_QGTE14_v1",
  "HLT_PAL1DoubleMu0_QGTE15_v1",
  "HLT_PAL1DoubleMu10_v1",
  "HLT_PAL2DoubleMu10_v1",
  "HLT_PAL2DoubleMuOpen_v1",
  "HLT_PAL3DoubleMuOpen_HIon_v1",
  "HLT_PAL3DoubleMuOpen_v1",
  "HLT_PAL3DoubleMu10_HIon_v1",
  "HLT_PAL3DoubleMu10_v1",
  "HLT_PAL1MuOpen_v1",
  "HLT_PAL1Mu0_NoBptxAND_v1",
  "HLT_PAL1Mu0_v1",
  "HLT_PAL1Mu12_NoBptxAND_v1",
  "HLT_PAL1Mu12_v1",
  "HLT_PAL1Mu15_v1",
  "HLT_PAL2Mu0_v1",
  "HLT_PAL2Mu12_v1",
  "HLT_PAL2Mu15_v1",
  "HLT_PAL3Mu0_HIon_v1",
  "HLT_PAL3Mu0_v1",
  "HLT_PAL3Mu3_v1",
  "HLT_PAL3Mu5_v1",
  "HLT_PAL3Mu7_v1",
  "HLT_PAL3Mu12_v1",
  "HLT_PAL3Mu15_v1"
  ]

process.hltOniaHI.throw = False
process.hltOniaHI.andOr = True
process.hltOniaHI.TriggerResultsTag = cms.InputTag("TriggerResults","",HLTProName)

from HiSkim.HiOnia2MuMu.onia2MuMuPAT_cff import *
onia2MuMuPAT(process, GlobalTag=process.GlobalTag.globaltag, MC=isMC, HLT=HLTProName, Filter=False)

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
                                applyCuts          = cms.bool(False),
                                selTightGlobalMuon = cms.bool(False),
                                storeEfficiency    = cms.bool(False),
                      
                                removeSignalEvents = cms.untracked.bool(False),  # Remove/Keep signal events
                                removeTrueMuons    = cms.untracked.bool(False),  # Remove/Keep gen Muons
                                storeSameSign      = cms.untracked.bool(True),   # Store/Drop same sign dimuons
                                
                                #-- Gen Details
                                oniaPDG = cms.int32(23),
                                muonSel = cms.string(muonSelection),
                                isHI = cms.untracked.bool(False),
                                isPA = cms.untracked.bool(False),
                                isMC = cms.untracked.bool(isMC),
                                isPromptMC = cms.untracked.bool(True),
                                useEvtPlane = cms.untracked.bool(False),
                                useGeTracks = cms.untracked.bool(False),
                                runVersionChange = cms.untracked.uint32(182133),

                                #-- Histogram configuration
                                combineCategories = cms.bool(False),
                                fillRooDataSet    = cms.bool(False),
                                fillTree          = cms.bool(True),
                                fillHistos        = cms.bool(False),
                                minimumFlag       = cms.bool(True),
                                fillSingleMuons   = cms.bool(True),
                                fillRecoTracks    = cms.bool(False),
                                histFileName      = cms.string(options.outputFile),		
                                dataSetName       = cms.string(options.secondaryOutputFile),
                                
                                # HLT pPb TEST MENU:  /users/anstahll/PA2016/HIDileptonPA2016/V10
                                
                                dblTriggerPathNames = cms.vstring("HLT_PAL1DoubleMu0_MassGT1_v1",
                                                                  "HLT_PAL1DoubleMuOpen_MassGT1_v1",
                                                                  "HLT_PAL1DoubleMuOpen_OS_v1",
                                                                  "HLT_PAL1DoubleMuOpen_SS_v1",
                                                                  "HLT_PAL1DoubleMuOpen_v1",
                                                                  "HLT_PAL1DoubleMu0_v1",
                                                                  "HLT_PAL1DoubleMu0_QGTE8_v1",
                                                                  "HLT_PAL1DoubleMu0_QGTE9_v1",
                                                                  "HLT_PAL1DoubleMu0_QGTE10_v1",
                                                                  "HLT_PAL1DoubleMu0_QGTE11_v1",
                                                                  "HLT_PAL1DoubleMu0_QGTE12_v1",
                                                                  "HLT_PAL1DoubleMu0_QGTE13_v1",
                                                                  "HLT_PAL1DoubleMu0_QGTE14_v1",
                                                                  "HLT_PAL1DoubleMu0_QGTE15_v1",
                                                                  "HLT_PAL1DoubleMu10_v1",
                                                                  "HLT_PAL2DoubleMu10_v1",
                                                                  "HLT_PAL2DoubleMuOpen_v1",
                                                                  "HLT_PAL3DoubleMuOpen_HIon_v1",
                                                                  "HLT_PAL3DoubleMuOpen_v1",
                                                                  "HLT_PAL3DoubleMu10_HIon_v1",
                                                                  "HLT_PAL3DoubleMu10_v1"),
                                
                                dblTriggerFilterNames = cms.vstring("hltL1fL1DoubleMu0L1Filtered0MassGT1",
                                                                    "hltL1fL1DoubleMuOpenL1Filtered0MassGT1",
                                                                    "hltL1fL1DoubleMuOpenL1Filtered0OS",
                                                                    "hltL1fL1DoubleMuOpenL1Filtered0SS",
                                                                    "hltL1fL1DoubleMuOpenL1Filtered0",
                                                                    "hltL1fL1DoubleMu0L1Filtered0",
                                                                    "hltL1fL1DoubleMu0L1Filtered0GTE8",
                                                                    "hltL1fL1DoubleMu0L1Filtered0GTE9",
                                                                    "hltL1fL1DoubleMu0L1Filtered0GTE10",
                                                                    "hltL1fL1DoubleMu0L1Filtered0GTE11",
                                                                    "hltL1fL1DoubleMu0L1Filtered0GTE12",
                                                                    "hltL1fL1DoubleMu0L1Filtered0GTE13",
                                                                    "hltL1fL1DoubleMu0L1Filtered0GTE14",
                                                                    "hltL1fL1DoubleMu0L1Filtered0GTE15",
                                                                    "hltL1fL1DoubleMu10L1Filtered0",
                                                                    "hltL2fL1DoubleMu10L2Filtered0",
                                                                    "hltL2fL1DoubleMuOpenL2Filtered0",
                                                                    "hltHIL3fL2DimuonMuOpenL3Filtered0",
                                                                    "hltL3fL2DimuonMuOpenL3Filtered0",
                                                                    "hltHIL3fL2DimuonMu10L3Filtered10",
                                                                    "hltL3fL2DimuonMu10L3Filtered10"),
                                
                                sglTriggerPathNames = cms.vstring("HLT_PAL1MuOpen_v1",
                                                                  "HLT_PAL1Mu0_NoBptxAND_v1",
                                                                  "HLT_PAL1Mu0_v1",
                                                                  "HLT_PAL1Mu12_NoBptxAND_v1",
                                                                  "HLT_PAL1Mu12_v1",
                                                                  "HLT_PAL1Mu15_v1",
                                                                  "HLT_PAL2Mu0_v1",
                                                                  "HLT_PAL2Mu12_v1",
                                                                  "HLT_PAL2Mu15_v1",
                                                                  "HLT_PAL3Mu0_HIon_v1",
                                                                  "HLT_PAL3Mu0_v1",
                                                                  "HLT_PAL3Mu3_v1",
                                                                  "HLT_PAL3Mu5_v1",
                                                                  "HLT_PAL3Mu7_v1",
                                                                  "HLT_PAL3Mu12_v1",
                                                                  "HLT_PAL3Mu15_v1"),
                                
                                sglTriggerFilterNames = cms.vstring("hltL1fL1sMuOpenL1Filtered0",
                                                                    "hltL1fL1sMu0L1Filtered0NoBptxAND",
                                                                    "hltL1fL1sMu0L1Filtered0",
                                                                    "hltL1fL1sMu12L1Filtered12NoBptxAND",
                                                                    "hltL1fL1sMu12L1Filtered12",
                                                                    "hltL1fL1sMu15L1Filtered15",
                                                                    "hltL2fL1sMu0L2Filtered0",
                                                                    "hltL2fL1sMu12L2Filtered12",
                                                                    "hltL2fL1sMu15L2Filtered15",
                                                                    "hltHIL3fL2sMu0L3Filtered0",
                                                                    "hltL3fL2sMu0L3Filtered0",
                                                                    "hltL3fL2sMu3L3Filtered3",
                                                                    "hltL3fL2sMu5L3Filtered5",
                                                                    "hltL3fL2sMu7L3Filtered7",
                                                                    "hltL3fL2sMu12L3Filtered12",
                                                                    "hltL3fL2sMu15L3Filtered15")
                                )

process.hionia.primaryVertexTag = cms.InputTag("offlinePrimaryVertices")
process.hionia.genParticles     = cms.InputTag("genParticles")
process.hionia.muonLessPV       = cms.bool(False)
process.hionia.CentralitySrc    = cms.InputTag("")
process.hionia.CentralityBinSrc = cms.InputTag("")
process.hionia.srcTracks        = cms.InputTag("generalTracks")       

process.oniaTree = cms.EndPath(process.hionia)

process.options = cms.untracked.PSet(wantSummary = cms.untracked.bool(True))
process.source.fileNames      = cms.untracked.vstring(options.inputFiles)        
process.maxEvents             = cms.untracked.PSet( input = cms.untracked.int32(options.maxEvents) )
process.TFileService = cms.Service("TFileService", fileName = cms.string( options.outputFile ) )
process.schedule              = cms.Schedule(
    process.Onia2MuMuPAT,
    process.oniaTree
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
    cms.InputTag("hltL1fL1DoubleMuOpenL1Filtered0MassGT1",""), # 1
    cms.InputTag("hltL1fL1DoubleMuOpenL1Filtered0OS",""),      # 2
    cms.InputTag("hltL1fL1DoubleMuOpenL1Filtered0SS",""),      # 3
    cms.InputTag("hltL1fL1DoubleMuOpenL1Filtered0",""),        # 4
    cms.InputTag("hltL1fL1DoubleMu0L1Filtered0",""),           # 5
    cms.InputTag("hltL1fL1DoubleMu0L1Filtered0GTE8",""),       # 6
    cms.InputTag("hltL1fL1DoubleMu0L1Filtered0GTE9",""),       # 7
    cms.InputTag("hltL1fL1DoubleMu0L1Filtered0GTE10",""),      # 8
    cms.InputTag("hltL1fL1DoubleMu0L1Filtered0GTE11",""),      # 9
    cms.InputTag("hltL1fL1DoubleMu0L1Filtered0GTE12",""),      # 10
    cms.InputTag("hltL1fL1DoubleMu0L1Filtered0GTE13",""),      # 11
    cms.InputTag("hltL1fL1DoubleMu0L1Filtered0GTE14",""),      # 12
    cms.InputTag("hltL1fL1DoubleMu0L1Filtered0GTE15",""),      # 13
    cms.InputTag("hltL1fL1DoubleMu10L1Filtered0",""),          # 14
    cms.InputTag("hltL2fL1DoubleMu10L2Filtered0",""),          # 15
    cms.InputTag("hltL2fL1DoubleMuOpenL2Filtered0",""),        # 16
    cms.InputTag("hltHIL3fL2DimuonMuOpenL3Filtered0",""),      # 17
    cms.InputTag("hltL3fL2DimuonMuOpenL3Filtered0",""),        # 18
    cms.InputTag("hltHIL3fL2DimuonMu10L3Filtered10",""),       # 19
    cms.InputTag("hltL3fL2DimuonMu10L3Filtered10",""),         # 20
    cms.InputTag("hltL1fL1sMuOpenL1Filtered0",""),             # 21
    cms.InputTag("hltL1fL1sMu0L1Filtered0NoBptxAND",""),       # 22
    cms.InputTag("hltL1fL1sMu0L1Filtered0",""),                # 23
    cms.InputTag("hltL1fL1sMu12L1Filtered12NoBptxAND",""),     # 24
    cms.InputTag("hltL1fL1sMu12L1Filtered12",""),              # 25
    cms.InputTag("hltL1fL1sMu15L1Filtered15",""),              # 26
    cms.InputTag("hltL2fL1sMu0L2Filtered0",""),                # 27
    cms.InputTag("hltL2fL1sMu12L2Filtered12",""),              # 28
    cms.InputTag("hltL2fL1sMu15L2Filtered15",""),              # 29
    cms.InputTag("hltHIL3fL2sMu0L3Filtered0",""),              # 30
    cms.InputTag("hltL3fL2sMu0L3Filtered0",""),                # 31
    cms.InputTag("hltL3fL2sMu3L3Filtered3",""),                # 32
    cms.InputTag("hltL3fL2sMu5L3Filtered5",""),                # 33
    cms.InputTag("hltL3fL2sMu7L3Filtered7",""),                # 34
    cms.InputTag("hltL3fL2sMu12L3Filtered12",""),              # 35
    cms.InputTag("hltL3fL2sMu15L3Filtered15""")                # 36
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
