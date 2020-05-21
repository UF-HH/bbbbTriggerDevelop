import FiltersDef

"""
    Functions that modify menus
"""

def countBrack(line):
    return line.count("(") -line.count(")")


def FindModule(menu_file, module_name):

    moduleDef = []
    count_lines = 0

    f = open(menu_file, "r")
    contents = f.readlines()
    f.close()

    for i in range(len(contents)):
        line = contents[i]
        if module_name in line:
            moduleDef.append(line)
            count_brack = 0
            if countBrack(line) == 0:
                return i-1, moduleDef #index i is one step ahead
            else:
                while 1:
                    line = contents[i]
                    count_brack += line.count("(") - line.count(")")
                    if line not in moduleDef: moduleDef.append(line)
                    if count_brack == 0:
                        return i, moduleDef #index i is one step ahead
                    else:
                        i = i+1


def FindLine(menu, name):

    f = open(menu, "r")
    contents = f.readlines()
    f.close()

    count_lines = 0
    for line in contents:
        if name in line:
            break
        count_lines +=1

    return count_lines

def ModifyModuleName(module, new_name):
    for i in range(len(module)):
        if "." in module[i]:
            name = (module[i].split('.'))[1].split("=")[0]
            module[i] = module[i].replace(name, new_name)
            break
    return module

def ModifyModuleThreshold(module, th_name, new_th):
    for i in range(len(module)):
        if th_name in module[i]:
            th = (module[i].split('('))[1].split(")")[0]
            module[i] = module[i].replace(th, new_th)
            break
    return module

def AppendModuleInMenu(menu, old_mod_name, new_mod_list):

    #find index of last line of the module
    f = open(menu, "r")
    contents = f.readlines()
    f.close()

    index, _ = FindModule(menu, old_mod_name)

    for line in new_mod_list:
        contents.insert(index, line)
        index+=1

    f = open(menu, "w")
    contents = "".join(contents)
    f.write(contents)
    f.close()   

def Insert(menu, line, index):
    f = open(menu, "r")
    contents = f.readlines()
    f.close()

    contents.insert(index, line)

    f = open(menu, "w")
    contents = "".join(contents)
    f.write(contents)
    f.close()  

def AddToLine(menu, line, to_add):

    f = open(menu, "r")
    contents = f.readlines()
    f.close()

    contents[line] += to_add

    f = open(menu, "w")
    contents = "".join(contents)
    f.write(contents)
    f.close()  

def ReplaceLine(menu, line, to_add):

    f = open(menu, "r")
    contents = f.readlines()
    f.close()

    contents[line] = to_add

    f = open(menu, "w")
    contents = "".join(contents)
    f.write(contents)
    f.close()  


def FindFirstPath(menu_file):
    count_lines = 0
    with open(menu_file) as file:
        line = file.readline()
        while(line):
            count_lines+=1
            if "cms.Path" not in line:
                line = file.readline()
            else:
                return count_lines-1

def FindFirstSequence(menu_file):
    count_lines = 0
    with open(menu_file) as file:
        line = file.readline()
        while(line):
            count_lines+=1
            if "cms.Sequence" not in line:
                line = file.readline()
            else:
                return count_lines-1

def FindLastPath(menu_file):
    count_lines = 0
    with open(menu_file) as file:
        line = file.readline()
        while(line):
            count_lines+=1
            if "cms.Path" not in line:
                line = file.readline()
            else:
                while(1):
                    if "cms.Path"  in line:
                        count_lines+=1
                        line = file.readline()
                    else:
                        return count_lines-1

def FindLastSequence(menu_file):
    count_lines = 0
    with open(menu_file) as file:
        line = file.readline()
        while(line):
            count_lines+=1
            if "cms.Sequence" not in line:
                line = file.readline()
            else:
                while(1):
                    if "cms.Sequence"  in line:
                        count_lines+=1
                        line = file.readline()
                    else:
                        return count_lines-1

def FindLastLine(menu_file):

    f = open(menu_file, "r")
    contents = f.readlines()
    f.close()

    return len(contents)-1


def AddPath(menu, Path):

    lastpath_index = FindLastPath(menu)
    f = open(menu, "r")
    contents = f.readlines()
    f.close()

    contents.insert(lastpath_index, Path)
    
    f = open(menu, "w")
    contents = "".join(contents)
    f.write(contents)
    f.close() 

