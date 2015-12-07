# for the list of used tags please see:
# https://twiki.cern.ch/twiki/bin/view/CMS/Onia2MuMuSamples

import FWCore.ParameterSet.Config as cms
import FWCore.ParameterSet.VarParsing as VarParsing

# set up process
process = cms.Process("Onia2MuMuPAT")

# Conditions
HLTProName = "HLT"
isPbPb = True;          
isMC = False;
keepGeneralTracks = False;
keepEventPlane = True;
muonSelection = "GlbTrk" # Single muon selection: Glb(isGlobal), GlbTrk(isGlobal&&isTracker), Trk(isTracker) are availale

# setup 'analysis'  options
options = VarParsing.VarParsing ('analysis')

# setup any defaults you want
options.inputFiles = '/store/hidata/HIRun2015/HIOniaL1DoubleMu0/AOD/PromptReco-v1/000/262/694/00000/16147348-3A99-E511-84D5-02163E0141A8.root'
options.outputFile = 'onia2MuMuPAT_DATA_75X.root'

options.maxEvents = -1 # -1 means all events

# get and parse the command line arguments
options.parseArguments()
process.load("FWCore.MessageService.MessageLogger_cfi")
process.MessageLogger.cerr.FwkReport.reportEvery = 1000

process.load('Configuration.StandardSequences.Services_cff')
process.load('SimGeneral.HepPDTESSource.pythiapdt_cfi')
process.load('FWCore.MessageService.MessageLogger_cfi')
process.load('Configuration.EventContent.EventContentHeavyIons_cff')
process.load('SimGeneral.MixingModule.mixNoPU_cfi')
process.load('Configuration.StandardSequences.GeometryRecoDB_cff')
process.load('Configuration.StandardSequences.ReconstructionHeavyIons_cff')
process.load("Configuration.StandardSequences.MagneticField_38T_cff")

# Global Tag
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_condDBv2_cff')
from Configuration.AlCa.GlobalTag_condDBv2 import GlobalTag
if isMC:
  if isPbPb:
    process.GlobalTag = GlobalTag(process.GlobalTag, 'auto:run2_mc_HIon', '')
  else:
    process.GlobalTag = GlobalTag(process.GlobalTag, 'auto:run2_mc', '')
else:  
  process.GlobalTag = GlobalTag(process.GlobalTag, '75X_dataRun2_PromptHI_v3', '')
process.GlobalTag.snapshotTime = cms.string("9999-12-31 23:59:59.000")

'''
# BSC or HF coincidence (masked unprescaled L1 bits)
process.load('L1Trigger.Skimmer.l1Filter_cfi')
process.bscOrHfCoinc = process.l1Filter.clone(
    algorithms = cms.vstring('*','L1_HcalHfCoincPmORBscMinBiasThresh1_BptxAND_instance1', 'L1_NotBsc2_BscMinBiasOR', 'L1_HcalHfCoincidencePm')
    )
'''
# HLT Dimuon Triggers
import HLTrigger.HLTfilters.hltHighLevel_cfi
process.hltOniaHI = HLTrigger.HLTfilters.hltHighLevel_cfi.hltHighLevel.clone()
# HLT PbPbP MENU:  /online/collisions/2015/HeavyIons/v1.0/HLT/V6
process.hltOniaHI.HLTPaths =  [
    "HLT_HIL1DoubleMu0_v1",
    "HLT_HIL1DoubleMu0_2HF_v1",
    "HLT_HIL1DoubleMu0_2HF0_v1",
    "HLT_HIL1DoubleMu10_v1",
    "HLT_HIL2DoubleMu0_NHitQ_v2",
    "HLT_HIL2DoubleMu0_NHitQ_2HF_v1",
    "HLT_HIL2DoubleMu0_NHitQ_2HF0_v1",
    "HLT_HIL1DoubleMu0_2HF_Cent30100_v1",
    "HLT_HIL1DoubleMu0_2HF0_Cent30100_v1",
    "HLT_HIL2DoubleMu0_2HF_Cent30100_NHitQ_v1",
    "HLT_HIL1DoubleMu0_Cent30_v1",
    "HLT_HIL2DoubleMu0_2HF0_Cent30100_NHitQ_v1",
    "HLT_HIL2DoubleMu0_Cent30_NHitQ_v1",
    "HLT_HIL2DoubleMu0_Cent30_OS_NHitQ_v1",
    "HLT_HIL3DoubleMu0_Cent30_v1",
    "HLT_HIL3DoubleMu0_Cent30_OS_m2p5to4p5_v1",
    "HLT_HIL3DoubleMu0_Cent30_OS_m7to14_v1",
    "HLT_HIL3DoubleMu0_OS_m2p5to4p5_v1",
    "HLT_HIL3DoubleMu0_OS_m7to14_v1",
    "HLT_HIL2Mu3_NHitQ10_2HF_v1",
    "HLT_HIL2Mu3_NHitQ10_2HF0_v1",
    "HLT_HIL3Mu3_NHitQ15_2HF_v1",
    "HLT_HIL3Mu3_NHitQ15_2HF0_v1",
    "HLT_HIL2Mu5_NHitQ10_2HF_v1",
    "HLT_HIL2Mu5_NHitQ10_2HF0_v1",
    "HLT_HIL3Mu5_NHitQ15_2HF_v1",
    "HLT_HIL3Mu5_NHitQ15_2HF0_v1",
    "HLT_HIL2Mu7_NHitQ10_2HF_v1",
    "HLT_HIL2Mu7_NHitQ10_2HF0_v1",
    "HLT_HIL3Mu7_NHitQ15_2HF_v1",
    "HLT_HIL3Mu7_NHitQ15_2HF0_v1",
    "HLT_HIL2Mu15_v2",
    "HLT_HIL2Mu15_2HF_v1",
    "HLT_HIL2Mu15_2HF0_v1",
    "HLT_HIL3Mu15_v1",
    "HLT_HIL3Mu15_2HF_v1",
    "HLT_HIL3Mu15_2HF0_v1",
    "HLT_HIL2Mu20_v1",
    "HLT_HIL2Mu20_2HF_v1",	
    "HLT_HIL2Mu20_2HF0_v1",
    "HLT_HIL3Mu20_v1",
    "HLT_HIL3Mu20_2HF_v1",
    "HLT_HIL3Mu20_2HF0_v1"
    ]
