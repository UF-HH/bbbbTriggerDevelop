import ROOT
import numpy as np
import pandas
import matplotlib.pyplot as plt
import math as mt
from sklearn.metrics import auc
from tqdm import tqdm
from sklearn.metrics import roc_curve, auc
from TrigFilters import SumBTag, Trig2018Filters


f = ROOT.TFile("../ggMCRAW_GenJets.root")
tree_sig = f.Get("SaveAllJets/Jets")

f_bkg = ROOT.TFile("../323725_37_57_PU50.root")
tree_bkg = f_bkg.Get("SaveAllJets/Jets")


sig_pf = []
bkg_pf = []

minbtag = np.arange(1, 2, 0.05)

for minb in tqdm(minbtag):
    
    count_sig = 0
    count_bkg = 0

    for event in tree_sig:
        calo_btag = event.calo_btag
        calo_pt = event.calo_pt
        calo_eta = event.calo_eta
        pf_btag = event.pf_btag
        pf_pt = event.pf_pt
        pf_eta = event.pf_eta
        pf_phi = event.pf_phi

        if(Trig2018Filters.first(calo_pt, calo_eta) and  Trig2018Filters.sixth(calo_pt, calo_eta) and SumBTag.SumBTag2ER(calo_pt, calo_eta, calo_btag, minb) and Trig2018Filters.first(pf_pt, pf_eta) and Trig2018Filters.sixth(pf_pt, pf_eta) and SumBTag.SumBTag2ER(pf_pt, pf_eta, pf_btag, minb)):
            count_sig+=1


    for event in tree_bkg:
        calo_btag = event.calo_btag
        calo_pt = event.calo_pt
        calo_eta = event.calo_eta
        pf_btag = event.pf_btag
        pf_pt = event.pf_pt
        pf_eta = event.pf_eta
        pf_phi = event.pf_phi

        if(Trig2018Filters.first(calo_pt, calo_eta) and  Trig2018Filters.sixth(calo_pt, calo_eta) and SumBTag.SumBTag2ER(calo_pt, calo_eta, calo_btag, minb) and Trig2018Filters.first(pf_pt, pf_eta) and Trig2018Filters.sixth(pf_pt, pf_eta) and SumBTag.SumBTag2ER(pf_pt, pf_eta, pf_btag, minb)):
            count_bkg+=1
            
    
    sig_pf.append(float(count_sig)/50000)
    bkg_pf.append(float(count_bkg)/26 * 3.96)
    
fig = plt.figure(figsize=(13,7), dpi=100)
plt.scatter(sig_pf, bkg_pf, c=minbtag, cmap=plt.cm.get_cmap('cool'))
cbar = plt.colorbar()
plt.scatter(0.226, 3.96, marker='X', c='black', s=100, label='2018 Default')
cbar.set_label(label='Sum BTag Threshold', size=13)
plt.tick_params(axis='both', which='major', labelsize=13)
plt.gcf().text(0.14, 0.9, r"$gg\rightarrow HH \rightarrow bbbb$", fontsize=15)
plt.gcf().text(0.5, 0.9, r"$323725 \: Lumi [37:57] \: PU > 50$", fontsize=15)
plt.xlabel("Signal Acceptance", size=15)
plt.ylabel("Rate (Hz)", size=15)
leg = plt.legend(loc="upper left", prop={'size':15})
leg.get_frame().set_linewidth(0.0)
plt.show()
fig.savefig("Sum2BTrig.png")
