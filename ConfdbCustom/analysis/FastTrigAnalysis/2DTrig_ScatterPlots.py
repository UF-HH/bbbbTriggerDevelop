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

minimum_ev = min(tree_sig.GetEntries(), tree_bkg.GetEntries())

sig_1stcalo_2ndcalo = []
sig_3rdcalo_4thcalo = []
sig_2ndcaloB_3rdcaloB = []
sig_1stpf_2ndpf = []
sig_3rdpf_4thpf = []
sig_2ndpfB_3rdpfB = []

counter = 0
for event in tree_sig:
    counter += 1
    if counter == minimum_ev: break
    pf_pt = event.pf_pt
    pf_eta = event.pf_eta
    calo_pt = event.calo_pt
    calo_eta = event.calo_eta
    calo_btag = event.calo_btag
    pf_btag = event.pf_btag
    
    sig_1stcalo_2ndcalo.append([sorted(calo_pt)[-1], sorted(calo_pt)[-2]])
    
    if(Trig2DFilters.myFirst(calo_pt, calo_eta)):
        sig_3rdcalo_4thcalo.append([sorted(calo_pt)[-3], sorted(calo_pt)[-4]])
        
    if(Trig2DFilters.myFirst(calo_pt, calo_eta) and Trig2DFilters.mySecond(calo_pt, calo_eta)):
        sig_2ndcaloB_3rdcaloB.append([sorted(calo_btag)[-2], sorted(calo_btag)[-3]])
    
    if(Trig2DFilters.myFirst(calo_pt, calo_eta) and Trig2DFilters.mySecond(calo_pt, calo_eta) and Trig2DFilters.myThird(calo_pt, calo_eta, calo_btag)):
        sig_1stpf_2ndpf.append([sorted(pf_pt)[-1], sorted(pf_pt)[-2]])
        
    if(Trig2DFilters.myFirst(calo_pt, calo_eta) and Trig2DFilters.mySecond(calo_pt, calo_eta) and Trig2DFilters.myThird(calo_pt, calo_eta, calo_btag) and Trig2DFilters.myFourth(pf_pt, pf_eta)):
        sig_3rdpf_4thpf.append([sorted(pf_pt)[-3], sorted(pf_pt)[-4]])
        
    if(Trig2DFilters.myFirst(calo_pt, calo_eta) and Trig2DFilters.mySecond(calo_pt, calo_eta) and Trig2DFilters.myThird(calo_pt, calo_eta, calo_btag) and Trig2DFilters.myFourth(pf_pt, pf_eta) and Trig2DFilters.myFifth(pf_pt, pf_eta)):
        sig_2ndpfB_3rdpfB.append([sorted(pf_btag)[-2], sorted(pf_btag)[-3]])
        
bkg_1stcalo_2ndcalo = []
bkg_3rdcalo_4thcalo = []
bkg_2ndcaloB_3rdcaloB = []
bkg_1stpf_2ndpf = []
bkg_3rdpf_4thpf = []
bkg_2ndpfB_3rdpfB = []

counter = 0
for event in tree_bkg:
    if counter == minimum_ev: break
    counter += 1
    pf_pt = event.pf_pt
    pf_eta = event.pf_eta
    calo_pt = event.calo_pt
    calo_eta = event.calo_eta
    calo_btag = event.calo_btag
    pf_btag = event.pf_btag
    
    bkg_1stcalo_2ndcalo.append([sorted(calo_pt)[-1], sorted(calo_pt)[-2]])
    
    if(myFirst(calo_pt, calo_eta)):
        bkg_3rdcalo_4thcalo.append([sorted(calo_pt)[-3], sorted(calo_pt)[-4]])
        
    if(myFirst(calo_pt, calo_eta) and mySecond(calo_pt, calo_eta)):
        bkg_2ndcaloB_3rdcaloB.append([sorted(calo_btag)[-2], sorted(calo_btag)[-3]])
    
    if(myFirst(calo_pt, calo_eta) and mySecond(calo_pt, calo_eta) and myThird(calo_pt, calo_eta, calo_btag)):
        bkg_1stpf_2ndpf.append([sorted(pf_pt)[-1], sorted(pf_pt)[-2]])
        
    if(myFirst(calo_pt, calo_eta) and mySecond(calo_pt, calo_eta) and myThird(calo_pt, calo_eta, calo_btag) and myFourth(pf_pt, pf_eta)):
        bkg_3rdpf_4thpf.append([sorted(pf_pt)[-3], sorted(pf_pt)[-4]])
        
    if(myFirst(calo_pt, calo_eta) and mySecond(calo_pt, calo_eta) and myThird(calo_pt, calo_eta, calo_btag) and myFourth(pf_pt, pf_eta) and myFifth(pf_pt, pf_eta)):
        bkg_2ndpfB_3rdpfB.append([sorted(pf_btag)[-2], sorted(pf_btag)[-3]])