process.hltOniaHI.throw = False
process.hltOniaHI.andOr = True
process.hltOniaHI.TriggerResultsTag = cms.InputTag("TriggerResults","",HLTProName)

from HiSkim.HiOnia2MuMu.onia2MuMuPAT_cff import *
onia2MuMuPAT(process, GlobalTag=process.GlobalTag.globaltag, MC=isMC, HLT=HLTProName, Filter=True)

### Temporal fix for the PAT Trigger prescale warnings.
process.patTriggerFull = cms.EDProducer( "PATTriggerProducer",
                                         l1GtReadoutRecordInputTag = cms.InputTag("gtDigis","","RECO"),
                                         onlyStandAlone = cms.bool( True ),
                                         processName    = cms.string( HLTProName )                    
                                         )
###

##### Onia2MuMuPAT input collections/options
process.onia2MuMuPatGlbGlb.dimuonSelection          = cms.string("mass > 0")
process.onia2MuMuPatGlbGlb.resolvePileUpAmbiguity   = False
if isPbPb:
  process.onia2MuMuPatGlbGlb.primaryVertexTag         = cms.InputTag("hiSelectedVertex")
  process.patMuonsWithoutTrigger.pvSrc                = cms.InputTag("hiSelectedVertex")
  process.onia2MuMuPatGlbGlb.addMuonlessPrimaryVertex = False
  if isMC:
    process.genMuons.src = "genParticles"
    process.onia2MuMuPatGlbGlb.genParticles = "genParticles"
else: # ispp
  process.onia2MuMuPatGlbGlb.primaryVertexTag         = cms.InputTag("offlinePrimaryVertices")
  process.patMuonsWithoutTrigger.pvSrc                = cms.InputTag("offlinePrimaryVertices")
  # Adding muonLessPV gives you lifetime values wrt. muonLessPV only
  process.onia2MuMuPatGlbGlb.addMuonlessPrimaryVertex = True
  if isMC:
    process.genMuons.src = "genParticles"
    process.onia2MuMuPatGlbGlb.genParticles = "genParticles"

##### Remove few paths for MC
if isMC:
  process.patMuonSequence.remove(process.hltOniaHI)

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

process.outOnia2MuMu.outputCommands.append("keep recoConversions_*_*_*")
##### If single track collection has to be kept
if keepGeneralTracks:
  process.outOnia2MuMu.outputCommands.append("keep *_standAloneMuons_*_*")
  if isPbPb: process.outOnia2MuMu.outputCommands.append("keep *_hiGeneralTracks_*_*")
  else: process.outOnia2MuMu.outputCommands.append("keep *_generalTracks_*_*")
##### If event plane collection has to be kept
if keepEventPlane:
  process.outOnia2MuMu.outputCommands.append("keep *_hiEvtPlane_*_*")



