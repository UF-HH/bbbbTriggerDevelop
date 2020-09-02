import FWCore.ParameterSet.Config as cms
import os
import subprocess
import shlex
import json

def remove_duplicates(files):
    """
        Removing duplicate files to avoid double counting of
        entries.
        files: one-dimensional list of file paths from das query.
    """
    new_file_list = []
    for i in files:
        if i not in new_file_list:
            new_file_list.append(i)


    return new_file_list



def QueryFilesFromRuns(run=None, lumi=None):
    """
        Query to Das database to find files based on:
        run: list of runs. If only one run needed, mandatory to put it in brackets ex: [323725]
        lumi: list of lumisection. Valid what said for runs

    """

    print('...Query to Das, runs: {}, lumis: {}'.format(run,lumi))

    datasets = []
    for i in range(1,9):
        #if (i!= 2 and i!= 3 and i!= 6 and i!= 5 and i!=7):
        datasets.append('/EphemeralHLTPhysics{}/Run2018D-v1/RAW'.format(i))

    #REMOVE ONLY FOR THE MOMENT THE DTATASET 5 THAT IS 
    #INACCESSIBLE


    tot_file_list = []

    if (lumi is None and run is None):
        print('No runs or lumi selected')
        return tot_file_list


    if (lumi is None and len(run) > 0): #all lumis considered
        for dataset in datasets:
            parse = 'dasgoclient --query="file dataset={} run in {}"'.format(dataset, run)
            files = subprocess.Popen(shlex.split(parse), stdout=subprocess.PIPE)
            file_list = files.communicate()[0].split("\n") #dividing single tuple by \n
            file_list = file_list[:len(file_list)-1] #last element is ''

            for file in file_list:
                tot_file_list.append(file)

    elif (len(lumi) > 0 and len(run) > 0):
        for l in lumi:
            for dataset in datasets:
                parse = 'dasgoclient --query="file dataset={} run in {} lumi={}"'.format(dataset, run,l)
                files = subprocess.Popen(shlex.split(parse), stdout=subprocess.PIPE)
                file_list = files.communicate()[0].split("\n") #dividing single tuple by \n
                file_list = file_list[:len(file_list)-1] #last element is ''


                for file in file_list:
                    tot_file_list.append(file)

    to_ret = remove_duplicates(tot_file_list)
    print('...Queried DAS for a total of {} files'.format(len(to_ret)))
    return to_ret