bkg_1stcalo_2ndcalo = []
bkg_3rdcalo_4thcalo = []
bkg_2ndcaloB_3rdcaloB = []
bkg_1stpf_2ndpf = []
bkg_3rdpf_4thpf = []
bkg_2ndpfB_3rdpfB = []

counter = 0
for event in tree_bkg:
    if counter == minimum_ev: break
    counter += 1
    pf_pt = event.pf_pt
    pf_eta = event.pf_eta
    calo_pt = event.calo_pt
    calo_eta = event.calo_eta
    calo_btag = event.calo_btag
    pf_btag = event.pf_btag
    
    bkg_1stcalo_2ndcalo.append([sorted(calo_pt)[-1], sorted(calo_pt)[-2]])
    
    if(Trig2DFilters.myFirst(calo_pt, calo_eta)):
        bkg_3rdcalo_4thcalo.append([sorted(calo_pt)[-3], sorted(calo_pt)[-4]])
        
    if(Trig2DFilters.myFirst(calo_pt, calo_eta) and Trig2DFilters.mySecond(calo_pt, calo_eta)):
        bkg_2ndcaloB_3rdcaloB.append([sorted(calo_btag)[-2], sorted(calo_btag)[-3]])
    
    if(Trig2DFilters.myFirst(calo_pt, calo_eta) and Trig2DFilters.mySecond(calo_pt, calo_eta) and Trig2DFilters.myThird(calo_pt, calo_eta, calo_btag)):
        bkg_1stpf_2ndpf.append([sorted(pf_pt)[-1], sorted(pf_pt)[-2]])
        
    if(Trig2DFilters.myFirst(calo_pt, calo_eta) and Trig2DFilters.mySecond(calo_pt, calo_eta) and Trig2DFilters.myThird(calo_pt, calo_eta, calo_btag) and Trig2DFilters.myFourth(pf_pt, pf_eta)):
        bkg_3rdpf_4thpf.append([sorted(pf_pt)[-3], sorted(pf_pt)[-4]])
        
    if(Trig2DFilters.myFirst(calo_pt, calo_eta) and Trig2DFilters.mySecond(calo_pt, calo_eta) and Trig2DFilters.myThird(calo_pt, calo_eta, calo_btag) and Trig2DFilters.myFourth(pf_pt, pf_eta) and Trig2DFilters.myFifth(pf_pt, pf_eta)):
        bkg_2ndpfB_3rdpfB.append([sorted(pf_btag)[-2], sorted(pf_btag)[-3]])
        
        
max_y = max(max([i[1] for i in sig_1stcalo_2ndcalo]), max([i[1] for i in bkg_1stcalo_2ndcalo]))
max_x = max(max([i[0] for i in sig_1stcalo_2ndcalo]), max([i[0] for i in bkg_1stcalo_2ndcalo]))
x = np.arange(0, max_x, 0.01)
y = -x+ 180
x_2016 = np.arange(0, max_x, 0.01)
y_2016 = [90]*len(x_2016)
fig = plt.figure(figsize=(8,8), dpi = 90)
plt.scatter([row[0] for row in sig_1stcalo_2ndcalo ], [row[1] for row in sig_1stcalo_2ndcalo ], c='fuchsia', alpha=0.2, label ='Signal')
plt.scatter([row[0] for row in bkg_1stcalo_2ndcalo ], [row[1] for row in bkg_1stcalo_2ndcalo ], c='blue', alpha=0.2, label = 'Background')
plt.plot(x, y, '--', c='black', label='Cut')
plt.plot(x_2016, y_2016, '--', c='gray', label='Cut 2016')
plt.xlabel("First Pt Calo", size=13)
plt.ylabel("Second Pt Calo", size=13)
leg = plt.legend(loc="upper left", prop={'size': 15})
plt.gcf().text(0.14, 0.9, r"$gg\rightarrow HH \rightarrow bbbb$", fontsize=15)
plt.gcf().text(0.52, 0.9, r"$323725 \: Lumi [37:57] \: PU > 50$", fontsize=15)
leg.get_frame().set_linewidth(0.0)
plt.tick_params(axis='both', which='major', labelsize=11)
plt.xlim(0, max_x)
plt.ylim(0, max_y)
plt.show()
fig.savefig("1stCalo_2ndCalo_cut.png")