process.source.fileNames      = cms.untracked.vstring(options.inputFiles)        
process.maxEvents             = cms.untracked.PSet( input = cms.untracked.int32(options.maxEvents) )
process.outOnia2MuMu.fileName = cms.untracked.string( options.outputFile )
process.e                     = cms.EndPath(process.outOnia2MuMu)
process.schedule              = cms.Schedule(process.Onia2MuMuPAT,process.e)

from Configuration.Applications.ConfigBuilder import MassReplaceInputTag
MassReplaceInputTag(process)



'''

FOR PbPb: /online/collisions/2015/HeavyIons/v1.0/HLT/V6

process.hltOniaHI.HLTPaths = [
    "HLT_HIL1DoubleMu0_v1",
    "HLT_HIL1DoubleMu0_2HF_v1",
    "HLT_HIL1DoubleMu0_2HF0_v1",
    "HLT_HIL1DoubleMu10_v1",
    "HLT_HIL2DoubleMu0_NHitQ_v2",
    "HLT_HIL2DoubleMu0_NHitQ_2HF_v1",
    "HLT_HIL2DoubleMu0_NHitQ_2HF0_v1",
    "HLT_HIL1DoubleMu0_2HF_Cent30100_v1",
    "HLT_HIL1DoubleMu0_2HF0_Cent30100_v1",
    "HLT_HIL2DoubleMu0_2HF_Cent30100_NHitQ_v1",
    "HLT_HIL1DoubleMu0_Cent30_v1",
    "HLT_HIL2DoubleMu0_2HF0_Cent30100_NHitQ_v1",
    "HLT_HIL2DoubleMu0_Cent30_NHitQ_v1",
    "HLT_HIL2DoubleMu0_Cent30_OS_NHitQ_v1",
    "HLT_HIL3DoubleMu0_Cent30_v1",
    "HLT_HIL3DoubleMu0_Cent30_OS_m2p5to4p5_v1",
    "HLT_HIL3DoubleMu0_Cent30_OS_m7to14_v1",
    "HLT_HIL3DoubleMu0_OS_m2p5to4p5_v1",
    "HLT_HIL3DoubleMu0_OS_m7to14_v1",
    "HLT_HIL2Mu3_NHitQ10_2HF_v1",
    "HLT_HIL2Mu3_NHitQ10_2HF0_v1",
    "HLT_HIL3Mu3_NHitQ15_2HF_v1",
    "HLT_HIL3Mu3_NHitQ15_2HF0_v1",
    "HLT_HIL2Mu5_NHitQ10_2HF_v1",
    "HLT_HIL2Mu5_NHitQ10_2HF0_v1",
    "HLT_HIL3Mu5_NHitQ15_2HF_v1",
    "HLT_HIL3Mu5_NHitQ15_2HF0_v1",
    "HLT_HIL2Mu7_NHitQ10_2HF_v1",
    "HLT_HIL2Mu7_NHitQ10_2HF0_v1",
    "HLT_HIL3Mu7_NHitQ15_2HF_v1",
    "HLT_HIL3Mu7_NHitQ15_2HF0_v1",
    "HLT_HIL2Mu15_v2",
    "HLT_HIL2Mu15_2HF_v1",
    "HLT_HIL2Mu15_2HF0_v1",
    "HLT_HIL3Mu15_v1",
    "HLT_HIL3Mu15_2HF_v1",
    "HLT_HIL3Mu15_2HF0_v1",
    "HLT_HIL2Mu20_v1",
    "HLT_HIL2Mu20_2HF_v1",	
    "HLT_HIL2Mu20_2HF0_v1",
    "HLT_HIL3Mu20_v1",
    "HLT_HIL3Mu20_2HF_v1",
    "HLT_HIL3Mu20_2HF0_v1"
    ]

FOR PP: /users/HiMuonTrigDev/pp5TeV/NovDev/V4

process.hltOniaHI.HLTPaths = [
    "HLT_HIL1DoubleMu0_v1",
    "HLT_HIL1DoubleMu10_v1",
    "HLT_HIL2DoubleMu0_NHitQ_v1",
    "HLT_HIL3DoubleMu0_OS_m2p5to4p5_v1",
    "HLT_HIL3DoubleMu0_OS_m7to14_v1",
    "HLT_HIL2Mu3_NHitQ10_v1",
    "HLT_HIL3Mu3_NHitQ15_v1",
    "HLT_HIL2Mu5_NHitQ10_v1",
    "HLT_HIL3Mu5_NHitQ15_v1",
    "HLT_HIL2Mu7_NHitQ10_v1",
    "HLT_HIL3Mu7_NHitQ15_v1",
    "HLT_HIL2Mu15_v1",
    "HLT_HIL3Mu15_v1",
    "HLT_HIL2Mu20_v1",
    "HLT_HIL3Mu20_v1"
    ]

'''
