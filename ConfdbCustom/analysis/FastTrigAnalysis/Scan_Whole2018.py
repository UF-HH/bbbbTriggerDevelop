import matplotlib.pyplot as plt
import numpy as np
import pandas as pd

"""
    csv created with default 2018 config and varying one filter threshold.
    This creates 2d scatter plots, colorbar as defined threshold.
"""

#CaloBTag

df = pd.read_csv("CaloBTagSpan.csv", index_col=[0], header=[0,1])
totbkg = df["totbkg"]["count"].values[0]
totsig = df["totsig"]["count"].values[0]
th = df["hltBTagCaloDeepCSVp17Double"]["setBTagMin"].values

sig = df["countsig"]["count"].values
bkg = df["countbkg"]["count"].values

rate = []
siga = []


for i,j in zip(sig, bkg):
    rate.append(1100*float(j)/totbkg *31.40)
    siga.append(float(i)/totsig)

plt.figure(figsize=(13,8))
plt.scatter(siga, rate, c=th, label = 'hltBTagCaloDeepCSVp17Double Threshold', cmap=plt.cm.get_cmap('cool'))
cbar = plt.colorbar()
plt.scatter(0.22594, 3.5572, s=200, marker="P", c="black", label = "2018 Default, Th = 0.17")
leg = plt.legend(loc='upper left', prop={"size":15}, frameon=False)
plt.xlabel('Signal Acceptance', size=15)
plt.ylabel('Rate (Hz)', size=15)
cbar.ax.tick_params(labelsize=13)
cbar.set_label(label='BTag Threshold', size=13)
plt.xticks(np.arange(0.019, 0.22+0.01, 0.021))
plt.tick_params(axis='both', which='major', labelsize=13)
plt.gcf().text(0.14, 0.9, r"$gg\rightarrow HH \rightarrow bbbb$", fontsize=15)
plt.gcf().text(0.5, 0.9, r"$323725 \: Lumi [37:57] \: PU > 50$", fontsize=15)


#CaloHT scan

df = pd.read_csv("CaloHTScan.csv", index_col=[0], header=[0,1])
totbkg = df["totbkg"]["count"].values[0]
totsig = df["totsig"]["count"].values[0]
th = df["hltCaloQuadJet30HT320"]["setHTMin"].values

sig = df["countsig"]["count"].values
bkg = df["countbkg"]["count"].values

rate = []
siga = []


for i,j in zip(sig, bkg):
    rate.append(1100*float(j)/totbkg *31.40)
    siga.append(float(i)/totsig)

plt.figure(figsize=(13,8))
plt.scatter(siga, rate, c=th, label = 'hltCaloQuadJet30HT320 Threshold', cmap=plt.cm.get_cmap('cool'))
cbar = plt.colorbar()
plt.scatter(0.22594, 3.5572, s=200, marker="P", c="black", label = "2018 Default, Th = 320 GeV")
leg = plt.legend(loc='upper left', prop={"size":15}, frameon=False)
plt.xlabel('Signal Acceptance', size=15)
plt.ylabel('Rate (Hz)', size=15)
cbar.ax.tick_params(labelsize=13)
cbar.set_label(label='HT Threshold (GeV)', size=13)
#plt.xticks(np.arange(0.019, 0.22+0.01, 0.021))
plt.tick_params(axis='both', which='major', labelsize=13)
plt.gcf().text(0.14, 0.9, r"$gg\rightarrow HH \rightarrow bbbb$", fontsize=15)
plt.gcf().text(0.5, 0.9, r"$323725 \: Lumi [37:57] \: PU > 50$", fontsize=15)

#CaloQuad Jet scan

df = pd.read_csv("CaloQuadJetScan.csv", index_col=[0], header=[0,1])
totbkg = df["totbkg"]["count"].values[0]
totsig = df["totsig"]["count"].values[0]
th = df["hltQuadCentralJet30"]["setPtMin"].values

sig = df["countsig"]["count"].values
bkg = df["countbkg"]["count"].values

rate = []
siga = []


for i,j in zip(sig, bkg):
    rate.append(1100*float(j)/totbkg *31.40)
    siga.append(float(i)/totsig)

