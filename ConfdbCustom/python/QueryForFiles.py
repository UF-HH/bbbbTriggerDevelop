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
        if (i!= 2 and i!= 3 and i!= 6 and i!= 5 and i!=7):
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


    





        

