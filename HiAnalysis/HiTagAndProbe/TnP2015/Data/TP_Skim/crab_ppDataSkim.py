from WMCore.Configuration import Configuration
config = Configuration()

config.section_("General")
config.General.requestName = 'TnPSkim_pp_Data_20150911'
config.General.workArea = 'TnP_pp_2013'
config.General.transferLogs = True

config.section_("JobType")
config.JobType.pluginName = 'Analysis'
config.JobType.psetName = 'Onia2MuMuPAT_TnP2015_ppData_cfg.py'

config.section_("Data")
config.Data.inputDBS = 'phys01'
config.Data.inputDataset = '/PPMuon/miheejo-AllMass_OniaSkim_pp_Runs211739-211831_GlbGlb-f690500131436c8bcda60574aa65181b/USER'
config.Data.splitting = 'LumiBased'
config.Data.unitsPerJob = 10
config.Data.lumiMask = 'Cert_211739-211831_2760GeV_PromptReco_Collisions13_JSON_MuonPhys.txt'
config.Data.runRange = '211739-211831'
config.Data.ignoreLocality = True
config.Data.publication = True
# This string is used to construct the output dataset name
config.Data.publishDBS = 'phys03'
config.Data.publishDataName = 'TnPskim_data_pp_2013_20150911'
config.Data.outLFNDirBase = '/store/group/phys_heavyions/chflores/'

config.section_("Site")
config.Site.storageSite = 'T2_CH_CERN'

