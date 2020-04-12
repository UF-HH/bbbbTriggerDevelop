from CRABClient.UserUtilities import config, getUsernameFromSiteDB
config = config()

config.General.requestName = 'reqname'
config.General.workArea    = 'crab3'
config.General.transferOutputs = True
config.General.transferLogs    = False

config.JobType.pluginName    = 'Analysis'
config.JobType.psetName      = 'hlt_wbtag.py'

config.Data.inputDataset     = 'mydataset'
config.Data.inputDBS         = 'global'
config.Data.splitting        = 'Automatic'
config.Data.runRange = '325022-325022'

config.JobType.numCores = 4
config.Data.unitsPerJob      = 180
config.Data.outLFNDirBase    = '/store/user/gboldrin/325022/'
config.Data.publication      = False
config.Data.outputDatasetTag = 'outtag'
config.Data.lumiMask="Json/json_2018D_Ephemeral_20181022.txt"

config.Site.storageSite      = 'T3_US_FNALLPC'