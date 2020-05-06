import matplotlib.pyplot as plt
import numpy as np
import pandas as pd

"""
    Plot calo filters scan of 2018 config. All other filters fixed at default
    threshold.
"""



fig, (ax1, ax2, ax3) = plt.subplots(1,3, figsize=(35, 7))

#first
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


a0 = ax1.scatter(siga, rate, c=th, label = 'hltQuadCentralJet30 Threshold', cmap=plt.cm.get_cmap('cool'))
cbar1 = fig.colorbar(a0, ax=ax1)
ax1.scatter(0.22594, 3.5572, s=200, marker="P", c="black", label = "2018 Default, Th = 30 GeV")
leg = ax1.legend(loc='upper left', prop={"size":15}, frameon=False)
ax1.set_xlabel('Signal Acceptance', size=15)
ax1.set_ylabel('Rate (Hz)', size=15)
cbar1.ax.tick_params(labelsize=13)
cbar1.set_label(label='$P_{t}$ Threshold (GeV)', size=13)
ax1.tick_params(axis='both', which='major', labelsize=13)
plt.gcf().text(0.03, 1, r"$gg\rightarrow HH \rightarrow bbbb$", fontsize=15)
plt.gcf().text(0.18, 1, r"$323725 \: Lumi [37:57] \: PU > 50$", fontsize=15)


#second
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
    
a1 = ax2.scatter(siga, rate, c=th, label = 'hltCaloQuadJet30HT320 Threshold', cmap=plt.cm.get_cmap('cool'))
cbar2 = fig.colorbar(a1, ax=ax2)
ax2.scatter(0.22594, 3.5572, s=200, marker="P", c="black", label = "2018 Default, Th = 320 GeV")
leg = ax2.legend(loc='upper left', prop={"size":15}, frameon=False)
ax2.set_xlabel('Signal Acceptance', size=15)
ax2.set_ylabel('Rate (Hz)', size=15)
cbar2.ax.tick_params(labelsize=13)
cbar2.set_label(label='HT Threshold (GeV)', size=13)
ax2.tick_params(axis='both', which='major', labelsize=13)
plt.gcf().text(0.36, 1, r"$gg\rightarrow HH \rightarrow bbbb$", fontsize=15)
plt.gcf().text(0.51, 1, r"$323725 \: Lumi [37:57] \: PU > 50$", fontsize=15)


#third

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

a2 = ax3.scatter(siga, rate, c=th, label = 'hltBTagCaloDeepCSVp17Double Threshold', cmap=plt.cm.get_cmap('cool'))
cbar3 = fig.colorbar(a2, ax=ax3)
ax3.scatter(0.22594, 3.5572, s=200, marker="P", c="black", label = "2018 Default, Th = 0.17")
leg = ax3.legend(loc='upper left', prop={"size":15}, frameon=False)
ax3.set_xlabel('Signal Acceptance', size=15)
ax3.set_ylabel('Rate (Hz)', size=15)
cbar3.ax.tick_params(labelsize=13)
cbar3.set_label(label='BTag Threshold', size=13)
ax3.tick_params(axis='both', which='major', labelsize=13)
plt.gcf().text(0.69, 1, r"$gg\rightarrow HH \rightarrow bbbb$", fontsize=15)
plt.gcf().text(0.84, 1, r"$323725 \: Lumi [37:57] \: PU > 50$", fontsize=15)

fig.tight_layout()

fig.savefig("./CaloFilters.pdf")
