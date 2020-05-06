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
    
    

df2 = pd.read_csv("BtagsCombo.csv", index_col=[0], header=[0,1])

twob_calo = df2[df2["hltBTagCaloDeepCSVp17Double"]["setMinN"] == 2]
twobcalo_2bpf = twob_calo[twob_calo["hltBTagPFDeepCSV4p5Triple"]["setMinN"] == 2]

threeb_calo = df2[df2["hltBTagCaloDeepCSVp17Double"]["setMinN"] == 3]
threebcalo_2bpf = threeb_calo[threeb_calo["hltBTagPFDeepCSV4p5Triple"]["setMinN"] == 2]

twob_calo = df2[df2["hltBTagCaloDeepCSVp17Double"]["setMinN"] == 2]
twobcalo_3bpf = twob_calo[twob_calo["hltBTagPFDeepCSV4p5Triple"]["setMinN"] == 3]

threeb_calo = df2[df2["hltBTagCaloDeepCSVp17Double"]["setMinN"] == 3]
threebcalo_3bpf = threeb_calo[threeb_calo["hltBTagPFDeepCSV4p5Triple"]["setMinN"] == 3]

totbkg = twobcalo_2bpf["totbkg"]["count"].values[0]
totsig = twobcalo_2bpf["totsig"]["count"].values[0]

bkg_2b_2b = twobcalo_2bpf["countbkg"]["count"].values
sig_2b_2b = twobcalo_2bpf["countsig"]["count"].values

bkg_3b_2b = threebcalo_2bpf["countbkg"]["count"].values
sig_3b_2b = threebcalo_2bpf["countsig"]["count"].values

bkg_2b_3b = twobcalo_3bpf["countbkg"]["count"].values
sig_2b_3b = twobcalo_3bpf["countsig"]["count"].values

bkg_3b_3b = threebcalo_3bpf["countbkg"]["count"].values
sig_3b_3b = threebcalo_3bpf["countsig"]["count"].values


ratebkg_2b_2b = []
sigacc_2b_2b = []
ratebkg_3b_2b = []
sigacc_3b_2b = []
ratebkg_2b_3b = []
sigacc_2b_3b = []
ratebkg_3b_3b = []
sigacc_3b_3b = []

for i,j in zip(sig_2b_2b, bkg_2b_2b):
    ratebkg_2b_2b.append(1100*float(j)/totbkg *31.40)
    sigacc_2b_2b.append(float(i)/totsig)
    
for i,j in zip(sig_2b_3b, bkg_2b_3b):
    ratebkg_2b_3b.append(1100*float(j)/totbkg *31.40)
    sigacc_2b_3b.append(float(i)/totsig)
    
for i,j in zip(sig_3b_2b, bkg_3b_2b):
    ratebkg_3b_2b.append(1100*float(j)/totbkg *31.40)
    sigacc_3b_2b.append(float(i)/totsig)
    
for i,j in zip(sig_3b_3b, bkg_3b_3b):
    ratebkg_3b_3b.append(1100*float(j)/totbkg *31.40)
    sigacc_3b_3b.append(float(i)/totsig)
    
g = ROOT.TGraph(len(sigacc_2b_2b), array('f', sigacc_2b_2b), array('f', ratebkg_2b_2b))
g.SetTitle("")
g.SetMarkerStyle(21)
g.SetMarkerColor(ROOT.kMagenta)
g.SetMarkerSize(1)

g1 = ROOT.TGraph(len(sigacc_3b_2b), array('f', sigacc_3b_2b), array('f', ratebkg_3b_2b))
g1.SetTitle("")
g1.SetMarkerStyle(21)
g1.SetMarkerColor(ROOT.kAzure-2)
g1.SetMarkerSize(1)

g2 = ROOT.TGraph(len(sigacc_2b_3b), array('f', sigacc_2b_3b), array('f', ratebkg_2b_3b))
g2.SetTitle("")
g2.SetMarkerStyle(24)
g2.SetMarkerColor(ROOT.kViolet-5)
g2.SetMarkerSize(1)

g3 = ROOT.TGraph(len(sigacc_3b_3b), array('f', sigacc_3b_3b), array('f', ratebkg_3b_3b))
g3.SetTitle("")
g3.SetMarkerStyle(24)
g3.SetMarkerColor(ROOT.kBlue)
g3.SetMarkerSize(1)

g.GetXaxis().SetLimits(0.1, 0.33)
g.GetHistogram().SetMaximum(20.)
g.GetHistogram().SetMinimum(1)
g.GetXaxis().SetTitle("Signal Acceptance")
g.GetYaxis().SetTitle("Rate (Hz)")

maxi = ROOT.TLine(0.1, 10, 0.33, 10)
maxi.SetLineColor(ROOT.kGray)
maxi.SetLineStyle(ROOT.kDashed)
maxi.SetLineWidth(3)

mini = ROOT.TLine(0.1, 4, 0.33, 4)
mini.SetLineColor(ROOT.kGray)
mini.SetLineStyle(ROOT.kDashed)
mini.SetLineWidth(3)

m = ROOT.TMarker(0.22594, 3.69, 34)
m.SetMarkerSize(2)
m.SetMarkerColor(ROOT.kBlack)

leg = ROOT.TLegend(0.11, 0.89, 0.38, 0.7)
leg.AddEntry(m, "2018 Default", "P")
leg.AddEntry(g, "2b Calo, 2b PF ", "P")
leg.AddEntry(g1, "3b Calo, 2b PF", "P")
leg.AddEntry(g2, "2b Calo, 3b PF", "P")
leg.AddEntry(g3, "3b Calo, 3b PF", "P")
leg.SetBorderSize(0)

c = ROOT.TCanvas("c", "c", 1000, 1000, 1000, 700)
g.Draw("AP")
g1.Draw("P same")
g2.Draw("P same")
g3.Draw("P same")
m.Draw("same")
leg.Draw()
T = ROOT.TLatex()
T1 = ROOT.TLatex()
maxi.Draw("same")
mini.Draw("same")
T.DrawLatexNDC(.12, .92, "#scale[0.8]{#font[52]{ggF#rightarrow HH #rightarrow bbbb }}")
T1.DrawLatexNDC(.55, .92, "#scale[0.8]{#font[52]{323725 Lumi [37:57] PU > 50}}")
c.Draw()
c.SaveAs("23CaloPFTh_20.pdf")
