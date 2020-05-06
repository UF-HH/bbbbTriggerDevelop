import numpy as np
import pandas
import matplotlib.pyplot as plt
import math as mt

"""
    This filters must me made sooo much faster.
"""

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
        
        if(btag_er[-3] + btag_er[-2] >= 0.7):
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
        
        if(btag_er[-3] + btag_er[-2] >= 0.7):
            return True
        else:
            return False
            

class SumBTag:
    
    def SumBTag2min(pf_btag, minim):
        pf_btag = sorted(event.pf_btag)
        sum_pf = sum(pf_btag[-2:])
        return sum_pf >= minim

    def SumBTag2ER(calo_pt, calo_eta, calo_btag, btag_min):
        btag_er = []
        for i in range(0, len(calo_pt)):
            if(calo_pt[i] >= 30 and abs(calo_eta[i]) <= 2.5):
                btag_er.append(calo_btag[i])
                
        btag_er = sorted(btag_er)
        
        if(len(btag_er) >= 2 and btag_er[-1] + btag_er[-2] >= btag_min):
            return True
        else:
            return False
        
    def SumBTag3ER(calo_pt, calo_eta, calo_btag, btag_min):
        btag_er = []
        for i in range(0, len(calo_pt)):
            if(calo_pt[i] >= 30 and abs(calo_eta[i]) <= 2.5):
                btag_er.append(calo_btag[i])
                
        btag_er = sorted(btag_er)
        
        if(btag_er[-1] + btag_er[-2] + btag_er[-3]>= btag_min):
            return True
        else:
            return False
        
    def SumBTag4ER(calo_pt, calo_eta, calo_btag, btag_min):
        btag_er = []
        for i in range(0, len(calo_pt)):
            if(calo_pt[i] >= 30 and abs(calo_eta[i]) <= 2.5):
                btag_er.append(calo_btag[i])
                
        btag_er = sorted(btag_er)
        
        if(btag_er[-1] + btag_er[-2] + btag_er[-3] + btag_er[-4]>= btag_min):
            return True
        else:
            return False
        
    def SumBTag4ER_return(calo_pt, calo_eta, calo_btag):
        btag_er = []
        for i in range(0, len(calo_pt)):
            if(calo_pt[i] >= 30 and abs(calo_eta[i]) <= 2.5):
                btag_er.append(calo_btag[i])
                
        btag_er = sorted(btag_er)
        
        return btag_er[-1] + btag_er[-2] + btag_er[-3] + btag_er[-4]
        
    def SumBTag2WeightER(calo_pt, calo_eta, calo_btag, btag_min, beta):
        btag_er = []
        pt_er = []
        for i in range(0, len(calo_pt)):
            if(calo_pt[i] >= 30 and abs(calo_eta[i]) <= 2.5):
                btag_er.append(calo_btag[i])
                pt_er.append(calo_pt[i])
                
        btag_er, pt_er = (list(t) for t in zip(*sorted(zip(btag_er, pt_er))))
        
        if(len(btag_er) >= 2 and btag_er[-1]/pow(pt_er[-1],beta) + btag_er[-2]/pow(pt_er[-2], beta) >= btag_min):
            return True
        else:
            return False

    def SumBTag2Weight_return(calo_pt, calo_eta, calo_btag, btag_min, beta):
        btag_er = []
        pt_er = []
        for i in range(0, len(calo_pt)):
            if(calo_pt[i] >= 30 and abs(calo_eta[i]) <= 2.5):
                btag_er.append(calo_btag[i])
                pt_er.append(calo_pt[i])
                
        btag_er, pt_er = (list(t) for t in zip(*sorted(zip(btag_er, pt_er))))
        
        return btag_er[-1]/pow(pt_er[-1],beta) + btag_er[-2]/pow(pt_er[-2], beta)

    def SumBTagExp2_return(calo_pt, calo_eta, calo_btag, beta):
        btag_er = []
        pt_er = []
        for i in range(0, len(calo_pt)):
            if(calo_pt[i] >= 30 and abs(calo_eta[i]) <= 2.5):
                btag_er.append(calo_btag[i])
                pt_er.append(calo_pt[i])
                
        btag_er, pt_er = (list(t) for t in zip(*sorted(zip(btag_er, pt_er))))
        
        return pow(pt_er[-1] + pt_er[-2], (btag_er[-2] + btag_er[-1])*beta)

    def SumBTagExp_return(calo_pt, calo_eta, calo_btag):
        btag_er = []
        pt_er = []
        for i in range(0, len(calo_pt)):
            if(calo_pt[i] >= 30 and abs(calo_eta[i]) <= 2.5):
                btag_er.append(calo_btag[i])
                pt_er.append(calo_pt[i])
                
        btag_er, pt_er = (list(t) for t in zip(*sorted(zip(btag_er, pt_er))))
        
        return mt.exp(2*(btag_er[-2] + btag_er[-1]))-1
    
    def SumBTag(pf_btag):
        pf_btag = sorted(pf_btag)
        sum_pf = sum(pf_btag[-4:])
        return sum_pf >= 2.1

    def SumBTag3(pf_btag):
        pf_btag = sorted(pf_btag)
        sum_pf = sum(pf_btag[-3:])
        return sum_pf >= 1.8

    def SumBTag2(pf_btag):
        pf_btag = sorted(pf_btag)
        sum_pf = sum(pf_btag[-2:])
        return sum_pf >= 1.5
