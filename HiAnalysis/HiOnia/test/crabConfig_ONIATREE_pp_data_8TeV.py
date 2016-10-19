from CRABClient.UserUtilities import config, getUsernameFromSiteDB
config = config()

config.section_('General')
config.General.requestName = 'SingleMu-Run2012A-20161018'
config.General.workArea = 'crab_projects'
config.General.transferOutputs = True
config.General.transferLogs = True

config.section_('JobType')
config.JobType.pluginName = 'Analysis'
config.JobType.psetName = 'hioniaproducer_pp_8TeV_Data_cfg.py'
config.JobType.maxMemoryMB = 2500
config.JobType.outputFiles = ['OniaTree.root']

config.section_('Data')
config.Data.inputDataset ='/SingleMu/Run2012A-22Jan2013-v1/AOD'
config.Data.inputDBS = 'global'
config.Data.unitsPerJob = 10
config.Data.splitting = 'LumiBased'
config.Data.runRange = '190456-208686'
config.Data.lumiMask = '/afs/cern.ch/cms/CAF/CMSCOMM/COMM_DQM/certification/Collisions12/8TeV/Reprocessing/Cert_190456-208686_8TeV_22Jan2013ReReco_Collisions12_JSON.txt'
config.Data.outLFNDirBase = '/store/user/%s/pp8TeV/%s' % (getUsernameFromSiteDB(), config.General.requestName)
config.Data.publication = False

config.section_('Site')
# config.Site.whitelist = ["T2_FR_GRIF_LLR"]
config.Site.storageSite = 'T2_FR_GRIF_LLR'

# If your site is blacklisted by crab, use:
#config.Data.ignoreLocality = True
#config.Site.whitelist = ["T1_FR*","T2_FR*","T3_FR*"]