def QueryFilesFromJson(file, runs=None, lumi=None):
    """
        Query to Das database to find files based on:
        json_file: usually golden json. Good runs and lumis are extracted 
                    and a query is made to das for those.
        runs=None : Select a runs of interest. the run has to be in the 
                    json. The program will scan all the lumis in the json
                    for that run and retrieve the files path.

    """

    print('...Query to Das, json: {}, runs: {}, lumis: {}'.format(file, runs, lumi))

    datasets = []
    tot_file_list = []

    for i in range(1,9):
        if (i!= 2 and i!= 3 and i!= 6 and i!= 5 and i!=7):
            datasets.append('/EphemeralHLTPhysics{}/Run2018D-v1/RAW'.format(i))


    with open(file) as json_file:
        data = json.load(json_file)


    if (len(data)==0):
        print('Json is empty')
        return tot_file_list

    good_runs = [key for key in data.keys()]


    if runs is None: #scan all good runs
        if lumi is None: #scan all the json
            for r in good_runs:
                lumis = data[run]
                if(len(lumis) > 0): #if the json selects specific lumis
                    for lumi_range in lumis:
                        for lumi in range(lumi_range[0], lumi_range[len(lumi_range)-1]):
                            for dataset in datasets:
                                parse = 'dasgoclient --query="file dataset={} run in [{}] lumi={}"'.format(dataset, r,lumi)
                                files = subprocess.Popen(shlex.split(parse), stdout=subprocess.PIPE)
                                file_list = files.communicate()[0].split("\n") #dividing single tuple by \n
                                file_list = file_list[:len(file_list)-1] #last element is ''

                                for file in file_list:
                                    tot_file_list.append(file)

                else: #if only run is present then query for all run
                    for dataset in datasets:
                        parse = 'dasgoclient --query="file dataset={} run in [{}] "'.format(dataset, r)
                        files = subprocess.Popen(shlex.split(parse), stdout=subprocess.PIPE)
                        file_list = files.communicate()[0].split("\n") #dividing single tuple by \n
                        file_list = file_list[:len(file_list)-1] #last element is ''

                        for file in file_list:
                            tot_file_list.append(file)
        else:
            for r in good_runs:
                lumis = data[run]
                
                if(len(lumis) > 0): #if the json selects specific lumis
                    for lumi_range in lumis:
                        for l in lumi:
                            if l in lumi_range:
                                for dataset in datasets:
                                    parse = 'dasgoclient --query="file dataset={} run in [{}] lumi={}"'.format(dataset, r,lumi)
                                    files = subprocess.Popen(shlex.split(parse), stdout=subprocess.PIPE)
                                    file_list = files.communicate()[0].split("\n") #dividing single tuple by \n
                                    file_list = file_list[:len(file_list)-1] #last element is ''

                                    for file in file_list:
                                        tot_file_list.append(file)

                else: #if only run is present then query for all run selected lumi
                    for l in lumi:
                        for dataset in datasets:
                            parse = 'dasgoclient --query="file dataset={} run in [{}] lumi={} "'.format(dataset, r,l)
                            files = subprocess.Popen(shlex.split(parse), stdout=subprocess.PIPE)
                            file_list = files.communicate()[0].split("\n") #dividing single tuple by \n
                            file_list = file_list[:len(file_list)-1] #last element is ''

                            for file in file_list:
                                tot_file_list.append(file)



    #if run is not None and lumi is none then scan for only that run all lumisections
    else: 
        if lumi is None:
            for run in runs:
                run = str(run)
                if run in good_runs:
                    lumis = data[run]
                    if(len(lumis) > 0): #if the json selects specific lumis
                        for lumi_range in lumis:
                            for lumi in range(lumi_range[0], lumi_range[len(lumi_range)-1]):
                                for dataset in datasets:
                                    parse = 'dasgoclient --query="file dataset={} run in [{}] lumi={}"'.format(dataset, run,lumi)
                                    files = subprocess.Popen(shlex.split(parse), stdout=subprocess.PIPE)
                                    file_list = files.communicate()[0].split("\n") #dividing single tuple by \n
                                    file_list = file_list[:len(file_list)-1] #last element is ''

                                    for file in file_list:
                                        tot_file_list.append(file)

                    else: #if only run is present then query for all run
                        for dataset in datasets:
                            parse = 'dasgoclient --query="file dataset={} run in [{}] "'.format(dataset, r)
                            files = subprocess.Popen(shlex.split(parse), stdout=subprocess.PIPE)
                            file_list = files.communicate()[0].split("\n") #dividing single tuple by \n
                            file_list = file_list[:len(file_list)-1] #last element is ''

                            for file in file_list:
                                tot_file_list.append(file)
                else:
                    print('Selected run {} is not in the json'.format(run))

        #if lumisection list is present:
        else:
            for run in runs:
                run = str(run)
                if run in good_runs:
                    lumis = data[run]
                    if(len(lumis) > 0): #if the json selects specific lumis
                        for lumi_range in lumis:
                            for l in lumi:
                                if l in range(lumi_range[0], lumi_range[len(lumi_range)-1]):
                                    for dataset in datasets:
                                        parse = 'dasgoclient --query="file dataset={} run in [{}] lumi={}"'.format(dataset, run,l)
                                        files = subprocess.Popen(shlex.split(parse), stdout=subprocess.PIPE)
                                        file_list = files.communicate()[0].split("\n") #dividing single tuple by \n
                                        file_list = file_list[:len(file_list)-1] #last element is ''

                                        for file in file_list:
                                            tot_file_list.append(file)

                    else: #if only run is present then query for all run
                        for l in lumi:
                            for dataset in datasets:
                                parse = 'dasgoclient --query="file dataset={} run in [{}] lumi={} "'.format(dataset, r,l)
                                files = subprocess.Popen(shlex.split(parse), stdout=subprocess.PIPE)
                                file_list = files.communicate()[0].split("\n") #dividing single tuple by \n
                                file_list = file_list[:len(file_list)-1] #last element is ''

                                for file in file_list:
                                    tot_file_list.append(file)
                else:
                    print('Selected run {} is not in the json'.format(run))





    to_ret = remove_duplicates(tot_file_list)
    print('...Queried DAS for a total of {} files'.format(len(to_ret)))
    return to_ret


