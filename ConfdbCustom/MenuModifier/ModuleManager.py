import sys
import os
import ModMenu
import re
sys.path.append(os.path.dirname("QueryForFiles.py"))
import QueryForFiles

class ModMan:

    def __init__(self, menu_path):
        self.menu = menu_path

    def SetInClass(self, mod_def, in_class):

        named_dict = {}
        mod_name = (mod_def[0].split("."))[1].split(" ")[0].strip()
        mod_type = mod_def[0][mod_def[0].index("(")+1:mod_def[0].index(",")].strip()
        mod_cmssw = mod_def[0][mod_def[0].index("ED"):mod_def[0].index("(")]
        named_dict["name"] = mod_name
        named_dict["type"] = mod_type
        named_dict["cmssw"] = mod_cmssw

        for line in mod_def[1:]:
            if "cms" in line:
                attr_name = line[:line.index("=")].split()[0]
                attr_type = line[line.index("cms."):line.index("(")]
                attr_value = line[line.index("(")+1:line.index(")")]

                attr_value = [attr_type, attr_value]
                if "True" in attr_value[1] or "False" in attr_value[1]:
                    attr_value[1] = bool(attr_value[1]) #then it was a float
                if "int" in attr_value[0]:
                    attr_value[1] = int(attr_value[1])
                else:
                    try:
                        attr_value[1] = float(attr_value[1]) #try if it is a float convertible
                    except:
                        pass #else it is a string then continue
                named_dict[attr_name] = attr_value

        setattr(self, in_class, named_dict)

    def CloneModule(self, module_name, in_class="Module"):

        line, mod_def = ModMenu.FindModule(self.menu, module_name)
        self.SetInClass(mod_def, in_class)
        

    def PrintModule(self, in_class="Module"):
        print(getattr(self, in_class))

    def ModifyPar(self, in_class, attr_name, attr_value):

        if not isinstance(in_class, list):
            in_class = [in_class, in_class] #list first element to access the attribute, second to save into class. If equal overwriting
    
        dic_ = dict(getattr(self, in_class[0])) #redefine dict to not modify attribute 

        if isinstance(attr_value, list):
            dic_[attr_name] = attr_value
        else:
            dic_[attr_name][1] = attr_value
            
        setattr(self, in_class[1], dic_)

    def CreateFromLocal(self, in_class, mod_name, **kwargs):
        module = ModMenu.ModuleLocal(self.menu, mod_name, **kwargs)
        mod_def = "process.{} = ".format(mod_name) + module #just to make it compatible to SetInClass Method
        mod_def = mod_def.split("\n")
        
        
        self.SetInClass(mod_def, in_class)

    def ConvertModuleToString(self, in_class, process_name = 'in_class'):

        module_dict = dict(getattr(self, in_class))
        if process_name == 'in_class': process_name = in_class

        prNm = "process.{} = ".format(process_name) + "cms.{}( ".format(module_dict["cmssw"]) + module_dict["type"] + ",\n"
        for key, value in zip(module_dict.keys(), module_dict.values()):
            if key == "cmssw" or key == "name" or key=="type": continue
            prNm += str(key) + " = " + str(value[0]) + " ( " + str(value[1]) + " ),\n"
        prNm = prNm[:-2] + "\n" #deleting last comma
        prNm += ")\n"

        return prNm

    def InsertInMenu(self, in_class,process_name = 'in_class', line=False):

        if line:
            self.currentline = line

        if process_name == 'in_class': process_name = in_class
        to_insert = self.ConvertModuleToString(in_class, process_name)
        ModMenu.Insert(self.menu, to_insert, self.currentline)
        self.currentline += len(to_insert.split("\n"))

    def Insert(self, line, ind = False):
        if type(ind)==int:
            self.currentline = ind
        ModMenu.Insert(self.menu, line, self.currentline)
        n_ = len(line.split("\n"))-1
        self.currentline += n_

    def MakeSpace(self, n=10):
        for _ in range(n):
            self.Insert("\n")

    def SetCurrentLine(self, option_mod=False, option_str=False):
        
        assert (option_mod and not option_str) or (option_str and not option_mod), "[Error] duplicate find options" 
        assert isinstance(option_mod, int) or isinstance(option_mod, str), "[Error] Invalid option type"
        assert isinstance(option_str, int) or isinstance(option_str, str), "[Error] Invalid option type"

        if option_mod:
            if isinstance(option_mod, int):
                self.currentline = option_mod
            else:
                where = option_mod.split(":")[0]
                mod_name = option_mod.split(":")[1]
                l, m = ModMenu.FindModule(self.menu, mod_name)
                if where == "after":
                    self.currentline = l+1
                elif where == "before":
                    self.currentline = l-len(m)+1
        elif option_str:
            if isinstance(option_str, int):
                self.currentline = option_str
            else:
                where = option_str.split(":")[0]
                mod_name = option_str.split(":")[1]
                l = ModMenu.FindLine(self.menu, mod_name)
                if where == "after":
                    self.currentline = l+1
                elif where == "before":
                    self.currentline = l-1


    def AddLuminosityJson(self,json_file, line=False):
        if line:
            self.currentline = line
        to_add = ModMenu.AddLumiAsInput(json_file)
        self.Insert( to_add, self.currentline)
        self.json = json_file

    def AddLuminosityToModule(self, process, empty=True, line=False):
        if line:
            self.currentline = line
        to_add = ModMenu.AddLumiToModule(process, empty=empty)
        if not hasattr(self, "json"):
            to_add = to_add
        self.Insert( to_add, self.currentline)
        

    def AddTFileService(self, file_name, line=False):
        if line:
            self.currentline = line
        to_add = ModMenu.AddTFile(file_name)
        self.Insert( to_add, self.currentline)

    def AddDASQuery(self, QueryType="QueryFilesMCgg", at = "_customInfo['inputFile' ]"):
        line = ModMenu.FindLine(self.menu, at)
        querier = getattr(QueryForFiles, QueryType)
        query = querier()
        to_add = at + " = " + str(query) + "\n"
        ModMenu.ReplaceLine(self.menu, line, to_add)

    def AddGenParticleProd(self):
        genp = ModMenu.AddGenParticleProducer()
        self.Insert( genp, self.currentline)
        