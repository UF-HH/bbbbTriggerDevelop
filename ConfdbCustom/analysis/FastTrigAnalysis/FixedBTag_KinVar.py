import pandas as pd
import matplotlib.pyplot as plt
import ROOT
from array import array

df = pd.read_csv("../RateVsSig/CaloRangeAndbtagTh.csv", index_col=[0], header=[0,1])

df1 = df[df['hltBTagPFDeepCSV4p5Triple']['setBTagMin'] == 0.1]
df2 = df[df['hltBTagPFDeepCSV4p5Triple']['setBTagMin'] == 0.15]
df3 = df[df['hltBTagPFDeepCSV4p5Triple']['setBTagMin'] == 0.2]
df4 = df[df['hltBTagPFDeepCSV4p5Triple']['setBTagMin'] == 0.22]
df5 = df[df['hltBTagPFDeepCSV4p5Triple']['setBTagMin'] == 0.25]
df6 = df[df['hltBTagPFDeepCSV4p5Triple']['setBTagMin'] == 0.30]
df7 = df[df['hltBTagPFDeepCSV4p5Triple']['setBTagMin'] == 0.32]

totbkg = df["totbkg"]["count"].values[0]
totsig = df["totsig"]["count"].values[0]

sig_01 = df1["countsig"]["count"].values
bkg_01 = df1["countbkg"]["count"].values

sig_015 = df2["countsig"]["count"].values
bkg_015 = df2["countbkg"]["count"].values

sig_02 = df3["countsig"]["count"].values
bkg_02 = df3["countbkg"]["count"].values

sig_022 = df4["countsig"]["count"].values
bkg_022 = df4["countbkg"]["count"].values

sig_025 = df5["countsig"]["count"].values
bkg_025 = df5["countbkg"]["count"].values

sig_03 = df6["countsig"]["count"].values
bkg_03 = df6["countbkg"]["count"].values

sig_032 = df7["countsig"]["count"].values
bkg_032 = df7["countbkg"]["count"].values



rate_01 = []
siga_01 = []

rate_015 = []
siga_015 = []

rate_02 = []
siga_02 = []

rate_022 = []
siga_022 = []

rate_025 = []
siga_025 = []

rate_03 = []
siga_03 = []

rate_032 = []
siga_032 = []


for i,j in zip(sig_01, bkg_01):
    rate_01.append(1100*float(j)/totbkg *31.40)
    siga_01.append(float(i)/totsig)
    
for i,j in zip(sig_015, bkg_015):
    rate_015.append(1100*float(j)/totbkg *31.40)
    siga_015.append(float(i)/totsig)
    
for i,j in zip(sig_02, bkg_02):
    rate_02.append(1100*float(j)/totbkg *31.40)
    siga_02.append(float(i)/totsig)
    
for i,j in zip(sig_022, bkg_022):
    rate_022.append(1100*float(j)/totbkg *31.40)
    siga_022.append(float(i)/totsig)
    
for i,j in zip(sig_025, bkg_025):
    rate_025.append(1100*float(j)/totbkg *31.40)
    siga_025.append(float(i)/totsig)
    
for i,j in zip(sig_03, bkg_03):
    rate_03.append(1100*float(j)/totbkg *31.40)
    siga_03.append(float(i)/totsig)
    
for i,j in zip(sig_032, bkg_032):
    rate_032.append(1100*float(j)/totbkg *31.40)
    siga_032.append(float(i)/totsig)


g = ROOT.TGraph(len(siga_01), array('f', siga_01), array('f', rate_01))
g.SetMarkerStyle(24)
g.SetMarkerColor(ROOT.kGreen-1)
g.SetMarkerSize(1)

g.SetTitle("")
g.GetXaxis().SetTitle("Signal Acceptance")
g.GetYaxis().SetTitle("Rate (Hz)")
g.GetXaxis().SetLimits(0.18, 0.3)
g.GetHistogram().SetMaximum(40.)
g.GetHistogram().SetMinimum(1)

g1 = ROOT.TGraph(len(siga_015), array('f', siga_015), array('f', rate_015))
g1.SetMarkerStyle(24)
g1.SetMarkerColor(ROOT.kBlue)
g1.SetMarkerSize(1)

g2 = ROOT.TGraph(len(siga_02), array('f', siga_02), array('f', rate_02))
g2.SetMarkerStyle(24)
g2.SetMarkerColor(ROOT.kRed)
g2.SetMarkerSize(1)

g3 = ROOT.TGraph(len(siga_022), array('f', siga_022), array('f', rate_022))
g3.SetMarkerStyle(24)
g3.SetMarkerColor(ROOT.kGreen)
g3.SetMarkerSize(1)

g4 = ROOT.TGraph(len(siga_025), array('f', siga_025), array('f', rate_025))
g4.SetMarkerStyle(24)
g4.SetMarkerColor(ROOT.kMagenta)
g4.SetMarkerSize(1)

g5 = ROOT.TGraph(len(siga_03), array('f', siga_03), array('f', rate_03))
g5.SetMarkerStyle(24)
g5.SetMarkerColor(ROOT.kAzure-4)
g5.SetMarkerSize(1)

g6 = ROOT.TGraph(len(siga_032), array('f', siga_032), array('f', rate_032))
g6.SetMarkerStyle(24)
g6.SetMarkerColor(ROOT.kViolet+1)
g6.SetMarkerSize(1)

m = ROOT.TMarker(0.22594, 3.69, 34)
m.SetMarkerSize(2)
m.SetMarkerColor(ROOT.kBlack)

leg = ROOT.TLegend(0.11, 0.89, 0.5, 0.6)
leg.AddEntry(m, "2018 Default", "P")
leg.AddEntry(g, "PF BTag : 0.1", "P")
leg.AddEntry(g1, "PF BTag : 0.15", "P")
leg.AddEntry(g2, "PF BTag : 0.2", "P")
leg.AddEntry(g3, "PF BTag : 0.22", "P")
leg.AddEntry(g4, "PF BTag : 0.25", "P")
leg.AddEntry(g5, "PF BTag : 0.3", "P")
leg.AddEntry(g6, "PF BTag : 0.32", "P")
leg.SetBorderSize(0)

c = ROOT.TCanvas("c", "c", 1000, 1000, 1000, 700)
g.Draw("AP")
g1.Draw("P same")
g2.Draw("P same")
g3.Draw("P same")
g4.Draw("P same")
g5.Draw("P same")
g6.Draw("P same")
m.Draw("same")
leg.Draw("same")

T = ROOT.TLatex()
T1 = ROOT.TLatex()
T.DrawLatexNDC(.12, .92, "#scale[0.8]{#font[52]{ggF#rightarrow HH #rightarrow bbbb }}")
T1.DrawLatexNDC(.55, .92, "#scale[0.8]{#font[52]{323725 Lumi [37:57] PU > 50}}")
c.Draw()
c.SaveAs("ALLPF_CaloQuad_CaloHT.pdf")
