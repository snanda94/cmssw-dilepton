from CRABClient.UserUtilities import config, getUsernameFromSiteDB
config = config()

config.section_('General')
config.General.requestName = 'JPsiMM_TuneCUETP8M1_5p02TeV_pythia8_05082019_ptHatMin45_ONIATREE'
config.General.workArea = 'crab_projects'
config.General.transferOutputs = True
config.General.transferLogs = True

config.section_('JobType')
config.JobType.pluginName = 'Analysis'
config.JobType.psetName = 'HiAnalysis/HiOnia/test/hioniaanalyzer_ppPrompt_trimuons_94X_MC_cfg.py'
#config.JobType.outputFiles = ['Oniatree_MC_trimuons.root']
config.JobType.maxMemoryMB = 2500
config.JobType.maxJobRuntimeMin = 230

config.section_('Data')
config.Data.inputDataset = '/JPsiMM_pThat-45_TuneCUETP8M1_5p02TeV_pythia8/RunIIpp5Spring18DR-94X_mc2017_realistic_forppRef5TeV-v2/AODSIM'#'/JPsiMM_pThat-35_TuneCUETP8M1_5p02TeV_pythia8/RunIIpp5Spring18DR-94X_mc2017_realistic_forppRef5TeV-v2/AODSIM'#'/JPsiMM_pThat-25_TuneCUETP8M1_5p02TeV_pythia8/RunIIpp5Spring18DR-94X_mc2017_realistic_forppRef5TeV-v2/AODSIM'#'/JPsiMM_pThat-15_TuneCUETP8M1_5p02TeV_pythia8/RunIIpp5Spring18DR-94X_mc2017_realistic_forppRef5TeV-v2/AODSIM'#'/JPsiMM_TuneCUETP8M1_5p02TeV_pythia8/RunIIpp5Spring18DR-94X_mc2017_realistic_forppRef5TeV-v2/AODSIM'#
config.Data.inputDBS = 'global'
config.Data.unitsPerJob = 2
#config.Data.totalUnits = 50
config.Data.splitting = 'FileBased'

config.Data.outLFNDirBase = '/store/user/%s/Bc_analysis/MC/%s' % (getUsernameFromSiteDB(), config.General.requestName)
config.Data.publication = False
config.Data.outputDatasetTag = config.General.requestName

config.section_('Site')
#config.Data.ignoreLocality = True
config.Site.storageSite = 'T2_FR_GRIF_LLR'
