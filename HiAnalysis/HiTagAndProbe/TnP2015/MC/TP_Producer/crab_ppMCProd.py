from CRABClient.UserUtilities import config, getUsernameFromSiteDB
config = config()

config.section_('General')
config.General.transferOutputs = True
config.General.transferLogs = True
config.General.requestName = 'TnP_ppMC_All'
config.General.workArea = 'TnP_ppMC_All_Area'

config.section_('JobType')
config.JobType.psetName = 'tp_Prod_Merge.py'
config.JobType.pluginName = 'Analysis'

config.section_('Data')
config.Data.inputDataset = '/PYTHIA6_JPsiWithFSR_tuneD6T_2TeV76/chflores-TnPskim_mc_pp_2013_20150915-c501f2d2f60b9a8d2d0420b894e9918c/USER'
config.Data.inputDBS = 'phys03'
config.Data.unitsPerJob = 1
config.Data.splitting = 'FileBased'
config.Data.outLFNDirBase = '/store/user/chflores/'

config.section_('Site')
config.Site.storageSite = 'T2_CH_CERN'

