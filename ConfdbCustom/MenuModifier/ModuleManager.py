import sys
import os
import ModMenu
import re

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
            line = line.split(" ")
            line = list(filter(('').__ne__,line))
            if len(line) >=2:
                attr_name = line[0]
                for ind in range(1,len(line[1:])):
                    if "cms" in line[ind]:
                        attr_value = [line[ind], line[ind+1]]
                        break
                
                if "True" in attr_value[1] or "False" in attr_value[1]:
                    attr_value[1] = bool(attr_value[1]) #then it was a float
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
        dic_[attr_name] = attr_value
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
            if key == "type" or key == "name": continue
            prNm += "\t\t" + str(key) + " = " + str(value[0]) + " " + str(value[1]) + " ),\n"
        prNm += ")\n"

        return prNm

    def InsertInMenu(self, in_class,process_name = 'in_class', line=False):

        if line:
            self.currentline = line

        if process_name == 'in_class': process_name = in_class
        to_insert = self.ConvertModuleToString(in_class, process_name)
        ModMenu.Insert(self.menu, to_insert, self.currentline)
        self.currentline += len(to_insert.split("\n"))-1

    def Insert(self, line):
        ModMenu.Insert(self.menu, line, self.currentline)
        n_ = len(line.split("\n"))
        self.currentline += n_

    def SetCurrentLine(self, option):
        assert isinstance(option, int) or isinstance(option, str), "[Error] Invalid option type"
        if isinstance(option, int):
            self.currentline = line
        else:
            where = option.split(":")[0]
            mod_name = option.split(":")[1]
            l, m = ModMenu.FindModule(self.menu, mod_name)
            if where == "after":
                self.currentline = l+1
            elif where == "before":
                self.currentline = l-len(m)+1

    def AddLuminosity(self,json_file, line=False):
        if line:
            self.currentline = line
        to_add = ModMenu.AddLumiAsInput(json_file)
        ModMenu.Insert(self.menu, to_add, self.currentline)

    def AddLuminosityToModule(self, process, line=False):
        if line:
            self.currentline = line
        to_add = ModMenu.AddLumiToModule(process)
        ModMenu.Insert(self.menu, to_add, self.currentline)
        

    def AddTFileService(self, file_name, line=False):
        if line:
            self.currentline = line
        to_add = ModMenu.AddTFile(file_name)
        ModMenu.Insert(self.menu, to_add, self.currentline)





        



    
        


