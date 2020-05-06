import ROOT
import numpy as np
import pandas
import matplotlib.pyplot as plt
import math as mt
from sklearn.metrics import auc
from tqdm import tqdm
from TrigFilters import Trig2018Filters, Trig2DFilters

f = ROOT.TFile("../ggMCRAW_GenJets.root")
tree_sig = f.Get("SaveAllJets/Jets")

f_bkg = ROOT.TFile("../323725_37_57_PU50.root")
tree_bkg = f_bkg.Get("SaveAllJets/Jets")

#reproducing 2018 trigger

print("...Reproducing 2018 trigger")

counter_bkg = 0
for event in tree_bkg:
    pf_pt = event.pf_pt
    pf_eta = event.pf_eta
    calo_pt = event.calo_pt
    calo_eta = event.calo_eta
    calo_btag = event.calo_btag
    pf_btag = event.pf_btag
    calo_ht = sum(i for i in calo_pt if i > 30)
    if(Trig2018Filters.first(calo_pt, calo_eta) and Trig2018Filters.second(calo_pt, calo_eta) and Trig2018Filters.third(calo_pt, calo_eta, calo_btag) and Trig2018Filters.fourth(pf_pt, pf_eta) and Trig2018Filters.fifth(pf_pt, pf_eta) and Trig2018Filters.sixth(pf_pt, pf_eta) and Trig2018Filters.seventh(pf_pt, pf_eta) and Trig2018Filters.eighth(pf_pt, pf_eta) and Trig2018Filters.ninth(pf_pt, pf_eta) and Trig2018Filters.tenth(pf_pt, pf_eta, pf_btag)):
        counter_bkg += 1
        
counter_sig = 0
for event in tree_sig:
    pf_pt = event.pf_pt
    pf_eta = event.pf_eta
    calo_pt = event.calo_pt
    calo_eta = event.calo_eta
    calo_btag = event.calo_btag
    pf_btag = event.pf_btag
    calo_ht = sum(i for i in calo_pt if i > 30)
    if(Trig2018Filters.first(calo_pt, calo_eta) and Trig2018Filters.second(calo_pt, calo_eta) and Trig2018Filters.third(calo_pt, calo_eta, calo_btag) and Trig2018Filters.fourth(pf_pt, pf_eta) and Trig2018Filters.fifth(pf_pt, pf_eta) and Trig2018Filters.sixth(pf_pt, pf_eta) and Trig2018Filters.seventh(pf_pt, pf_eta) and Trig2018Filters.eighth(pf_pt, pf_eta) and Trig2018Filters.ninth(pf_pt, pf_eta) and Trig2018Filters.tenth(pf_pt, pf_eta, pf_btag)):
        counter_sig += 1
        
    
print("...2018 tirgger accepted signal/background events: {}, {}".format(counter_sig, counter_bkg))

#now building 2D trigger
print("...Reproducing 2D trigger")

counter_sig = 0
for event in tree_sig:
    pf_pt = event.pf_pt
    pf_eta = event.pf_eta
    calo_pt = event.calo_pt
    calo_eta = event.calo_eta
    calo_btag = event.calo_btag
    pf_btag = event.pf_btag
    
    if(Trig2DFilters.myFirst(calo_pt, calo_eta) and Trig2DFilters.mySecond(calo_pt, calo_eta) and Trig2DFilters.myThird(calo_pt, calo_eta, calo_btag) and Trig2DFilters.myFourth(pf_pt, pf_eta) and Trig2DFilters.myFifth(pf_pt, pf_eta) and Trig2DFilters.mySixth(pf_pt, pf_eta, pf_btag)):
        counter_sig += 1
        
counter_bkg = 0
for event in tree_bkg:
    pf_pt = event.pf_pt
    pf_eta = event.pf_eta
    calo_pt = event.calo_pt
    calo_eta = event.calo_eta
    calo_btag = event.calo_btag
    pf_btag = event.pf_btag
    
    if(Trig2DFilters.myFirst(calo_pt, calo_eta) and Trig2DFilters.mySecond(calo_pt, calo_eta) and Trig2DFilters.myThird(calo_pt, calo_eta, calo_btag) and Trig2DFilters.myFourth(pf_pt, pf_eta) and Trig2DFilters.myFifth(pf_pt, pf_eta) and Trig2DFilters.mySixth(pf_pt, pf_eta, pf_btag)):
        counter_bkg += 1
        
print("...2D tirgger accepted signal/background events: {}, {}".format(counter_sig, counter_bkg))
