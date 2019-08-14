from CRABClient.UserUtilities import config, getUsernameFromSiteDB
config = config()

config.section_('General')
config.General.requestName = 'BcToJpsiMuNu_BCVEGPY_PYTHIA8_pp5TeV_05082019_1_ONIATREE'#'BJPsiMM_TuneCUETP8M1_5p02TeV_pythia8_16052018_withLambdab_ptHatMin10_ONIATREE'#
config.General.workArea = 'crab_projects'
config.General.transferOutputs = True
config.General.transferLogs = True

config.section_('JobType')
config.JobType.pluginName = 'Analysis'
config.JobType.psetName = 'HiAnalysis/HiOnia/test/hioniaanalyzer_ppPrompt_trimuons_94X_MC_cfg.py'
#config.JobType.outputFiles = ['Oniatree_MC_trimuons.root']
config.JobType.maxMemoryMB = 2500
config.JobType.maxJobRuntimeMin = 120

config.section_('Data')
config.Data.inputDataset = '/BcToJpsiMuNu/gfalmagn-BcToJpsiMuNu_BCVEGPY_PYTHIA8_pp5TeV_22012019_1_reco_NoCuts-f2dad0906efe35dd2b4ba4e0d7abc43c/USER'#'/BJpsiMM/gfalmagn-BJPsiMM_TuneCUETP8M1_5p02TeV_pythia8_16052018_withLambdab_ptHatMin10_reco-679d0c7a470a830e53fe749c4e83e359/USER'#'/BJpsiMM/gfalmagn-BJPsiMM_TuneCUETP8M1_5p02TeV_pythia8_16052018_withLambdab_ptHatMin10_reco-679d0c7a470a830e53fe749c4e83e359/USER'#
config.Data.inputDBS = 'phys03'
config.Data.unitsPerJob = 10
#config.Data.totalUnits = 50
config.Data.splitting = 'FileBased'

config.Data.outLFNDirBase = '/store/user/%s/Bc_analysis/MC/%s' % (getUsernameFromSiteDB(), config.General.requestName)
config.Data.publication = False
config.Data.outputDatasetTag = config.General.requestName

config.section_('Site')
#config.Data.ignoreLocality = True
config.Site.storageSite = 'T2_FR_GRIF_LLR'
