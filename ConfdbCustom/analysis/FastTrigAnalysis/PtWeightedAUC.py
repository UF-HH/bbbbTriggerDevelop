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


beta = np.arange(-1, 2, 0.2)

fig = plt.figure(figsize=(13,7), dpi=100)

colors = plt.cm.cool(np.linspace(0,1,len(beta)))

for idx, b in tqdm(enumerate(beta)):
    
    b_sig = []
    b_bkg = []
    
    for event in tree_sig:
        calo_btag = event.calo_btag
        calo_pt = event.calo_pt
        calo_eta = event.calo_eta
        pf_btag = event.pf_btag
        pf_pt = event.pf_pt
        pf_eta = event.pf_eta
        pf_phi = event.pf_phi

        btag, pt = (list(t) for t in zip(*sorted(zip(calo_btag, calo_pt))))
        beta_w = btag[-1]/pow(pt[-1],b) + btag[-2]/pow(pt[-2],b)
        b_sig.append(beta_w)
        



    for event in tree_bkg:
        calo_btag = event.calo_btag
        calo_pt = event.calo_pt
        calo_eta = event.calo_eta
        pf_btag = event.pf_btag
        pf_pt = event.pf_pt
        pf_eta = event.pf_eta
        pf_phi = event.pf_phi

        btag, pt = (list(t) for t in zip(*sorted(zip(calo_btag, calo_pt))))
        beta_w = btag[-1]/pow(pt[-1],b) + btag[-2]/pow(pt[-2],b)
        b_bkg.append(beta_w)

        
    min_ = min(min(b_sig), min(b_bkg))
    max_ = max(max(b_sig), max(b_bkg))
    step = (max_-min_)/100
        
    th_ = np.arange(min_, max_ , step)
    
    tpr = []
    fpr = []

    for th in th_:
        count_sig = 0
        count_bkg = 0
        tot_sig = len(b_sig)
        tot_bkg = len(b_bkg)
        for i in b_sig:
            if i >= th:
                count_sig+=1
        for j in b_bkg:
            if j >= th:
                count_bkg+=1

        tpr.append(float(count_sig)/tot_sig)
        fpr.append(float(count_bkg)/tot_bkg)
        
    auc_ = auc(fpr, tpr)
        
    plt.plot(fpr, tpr, c=colors[idx], label=r'$\beta={:.3f}\; auc:{:.3f}$'.format(b, auc_))
    
leg = plt.legend(loc="lower right", prop={'size':10})
leg.get_frame().set_linewidth(0.0)
plt.xlabel("False Positive Rate", size=15)
plt.ylabel("True Positive Rate", size=15)

fig.savefig("output.pdf")

#After sum of two leading btags

beta = np.arange(-1, 2, 0.2)

fig = plt.figure(figsize=(13,7), dpi=100)

colors = plt.cm.cool(np.linspace(0,1,len(beta)))

