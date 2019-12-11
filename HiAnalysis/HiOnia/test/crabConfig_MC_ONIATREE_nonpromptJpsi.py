from CRABClient.UserUtilities import config, getUsernameFromSiteDB
config = config()

config.section_('General')
config.General.requestName = 'BJPsiMM_TuneCUETP8M1_5p02TeV_pythia8_05082019_ptHatMin2_ONIATREE'#'BcToJpsiMuNu_BCVEGPY_PYTHIA8_pp5TeV_24042019_3_ONIATREE_NoCuts'
config.General.workArea = 'crab_projects'
config.General.transferOutputs = True
config.General.transferLogs = True

config.section_('JobType')
config.JobType.pluginName = 'Analysis'
config.JobType.psetName = 'HiAnalysis/HiOnia/test/hioniaanalyzer_ppPrompt_trimuons_94X_MC_cfg.py'
#config.JobType.outputFiles = ['Oniatree_MC_trimuons.root']
#config.JobType.maxMemoryMB = 2500
config.JobType.maxJobRuntimeMin = 500

config.section_('Data')
config.Data.inputDataset = '/BJPsiMM_TuneCUETP8M1_5p02TeV_pythia8/RunIIpp5Spring18DR-94X_mc2017_realistic_forppRef5TeV_v1_ext1-v1/AODSIM'#'/BJpsiMM/gfalmagn-BJPsiMM_TuneCUETP8M1_5p02TeV_pythia8_16052018_withLambdab_ptHatMin10_reco-679d0c7a470a830e53fe749c4e83e359/USER'#'/BJpsiMM/gfalmagn-BJPsiMM_TuneCUETP8M1_5p02TeV_pythia8_16052018_withLambdab_ptHatMin2_reco-679d0c7a470a830e53fe749c4e83e359/USER'#'/BcToJpsiMuNu/gfalmagn-BcToJpsiMuNu_BCVEGPY_PYTHIA8_pp5TeV_22012019_3_reco_NoCuts-f2dad0906efe35dd2b4ba4e0d7abc43c/USER'#
config.Data.inputDBS ='global'#'phys03'#
config.Data.unitsPerJob = 2#15
#config.Data.totalUnits = 50
config.Data.splitting = 'FileBased'

config.Data.outLFNDirBase = '/store/user/%s/Bc_analysis/MC/%s' % (getUsernameFromSiteDB(), config.General.requestName)
config.Data.publication = False
config.Data.outputDatasetTag = config.General.requestName

config.section_('Site')
config.Site.storageSite = 'T2_FR_GRIF_LLR'
config.Data.ignoreLocality = True
config.Site.whitelist = ['T2_FR_*']#,'T2_CH_*','T2_BE_*'
