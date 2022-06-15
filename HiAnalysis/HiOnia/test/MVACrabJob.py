from CRABClient.UserUtilities import config, getUsername
config = config()

config.section_("General")
config.General.requestName = "MVAPbPbFullMC" 
config.General.workArea = "Crab_MVAPbPbFullMC"
config.General.transferLogs = True
config.General.transferOutputs = True

config.section_("JobType")
config.JobType.pluginName = "Analysis"
config.JobType.psetName = "hioniaanalyzer_PbPbPrompt_muonMVA_12_3_X_MC_cfg.py"
config.JobType.allowUndistributedCMSSW = True

config.section_("Data")
config.Data.inputDataset = "/JPsi_pThat-2_TuneCP5_HydjetDrumMB_5p02TeV_Pythia8/HINPbPbSpring21MiniAOD-mva98_112X_upgrade2018_realistic_HI_v9-v1/MINIAODSIM"
config.Data.inputDBS = 'global'
config.Data.splitting = 'FileBased'
config.Data.unitsPerJob = 10
config.Data.publication = False
config.Data.outputDatasetTag = 'PbPbFullMC_MVAanalysis'

config.section_("Site")
config.Site.storageSite = "T3_US_FNALLPC"
