import ROOT
import numpy as np
import pandas
import matplotlib.pyplot as plt
import math as mt
from sklearn.metrics import auc
from tqdm import tqdm

retained_events_sig = []
retained_events_bkg = []
auc_scores_second = []
auc_scores_third = []
auc_scores_fourth = []
auc_scores_ht = []

for pt_min in range(10, 100, 1):

    f = ROOT.TFile("../ggMCRAW_GenJets.root")
    tree_sig = f.Get("SaveAllJets/Jets")

    f_bkg = ROOT.TFile("../323725_37_57_PU50.root")
    tree_bkg = f_bkg.Get("SaveAllJets/Jets")

    minimum_ev = min(tree_sig.GetEntries(), tree_bkg.GetEntries())

    sig_second = ROOT.TH1F("sig_second", "sig_second", 80, 0, 1000)
    sig_third = ROOT.TH1F("sig_third", "sig_third", 80, 0, 1000)
    sig_fourth = ROOT.TH1F("sig_fourth", "sig_fourth", 80, 0, 1000)
    sig_ht = ROOT.TH1F("sig_ht", "sig_ht", 80, 0, 1000)

    bkg_second = ROOT.TH1F("bkg_second", "bkg_second", 80, 0, 1000)
    bkg_third = ROOT.TH1F("bkg_third", "bkg_third", 80, 0, 1000)
    bkg_fourth = ROOT.TH1F("bkg_fourth", "bkg_fourth", 80, 0, 1000)
    bkg_ht = ROOT.TH1F("bkg_ht", "bkg_ht", 80, 0, 1000)


    counter = 0
    for event in tree_sig:
        if counter == minimum_ev: break
        pf_pt = sorted(event.pf_pt)
        if(pf_pt[-1] >= pt_min):
            sig_second.Fill(pf_pt[-2])
            sig_third.Fill(pf_pt[-3])
            sig_fourth.Fill(pf_pt[-4])
            ht = sum(i for i in pf_pt if i > 30)
            sig_ht.Fill(ht)
        counter+=1

    counter = 0
    for event in tree_bkg:
        if counter == minimum_ev: break
        pf_pt = sorted(event.pf_pt)
        if(pf_pt[-1] >= pt_min):
            bkg_second.Fill(pf_pt[-2])
            bkg_third.Fill(pf_pt[-3])
            bkg_fourth.Fill(pf_pt[-4])
            ht = sum(i for i in pf_pt if i > 30)
            bkg_ht.Fill(ht)
        counter+=1
        
    tpr = []
    fpr = []
    tot_sig = sig_second.Integral()
    tot_bkg = bkg_second.Integral()
    
    retained_events_sig.append(tot_sig)
    retained_events_bkg.append(tot_bkg)
    
    for i in range(1,81):
        tpr.append(sig_second.Integral(i, 80)/tot_sig)
        fpr.append(bkg_second.Integral(i, 80)/tot_bkg)
    auc_scores_second.append(auc(fpr, tpr))
    
    tpr = []
    fpr = []
    tot_sig = sig_third.Integral()
    tot_bkg = bkg_third.Integral()
    for i in range(1,81):
        tpr.append(sig_third.Integral(i, 80)/tot_sig)
        fpr.append(bkg_third.Integral(i, 80)/tot_bkg)
    auc_scores_third.append(auc(fpr, tpr))
    
    tpr = []
    fpr = []
    tot_sig = sig_fourth.Integral()
    tot_bkg = bkg_fourth.Integral()
    for i in range(1,81):
        tpr.append(sig_fourth.Integral(i, 80)/tot_sig)
        fpr.append(bkg_fourth.Integral(i, 80)/tot_bkg)
    auc_scores_fourth.append(auc(fpr, tpr))
    
    tpr = []
    fpr = []
    tot_sig = sig_ht.Integral()
    tot_bkg = bkg_ht.Integral()
    for i in range(1,81):
        tpr.append(sig_ht.Integral(i, 80)/tot_sig)
        fpr.append(bkg_ht.Integral(i, 80)/tot_bkg)
    auc_scores_ht.append(auc(fpr, tpr))
    
    f_bkg.Close()
    f.Close()
    
fig, (ax1,ax2) = plt.subplots(1,2,figsize=(15,8))
ax1.plot([i for i in range(10,100,1)], auc_scores_second, label = r'$p_{t}$ 2nd AUC')
ax1.plot([i for i in range(10,100,1)], auc_scores_third, label = r'$p_{t}$ 3rd AUC')
ax1.plot([i for i in range(10,100,1)], auc_scores_fourth, label = r'$p_{t}$ 4th AUC')
ax1.plot([i for i in range(10,100,1)], auc_scores_ht, label = 'HT AUC')
ax1.set_xlabel(r"Leading p_{T} min")
ax1.set_ylabel("AUC score")
ax1.legend(loc='upper left', prop={'size':14})
ax1.set_ylim(0.5, 0.82)
#plt.plot([75, 0], [75, 0.550], '--', alpha= 0.3, c = 'grey')

sig_tot = sig_leading.Integral()
bkg_tot = bkg_leading.Integral()

