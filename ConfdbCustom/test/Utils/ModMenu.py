"""
    Functions that modify menus
"""

def FindModule(menu_file, module_name):
    moduleDef = []
    count_lines = 0
    with open(menu_file) as file:
        line = file.readline()
        while(line):
            count_lines +=1
            if module_name in line:
                count_brack = 1
                moduleDef.append(line)
                while count_brack!=0:
                    count_lines +=1
                    line = file.readline()
                    moduleDef.append(line)
                    if ")" in line:
                        count_brack -= 1
                    if "(" in line:
                        count_brack += 1

                break
            else:
                line = file.readline()

    return count_lines, moduleDef

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

def AddToPath(menu, Path):

    lastpath_index = FindLastPath(menu)
    f = open(menu, "r")
    contents = f.readlines()
    f.close()

    contents.insert(lastpath_index, Path)
    
    f = open(menu, "w")
    contents = "".join(contents)
    f.write(contents)
    f.close() 




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

    contents.pop(line) #removing poolsource line

    contents[line] = 'process.source = cms.Source ("PoolSource", \n'
    line +=1
    contents.insert(line, "fileNames=cms.untracked.vstring(")
    line+=1
    for file_ in primaryDataset:
        ins = "'file:" + file_ + "', \n"
        contents.insert(line, ins)
        line+=1
    contents[line-1] = contents[line-1][:-1]

    contents.insert(line, "), \n")
    line+=1
    contents.insert(line, "secondaryFileNames=cms.untracked.vstring( \n")
    line += 1
    for file_ in secondaryDataset:
        ins = "'file:" + file_ + "', \n"
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
    