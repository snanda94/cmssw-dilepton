from WMCore.Configuration import Configuration
config = Configuration()

config.section_("General")
config.General.requestName = 'TnP_ppMC_GenTrackSTA'
config.General.workArea = 'TnP_ppMC_GenTrackSTA'
config.General.transferLogs = True

config.section_("JobType")
config.JobType.pluginName = 'Analysis'
config.JobType.psetName = 'tp_ppMC_sta.py'

config.section_("Data")
config.Data.inputDataset = '/PYTHIA6_JPsiWithFSR_tuneD6T_2TeV76/HiWinter13-STARTHI53_V28-v1/GEN-SIM-RECO'
config.Data.splitting = 'FileBased'
config.Data.unitsPerJob = 2
config.Data.publication = False

config.section_("Site")
config.Site.storageSite = 'T2_CH_CERN'