max_y = max(max([i[1] for i in sig_3rdcalo_4thcalo]), max([i[1] for i in bkg_3rdcalo_4thcalo]))
max_x = max(max([i[0] for i in sig_3rdcalo_4thcalo]), max([i[0] for i in bkg_3rdcalo_4thcalo]))
x = np.arange(0, max_x, 0.01)
y = -x+ 90
x_2018 = np.arange(0, max_x, 0.01)
y_2018 = [30]*len(x_2018)
fig = plt.figure(figsize=(8,8), dpi = 90)
plt.scatter([row[0] for row in sig_3rdcalo_4thcalo ], [row[1] for row in sig_3rdcalo_4thcalo ], c='fuchsia', alpha=0.2, label ='Signal')
plt.scatter([row[0] for row in bkg_3rdcalo_4thcalo ], [row[1] for row in bkg_3rdcalo_4thcalo ], c='blue', alpha=0.2, label = 'Background')
plt.plot(x, y, '--', c='black', label='Cut')
plt.plot(x_2018, y_2018, '--', c='grey', label='Cut 2018')
plt.xlabel("Third Pt Calo", size=13)
plt.ylabel("Fourth Pt Calo", size=13)
leg = plt.legend(loc="upper left", prop={'size': 15})
plt.gcf().text(0.14, 0.9, r"$gg\rightarrow HH \rightarrow bbbb$", fontsize=15)
plt.gcf().text(0.52, 0.9, r"$323725 \: Lumi [37:57] \: PU > 50$", fontsize=15)
leg.get_frame().set_linewidth(0.0)
plt.tick_params(axis='both', which='major', labelsize=11)
plt.xlim(0, max_x)
plt.ylim(0, max_y)
plt.show()
fig.savefig("3rdCalo_4thCalo_cut.png")


max_y = max(max([i[1] for i in sig_2ndcaloB_3rdcaloB]), max([i[1] for i in sig_2ndcaloB_3rdcaloB]))
max_x = max(max([i[0] for i in sig_2ndcaloB_3rdcaloB]), max([i[0] for i in sig_2ndcaloB_3rdcaloB]))
y = -x+ 0.7
y_2018 = np.arange(0, max_y, 0.01)
x_2018 = [0.17]*len(y_2018)
fig = plt.figure(figsize=(8,8), dpi = 90)
plt.scatter([row[0] for row in sig_2ndcaloB_3rdcaloB ], [row[1] for row in sig_2ndcaloB_3rdcaloB ], c='fuchsia', alpha=0.2, label ='Signal')
plt.scatter([row[0] for row in bkg_2ndcaloB_3rdcaloB ], [row[1] for row in bkg_2ndcaloB_3rdcaloB ], c='blue', alpha=0.2, label = 'Background')
plt.plot(x, y, '--', c='black', label='Cut')
plt.plot(x_2018, y_2018, '--', c='gray', label='Cut 2018')
plt.xlabel("Second BTag Calo", size=13)
plt.ylabel("Third BTag Calo", size=13)
leg = plt.legend(loc="upper center", prop={'size': 15})
plt.gcf().text(0.14, 0.9, r"$gg\rightarrow HH \rightarrow bbbb$", fontsize=15)
plt.gcf().text(0.52, 0.9, r"$323725 \: Lumi [37:57] \: PU > 50$", fontsize=15)
leg.get_frame().set_linewidth(0.0)
plt.tick_params(axis='both', which='major', labelsize=11)
plt.xlim(0, max_x)
plt.ylim(0, max_y)
plt.show()
fig.savefig("2ndCaloB_3rdCaloB_cut.png")


max_y = max(max([i[1] for i in sig_1stpf_2ndpf]), max([i[1] for i in bkg_1stpf_2ndpf]))
max_x = max(max([i[0] for i in sig_1stpf_2ndpf]), max([i[0] for i in bkg_1stpf_2ndpf]))
x = np.arange(0, max_x, 0.01)
y = -x+ 180
x_2016 = np.arange(0, max_x, 0.01)
y_2016 = [60]*len(x_2016)

y_2018 = np.arange(0, max_y, 0.01)
x_2018 = [75]*len(y_2018)

