import pandas as pd
import matplotlib.pyplot as plt
import ROOT
from array import array

"""
    Again horrible scripts from notebooks.
    If second part gives segmentation faults copy whole on notebook.
"""

df = pd.read_csv("../RateVsSig/LeaveSomeOut.csv", index_col=[0], header=[0,1])

bkg = df["countbkg"]["count"].values
sig = df["countsig"]["count"].values
totbkg = df["totbkg"]["count"].values[0]
totsig = df["totsig"]["count"].values[0]


ratebkg = []
sigacc = []

for i,j in zip(sig, bkg):
    ratebkg.append(1100*float(j)/totbkg *31.40)
    sigacc.append(float(i)/totsig)

g = ROOT.TGraph(len(sigacc), array('f', sigacc), array('f', ratebkg))
g.SetMarkerStyle(24)
g.SetMarkerColor(ROOT.kBlack)
g.SetMarkerSize(1)
g.GetXaxis().SetLimits(0.21, .82)
g.GetHistogram().SetMaximum(1700.)
g.GetHistogram().SetMinimum(1)
g.SetTitle("")
g.GetXaxis().SetTitle("Signal Acceptance")
g.GetYaxis().SetTitle("Rate (Hz)")

c = ROOT.TCanvas("c", "c", 1000, 1000, 1000, 700)
g.Draw("AP")
T = ROOT.TLatex()
T1 = ROOT.TLatex()
T.DrawLatexNDC(.12, .92, "#scale[0.8]{#font[52]{ggF#rightarrow HH #rightarrow bbbb }}")
T1.DrawLatexNDC(.55, .92, "#scale[0.8]{#font[52]{323725 Lumi [37:57] PU > 50}}")
c.Draw()
c.SaveAs("LeaveSomeFiltersOut.pdf")

#Focusing and discriminating based on number of filters left out

df = pd.read_csv("../RateVsSig/LeaveSomeOut.csv", index_col=[0], header=[0,1])
df = df[df["countbkg"]["count"] < 200]

cols = ['hlt1PFCentralJetLooseID75', 'hlt2PFCentralJetLooseID60', 'hlt3PFCentralJetLooseID45', 'hlt4PFCentralJetLooseID40', 'hltBTagCaloDeepCSVp17Double', 'hltBTagPFDeepCSV4p5Triple', 'hltCaloQuadJet30HT320', 'hltPFCentralJetLooseIDQuad30', 'hltPFCentralJetsLooseIDQuad30HT330', 'hltQuadCentralJet30']
values = [1, 2, 3, 4, 2, 3, 320, 4, 330, 4]

totbkg = df["totbkg"]["count"].values[0]
totsig = df["totsig"]["count"].values[0]

sig_0 = []
bkg_0 = []

sig_1 = []
bkg_1 = []

sig_2 = []
bkg_2 = []

sig_3 = []
bkg_3 = []

sig_4 = []
bkg_4 = []

sig_5 = []
bkg_5 = []

sig_6 = []
bkg_6 = []

sig_7 = []
bkg_7 = []

sig_8 = []
bkg_8 = []

sig_9 = []
bkg_9 = []

df = df.reset_index()
for i in range(len(df)):
    entry = df.loc[i]
    counter = 0
    for j, k in zip(cols, values):
        if j == "hltCaloQuadJet30HT320" or j == "hltPFCentralJetsLooseIDQuad30HT330":
            if entry[j]["setHTMin"] == k:
                counter += 1
        else:
            if entry[j]["setMinN"] == k:
                counter += 1
    
    if(counter==len(cols)):
        sig_0.append(float(entry["countsig"]["count"])/totsig)
        bkg_0.append(1100*(float(entry["countbkg"]["count"])/totbkg)*31.40)
    if(counter==len(cols)-1):
        sig_1.append(float(entry["countsig"]["count"])/totsig)
        bkg_1.append(1100*(float(entry["countbkg"]["count"])/totbkg)*31.40)
    if(counter == len(cols)-2):
        sig_2.append(float(entry["countsig"]["count"])/totsig)
        bkg_2.append(1100*(float(entry["countbkg"]["count"])/totbkg)*31.40)
        
    if(counter == len(cols)-3):
        sig_3.append(float(entry["countsig"]["count"])/totsig)
        bkg_3.append(1100*(float(entry["countbkg"]["count"])/totbkg)*31.40)
        
    if(counter == len(cols)-4):
        sig_4.append(float(entry["countsig"]["count"])/totsig)
        bkg_4.append(1100*(float(entry["countbkg"]["count"])/totbkg)*31.40)
        
    if(counter == len(cols)-5):
        sig_5.append(float(entry["countsig"]["count"])/totsig)
        bkg_5.append(1100*(float(entry["countbkg"]["count"])/totbkg)*31.40)
        
    if(counter == len(cols)-6):
        sig_6.append(float(entry["countsig"]["count"])/totsig)
        bkg_6.append(1100*(float(entry["countbkg"]["count"])/totbkg)*31.40)
        
    if(counter == len(cols)-7):
        sig_7.append(float(entry["countsig"]["count"])/totsig)
        bkg_7.append(1100*(float(entry["countbkg"]["count"])/totbkg)*31.40)
    
    if(counter == len(cols)-8):
        sig_8.append(float(entry["countsig"]["count"])/totsig)
        bkg_8.append(1100*(float(entry["countbkg"]["count"])/totbkg)*31.40)

g = ROOT.TGraph(len(sig_0), array('f', sig_0), array('f', bkg_0))
g.SetTitle("")
g.SetMarkerStyle(34)
g.SetMarkerColor(ROOT.kBlue)
g.SetMarkerSize(3)