for idx, b in tqdm(enumerate(beta)):
    
    b_sig = []
    b_bkg = []
    
    for event in tree_sig:
        calo_btag = event.calo_btag
        calo_pt = event.calo_pt
        calo_eta = event.calo_eta
        pf_btag = event.pf_btag
        pf_pt = event.pf_pt
        pf_eta = event.pf_eta
        pf_phi = event.pf_phi

        btag, pt = (list(t) for t in zip(*sorted(zip(calo_btag, calo_pt))))
        if(sum(btag[-2:]) >= 1.5):
            beta_w = btag[-1]/pow(pt[-1],b) + btag[-2]/pow(pt[-2],b) + btag[-3]/pow(pt[-3],b)
            b_sig.append(beta_w)
        



    for event in tree_bkg:
        calo_btag = event.calo_btag
        calo_pt = event.calo_pt
        calo_eta = event.calo_eta
        pf_btag = event.pf_btag
        pf_pt = event.pf_pt
        pf_eta = event.pf_eta
        pf_phi = event.pf_phi

        btag, pt = (list(t) for t in zip(*sorted(zip(calo_btag, calo_pt))))
        if(sum(btag[-2:]) >= 1.5):
            beta_w = btag[-1]/pow(pt[-1],b) + btag[-2]/pow(pt[-2],b) + btag[-3]/pow(pt[-3],b)
            b_bkg.append(beta_w)

        
    min_ = min(min(b_sig), min(b_bkg))
    max_ = max(max(b_sig), max(b_bkg))
    step = (max_-min_)/100
        
    th_ = np.arange(min_, max_ , step)
    
    tpr = []
    fpr = []

    for th in th_:
        count_sig = 0
        count_bkg = 0
        tot_sig = len(b_sig)
        tot_bkg = len(b_bkg)
        for i in b_sig:
            if i >= th:
                count_sig+=1
        for j in b_bkg:
            if j >= th:
                count_bkg+=1

        tpr.append(float(count_sig)/tot_sig)
        fpr.append(float(count_bkg)/tot_bkg)
        
    auc_ = auc(fpr, tpr)
        
    plt.plot(fpr, tpr, c=colors[idx], label=r'$\beta={:.3f}\; auc:{:.3f}$'.format(b, auc_))
    
leg = plt.legend(loc="lower right", prop={'size':10})
leg.get_frame().set_linewidth(0.0)
plt.xlabel("False Positive Rate", size=15)
plt.ylabel("True Positive Rate", size=15)

fig.savefig("afterLead2BtagSum.png")

#Same

beta = np.arange(-1, 2, 0.2)

fig = plt.figure(figsize=(13,7), dpi=100)

colors = plt.cm.cool(np.linspace(0,1,len(beta)))

for idx, b in tqdm(enumerate(beta)):
    
    b_sig = []
    b_bkg = []
    
    for event in tree_sig:
        calo_btag = event.calo_btag
        calo_pt = event.calo_pt
        calo_eta = event.calo_eta
        pf_btag = event.pf_btag
        pf_pt = event.pf_pt
        pf_eta = event.pf_eta
        pf_phi = event.pf_phi

        btag, pt = (list(t) for t in zip(*sorted(zip(calo_btag, calo_pt))))
        if(sum(btag[-2:]) >= 1.5):
            beta_w = btag[-1]/pow(pt[-1],b) + btag[-2]/pow(pt[-2],b)
            b_sig.append(beta_w)
        



    for event in tree_bkg:
        calo_btag = event.calo_btag
        calo_pt = event.calo_pt
        calo_eta = event.calo_eta
        pf_btag = event.pf_btag
        pf_pt = event.pf_pt
        pf_eta = event.pf_eta
        pf_phi = event.pf_phi

        btag, pt = (list(t) for t in zip(*sorted(zip(calo_btag, calo_pt))))
        if(sum(btag[-2:]) >= 1.5):
            beta_w = btag[-1]/pow(pt[-1],b) + btag[-2]/pow(pt[-2],b)
            b_bkg.append(beta_w)

        
    min_ = min(min(b_sig), min(b_bkg))
    max_ = max(max(b_sig), max(b_bkg))
    step = (max_-min_)/100
        
    th_ = np.arange(min_, max_ , step)
    
    tpr = []
    fpr = []

    for th in th_:
        count_sig = 0
        count_bkg = 0
        tot_sig = len(b_sig)
        tot_bkg = len(b_bkg)
        for i in b_sig:
            if i >= th:
                count_sig+=1
        for j in b_bkg:
            if j >= th:
                count_bkg+=1

        tpr.append(float(count_sig)/tot_sig)
        fpr.append(float(count_bkg)/tot_bkg)
        
    auc_ = auc(fpr, tpr)
        
    plt.plot(fpr, tpr, c=colors[idx], label=r'$\beta={:.3f}\; auc:{:.3f}$'.format(b, auc_))
    
leg = plt.legend(loc="lower right", prop={'size':10})
leg.get_frame().set_linewidth(0.0)
plt.xlabel("False Positive Rate", size=15)
plt.ylabel("True Positive Rate", size=15)



