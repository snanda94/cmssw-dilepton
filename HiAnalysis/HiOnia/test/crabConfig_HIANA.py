from CRABClient.UserUtilities import config, getUsernameFromSiteDB
config = config()

config.section_('General')
config.General.requestName = ''                               # SET A NAME FOR ONIA TREE
config.General.workArea = 'crab_projects'
config.General.transferOutputs = True
config.General.transferLogs = True

config.section_('JobType')
config.JobType.pluginName = 'Analysis'
config.JobType.psetName = 'hioniaanalyzer_cfg.py'
config.JobType.maxMemoryMB = 2400
config.JobType.outputFiles = ['OniaTree.root']               # SET OUTPUT FILE NAME

config.section_('Data')
config.Data.inputDataset =''                                  # SET INPUT DATASET
config.Data.inputDBS = 'phys03'
config.Data.unitsPerJob = 1
config.Data.splitting = 'FileBased'
config.Data.outLFNDirBase = '/store/user/%s/PP_DATA/%s' % (getUsernameFromSiteDB(), config.General.requestName) # SET OUTPUT DIR BASE
config.Data.publication = False


config.section_('Site')
config.Site.storageSite = 'T2_FR_GRIF_LLR'