def QueryFilesMCgg():

    tot_file_list = []

    parse = 'dasgoclient --query="file dataset=/GluGluToHHTo4B_node_SM_TuneCP5_14TeV-madgraph-pythia8/Run3Winter20DRPremixMiniAOD-110X_mcRun3_2021_realistic_v6-v2/GEN-SIM-RAW"'
    files = subprocess.Popen(shlex.split(parse), stdout=subprocess.PIPE)
    file_list = files.communicate()[0].split("\n") #dividing single tuple by \n
    file_list = file_list[:len(file_list)-1] #last element is ''

    for file in file_list:
        tot_file_list.append(file)

    to_ret = remove_duplicates(tot_file_list)
    print('...Queried DAS for a total of {} MC files'.format(len(to_ret)))
    return to_ret

def QueryFilesFromDataset(dataset):

    tot_file_list = []

    parse = 'dasgoclient --query="file dataset={}"'.format(dataset)
    files = subprocess.Popen(shlex.split(parse), stdout=subprocess.PIPE)
    file_list = files.communicate()[0].split("\n") #dividing single tuple by \n
    file_list = file_list[:len(file_list)-1] #last element is ''

    for file in file_list:
        tot_file_list.append(file)

    to_ret = remove_duplicates(tot_file_list)
    print('...Queried DAS for a total of {} MC files'.format(len(to_ret)))
    return to_ret


def QueryFilesMCggRAWandAOD(convert_to_cms = False):

    tot_file_raw = []

    parse = 'dasgoclient --query="file dataset=/GluGluToHHTo4B_node_SM_TuneCP5_14TeV-madgraph-pythia8/Run3Winter20DRPremixMiniAOD-110X_mcRun3_2021_realistic_v6-v2/GEN-SIM-RAW"'
    files = subprocess.Popen(shlex.split(parse), stdout=subprocess.PIPE)
    file_list = files.communicate()[0].split("\n") #dividing single tuple by \n
    file_list = file_list[:len(file_list)-1] #last element is ''

    for file in file_list:
        tot_file_raw.append(file)

    tot_file_AOD = []

    parse = 'dasgoclient --query="file dataset=/GluGluToHHTo4B_node_SM_TuneCP5_14TeV-madgraph-pythia8/Run3Winter20DRPremixMiniAOD-110X_mcRun3_2021_realistic_v6-v2/MINIAODSIM"'
    files = subprocess.Popen(shlex.split(parse), stdout=subprocess.PIPE)
    file_list = files.communicate()[0].split("\n") #dividing single tuple by \n
    file_list = file_list[:len(file_list)-1] #last element is ''

    for file in file_list:
        tot_file_AOD.append(file)

    tot_file_raw = remove_duplicates(tot_file_raw)
    tot_file_AOD = remove_duplicates(tot_file_AOD)
    print('...Queried DAS for a total of {} RAW files'.format(len(tot_file_raw)))
    print('...Queried DAS for a total of {} AOD files'.format(len(tot_file_AOD)))

    if convert_to_cms:
        tot_file_raw = cms.untracked.vstring(tot_file_raw)
        tot_file_AOD = cms.untracked.vstring(tot_file_AOD)

    return tot_file_raw, tot_file_AOD


