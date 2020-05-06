import numpy as np
import pandas
import matplotlib.pyplot as plt
import math as mt

class Trig2018Filters:

    def first(calo_pt, calo_eta):
        counter = 0
        for i in range(0, len(calo_pt)):
            if(calo_pt[i] >= 30 and abs(calo_eta[i]) <= 2.5):
                counter+=1
        
        if counter >= 4:
            return True
        else:
            return False
        
    def second(calo_pt, calo_eta):
        ht = 0
        for i in range(0, len(calo_pt)):
            if(calo_pt[i] >= 30 and abs(calo_eta[i]) <= 2.5):
                ht += calo_pt[i]
                
        if ht >= 320:
            return True
        else:
            return False
        
    def third(calo_pt, calo_eta, calo_btag):
        counter = 0
        for i in range(0, len(calo_pt)):
            if(calo_pt[i] >= 30 and abs(calo_eta[i]) <= 2.4 and calo_btag[i] >= 0.17):
                counter += 1
                
        if counter >= 2:
            return True
        else:
            return False
        
    def fourth(pf_pt, pf_eta):
        counter = 0
        for i in range(0, len(pf_pt)):
            if(pf_pt[i] >= 30 and abs(pf_eta[i]) <= 2.5):
                counter+=1
        
        if counter >= 4:
            return True
        else:
            return False
        
    def fifth(pf_pt, pf_eta):
        counter = 0
        for i in range(0, len(pf_pt)):
            if(pf_pt[i] >= 75 and abs(pf_eta[i]) <= 2.5):
                counter+=1
        
        if counter >= 1:
            return True
        else:
            return False
        
    def sixth(pf_pt, pf_eta):
        counter = 0
        for i in range(0, len(pf_pt)):
            if(pf_pt[i] >= 60 and abs(pf_eta[i]) <= 2.5):
                counter+=1
        
        if counter >= 2:
            return True
        else:
            return False
        
    def seventh(pf_pt, pf_eta):
        counter = 0
        for i in range(0, len(pf_pt)):
            if(pf_pt[i] >= 45 and abs(pf_eta[i]) <= 2.5):
                counter+=1
        
        if counter >= 3:
            return True
        else:
            return False
        
    def eighth(pf_pt, pf_eta):
        counter = 0
        for i in range(0, len(pf_pt)):
            if(pf_pt[i] >= 40 and abs(pf_eta[i]) <= 2.5):
                counter+=1
        
        if counter >= 4:
            return True
        else:
            return False
        
    def ninth(pf_pt, pf_eta):
        ht = 0
        for i in range(0, len(pf_pt)):
            if(pf_pt[i] >= 30 and abs(pf_eta[i]) <= 2.5):
                ht += pf_pt[i]
                
        if ht >= 330:
            return True
        else:
            return False
        
    def tenth(pf_pt, pf_eta, pf_btag):
        counter = 0
        for i in range(0, len(pf_pt)):
            if(pf_pt[i] >= 30 and abs(pf_eta[i]) <= 2.6 and pf_btag[i] >= 0.24):
                counter += 1
                
        if counter >= 3:
            return True
        else:
            return False
            
class Trig2DFilters:
    
    #my trigger eta restricted
    def myFirst(calo_pt, calo_eta):
        pt_er = []
        for i in range(0, len(calo_pt)):
            if(abs(calo_eta[i]) <= 2.6):
                pt_er.append(calo_pt[i])
                
        pt_er = sorted(pt_er)
        
        if pt_er[-1]+pt_er[-2] >= 180:
            return True
        else:
            return False
        
    def mySecond(calo_pt, calo_eta):
        pt_er = []
        for i in range(0, len(calo_pt)):
            if(abs(calo_eta[i]) <= 2.6):
                pt_er.append(calo_pt[i])
                
        pt_er = sorted(pt_er)
        
        if pt_er[-3]+pt_er[-4] >= 90:
            return True
        else:
            return False
        
    def myThird(calo_pt, calo_eta, calo_btag):
        btag_er = []
        for i in range(0, len(calo_pt)):
            if(calo_pt[i] >= 30 and abs(calo_eta[i]) <= 2.6):
                btag_er.append(calo_btag[i])
                
        btag_er = sorted(btag_er)
        
        if(btag_er[-3] + btag_er[-2] >= 0.6):
            return True
        else:
            return False
        
    def myFourth(pf_pt, pf_eta):
        pt_er = []
        for i in range(0, len(pf_pt)):
            if(abs(pf_eta[i]) <= 2.6):
                pt_er.append(pf_pt[i])
                
        pt_er = sorted(pt_er)
        
        if pt_er[-1]+pt_er[-2] >= 180:
            return True
        else:
            return False
        
    def myFifth(pf_pt, pf_eta):
        pt_er = []
        for i in range(0, len(pf_pt)):
            if(abs(pf_eta[i]) <= 2.6):
                pt_er.append(pf_pt[i])
                
        pt_er = sorted(pt_er)
        
        if pt_er[-3]+pt_er[-4] >= 90:
            return True
        else:
            return False
        
    def mySixth(pf_pt, pf_eta, pf_btag):
        btag_er = []
        for i in range(0, len(pf_pt)):
            if(pf_pt[i] >= 30 and abs(pf_eta[i]) <= 2.6):
                btag_er.append(pf_btag[i])
                
        btag_er = sorted(btag_er)
        
        if(btag_er[-3] + btag_er[-2] >= 0.6):
            return True
        else:
            return False