plt.figure(figsize=(13,8))
plt.scatter(siga, rate, c=th, label = 'hltQuadCentralJet30 Threshold', cmap=plt.cm.get_cmap('cool'))
cbar = plt.colorbar()
plt.scatter(0.22594, 3.5572, s=200, marker="P", c="black", label = "2018 Default, Th = 30 GeV")
leg = plt.legend(loc='upper left', prop={"size":15}, frameon=False)
plt.xlabel('Signal Acceptance', size=15)
plt.ylabel('Rate (Hz)', size=15)
cbar.ax.tick_params(labelsize=13)
cbar.set_label(label='$P_{t}$ Threshold (GeV)', size=13)
#plt.xticks(np.arange(0.019, 0.22+0.01, 0.021))
plt.tick_params(axis='both', which='major', labelsize=13)
plt.gcf().text(0.14, 0.9, r"$gg\rightarrow HH \rightarrow bbbb$", fontsize=15)
plt.gcf().text(0.5, 0.9, r"$323725 \: Lumi [37:57] \: PU > 50$", fontsize=15)

#PFQuad Jet

df = pd.read_csv("PFQuadJetScan.csv", index_col=[0], header=[0,1])
totbkg = df["totbkg"]["count"].values[0]
totsig = df["totsig"]["count"].values[0]
th = df["hltPFCentralJetLooseIDQuad30"]["setPtMin"].values

sig = df["countsig"]["count"].values
bkg = df["countbkg"]["count"].values

rate = []
siga = []


for i,j in zip(sig, bkg):
    rate.append(1100*float(j)/totbkg *31.40)
    siga.append(float(i)/totsig)

plt.figure(figsize=(13,8))
plt.scatter(siga, rate, c=th, label = 'hltPFCentralJetLooseIDQuad30 Threshold', cmap=plt.cm.get_cmap('cool'))
cbar = plt.colorbar()
plt.scatter(0.22594, 3.5572, s=200, marker="P", c="black", label = "2018 Default, Th = 30 GeV")
leg = plt.legend(loc='upper left', prop={"size":15}, frameon=False)
plt.xlabel('Signal Acceptance', size=15)
plt.ylabel('Rate (Hz)', size=15)
cbar.ax.tick_params(labelsize=13)
cbar.set_label(label='$P_{t}$ Threshold (GeV)', size=13)
#plt.xticks(np.arange(0.019, 0.22+0.01, 0.021))
plt.tick_params(axis='both', which='major', labelsize=13)
plt.gcf().text(0.14, 0.9, r"$gg\rightarrow HH \rightarrow bbbb$", fontsize=15)
plt.gcf().text(0.5, 0.9, r"$323725 \: Lumi [37:57] \: PU > 50$", fontsize=15)

#1 PF 75

df = pd.read_csv("PF_1_75.csv", index_col=[0], header=[0,1])
totbkg = df["totbkg"]["count"].values[0]
totsig = df["totsig"]["count"].values[0]
th = df["hlt1PFCentralJetLooseID75"]["setPtMin"].values

sig = df["countsig"]["count"].values
bkg = df["countbkg"]["count"].values

rate = []
siga = []


for i,j in zip(sig, bkg):
    rate.append(1100*float(j)/totbkg *31.40)
    siga.append(float(i)/totsig)

plt.figure(figsize=(13,8))
plt.scatter(siga, rate, c=th, label = 'hlt1PFCentralJetLooseID75 Threshold', cmap=plt.cm.get_cmap('cool'))
cbar = plt.colorbar()
plt.scatter(0.22594, 3.5572, s=200, marker="P", c="black", label = "2018 Default, Th = 75 GeV")
leg = plt.legend(loc='upper left', prop={"size":15}, frameon=False)
plt.xlabel('Signal Acceptance', size=15)
plt.ylabel('Rate (Hz)', size=15)
cbar.ax.tick_params(labelsize=13)
cbar.set_label(label='$P_{t}$ Threshold (GeV)', size=13)
#plt.xticks(np.arange(0.019, 0.22+0.01, 0.021))
plt.tick_params(axis='both', which='major', labelsize=13)
plt.gcf().text(0.14, 0.9, r"$gg\rightarrow HH \rightarrow bbbb$", fontsize=15)
plt.gcf().text(0.5, 0.9, r"$323725 \: Lumi [37:57] \: PU > 50$", fontsize=15)

