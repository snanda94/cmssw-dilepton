from WMCore.Configuration import Configuration
config = Configuration()

config.section_("General")
config.General.requestName = 'TnPSkim_pp_MC_20150915'
config.General.workArea = 'TnP_pp_MC_2013'
config.General.transferLogs = True

config.section_("JobType")
config.JobType.pluginName = 'Analysis'
config.JobType.psetName = 'Onia2MuMuPAT_TnP2015_ppMC_cfg.py'

config.section_("Data")
config.Data.inputDBS = 'phys01'
config.Data.inputDataset = '/PYTHIA6_JPsiWithFSR_tuneD6T_2TeV76/miheejo-OniaSkim_JPsiWithFSR_HiWinter13-pp_STARTHI53_V28-v1-ccfdaebb33f7499d2adfd99ee89a7738/USER'
config.Data.splitting = 'FileBased'
config.Data.unitsPerJob = 10
config.Data.publication = True
# This string is used to construct the output dataset name
config.Data.publishDBS = 'phys03'
config.Data.publishDataName = 'TnPskim_mc_pp_2013_20150915'
config.Data.outLFNDirBase = '/store/group/phys_heavyions/chflores/'

config.section_("Site")
config.Site.storageSite = 'T2_CH_CERN'