glegend = ROOT.TGraph(len(sig_0), array('f', sig_0), array('f', bkg_0))
glegend.SetTitle("")
glegend.SetMarkerStyle(34)
glegend.SetMarkerColor(ROOT.kBlue-2)
glegend.SetMarkerSize(1)

g1 = ROOT.TGraph(len(sig_1), array('f', sig_1), array('f', bkg_1))
g1.SetTitle("")
g1.SetMarkerStyle(24)
#g1.SetMarkerColor(ROOT.kMagenta)
g1.SetMarkerSize(1)

g2 = ROOT.TGraph(len(sig_2), array('f', sig_2), array('f', bkg_2))
g2.SetTitle("")
g2.SetMarkerStyle(24)
#g2.SetMarkerColor(ROOT.kViolet)
g2.SetMarkerSize(1)


g3 = ROOT.TGraph(len(sig_3), array('f', sig_3), array('f', bkg_3))
g3.SetTitle("")
g3.SetMarkerStyle(24)
#g3.SetMarkerColor(ROOT.kAzure)
g3.SetMarkerSize(1)

g4 = ROOT.TGraph(len(sig_4), array('f', sig_4), array('f', bkg_4))
g4.SetTitle("")
g4.SetMarkerStyle(24)
#g4.SetMarkerColor(ROOT.kGreen)
g4.SetMarkerSize(1)

g5 = ROOT.TGraph(len(sig_5), array('f', sig_5), array('f', bkg_5))
g5.SetTitle("")
g5.SetMarkerStyle(24)
#g5.SetMarkerColor(ROOT.kBlack)
g5.SetMarkerSize(1)

g6 = ROOT.TGraph(len(sig_6), array('f', sig_6), array('f', bkg_6))
g6.SetTitle("")
g6.SetMarkerStyle(24)
#g6.SetMarkerColor(ROOT.kRed)
g6.SetMarkerSize(1)

g7 = ROOT.TGraph(len(sig_7), array('f', sig_7), array('f', bkg_7))
g7.SetTitle("")
g7.SetMarkerStyle(24)
#g7.SetMarkerColor(ROOT.kRed)
g7.SetMarkerSize(1)

g8 = ROOT.TGraph(len(sig_8), array('f', sig_8), array('f', bkg_8))
g8.SetTitle("")
g8.SetMarkerStyle(24)
#g8.SetMarkerColor(ROOT.kYellow)
g8.SetMarkerSize(1)

g.GetXaxis().SetLimits(0.21, .5)
g.GetHistogram().SetMaximum(18.)
g.GetHistogram().SetMinimum(1)
g.SetTitle("")
g.GetXaxis().SetTitle("Signal Acceptance")
g.GetYaxis().SetTitle("Rate (Hz)")

ROOT.gStyle.SetPalette(ROOT.kCool)

mg = ROOT.TMultiGraph()
mg_sub = ROOT.TMultiGraph()
mg.Add(g, "P")
mg.Add(g1, "P")
mg.Add(g2, "P")
mg.Add(g3, "P")
mg.Add(g4, "P")
mg.Add(g5, "P")
mg.Add(g6, "P")
mg.Add(g7, "P")
mg.Add(g8, "P")

mg_sub.Add(g, "P")
mg_sub.Add(g1, "P")
mg_sub.Add(g2, "P")
mg_sub.Add(g3, "P")
mg_sub.Add(g4, "P")
mg_sub.Add(g5, "P")
mg_sub.Add(g6, "P")
mg_sub.Add(g7, "P")
mg_sub.Add(g8, "P")

mg.GetXaxis().SetLimits(0.21, 0.5)
mg.GetHistogram().SetMaximum(30.)
mg.GetHistogram().SetMinimum(1)
mg.SetTitle("")
mg.GetXaxis().SetTitle("Signal Acceptance")
mg.GetYaxis().SetTitle("Rate (Hz)")

mg_sub.GetXaxis().SetLimits(0.21, 0.28)
mg_sub.GetHistogram().SetMaximum(10.)
mg_sub.GetHistogram().SetMinimum(1)
mg_sub.SetTitle("")

leg = ROOT.TLegend(0.7, 0.89, 0.89, 0.6)
leg.AddEntry(glegend, "2018 Default", "P")
leg.AddEntry(g1, "9 Filters", "P")
leg.AddEntry(g2, "8 Filters", "P")
leg.AddEntry(g3, "7 Filters", "P")
leg.AddEntry(g4, "6 Filters", "P")
leg.AddEntry(g5, "5 Filters", "P")
leg.AddEntry(g6, "4 Filters", "P")
leg.AddEntry(g7, "3 Filters", "P")
leg.AddEntry(g8, "2 Filters", "P")
leg.SetBorderSize(0)

c = ROOT.TCanvas("c", "c", 1000, 1000, 1000, 700)
g.Draw("AP")
mg.Draw("pmc plc same")
T = ROOT.TLatex()
T1 = ROOT.TLatex()
leg.Draw()
T.DrawLatexNDC(.12, .92, "#scale[0.8]{#font[52]{ggF#rightarrow HH #rightarrow bbbb }}")
T1.DrawLatexNDC(.55, .92, "#scale[0.8]{#font[52]{323725 Lumi [37:57] PU > 50}}")

subpad = ROOT.TPad("subpad","",0.14,0.5,0.5,0.89)
subpad.Draw()
subpad.cd()
mg_sub.Draw('pmc plc same')
c.cd()

c.Draw()
c.SaveAs("LeaveSomeFiltersOutZoom.pdf")