ax2.plot([i for i in range(10,100,1)], [float(i)/sig_tot for i in retained_events_sig], label = 'Retained Signal events')
ax2.plot([i for i in range(10,100,1)], [float(i)/bkg_tot for i in retained_events_bkg], label = 'Retained Background events')
ax2.set_xlabel(r"Leading p_{T} min")
ax2.set_ylabel("Events passing filter")
ax2.plot([75, 75], [0.7,1], '--', c='grey', label='2018 Default')
ax2.legend(loc='lower left', prop={'size':14})

fig.savefig("AucScan_LeadPt.png")


retained_events_sig = []
retained_events_bkg = []
auc_scores_third = []
auc_scores_fourth = []
auc_scores_ht = []

for pt_min in range(10, 100, 1):

    f = ROOT.TFile("../ggMCRAW_GenJets.root")
    tree_sig = f.Get("SaveAllJets/Jets")

    f_bkg = ROOT.TFile("../323725_37_57_PU50.root")
    tree_bkg = f_bkg.Get("SaveAllJets/Jets")

    minimum_ev = min(tree_sig.GetEntries(), tree_bkg.GetEntries())

    sig_second = ROOT.TH1F("sig_second", "sig_second", 80, 0, 1000)
    sig_third = ROOT.TH1F("sig_third", "sig_third", 80, 0, 1000)
    sig_fourth = ROOT.TH1F("sig_fourth", "sig_fourth", 80, 0, 1000)
    sig_ht = ROOT.TH1F("sig_ht", "sig_ht", 80, 0, 1000)

    bkg_second = ROOT.TH1F("bkg_second", "bkg_second", 80, 0, 1000)
    bkg_third = ROOT.TH1F("bkg_third", "bkg_third", 80, 0, 1000)
    bkg_fourth = ROOT.TH1F("bkg_fourth", "bkg_fourth", 80, 0, 1000)
    bkg_ht = ROOT.TH1F("bkg_ht", "bkg_ht", 80, 0, 1000)


    counter = 0
    for event in tree_sig:
        if counter == minimum_ev: break
        pf_pt = sorted(event.pf_pt)
        if(pf_pt[-1] >= 75):
            if(pf_pt[-2] >= pt_min):
                sig_third.Fill(pf_pt[-3])
                sig_fourth.Fill(pf_pt[-4])
                ht = sum(i for i in pf_pt if i > 30)
                sig_ht.Fill(ht)
        counter+=1
    
    counter = 0
    for event in tree_bkg:
        if counter == minimum_ev: break
        pf_pt = sorted(event.pf_pt)
        if(pf_pt[-1] >= 75):
            if(pf_pt[-2] >= pt_min):
                bkg_third.Fill(pf_pt[-3])
                bkg_fourth.Fill(pf_pt[-4])
                ht = sum(i for i in pf_pt if i > 30)
                bkg_ht.Fill(ht)
        counter+=1
    
    tpr = []
    fpr = []
    tot_sig = sig_third.Integral()
    tot_bkg = bkg_third.Integral()
    
    retained_events_sig.append(tot_sig)
    retained_events_bkg.append(tot_bkg)
    
    for i in range(1,81):
        tpr.append(sig_third.Integral(i, 80)/tot_sig)
        fpr.append(bkg_third.Integral(i, 80)/tot_bkg)
    auc_scores_third.append(auc(fpr, tpr))
    
    tpr = []
    fpr = []
    tot_sig = sig_fourth.Integral()
    tot_bkg = bkg_fourth.Integral()
    for i in range(1,81):
        tpr.append(sig_fourth.Integral(i, 80)/tot_sig)
        fpr.append(bkg_fourth.Integral(i, 80)/tot_bkg)
    auc_scores_fourth.append(auc(fpr, tpr))
    
    tpr = []
    fpr = []
    tot_sig = sig_ht.Integral()
    tot_bkg = bkg_ht.Integral()
    for i in range(1,81):
        tpr.append(sig_ht.Integral(i, 80)/tot_sig)
        fpr.append(bkg_ht.Integral(i, 80)/tot_bkg)
    auc_scores_ht.append(auc(fpr, tpr))
    
    f.Close()
    f_bkg.Close()
    
fig, (ax1,ax2) = plt.subplots(1,2,figsize=(15,8))
ax1.plot([i for i in range(10,100,1)], auc_scores_third, label = r'$p_{t}$ 3rd AUC')
ax1.plot([i for i in range(10,100,1)], auc_scores_fourth, label = r'$p_{t}$ 4th AUC')
ax1.plot([i for i in range(10,100,1)], auc_scores_ht, label = 'HT AUC')
ax1.set_xlabel(r"Second $p_{T}$ min")
ax1.set_ylabel("AUC score")
ax1.legend(loc='upper left', prop={'size':14})
ax1.set_ylim(0.5, 0.82)
#plt.plot([75, 0], [75, 0.550], '--', alpha= 0.3, c = 'grey')

ax2.plot([i for i in range(10,100,1)], [float(i) for i in retained_events_sig], label = 'Retained Signal events')
ax2.plot([i for i in range(10,100,1)], [float(i) for i in retained_events_bkg], label = 'Retained Background events')
ax2.set_xlabel(r"Second $p_{T}$ min")
ax2.set_ylabel("Events passing filter")
ax2.plot([60, 60], [0.1,11000], '--', c='grey', label='2018 Default')
ax2.legend(loc='lower left', prop={'size':14})

fig.savefig("AUCScan_2LeadPt.png")
    

