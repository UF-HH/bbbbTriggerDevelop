import pandas as pd
import matplotlib.pyplot as plt
import ROOT
from array import array

"""
    This type of horrible scripts (originally a IPy notebook) work with the .csv
    generated from the Rate.cpp script and relative .py
"""
def SliceOnStandard(df, defcols):

    filtersdef = {'hltQuadCentralJet30':[30, 1e99, -2.5, 2.5, 4],
                  'hltCaloQuadJet30HT320': [30, 1e99, -2.5, 2.5, 320, 0., 1],
                  'hltBTagCaloDeepCSVp17Double':[30, 1e99, -2.4, 2.4, 0.17, 2],
                  'hltPFCentralJetLooseIDQuad30': [30, 1e99, -2.5, 2.5, 4],
                  'hlt1PFCentralJetLooseID75':[75, 1e99, -2.5, 2.5, 1],
                  'hlt2PFCentralJetLooseID60':[60, 1e99, -2.5, 2.5, 2],
                  'hlt3PFCentralJetLooseID45': [45, 1e99, -2.5, 2.5, 3],
                  'hlt4PFCentralJetLooseID40':[40, 1e99, -2.5, 2.5, 4],
                  'hltPFCentralJetsLooseIDQuad30HT330':[30, 1e99, -2.5, 2.5, 330, 0., 1],
                  'hltBTagPFDeepCSV4p5Triple':[30, 1e99, -2.6, 2.6, 0.24, 3]
                 }

    for fil in filtersdef:
        if fil not in defcols:
            for action,value in zip(df[fil].columns[1:], filtersdef[fil]):
                if action != "setPtMax":
                    df = df[df[fil][action] == value]
            
    return df
    
    

pf = pd.read_csv("../RateVsSig/pfbtag.csv", index_col=[0], header=[0,1])
calo = pd.read_csv("../RateVsSig/calobtag.csv", index_col=[0], header=[0,1])

t_pf = pf[pf["hltBTagPFDeepCSV4p5Triple"]["setMinN"] == 3]
d_pf = pf[pf["hltBTagPFDeepCSV4p5Triple"]["setMinN"] == 2]
t_calo = calo[calo["hltBTagCaloDeepCSVp17Double"]["setMinN"] == 3]
d_calo = calo[calo["hltBTagCaloDeepCSVp17Double"]["setMinN"] == 2]

totbkg = d_pf["totbkg"]["count"].values[0]
totsig = d_pf["totsig"]["count"].values[0]

sig_pf_2b = d_pf["countsig"]["count"].values
bkg_pf_2b = d_pf["countbkg"]["count"].values
sig_pf_3b = t_pf["countsig"]["count"].values
bkg_pf_3b = t_pf["countbkg"]["count"].values

sig_calo_2b = d_calo["countsig"]["count"].values
bkg_calo_2b = d_calo["countbkg"]["count"].values
sig_calo_3b = t_calo["countsig"]["count"].values
bkg_calo_3b = t_calo["countbkg"]["count"].values

rate_pf_2b = []
siga_pf_2b = []
rate_pf_3b = []
siga_pf_3b = []
rate_calo_2b = []
siga_calo_2b = []
rate_calo_3b = []
siga_calo_3b = []

for i,j in zip(sig_pf_2b, bkg_pf_2b):
    rate_pf_2b.append(1100*float(j)/totbkg *31.40)
    siga_pf_2b.append(float(i)/totsig)
    
for i,j in zip(sig_pf_3b, bkg_pf_3b):
    rate_pf_3b.append(1100*float(j)/totbkg *31.40)
    siga_pf_3b.append(float(i)/totsig)
    
for i,j in zip(sig_calo_2b, bkg_calo_2b):
    rate_calo_2b.append(1100*float(j)/totbkg *31.40)
    siga_calo_2b.append(float(i)/totsig)
    
for i,j in zip(sig_calo_3b, bkg_calo_3b):
    rate_calo_3b.append(1100*float(j)/totbkg *31.40)
    siga_calo_3b.append(float(i)/totsig)
    
g2 = ROOT.TGraph(len(siga_pf_2b), array('f', siga_pf_2b), array('f', rate_pf_2b))
g3 = ROOT.TGraph(len(siga_pf_3b), array('f', siga_pf_3b), array('f', rate_pf_3b))
g4 = ROOT.TGraph(len(siga_calo_2b), array('f', siga_calo_2b), array('f', rate_calo_2b))
g5 = ROOT.TGraph(len(siga_calo_3b), array('f', siga_calo_3b), array('f', rate_calo_3b))

m = ROOT.TMarker(0.22594, 3.69, 34)
m.SetMarkerSize(2)
m.SetMarkerColor(ROOT.kBlack)

leg = ROOT.TLegend(0.11, 0.89, 0.35, 0.7)
leg.AddEntry(m, "2018 Default", "P")
leg.AddEntry(g2, "PF two bTag", "P")
leg.AddEntry(g3, "PF three bTag", "P")
leg.AddEntry(g4, "Calo two bTag", "P")
leg.AddEntry(g5, "Calo three bTag", "P")
leg.SetBorderSize(0)

g5.SetMarkerStyle(21)
g5.SetMarkerColor(ROOT.kMagenta)
g5.SetMarkerSize(1)

g4.SetMarkerStyle(21)
g4.SetMarkerColor(ROOT.kAzure-2)
g4.SetMarkerSize(1)

g3.SetMarkerStyle(24)
g3.SetMarkerColor(ROOT.kViolet-5)
g3.SetMarkerSize(1)

g2.SetMarkerStyle(24)
g2.SetMarkerColor(ROOT.kBlue)
g2.SetMarkerSize(1)

g2.GetXaxis().SetLimits(0.2, 0.31)
g2.GetHistogram().SetMaximum(20.)
g2.GetHistogram().SetMinimum(1)

g3.GetXaxis().SetLimits(0.2, 0.31)
g3.GetHistogram().SetMaximum(20.)
g3.GetHistogram().SetMinimum(1)

g2.SetTitle("")
g2.GetXaxis().SetTitle("Signal Acceptance")
g2.GetYaxis().SetTitle("Rate (Hz)")

maxi = ROOT.TLine(0.2, 10, 0.31, 10)
maxi.SetLineColor(ROOT.kGray)
maxi.SetLineStyle(ROOT.kDashed)
maxi.SetLineWidth(3)

mini = ROOT.TLine(0.2, 4, 0.31, 4)
mini.SetLineColor(ROOT.kGray)
mini.SetLineStyle(ROOT.kDashed)
mini.SetLineWidth(3)

c = ROOT.TCanvas("c", "c", 1000, 1000, 1000, 700)
g2.Draw("AP")
g3.Draw("P same")
g4.Draw("P same")
g5.Draw("P same")
leg.Draw("same")
T = ROOT.TLatex()
maxi.Draw("same")
mini.Draw("same")
m.Draw("same")
T1 = ROOT.TLatex()
T.DrawLatexNDC(.12, .92, "#scale[0.8]{#font[52]{ggF#rightarrow HH #rightarrow bbbb }}")
T1.DrawLatexNDC(.55, .92, "#scale[0.8]{#font[52]{323725 Lumi [37:57] PU > 50}}")
c.Draw()
c.SaveAs("23calo23pf.pdf")
