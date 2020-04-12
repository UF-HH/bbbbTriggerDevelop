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

ratebkg = []
with open("RateBkg.txt") as ratefile:
    line = ratefile.readline()
    while(line):
        line = line.split(",")
        for sec in line:
            if "Tot" in sec:
                tot = sec.split(":")[1]

            if "Accepted" in sec:
                acc = sec.split(":")[1]
        
        ratebkg.append(RateProportion(tot, acc))
        line = ratefile.readline()

ratesig = []
with open("RateSig.txt") as ratefile:
    line = ratefile.readline()
    while(line):
        line = line.split(",")
        for sec in line:
            if "Tot" in sec:
                tot = sec.split(":")[1]

            if "Accepted" in sec:
                acc = sec.split(":")[1]

        ratesig.append(float(acc)/float(tot))
        line = ratefile.readline()


print(len(ratesig))
c = ROOT.TCanvas("c", "c", 1000, 1000, 1000, 700)
leg = ROOT.TLegend(0.85, 0.4, 0.65, 0.18)
leg.SetBorderSize(0)
g = ROOT.TGraph(len(ratesig), array('f',ratesig), array('f',ratebkg))
leg.AddEntry(g, "323725 Lumi [37:57]", "P")
g.SetMarkerStyle(8)
g.SetMarkerColor(ROOT.kBlack)
g.GetXaxis().SetTitle("Signal Acceptance")
g.GetYaxis().SetTitle("Rate Background (Hz)")
g.SetTitle("")

g.Draw("AP")
leg.Draw()
T = ROOT.TLatex()
T.DrawLatexNDC(.1, .92, "#scale[0.8]{#font[52]{ggF#rightarrow HH #rightarrow bbbb }}")
T1 = ROOT.TLatex()
T1.DrawLatexNDC(.55, .92, "#scale[0.]{#font[52]{hltBTagPFDeepCSV4p5Triple}}")
c.Draw()
c.SaveAs("btagcalo_threshold.pdf")


