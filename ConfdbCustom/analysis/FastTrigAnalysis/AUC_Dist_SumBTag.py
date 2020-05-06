import ROOT
import numpy as np
import pandas
import matplotlib.pyplot as plt
import math as mt
from sklearn.metrics import auc
from tqdm import tqdm

from TrigFilters import Trig2018Filters, Trig2DFilters, SumBTag

def GetCanvas():
    c = ROOT.TCanvas("c", "c", 2200, 2200, 2200, 1000)
    return c


f = ROOT.TFile("../ggMCRAW_GenJets.root")
tree_sig = f.Get("SaveAllJets/Jets")

f_bkg = ROOT.TFile("../323725_37_57_PU50.root")
tree_bkg = f_bkg.Get("SaveAllJets/Jets")

signal_pf_4b = []
bkg_pf_4b = []

signal_pf_3b = []
bkg_pf_3b = []

signal_pf_2b = []
bkg_pf_2b = []


for event in tree_sig:
    pf_btag = sorted(event.pf_btag)
    
    sum_pf = sum(pf_btag[-4:])
    signal_pf_4b.append(sum_pf)
    
    sum_pf = sum(pf_btag[-3:])
    signal_pf_3b.append(sum_pf)
    
    sum_pf = sum(pf_btag[-2:])
    signal_pf_2b.append(sum_pf)
    
for event in tree_bkg:
    pf_btag = sorted(event.pf_btag)
    
    sum_pf = sum(pf_btag[-4:])
    bkg_pf_4b.append(sum_pf)
    
    sum_pf = sum(pf_btag[-3:])
    bkg_pf_3b.append(sum_pf)
    
    sum_pf = sum(pf_btag[-2:])
    bkg_pf_2b.append(sum_pf)
    
fig , ax = plt.subplots(1,3, figsize=(30, 6), dpi=100)

ax[0].hist(signal_pf_4b, bins = 30, density=True, histtype='step', color = 'fuchsia', label='Signal')
ax[0].hist(bkg_pf_4b, bins = 30, density=True, histtype='step', color = 'blue', label='Background')
ax[0].set_xlabel("Sum BTag PF [-4:]", size=13)
ax[0].set_ylabel("Events/ Integral", size=13)
leg = ax[0].legend(loc="upper center", prop={'size':15})
plt.gcf().text(0.13, 0.9, r"$gg\rightarrow HH \rightarrow bbbb$", fontsize=12)
plt.gcf().text(0.265, 0.9, r"$323725 \: Lumi [37:57] \: PU > 50$", fontsize=12)
plt.tick_params(axis='both', which='major', labelsize=11)
leg.get_frame().set_linewidth(0.0)

ax[1].hist(signal_pf_3b, bins = 30, density=True, histtype='step', color = 'fuchsia', label='Signal')
ax[1].hist(bkg_pf_3b, bins = 30, density=True, histtype='step', color = 'blue', label='Background')
ax[1].set_xlabel("Sum BTag Calo [-3:]", size=13)
ax[1].set_ylabel("Events/ Integral", size=13)
leg = ax[1].legend(loc="upper center", prop={'size':15})
plt.gcf().text(0.405, 0.9, r"$gg\rightarrow HH \rightarrow bbbb$", fontsize=12)
plt.gcf().text(0.54, 0.9, r"$323725 \: Lumi [37:57] \: PU > 50$", fontsize=12)
leg.get_frame().set_linewidth(0.0)

ax[2].hist(signal_pf_2b, bins = 30, density=True, histtype='step', color = 'fuchsia', label='Signal')
ax[2].hist(bkg_pf_2b, bins = 30, density=True, histtype='step', color = 'blue', label='Background')
ax[2].set_xlabel("Sum BTag Calo [-2:]", size=13)
ax[2].set_ylabel("Events/ Integral", size=13)
leg = ax[2].legend(loc="upper center", prop={'size':15})
plt.gcf().text(0.677, 0.9, r"$gg\rightarrow HH \rightarrow bbbb$", fontsize=12)
plt.gcf().text(0.81, 0.9, r"$323725 \: Lumi [37:57] \: PU > 50$", fontsize=12)
leg.get_frame().set_linewidth(0.0)

fig.savefig("BTagSum_4_3_2_.png")

#ROCS

tpr_4b = []
fpr_4b = []

tpr_3b = []
fpr_3b = []

tpr_2b = []
fpr_2b = []

th_4b = np.arange(0, max(max(signal_pf_4b), max(bkg_pf_4b)), 0.01)
for th in th_4b:
    count_sig = 0
    count_bkg = 0
    tot_sig = len(signal_pf_4b)
    tot_bkg = len(bkg_pf_4b)
    for i in signal_pf_4b:
        if i >= th:
            count_sig+=1
    for j in bkg_pf_4b:
        if j >= th:
            count_bkg+=1
       
    tpr_4b.append(float(count_sig)/tot_sig)
    fpr_4b.append(float(count_bkg)/tot_bkg)
    
th_3b = np.arange(0, max(max(signal_pf_3b), max(bkg_pf_3b)), 0.005)
for th in th_3b:
    count_sig = 0
    count_bkg = 0
    tot_sig = len(signal_pf_3b)
    tot_bkg = len(bkg_pf_3b)
    for i in signal_pf_3b:
        if i >= th:
            count_sig+=1
    for j in bkg_pf_3b:
        if j >= th:
            count_bkg+=1
       
    tpr_3b.append(float(count_sig)/tot_sig)
    fpr_3b.append(float(count_bkg)/tot_bkg)
    
th_2b = np.arange(0, max(max(signal_pf_2b), max(bkg_pf_2b)), 0.001)
for th in th_2b:
    count_sig = 0
    count_bkg = 0
    tot_sig = len(signal_pf_2b)
    tot_bkg = len(bkg_pf_2b)
    for i in signal_pf_2b:
        if i >= th:
            count_sig+=1
    for j in bkg_pf_2b:
        if j >= th:
            count_bkg+=1
       
    tpr_2b.append(float(count_sig)/tot_sig)
    fpr_2b.append(float(count_bkg)/tot_bkg)
    
fig = plt.figure(figsize=(13,7), dpi=100)
plt.plot([0,1], [0,1], '--', c = 'black')
plt.plot(fpr_4b, tpr_4b, color='fuchsia', alpha=0.7, label='Sum Leading 4 BTag (area = {:0.4f})'.format(auc(fpr_4b, tpr_4b)))
plt.plot(fpr_3b, tpr_3b, color='blue', alpha=0.7, label='Sum Leading 3 BTag (area = {:0.4f})'.format(auc(fpr_3b, tpr_3b)))
plt.plot(fpr_2b, tpr_2b, color='aqua', alpha=0.7, label='Sum Leading 2 BTag (area = {:0.4f})'.format(auc(fpr_2b, tpr_2b)))
leg = plt.legend(loc='lower right', prop={'size':15})
leg.get_frame().set_linewidth(0.0)
plt.xlabel("False Positive Rate", size=15)
plt.ylabel("True Positive Rate", size = 15)
plt.tick_params(axis='both', which='major', labelsize=13)
plt.gcf().text(0.14, 0.9, r"$gg\rightarrow HH \rightarrow bbbb$", fontsize=15)
plt.gcf().text(0.65, 0.9, r"$323725 \: Lumi [37:57] \: PU > 50$", fontsize=15)
fig.savefig("./SumBTag_4_3_2_ROC.png")
