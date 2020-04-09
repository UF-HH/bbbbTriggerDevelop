import json
import sys
import os
from array import array
import subprocess
from tqdm import tqdm
import argparse
import matplotlib
matplotlib.use('Agg')
import matplotlib.pyplot as plt
import ROOT
import numpy as np

def Rate(accepted):
    accepted = float(accepted)
    Nstreams = 8.
    NLumis = 21.
    PSHLT = 1100.
    TLs = 23.31041
    Lumidel = 1.634e34
    LumiExp = 2.e34

    return PSHLT/Nstreams * accepted/(NLumis*TLs) * LumiExp/Lumidel

def RateProportion(tot, accepted):
    RateHLT = 31.40 #specific for each run
    PSHLT = 1100.
    return PSHLT*float(accepted)/float(tot) * RateHLT




#Plotting the results 

ratebkg3b = []
with open("RateBkg3b.txt") as ratefile:
    line = ratefile.readline()
    while(line):
        line = line.split(",")
        for sec in line:
            if "Tot" in sec:
                tot = sec.split(":")[1]

            if "Accepted" in sec:
                acc = sec.split(":")[1]

        ratebkg3b.append(RateProportion(tot, acc))
        line = ratefile.readline()

ratesig3b = []
with open("RateSig3b.txt") as ratefile:
    line = ratefile.readline()
    while(line):
        line = line.split(",")
        for sec in line:
            if "Tot" in sec:
                tot = sec.split(":")[1]

            if "Accepted" in sec:
                acc = sec.split(":")[1]

        ratesig3b.append(float(acc)/float(tot))
        line = ratefile.readline()

ratebkg2b = []
with open("RateBkg2b.txt") as ratefile:
    line = ratefile.readline()
    while(line):
        line = line.split(",")
        for sec in line:
            if "Tot" in sec:
                tot = sec.split(":")[1]

            if "Accepted" in sec:
                acc = sec.split(":")[1]

        ratebkg2b.append(RateProportion(tot, acc))
        line = ratefile.readline()

ratesig2b = []
with open("RateSig2b.txt") as ratefile:
    line = ratefile.readline()
    while(line):
        line = line.split(",")
        for sec in line:
            if "Tot" in sec:
                tot = sec.split(":")[1]

            if "Accepted" in sec:
                acc = sec.split(":")[1]

        ratesig2b.append(float(acc)/float(tot))
        line = ratefile.readline()

btagth = np.arange(0,1, 0.01)

c = ROOT.TCanvas("c", "c", 1000, 1000, 1000, 700)
leg = ROOT.TLegend(0.15, 0.89, 0.4, 0.7)
leg.SetBorderSize(0)
g2 = ROOT.TGraph(len(ratesig2b), array('f',ratesig2b), array('f',ratebkg2b))
g3 = ROOT.TGraph(len(ratesig3b), array('f',ratesig3b), array('f',ratebkg3b))
leg.AddEntry(g2, "hltBTagPFDeepCSV4p5Double", "P")
leg.AddEntry(g3, "hltBTagPFDeepCSV4p5Triple", "P")
g3.SetMarkerStyle(8)
g3.SetMarkerColor(ROOT.kBlue)
g3.GetXaxis().SetTitle("Signal Acceptance")
g3.GetYaxis().SetTitle("Rate Background (Hz)")
g3.SetTitle("")
g2.SetMarkerStyle(8)
g2.SetMarkerColor(ROOT.kMagenta)

g3.GetXaxis().SetRangeUser(0.15, 0.33)
g3.Draw("AP")
g2.Draw("P same")
leg.Draw()
T = ROOT.TLatex()
T.DrawLatexNDC(.1, .92, "#scale[0.8]{#font[52]{ggF#rightarrow HH #rightarrow bbbb }}")
T1 = ROOT.TLatex()
T1.DrawLatexNDC(.55, .92, "#scale[0.8]{#font[52]{323725 Lumi [37:57] PU > 50}}")
c.Draw()
c.SaveAs("PFBTagDoubleTripleScan.pdf")

c1 = ROOT.TCanvas("c1", "c1", 1000, 1000, 1000, 700)
leg1 = ROOT.TLegend(0.85, 0.89, 0.65, 0.7)
leg1.SetBorderSize(0)
g21 = ROOT.TGraph(len(btagth), array('f',btagth), array('f',ratebkg2b))
g31 = ROOT.TGraph(len(btagth), array('f',btagth), array('f',ratebkg3b))
leg1.AddEntry(g2, "hltBTagPFDeepCSV4p5Double", "P")
leg1.AddEntry(g3, "hltBTagPFDeepCSV4p5Triple", "P")
g31.SetMarkerStyle(8)
g31.SetMarkerColor(ROOT.kBlue)
g31.GetXaxis().SetTitle("Threshold bTag")
g31.GetYaxis().SetTitle("Rate Background (Hz)")
g31.SetTitle("")
g21.SetMarkerStyle(8)
g21.SetMarkerColor(ROOT.kMagenta)

g31.Draw("AP")
g21.Draw("P same")
leg1.Draw()
T.DrawLatexNDC(.1, .92, "#scale[0.8]{#font[52]{ggF#rightarrow HH #rightarrow bbbb }}")
T1.DrawLatexNDC(.55, .92, "#scale[0.8]{#font[52]{323725 Lumi [37:57] PU > 50}}")
c1.Draw()
c1.SaveAs("PFBTagDoubleTripleScanTh.pdf")

c2 = ROOT.TCanvas("c2", "c2", 1000, 1000, 1000, 700)
leg2 = ROOT.TLegend(0.85, 0.89, 0.65, 0.7)
leg2.SetBorderSize(0)
g22 = ROOT.TGraph(len(btagth), array('f',btagth), array('f',ratesig2b))
g32 = ROOT.TGraph(len(btagth), array('f',btagth), array('f',ratesig3b))
leg2.AddEntry(g2, "hltBTagPFDeepCSV4p5Double", "P")
leg2.AddEntry(g3, "hltBTagPFDeepCSV4p5Triple", "P")
g32.SetMarkerStyle(8)
g32.SetMarkerColor(ROOT.kBlue)
g32.GetXaxis().SetTitle("Threshold bTag")
g32.GetYaxis().SetTitle("Signal Acceptance (Hz)")
g32.SetTitle("")
g22.SetMarkerStyle(8)
g22.SetMarkerColor(ROOT.kMagenta)

g32.Draw("AP")
g22.Draw("P same")
leg2.Draw()
T.DrawLatexNDC(.1, .92, "#scale[0.8]{#font[52]{ggF#rightarrow HH #rightarrow bbbb }}")
T1.DrawLatexNDC(.55, .92, "#scale[0.8]{#font[52]{323725 Lumi [37:57] PU > 50}}")
c2.Draw()
c2.SaveAs("PFBTagDoubleTripleScanThSig.pdf")


