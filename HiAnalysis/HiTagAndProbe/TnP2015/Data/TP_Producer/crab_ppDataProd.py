from CRABClient.UserUtilities import config, getUsernameFromSiteDB
config = config()

config.section_('General')
config.General.transferOutputs = True
config.General.transferLogs = True
config.General.requestName = 'TnP_ppData_All'
config.General.workArea = 'TnP_ppData_All_Area_v2'

config.section_('JobType')
config.JobType.psetName = 'tp_Prod_Merge.py'
config.JobType.pluginName = 'Analysis'

config.section_('Data')
config.Data.inputDataset = '/PPMuon/chflores-TnPskim_data_pp_2013_20150911-38163c845f3d717d8384a634cbae2033/USER'
config.Data.inputDBS = 'phys03'
config.Data.unitsPerJob = 11
config.Data.splitting = 'FileBased'
config.Data.outLFNDirBase = '/store/user/chflores/'

config.section_('Site')
config.Site.storageSite = 'T2_CH_CERN'