fig = plt.figure(figsize=(8,8), dpi = 90)
plt.scatter([row[0] for row in sig_1stpf_2ndpf ], [row[1] for row in sig_1stpf_2ndpf ], c='fuchsia', alpha=0.2, label ='Signal')
plt.scatter([row[0] for row in bkg_1stpf_2ndpf ], [row[1] for row in bkg_1stpf_2ndpf ], c='blue', alpha=0.2, label = 'Background')
plt.plot(x, y, '--', c='black', label='Cut')
plt.plot(x_2016, y_2016, '--', c='gray', label='Cut 2018')
plt.plot(x_2018, y_2018, '--', c='gray')
plt.xlabel("First Pt PF", size=13)
plt.ylabel("Second Pt PF", size=13)
leg = plt.legend(loc="upper left", prop={'size': 15})
plt.gcf().text(0.14, 0.9, r"$gg\rightarrow HH \rightarrow bbbb$", fontsize=15)
plt.gcf().text(0.52, 0.9, r"$323725 \: Lumi [37:57] \: PU > 50$", fontsize=15)
leg.get_frame().set_linewidth(0.0)
plt.tick_params(axis='both', which='major', labelsize=11)
plt.xlim(0, max_x)
plt.ylim(0, max_y)
plt.show()
fig.savefig("1stPF_2ndPF_cut.png")


max_y = max(max([row[1] for row in sig_3rdpf_4thpf ]), max([row[1] for row in bkg_3rdpf_4thpf ]))
max_x = max(max([row[0] for row in sig_3rdpf_4thpf ]), max([row[0] for row in bkg_3rdpf_4thpf ]))
x = np.arange(0, max_x, 0.01)
y = -x+ 90
x_2018 = np.arange(0, max_x, 0.01)
y_2018 = [30]*len(x_2018)
y_2018_2 = [40]*len(x_2018)

y_2018_1 = np.arange(0, max_y, 0.01)
x_2018_1 = [45]*len(y_2018_1)

fig = plt.figure(figsize=(8,8), dpi = 90)
plt.scatter([row[0] for row in sig_3rdpf_4thpf ], [row[1] for row in sig_3rdpf_4thpf ], c='fuchsia', alpha=0.2, label ='Signal')
plt.scatter([row[0] for row in bkg_3rdpf_4thpf ], [row[1] for row in bkg_3rdpf_4thpf ], c='blue', alpha=0.2, label = 'Background')
plt.plot(x, y, '--', c='black', label='Cut')
plt.plot(x_2018, y_2018, '--', c='grey')
plt.plot(x_2018_1, y_2018_1, '--', c='grey')
plt.plot(x_2018, y_2018_2, '--', c='grey', label='Cuts 2018')
plt.xlabel("Third Pt PF", size=13)
plt.ylabel("Fourth Pt PF", size=13)
leg = plt.legend(loc="upper left", prop={'size': 15})
plt.gcf().text(0.14, 0.9, r"$gg\rightarrow HH \rightarrow bbbb$", fontsize=15)
plt.gcf().text(0.52, 0.9, r"$323725 \: Lumi [37:57] \: PU > 50$", fontsize=15)
leg.get_frame().set_linewidth(0.0)
plt.tick_params(axis='both', which='major', labelsize=11)
plt.xlim(0, max_x)
plt.ylim(0, max_y)
plt.show()
fig.savefig("3rdPF_4thPF_cut.png")


max_y = max(max([row[1] for row in sig_2ndpfB_3rdpfB ]), max([row[1] for row in bkg_2ndpfB_3rdpfB ]))
max_x = max(max([row[0] for row in sig_2ndpfB_3rdpfB ]), max([row[0] for row in bkg_2ndpfB_3rdpfB ]))
x = np.arange(0, max_x, 0.01)
y = -x+ 0.7
x_2018 = np.arange(0, max_x, 0.01)
y_2018 = [0.24]*len(x_2018)
fig = plt.figure(figsize=(8,8), dpi = 100)
plt.scatter([row[0] for row in sig_2ndpfB_3rdpfB ], [row[1] for row in sig_2ndpfB_3rdpfB ], c='fuchsia', alpha=0.2, label ='Signal')
plt.scatter([row[0] for row in bkg_2ndpfB_3rdpfB ], [row[1] for row in bkg_2ndpfB_3rdpfB ], c='blue', alpha=0.2, label = 'Background')
plt.plot(x, y, '--', c='black', label='Cut')
plt.plot(x_2018, y_2018, '--', c='gray', label='Cut 2018')
plt.xlabel("Second PF BTag", size=13)
plt.ylabel("Third PF BTag", size=13)
leg = plt.legend(loc="upper center", prop={'size': 15})
plt.gcf().text(0.14, 0.9, r"$gg\rightarrow HH \rightarrow bbbb$", fontsize=15)
plt.gcf().text(0.52, 0.9, r"$323725 \: Lumi [37:57] \: PU > 50$", fontsize=15)
leg.get_frame().set_linewidth(0.0)
plt.tick_params(axis='both', which='major', labelsize=11)
plt.xlim(0, max_x)
plt.ylim(0, max_y)
plt.show()
fig.savefig("2ndPFB_3rdPFB_cut.png")
        
        
        