def AddToPath(menu, path, module, pos = "last"):
    assert "process" in module, "[Error] module not valid"

    f = open(menu, "r")
    contents = f.readlines()
    f.close()

    line = FindLine(menu, path)

    if pos == "last":
        to_add = " + {} ".format(module)
        ind = contents[line].index(")")-1
        contents[line] = contents[line][:ind] + to_add + contents[line][ind:]
    
    elif pos == "first":
        to_add = " {} + ".format(module)
        ind = contents[line].index("(")+1
        contents[line] = contents[line][:ind] + to_add + contents[line][ind:]

    else:
        assert isinstance(pos, int), "[Error] pos not int neither first or last"
        to_add = " + {} + ".format(module)
        contents[line] = contents[line][:pos] + to_add + contents[line][pos:]

    f = open(menu, "w")
    contents = "".join(contents)
    f.write(contents)
    f.close() 


def RemoveLines(menu, Lines):

    f = open(menu, "r")
    contents = f.readlines()
    f.close()

    if isinstance(Lines, list):
        assert len(Lines)==2, "[Error] Only [start, stop] as input accepted"
        start = Lines[0]
        end = Lines[1]

        for _ in range(start, end+1):
            contents.pop(start) #they will be shifted to start

        f = open(menu, "w")
        contents = "".join(contents)
        f.write(contents)
        f.close() 
    else:
        assert isinstance(Line, int), "[Error] not an integer nor list"
        content.pop(Line)
        f = open(menu, "w")
        contents = "".join(contents)
        f.write(contents)
        f.close() 

def RemoveValues(menu, value_list, range_=False):
    f = open(menu, "r")
    contents = f.readlines()
    f.close()

    ind = contents.index(value_list[0])
    for i in range(len(value_list)):
        contents.pop(ind)

    f = open(menu, "w")
    contents = "".join(contents)
    f.write(contents)
    f.close() 
    


def RemoveModule(menu, module):

    l, m = FindModule(menu, module)
    RemoveValues(menu, m, range_=[l, l+len(m)])

        
def FindSchedule(menu_file):
    count_lines = 0
    with open(menu_file) as file:
        line = file.readline()
        while(line):
            count_lines+=1
            if "cms.Schedule" not in line:
                line = file.readline()
            else:
                return count_lines-1

def AddToSchedule(menu, Path):

    new_process = " process." + str(Path)
    sched_index = FindSchedule(menu)
    f = open(menu, "r")
    contents = f.readlines()
    f.close()

    comma_find = contents[sched_index].find(",")
    contents[sched_index] = contents[sched_index][:comma_find+1] + new_process + contents[sched_index][comma_find:]
    
    f = open(menu, "w")
    contents = "".join(contents)
    f.write(contents)
    f.close() 

def AddPrimarySecondaryPoolSource(menu, primaryDataset, secondaryDataset, line):

    f = open(menu, "r")
    contents = f.readlines()
    f.close()


    contents[line] = 'process.source = cms.Source ("PoolSource", \n'
    line +=1
    contents.insert(line, "fileNames=cms.untracked.vstring(")
    line+=1
    for file_ in secondaryDataset:
        ins = "'" + file_ + "', \n"
        contents.insert(line, ins)
        line+=1
    contents[line-1] = contents[line-1][:-1]

    contents.insert(line, "), \n")
    line+=1
    contents.insert(line, "secondaryFileNames=cms.untracked.vstring( \n")
    line += 1
    for file_ in primaryDataset:
        ins = "'" + file_ + "', \n"
        contents.insert(line, ins)
        line+=1
    #contents[line-1] = contents[line-1][:-1]
    contents.insert(line, ") \n")
    line+=1
    contents.insert(line, ") \n")                
    
    f = open(menu, "w")
    contents = "".join(contents)
    f.write(contents)
    f.close() 

def AddModuleLocal(menu, process_name, ftype, line, **kwargs):

    prcNm = 'process.{} = '.format(process_name)
    mod_fun = getattr(FiltersDef, ftype)
    if not kwargs:
        module = mod_fun()
    else:
        module = mod_fun(**kwargs)


    module = prcNm + module
    Insert(menu, module, line)

    return module

def ModuleLocal(menu, ftype, **kwargs):

    mod_fun = getattr(FiltersDef, ftype)
    if not kwargs:
        module = mod_fun()
    else:
        module = mod_fun(**kwargs)

    return module

def AddLumiAsInput(json_file):
    inputs = 'JSONfile = "{}"\n'.format(json_file)
    def_list = "myList = LumiList.LumiList (filename = JSONfile).getCMSSWString().split(',')\n"
    global_json = 'process.source.lumisToProcess = cms.untracked.VLuminosityBlockRange()\nprocess.source.lumisToProcess.extend(myList)\n'

    return inputs + def_list + global_json

