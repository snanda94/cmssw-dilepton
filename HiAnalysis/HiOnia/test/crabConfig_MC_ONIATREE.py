from WMCore.Configuration import Configuration

config = Configuration()

config.section_("General")
config.General.requestName = 'BToJpsi_pThat-2_TuneCP5-EvtGen_HydjetDrumMB_HINPbPbAutumn18DR_trimuons_oniatree_25012021'#'BcToJpsiMuNu_BCVEGPY_PYTHIA8_2018PbPb5TeV_HINPbPbAutumn18DR-00196_09012020_2_ONIATREE'#"BToJpsi_pThat-2_TuneCP5-EvtGen_HydjetDrumMB_trimuons_oniatree_09012020"#"HIDoubleMuonPsiPeri_Run2018A_AOD_OniaTree_Run_327123_327564_flippedJpsi_BcTrimuon_27122019"#BcToJpsiMuNu_BCVEGPY_PYTHIA8_2018PbPb5TeV_09012020_1_ONIATREE
config.General.workArea = 'crab_projects'
config.General.transferOutputs = True
config.General.transferLogs = True

config.section_("JobType")
config.JobType.pluginName = "Analysis"
config.JobType.psetName = "HiAnalysis/HiOnia/test/hioniaanalyzer_PbPbPrompt_trimuons_103X_MC_cfg.py"#"HiAnalysis/HiOnia/test/hioniaanalyzer_PbPbPrompt_trimuons_103X_DATA_cfg.py"
#config.JobType.maxMemoryMB = 2500         # request high memory machines.
#config.JobType.numCores = 4
config.JobType.allowUndistributedCMSSW = True #Problems with slc7
config.JobType.maxJobRuntimeMin = 500 #2750    # request longer runtime, ~48 hours.

config.section_("Data")
config.Data.inputDataset = '/BToJpsi_pThat-2_TuneCP5-EvtGen_HydjetDrumMB_5p02TeV_pythia8/HINPbPbAutumn18DR-mva98_103X_upgrade2018_realistic_HI_v11-v1/AODSIM'#'/JPsi_pThat-2_TuneCP5_HydjetDrumMB_5p02TeV_Pythia8/HINPbPbAutumn18DR-mva98_103X_upgrade2018_realistic_HI_v11-v1/AODSIM'#'/BcToJpsiMuNu_TuneCP5_5p02TeV_BCVEGPY_pythia8-evtgen/HINPbPbAutumn18DR-FixL1CaloGT_103X_upgrade2018_realistic_HI_v13-v2/AODSIM'#/BcToJpsiMuNu/gfalmagn-BcToJpsiMuNu_BCVEGPY_PYTHIA8_2018PbPb5TeV_18072019_1_reco-d2c80626fee9f1a8e69409e960d0a051/USER #goes until sample 3_reco
config.Data.inputDBS ='global'
config.Data.unitsPerJob = 14#8000#70000
#config.Data.totalUnits = -1
config.Data.splitting = "FileBased"#EventAwareLumiBased
#config.Data.allowNonValidInputDataset = True

config.Data.outLFNDirBase = '/store/user/gfalmagn/Bc_analysis/MC/%s' % (config.General.requestName)
config.Data.publication = False
#config.Data.runRange = '327123-327564'#'327123-327564'#'326381-327122' or 326382 or 327564
#config.Data.lumiMask = 'https://cms-service-dqm.web.cern.ch/cms-service-dqm/CAF/certification/Collisions18/HI/PromptReco/Cert_326381-327564_HI_PromptReco_Collisions18_JSON_HF_and_MuonPhys.txt'

config.section_("Site")
config.Site.storageSite = "T2_FR_GRIF_LLR"
#config.Site.whitelist = ["T2_CH_CERN"]
