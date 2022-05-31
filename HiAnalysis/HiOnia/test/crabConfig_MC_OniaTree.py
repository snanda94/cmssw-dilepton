from WMCore.Configuration import Configuration

config = Configuration()

config.section_("General")
config.General.requestName = "Run3cond_SingleMuEmbedded_0p5_3"
config.General.workArea = 'crab_projects'
config.General.transferOutputs = True
config.General.transferLogs = True

config.section_("JobType")
config.JobType.pluginName = "Analysis"
config.JobType.psetName = "hioniaanalyzer_PbPbPrompt_12_3_X_MC_cfg.py"
#config.JobType.maxMemoryMB = 2500         # request high memory machines.
#config.JobType.numCores = 4
config.JobType.allowUndistributedCMSSW = True #Problems with slc7
#config.JobType.maxJobRuntimeMin = 1000 #2750    # request longer runtime, ~48 hours.

config.section_("Data")
config.Data.inputDataset = '/SingleMuEMBPt_0p5_3_pythia8_CMSSW_12_3_0/soohwan-RECO_MC_SingleMuEMBPt_0p5_3_CMSSW_12_3_0_220529_v2-1053bdd6c72aaff65c2568bf089c019a/USER'
#config.Data.inputDataset = '/SingleMuEMBPt_3_100_pythia8_CMSSW_12_3_0/soohwan-RECO_MC_SingleMuEMBPt_3_100_CMSSW_12_3_0_220530_v1-1053bdd6c72aaff65c2568bf089c019a/USER'
config.Data.inputDBS = 'phys03'
#config.Data.unitsPerJob = 10
#config.Data.totalUnits = -1
config.Data.splitting = "Automatic"
config.Data.allowNonValidInputDataset = True

config.Data.outLFNDirBase = '/store/user/fdamas/2022DileptonRunPrep/'
config.Data.outputDatasetTag = config.General.requestName
config.Data.publication = False
#config.Data.runRange = '327237-327237'#'327123-327564'#'326381-327122' or 326382 or 327564
#config.Data.lumiMask = '/afs/cern.ch/cms/CAF/CMSCOMM/COMM_DQM/certification/Collisions18/HI/PromptReco/Cert_326381-327564_HI_PromptReco_Collisions18_JSON_HF_and_MuonPhys.txt'

config.section_("Site")
config.Site.storageSite = 'T2_FR_GRIF_LLR'
#config.Site.whitelist = ['T2_FR_*']