def QueryForRAWandAOD(raw_data, aod_data, convert_to_cms = False):

    tot_file_raw = []

    parse = 'dasgoclient --query="file dataset={}"'.format(raw_data)
    files = subprocess.Popen(shlex.split(parse), stdout=subprocess.PIPE)
    file_list = files.communicate()[0].split("\n") #dividing single tuple by \n
    file_list = file_list[:len(file_list)-1] #last element is ''

    for file in file_list:
        tot_file_raw.append(file)

    tot_file_AOD = []

    parse = 'dasgoclient --query="file dataset={}"'.format(aod_data)
    files = subprocess.Popen(shlex.split(parse), stdout=subprocess.PIPE)
    file_list = files.communicate()[0].split("\n") #dividing single tuple by \n
    file_list = file_list[:len(file_list)-1] #last element is ''

    for file in file_list:
        tot_file_AOD.append(file)

    tot_file_raw = remove_duplicates(tot_file_raw)
    tot_file_AOD = remove_duplicates(tot_file_AOD)
    print('...Queried DAS for a total of {} RAW files'.format(len(tot_file_raw)))
    print('...Queried DAS for a total of {} AOD files'.format(len(tot_file_AOD)))

    if convert_to_cms:
        tot_file_raw = cms.untracked.vstring(tot_file_raw)
        tot_file_AOD = cms.untracked.vstring(tot_file_AOD)

    return tot_file_raw, tot_file_AOD