def AddLumiToModule(module_name, empty=True):

    def_inputs = 'process.{}.inputs = cms.PSet (\n\t\tlumisToProcess = CfgTypes.untracked(CfgTypes.VLuminosityBlockRange())\n)\n'.format(module_name)
    if not empty:
        def_inputs += 'process.{}.inputs.lumisToProcess.extend(myList)\n'.format(module_name)

    return def_inputs

def AddTFile(output_name):
    Service = 'process.TFileService = cms.Service("TFileService",\n\t\tfileName = cms.string("{}")\n)\n'.format(output_name)
    return Service

def AddGenParticleProducer():
    gpp = """process.prunedGenParticles = cms.EDProducer('GenParticlePruner',
    src = cms.InputTag('genParticles'),
    select = cms.vstring(
        'drop  *', # this is the default
        '++keep abs(pdgId) == 11 || abs(pdgId) == 13 || abs(pdgId) == 15', # keep leptons, with history
        'keep abs(pdgId) == 12 || abs(pdgId) == 14 || abs(pdgId) == 16',   # keep neutrinos
        'drop   status == 2',                                              # drop the shower part of the history
        '+keep pdgId == 22 && status == 1 && (pt > 10 || isPromptFinalState())', # keep gamma above 10 GeV (or all prompt) and its first parent
        '+keep abs(pdgId) == 11 && status == 1 && (pt > 3 || isPromptFinalState())', # keep first parent of electrons above 3 GeV (or prompt)
        'keep++ abs(pdgId) == 15',                                         # but keep keep taus with decays
	      'drop  status > 30 && status < 70 ', 				   #remove pythia8 garbage
	      'drop  pdgId == 21 && pt < 5',                                    #remove pythia8 garbage
        'drop   status == 2 && abs(pdgId) == 21',                          # but remove again gluons in the inheritance chain
        'keep abs(pdgId) == 23 || abs(pdgId) == 24 || abs(pdgId) == 25 || abs(pdgId) == 6 || abs(pdgId) == 37 ',   # keep VIP(articles)s
        'keep abs(pdgId) == 310 && abs(eta) < 2.5 && pt > 1 ',                                                     # keep K0
# keep heavy flavour quarks for parton-based jet flavour
	      'keep (4 <= abs(pdgId) <= 5) & (status = 2 || status = 11 || status = 71 || status = 72)',
# keep light-flavour quarks and gluons for parton-based jet flavour
	      'keep (1 <= abs(pdgId) <= 3 || pdgId = 21) & (status = 2 || status = 11 || status = 71 || status = 72) && pt>5', 
# keep b and c hadrons for hadron-based jet flavour
	      'keep (400 < abs(pdgId) < 600) || (4000 < abs(pdgId) < 6000)',
# additional c hadrons for jet fragmentation studies
	      'keep abs(pdgId) = 10411 || abs(pdgId) = 10421 || abs(pdgId) = 10413 || abs(pdgId) = 10423 || abs(pdgId) = 20413 || abs(pdgId) = 20423 || abs(pdgId) = 10431 || abs(pdgId) = 10433 || abs(pdgId) = 20433', 
# additional b hadrons for jet fragmentation studies
	      'keep abs(pdgId) = 10511 || abs(pdgId) = 10521 || abs(pdgId) = 10513 || abs(pdgId) = 10523 || abs(pdgId) = 20513 || abs(pdgId) = 20523 || abs(pdgId) = 10531 || abs(pdgId) = 10533 || abs(pdgId) = 20533 || abs(pdgId) = 10541 || abs(pdgId) = 10543 || abs(pdgId) = 20543', 
#keep SUSY particles
	      'keep (1000001 <= abs(pdgId) <= 1000039 ) || ( 2000001 <= abs(pdgId) <= 2000015)',
# keep protons 
        'keep pdgId = 2212',
        'keep status == 3 || ( 21 <= status <= 29) || ( 11 <= status <= 19)',  #keep event summary (status=3 for pythia6, 21 <= status <= 29 for pythia8)
        'keep isHardProcess() || fromHardProcessFinalState() || fromHardProcessDecayed() || fromHardProcessBeforeFSR() || (statusFlags().fromHardProcess() && statusFlags().isLastCopy())',  #keep event summary based on status flags
    )
)\n"""
    return gpp