#2 PF 60

df = pd.read_csv("PF_2_60.csv", index_col=[0], header=[0,1])
totbkg = df["totbkg"]["count"].values[0]
totsig = df["totsig"]["count"].values[0]
th = df["hlt2PFCentralJetLooseID60"]["setPtMin"].values

sig = df["countsig"]["count"].values
bkg = df["countbkg"]["count"].values

rate = []
siga = []


for i,j in zip(sig, bkg):
    rate.append(1100*float(j)/totbkg *31.40)
    siga.append(float(i)/totsig)

plt.figure(figsize=(13,8))
plt.scatter(siga, rate, c=th, label = 'hlt2PFCentralJetLooseID60 Threshold', cmap=plt.cm.get_cmap('cool'))
cbar = plt.colorbar()
plt.scatter(0.22594, 3.5572, s=200, marker="P", c="black", label = "2018 Default, Th = 60 GeV")
leg = plt.legend(loc='upper left', prop={"size":15}, frameon=False)
plt.xlabel('Signal Acceptance', size=15)
plt.ylabel('Rate (Hz)', size=15)
cbar.ax.tick_params(labelsize=13)
cbar.set_label(label='$P_{t}$ Threshold (GeV)', size=13)
#plt.xticks(np.arange(0.019, 0.22+0.01, 0.021))
plt.tick_params(axis='both', which='major', labelsize=13)
plt.gcf().text(0.14, 0.9, r"$gg\rightarrow HH \rightarrow bbbb$", fontsize=15)
plt.gcf().text(0.5, 0.9, r"$323725 \: Lumi [37:57] \: PU > 50$", fontsize=15)

#3 PF 45

df = pd.read_csv("PF_3_45.csv", index_col=[0], header=[0,1])
totbkg = df["totbkg"]["count"].values[0]
totsig = df["totsig"]["count"].values[0]
th = df["hlt3PFCentralJetLooseID45"]["setPtMin"].values

sig = df["countsig"]["count"].values
bkg = df["countbkg"]["count"].values

rate = []
siga = []


for i,j in zip(sig, bkg):
    rate.append(1100*float(j)/totbkg *31.40)
    siga.append(float(i)/totsig)

plt.figure(figsize=(13,8))
plt.scatter(siga, rate, c=th, label = 'hlt3PFCentralJetLooseID45 Threshold', cmap=plt.cm.get_cmap('cool'))
cbar = plt.colorbar()
plt.scatter(0.22594, 3.5572, s=200, marker="P", c="black", label = "2018 Default, Th = 45 GeV")
leg = plt.legend(loc='upper left', prop={"size":15}, frameon=False)
plt.xlabel('Signal Acceptance', size=15)
plt.ylabel('Rate (Hz)', size=15)
cbar.ax.tick_params(labelsize=13)
cbar.set_label(label='$P_{t}$ Threshold (GeV)', size=13)
#plt.xticks(np.arange(0.019, 0.22+0.01, 0.021))
plt.tick_params(axis='both', which='major', labelsize=13)
plt.gcf().text(0.14, 0.9, r"$gg\rightarrow HH \rightarrow bbbb$", fontsize=15)
plt.gcf().text(0.5, 0.9, r"$323725 \: Lumi [37:57] \: PU > 50$", fontsize=15)

# 4 PF 40

df = pd.read_csv("PF_4_40.csv", index_col=[0], header=[0,1])
totbkg = df["totbkg"]["count"].values[0]
totsig = df["totsig"]["count"].values[0]
th = df["hlt4PFCentralJetLooseID40"]["setPtMin"].values

sig = df["countsig"]["count"].values
bkg = df["countbkg"]["count"].values

rate = []
siga = []


for i,j in zip(sig, bkg):
    rate.append(1100*float(j)/totbkg *31.40)
    siga.append(float(i)/totsig)