def QueryForTimingFiles(run):
    #Pay attention, this can only be used in vocms3/4
    if run in [325022, 324878]:
        run = [325022, 324878]
        pu = [36,52]
        ps = "2.0e34+ZB+HLTPhysics"
        file_names = ["EphemeralHLTPhysics_PU36-38_325022and324878.root", "EphemeralHLTPhysics_PU36-38_325022and324878001.root", "EphemeralHLTPhysics_PU36-38_325022and324878002.root", "EphemeralHLTPhysics_PU36-38_325022and324878003.root", "EphemeralHLTPhysics_PU36-38_325022and324878004.root", "EphemeralHLTPhysics_PU36-38_325022and324878005.root",
        "EphemeralHLTPhysics_PU38-40_325022000.root", "EphemeralHLTPhysics_PU38-40_325022001.root", "EphemeralHLTPhysics_PU38-40_325022002.root", "EphemeralHLTPhysics_PU38-40_325022003.root", "EphemeralHLTPhysics_PU38-40_325022004.root", "EphemeralHLTPhysics_PU40-42_325022and324878.root", "EphemeralHLTPhysics_PU40-42_325022and324878001.root",
        "EphemeralHLTPhysics_PU40-42_325022and324878002.root", "EphemeralHLTPhysics_PU40-42_325022and324878003.root", "EphemeralHLTPhysics_PU40-42_325022and324878004.root", "EphemeralHLTPhysics_PU40-42_325022and324878005.root", "EphemeralHLTPhysics_PU40-42_325022and324878006.root", "EphemeralHLTPhysics_PU42-44_325022and324878.root", "EphemeralHLTPhysics_PU42-44_325022and324878001.root",
        "EphemeralHLTPhysics_PU42-44_325022and324878002.root", "EphemeralHLTPhysics_PU42-44_325022and324878003.root", "EphemeralHLTPhysics_PU42-44_325022and324878004.root", "EphemeralHLTPhysics_PU42-44_325022and324878005.root", "EphemeralHLTPhysics_PU42-44_325022and324878006.root", "EphemeralHLTPhysics_PU44-46_325022and324878.root","EphemeralHLTPhysics_PU44-46_325022and324878001.root",
        "EphemeralHLTPhysics_PU44-46_325022and324878002.root", "EphemeralHLTPhysics_PU44-46_325022and324878003.root", "EphemeralHLTPhysics_PU44-46_325022and324878004.root", "EphemeralHLTPhysics_PU44-46_325022and324878005.root", "EphemeralHLTPhysics_PU46-48_325022and324878.root", "EphemeralHLTPhysics_PU46-48_325022and324878001.root", "EphemeralHLTPhysics_PU46-48_325022and324878002.root", 
        "EphemeralHLTPhysics_PU46-48_325022and324878003.root", "EphemeralHLTPhysics_PU46-48_325022and324878004.root", "EphemeralHLTPhysics_PU48-50_325022and324878.root", "EphemeralHLTPhysics_PU48-50_325022and324878001.root", "EphemeralHLTPhysics_PU48-50_325022and324878002.root", "EphemeralHLTPhysics_PU48-50_325022and324878003.root", "EphemeralHLTPhysics_PU48-50_325022and324878004.root", 
        "EphemeralHLTPhysics_PU50-52_325022and324878.root", "EphemeralHLTPhysics_PU50-52_325022and324878001.root", "EphemeralHLTPhysics_PU50-52_325022and324878002.root", "EphemeralHLTPhysics_PU50-52_325022and324878003.root", "EphemeralHLTPhysics_PU50-52_325022and324878004.root",]
        files = ["file:/data/user/ecarrera/timing_data/temp_Run2_TimingVsPU_study/skim2018_Ephemeral_325022and32487800_PUstudy/{}".format(i) for i in file_names]

    elif run in [319941]:
        run = [319941]
        pu = [48,49]
        ps = "2.0e34+ZB+HLTPhysics"
        file_names = ["EphemeralHLTPhysics_PU48-49_319941000.root","EphemeralHLTPhysics_PU48-49_319941001.root","EphemeralHLTPhysics_PU48-49_319941002.root","EphemeralHLTPhysics_PU48-49_319941003.root","EphemeralHLTPhysics_PU48-49_319941004.root","EphemeralHLTPhysics_PU48-49_319941005.root",]
        files = ["file:/data/user/ecarrera/timing_data/skim_Ephemeral_319941/{}".format(i) for i in file_names]

        return run , pu, ps, files

    elif run in [316457]:
        run = [316457]
        pu = [49,51]
        ps = "2.0e34+ZB+HLTPhysics"
        file_names = ["EphemeralHLTPhysics_316457000.root","EphemeralHLTPhysics_316457004.root","EphemeralHLTPhysics_316457001.root","EphemeralHLTPhysics_316457005.root","EphemeralHLTPhysics_316457002.root","EphemeralHLTPhysics_316457006.root","EphemeralHLTPhysics_316457003.root"]
        files = ["file:/data/user/ecarrera/timing_data/skim_Ephemeral_316457/{}".format(i) for i in file_names]
        return run , pu, ps, files

    elif run in [316058]:
        run = [316058]
        pu = [49,52]
        ps = "2.0e34+ZB+HLTPhysics"
        file_names = ["Ephemeral_PU49-52_316058000.root","Ephemeral_PU49-52_316058005.root", "Ephemeral_PU49-52_316058001.root","Ephemeral_PU49-52_316058006.root","Ephemeral_PU49-52_316058002.root","Ephemeral_PU49-52_316058007.root","Ephemeral_PU49-52_316058003.root","Ephemeral_PU49-52_316058008.root","Ephemeral_PU49-52_316058004.root"]
        files = ["file:/data/user/ecarrera/timing_data/skim_Ephemeral_316058/{}".format(i) for i in file_names]
        return run , pu, ps, files

    elif run in [315188]:
        run = [315188]
        pu = [55,58]
        ps = "2.0e34+ZB+HLTPhysics"
        file_names = ["HLTPhysics_PU55-58_315188000.root","HLTPhysics_PU55-58_315188003.root","HLTPhysics_PU55-58_315188001.root","HLTPhysics_PU55-58_315188004.root","HLTPhysics_PU55-58_315188002.root","HLTPhysics_PU55-58_315188005.root"]
        files = ["file:/data/user/ecarrera/timing_data/skim_HLTPhysics_315188/{}".format(i) for i in file_names]
        return run , pu, ps, files

    elif run in [315106]:
        run = [315106]
        pu = [51,52]
        ps = "2.0e34+ZB+HLTPhysics"
        file_names = ["HLTPhysics_PU51-52_315106000.root","HLTPhysics_PU51-52_315106004.root","HLTPhysics_PU51-52_315106001.root","HLTPhysics_PU51-52_315106005.root","HLTPhysics_PU51-52_315106002.root","HLTPhysics_PU51-52_315106006.root","HLTPhysics_PU51-52_315106003.root"]
        files = ["file:/data/user/ecarrera/timing_data/skim_HLTPhysics_315106/{}".format(i) for i in file_names]
        return run , pu, ps, files