plt.figure(figsize=(13,8))
plt.scatter(siga, rate, c=th, label = 'hlt4PFCentralJetLooseID40 Threshold', cmap=plt.cm.get_cmap('cool'))
cbar = plt.colorbar()
plt.scatter(0.22594, 3.5572, s=200, marker="P", c="black", label = "2018 Default, Th = 40 GeV")
leg = plt.legend(loc='upper left', prop={"size":15}, frameon=False)
plt.xlabel('Signal Acceptance', size=15)
plt.ylabel('Rate (Hz)', size=15)
cbar.ax.tick_params(labelsize=13)
cbar.set_label(label='$P_{t}$ Threshold (GeV)', size=13)
#plt.xticks(np.arange(0.019, 0.22+0.01, 0.021))
plt.tick_params(axis='both', which='major', labelsize=13)
plt.gcf().text(0.14, 0.9, r"$gg\rightarrow HH \rightarrow bbbb$", fontsize=15)
plt.gcf().text(0.5, 0.9, r"$323725 \: Lumi [37:57] \: PU > 50$", fontsize=15)

#PF HT 330

df = pd.read_csv("PFHTScan.csv", index_col=[0], header=[0,1])
totbkg = df["totbkg"]["count"].values[0]
totsig = df["totsig"]["count"].values[0]
th = df["hltPFCentralJetsLooseIDQuad30HT330"]["setHTMin"].values

sig = df["countsig"]["count"].values
bkg = df["countbkg"]["count"].values

rate = []
siga = []


for i,j in zip(sig, bkg):
    rate.append(1100*float(j)/totbkg *31.40)
    siga.append(float(i)/totsig)

plt.figure(figsize=(13,8))
plt.scatter(siga, rate, c=th, label = 'hltPFCentralJetsLooseIDQuad30HT330 Threshold', cmap=plt.cm.get_cmap('cool'))
cbar = plt.colorbar()
plt.scatter(0.22594, 3.5572, s=200, marker="P", c="black", label = "2018 Default, Th = 330 GeV")
leg = plt.legend(loc='upper left', prop={"size":15}, frameon=False)
plt.xlabel('Signal Acceptance', size=15)
plt.ylabel('Rate (Hz)', size=15)
cbar.ax.tick_params(labelsize=13)
cbar.set_label(label='$P_{t}$ Threshold (GeV)', size=13)
#plt.xticks(np.arange(0.019, 0.22+0.01, 0.021))
plt.tick_params(axis='both', which='major', labelsize=13)
plt.gcf().text(0.14, 0.9, r"$gg\rightarrow HH \rightarrow bbbb$", fontsize=15)
plt.gcf().text(0.5, 0.9, r"$323725 \: Lumi [37:57] \: PU > 50$", fontsize=15)

#PF 3 BTag

df = pd.read_csv("PFBTagScan.csv", index_col=[0], header=[0,1])
totbkg = df["totbkg"]["count"].values[0]
totsig = df["totsig"]["count"].values[0]
th = df["hltBTagPFDeepCSV4p5Triple"]["setBTagMin"].values

sig = df["countsig"]["count"].values
bkg = df["countbkg"]["count"].values

rate = []
siga = []


for i,j in zip(sig, bkg):
    rate.append(1100*float(j)/totbkg *31.40)
    siga.append(float(i)/totsig)

plt.figure(figsize=(13,8))
plt.scatter(siga, rate, c=th, label = 'hltBTagPFDeepCSV4p5Triple Threshold', cmap=plt.cm.get_cmap('cool'))
cbar = plt.colorbar()
plt.scatter(0.22594, 3.5572, s=200, marker="P", c="black", label = "2018 Default, Th = 0.24 GeV")
leg = plt.legend(loc='upper left', prop={"size":15}, frameon=False)
plt.xlabel('Signal Acceptance', size=15)
plt.ylabel('Rate (Hz)', size=15)
cbar.ax.tick_params(labelsize=13)
cbar.set_label(label='$P_{t}$ Threshold (GeV)', size=13)
#plt.xticks(np.arange(0.019, 0.22+0.01, 0.021))
plt.tick_params(axis='both', which='major', labelsize=13)
plt.gcf().text(0.14, 0.9, r"$gg\rightarrow HH \rightarrow bbbb$", fontsize=15)
plt.gcf().text(0.5, 0.9, r"$323725 \: Lumi [37:57] \: PU > 50$", fontsize=15)





