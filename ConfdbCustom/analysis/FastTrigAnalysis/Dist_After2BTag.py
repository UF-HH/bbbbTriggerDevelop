import ROOT
import numpy as np
import pandas
import matplotlib.pyplot as plt
import math as mt
from sklearn.metrics import auc
from tqdm import tqdm

from TrigFilters import Trig2018Filters, Trig2DFilters, SumBTag

def GetCanvas():
    c = ROOT.TCanvas("c", "c", 2200, 2200, 2200, 1000)
    return c


f = ROOT.TFile("../ggMCRAW_GenJets.root")
tree_sig = f.Get("SaveAllJets/Jets")

f_bkg = ROOT.TFile("../323725_37_57_PU50.root")
tree_bkg = f_bkg.Get("SaveAllJets/Jets")

#Example triggers based on sum btag scores

count_sig = 0
count_bkg = 0


for event in tree_sig:
    calo_btag = event.calo_btag
    calo_pt = event.calo_pt
    calo_eta = event.calo_eta
    pf_btag = event.pf_btag
    pf_pt = event.pf_pt
    pf_eta = event.pf_eta
    pf_phi = event.pf_phi
    
    if(SumBTag.SumBTag(pf_btag) and Trig2018Filters.third(calo_pt, calo_eta, calo_btag) and Trig2018Filters.sixth(pf_pt, pf_eta) and Trig2018Filters.tenth(pf_pt, pf_eta, pf_btag)): count_sig+=1

    
for event in tree_bkg:
    calo_btag = event.calo_btag
    calo_pt = event.calo_pt
    calo_eta = event.calo_eta
    pf_btag = event.pf_btag
    pf_pt = event.pf_pt
    pf_eta = event.pf_eta
    pf_phi = event.pf_phi
    
    if(SumBTag.SumBTag(calo_btag) and Trig2018Filters.third(calo_pt, calo_eta, calo_btag) and Trig2018Filters.sixth(pf_pt, pf_eta) and Trig2018Filters.tenth(pf_pt, pf_eta, pf_btag)): count_bkg+=1
        
print(count_sig, count_bkg)

#Only 2btag sum leading
count_sig = 0
count_bkg = 0


for event in tree_sig:
    calo_btag = event.calo_btag
    calo_pt = event.calo_pt
    calo_eta = event.calo_eta
    pf_btag = event.pf_btag
    pf_pt = event.pf_pt
    pf_eta = event.pf_eta
    pf_phi = event.pf_phi
    
    if(SumBTag.SumBTag(calo_btag)) : count_sig+=1

    
for event in tree_bkg:
    calo_btag = event.calo_btag
    calo_pt = event.calo_pt
    calo_eta = event.calo_eta
    pf_btag = event.pf_btag
    pf_pt = event.pf_pt
    pf_eta = event.pf_eta
    pf_phi = event.pf_phi

    if(SumBTag.SumBTag(calo_btag))  : count_bkg+=1
       
print(count_sig, count_bkg)

#3 btag sum

count_sig = 0
count_bkg = 0


for event in tree_sig:
    calo_btag = event.calo_btag
    calo_pt = event.calo_pt
    calo_eta = event.calo_eta
    pf_btag = event.pf_btag
    pf_pt = event.pf_pt
    pf_eta = event.pf_eta
    pf_phi = event.pf_phi
    
    if(SumBTag.SumBTag3(pf_btag)) : count_sig+=1

    
for event in tree_bkg:
    calo_btag = event.calo_btag
    calo_pt = event.calo_pt
    calo_eta = event.calo_eta
    pf_btag = event.pf_btag
    pf_pt = event.pf_pt
    pf_eta = event.pf_eta
    pf_phi = event.pf_phi

    if(SumBTag.SumBTag3(pf_btag))  : count_bkg+=1
       
print(count_sig, count_bkg)

#sum 2 btag

count_sig = 0
count_bkg = 0


for event in tree_sig:
    calo_btag = event.calo_btag
    calo_pt = event.calo_pt
    calo_eta = event.calo_eta
    pf_btag = event.pf_btag
    pf_pt = event.pf_pt
    pf_eta = event.pf_eta
    pf_phi = event.pf_phi
    
    if(SumBTag.SumBTag2(calo_btag)) : count_sig+=1

    
for event in tree_bkg:
    calo_btag = event.calo_btag
    calo_pt = event.calo_pt
    calo_eta = event.calo_eta
    pf_btag = event.pf_btag
    pf_pt = event.pf_pt
    pf_eta = event.pf_eta
    pf_phi = event.pf_phi

    if(SumBTag.SumBTag2(pf_btag))  : count_bkg+=1
       
print(count_sig, count_bkg)


#plotting dist after sum of only 2 btag

sig_fourth_calo = ROOT.TH1F("sig_fourth_calo", "sig_fourth_calo", 80, 0, 200)
sig_fourth_calo.SetLineColor(ROOT.kMagenta)
sig_fourth_calo.SetLineWidth(1)
sig_ht_calo = ROOT.TH1F("sig_ht_calo", "sig_ht_calo", 80, 0, 1000)
sig_ht_calo.SetLineColor(ROOT.kMagenta)
sig_ht_calo.SetLineWidth(1)
sig_secondb_calo = ROOT.TH1F("sig_secondb_calo", "sig_secondb_calo", 80, 0, 1)
sig_secondb_calo.SetLineColor(ROOT.kMagenta)
sig_secondb_calo.SetLineWidth(1)

sig_leading = ROOT.TH1F("sig_lead", "sig_lead", 80, 0, 1000)
sig_leading.SetLineColor(ROOT.kMagenta)
sig_leading.SetLineWidth(1)
sig_second = ROOT.TH1F("sig_second", "sig_second", 100, 0, 700)
sig_second.SetLineColor(ROOT.kMagenta)
sig_second.SetLineWidth(1)
sig_third = ROOT.TH1F("sig_third", "sig_third", 80, 0, 400)
sig_third.SetLineColor(ROOT.kMagenta)
sig_third.SetLineWidth(1)
sig_fourth = ROOT.TH1F("sig_fourth", "sig_fourth", 80, 0, 200)
sig_fourth.SetLineColor(ROOT.kMagenta)
sig_fourth.SetLineWidth(1)
sig_fourth_1 = ROOT.TH1F("sig_fourth_1", "sig_fourth_1", 80, 0, 200)
sig_fourth_1.SetLineColor(ROOT.kMagenta)
sig_fourth_1.SetLineWidth(1)
sig_ht = ROOT.TH1F("sig_ht", "sig_ht", 80, 0, 1000)
sig_ht.SetLineColor(ROOT.kMagenta)
sig_ht.SetLineWidth(1)
sig_thirdb_pf = ROOT.TH1F("sig_thirdb_pf", "sig_thirdb_pf", 80, 0, 1)
sig_thirdb_pf.SetLineColor(ROOT.kMagenta)
sig_thirdb_pf.SetLineWidth(1)

bkg_fourth_calo = ROOT.TH1F("bkg_fourth_calo", "bkg_fourth_calo", 80, 0, 200)
bkg_fourth_calo.SetLineColor(ROOT.kBlue)
bkg_fourth_calo.SetLineWidth(1)
bkg_ht_calo = ROOT.TH1F("bkg_ht_calo", "bkg_ht_calo", 80, 0, 1000)
bkg_ht_calo.SetLineColor(ROOT.kBlue)
bkg_ht_calo.SetLineWidth(1)
bkg_secondb_calo = ROOT.TH1F("bkg_secondb_calo", "bkg_secondb_calo", 80, 0, 1)
bkg_secondb_calo.SetLineColor(ROOT.kBlue)
bkg_secondb_calo.SetLineWidth(1)

bkg_leading = ROOT.TH1F("bkg_lead", "bkg_lead", 80, 0, 1000)
bkg_leading.SetLineColor(ROOT.kBlue)
bkg_leading.SetLineWidth(1)
bkg_second = ROOT.TH1F("bkg_second", "bkg_second", 80, 0, 700)
bkg_second.SetLineColor(ROOT.kBlue)
bkg_second.SetLineWidth(1)
bkg_third = ROOT.TH1F("bkg_third", "bkg_third", 80, 0, 400)
bkg_third.SetLineColor(ROOT.kBlue)
bkg_third.SetLineWidth(1)
bkg_fourth = ROOT.TH1F("bkg_fourth", "bkg_fourth", 80, 0, 200)
bkg_fourth.SetLineColor(ROOT.kBlue)
bkg_fourth.SetLineWidth(1)
bkg_fourth_1 = ROOT.TH1F("bkg_fourth_1", "bkg_fourth_1", 80, 0, 200)
bkg_fourth_1.SetLineColor(ROOT.kBlue)
bkg_fourth_1.SetLineWidth(1)
bkg_ht = ROOT.TH1F("bkg_ht", "bkg_ht", 80, 0, 1000)
bkg_ht.SetLineColor(ROOT.kBlue)
bkg_ht.SetLineWidth(1)
bkg_thirdb_pf = ROOT.TH1F("bkg_thirdb_pf", "bkg_thirdb_pf", 80, 0, 1)
bkg_thirdb_pf.SetLineColor(ROOT.kBlue)
bkg_thirdb_pf.SetLineWidth(1)


for event in tree_sig:
    calo_btag = event.calo_btag
    calo_pt = event.calo_pt
    calo_eta = event.calo_eta
    pf_btag = event.pf_btag
    pf_pt = event.pf_pt
    pf_eta = event.pf_eta
    pf_phi = event.pf_phi
    
    if(SumBTag.SumBTag2(pf_btag)):
        pf_pt = sorted(event.pf_pt)
        calo_pt = sorted(event.calo_pt)
        calo_btag = sorted(event.calo_btag)
        pf_btag = sorted(event.pf_btag)
        sig_fourth_calo.Fill(calo_pt[-4])
        ht = sum(i for i in calo_pt if i > 30)
        sig_ht_calo.Fill(ht)
        sig_secondb_calo.Fill(calo_btag[-2])
        sig_leading.Fill(pf_pt[-1])
        sig_second.Fill(pf_pt[-2])
        sig_third.Fill(pf_pt[-3])
        sig_fourth.Fill(pf_pt[-4])
        ht = sum(i for i in pf_pt if i > 30)
        sig_ht.Fill(ht)
        sig_thirdb_pf.Fill(pf_btag[-3])

    
for event in tree_bkg:
    calo_btag = event.calo_btag
    calo_pt = event.calo_pt
    calo_eta = event.calo_eta
    pf_btag = event.pf_btag
    pf_pt = event.pf_pt
    pf_eta = event.pf_eta
    pf_phi = event.pf_phi
    
    if(SumBTag.SumBTag2(pf_btag)):
        
        pf_pt = sorted(event.pf_pt)
        calo_pt = sorted(event.calo_pt)
        calo_btag = sorted(event.calo_btag)
        pf_btag = sorted(event.pf_btag)
        bkg_fourth_calo.Fill(calo_pt[-4])
        ht = sum(i for i in calo_pt if i > 30)
        bkg_ht_calo.Fill(ht)
        bkg_secondb_calo.Fill(calo_btag[-2])
        bkg_leading.Fill(pf_pt[-1])
        bkg_second.Fill(pf_pt[-2])
        bkg_third.Fill(pf_pt[-3])
        bkg_fourth.Fill(pf_pt[-4])
        ht = sum(i for i in pf_pt if i > 30)
        bkg_ht.Fill(ht)
        bkg_thirdb_pf.Fill(pf_btag[-3])


c = GetCanvas()
c.Divide(3,3)

ROOT.gStyle.SetOptStat(0)

c.cd(1)
bkg_fourth_calo.Scale(1./bkg_fourth_calo.Integral())
sig_fourth_calo.Scale(1./sig_fourth_calo.Integral())
leg = ROOT.TLegend(0.7, 0.89, 0.89, 0.6)
leg.AddEntry(bkg_fourth_calo, "Background", "L")
leg.AddEntry(sig_fourth_calo, "Signal", "L")
leg.SetBorderSize(0)
line = ROOT.TLine(30, 0, 30, bkg_fourth_calo.GetMaximum()+0.001)
line.SetLineColorAlpha(ROOT.kBlack, 0.7)
line.SetLineStyle(2)
bkg_fourth_calo.SetTitle("4th Leading Calo")
bkg_fourth_calo.GetXaxis().SetTitle("p_{T}")
bkg_fourth_calo.GetYaxis().SetTitle("Events")
bkg_fourth_calo.Draw("hist")
line.Draw("same")
sig_fourth_calo.Draw("hist same")

leg.Draw()


c.cd(2)
bkg_ht_calo.Scale(1./bkg_ht_calo.Integral())
sig_ht_calo.Scale(1./sig_ht_calo.Integral())
line1 = ROOT.TLine(320, 0, 320, bkg_ht_calo.GetMaximum()+0.001)
line1.SetLineColorAlpha(ROOT.kBlack, 0.7)
line1.SetLineStyle(2)
bkg_ht_calo.SetTitle("Calo HT")
bkg_ht_calo.GetXaxis().SetTitle("HT")
bkg_ht_calo.GetYaxis().SetTitle("Events")
bkg_ht_calo.Draw("hist")
line1.Draw("same")
sig_ht_calo.Draw("hist same")
leg.Draw()

c.cd(3)
bkg_secondb_calo.Scale(1./bkg_secondb_calo.Integral())
sig_secondb_calo.Scale(1./sig_secondb_calo.Integral())
line2 = ROOT.TLine(0.17, 0, 0.17, bkg_secondb_calo.GetMaximum()+0.001)
line2.SetLineColorAlpha(ROOT.kBlack, 0.7)
line2.SetLineStyle(2)
bkg_secondb_calo.SetTitle("Second BTag calo")
bkg_secondb_calo.GetXaxis().SetTitle("btag score")
bkg_secondb_calo.GetYaxis().SetTitle("Events")
bkg_secondb_calo.Draw("hist")
line2.Draw("same")
sig_secondb_calo.Draw("hist same")
leg.Draw()

c.cd(4)
sig_leading.Scale(1./sig_leading.Integral())
bkg_leading.Scale(1./bkg_leading.Integral())
line3 = ROOT.TLine(75, 0, 75, sig_leading.GetMaximum()+0.001)
line3.SetLineColorAlpha(ROOT.kBlack, 0.7)
line3.SetLineStyle(2)
sig_leading.SetTitle("Leading PF")
sig_leading.GetXaxis().SetTitle("p_{T}")
sig_leading.GetYaxis().SetTitle("Events")
sig_leading.Draw("hist")
line3.Draw("same")
bkg_leading.Draw("hist same")
leg.Draw()

c.cd(5)
bkg_second.Scale(1./bkg_second.Integral())
sig_second.Scale(1./sig_second.Integral())
line4 = ROOT.TLine(60, 0, 60, bkg_second.GetMaximum()+0.001)
line4.SetLineColorAlpha(ROOT.kBlack, 0.7)
line4.SetLineStyle(2)
bkg_second.SetTitle("2nd Leading PF")
bkg_second.GetXaxis().SetTitle("p_{T}")
bkg_second.GetYaxis().SetTitle("Events")
bkg_second.Draw("hist")
line4.Draw("same")
sig_second.Draw("hist same")
leg.Draw()

c.cd(6)
bkg_third.Scale(1./bkg_third.Integral())
sig_third.Scale(1./sig_third.Integral())
line5 = ROOT.TLine(45, 0, 45, bkg_third.GetMaximum()+0.001)
line5.SetLineColorAlpha(ROOT.kBlack, 0.7)
line5.SetLineStyle(2)
bkg_third.SetTitle("3rd Leading PF")
bkg_third.GetXaxis().SetTitle("p_{T}")
bkg_third.GetYaxis().SetTitle("Events")
bkg_third.Draw("hist")
line5.Draw("same")
sig_third.Draw("hist same")
leg.Draw()

c.cd(7)
bkg_fourth.Scale(1./bkg_fourth.Integral())
sig_fourth.Scale(1./sig_fourth.Integral())
line6 = ROOT.TLine(40, 0, 40, bkg_fourth.GetMaximum()+0.001)
line6.SetLineColorAlpha(ROOT.kBlack, 0.7)
line6.SetLineStyle(2)
bkg_fourth.SetTitle("4th Leading PF")
bkg_fourth.GetXaxis().SetTitle("p_{T}")
bkg_fourth.GetYaxis().SetTitle("Events")
bkg_fourth.Draw("hist")
line6.Draw("same")
sig_fourth.Draw("hist same")
leg.Draw()

c.cd(8)
bkg_ht.Scale(1./bkg_ht.Integral())
sig_ht.Scale(1./sig_ht.Integral())
line7 = ROOT.TLine(330, 0, 330, bkg_ht.GetMaximum()+0.001)
line7.SetLineColorAlpha(ROOT.kBlack, 0.7)
line7.SetLineStyle(2)
bkg_ht.SetTitle("PF HT")
bkg_ht.GetXaxis().SetTitle("HT")
bkg_ht.GetYaxis().SetTitle("Events")
bkg_ht.Draw("hist")
line7.Draw("same")
sig_ht.Draw("hist same")
leg.Draw()

c.cd(9)
bkg_thirdb_pf.Scale(1./bkg_thirdb_pf.Integral())
sig_thirdb_pf.Scale(1./sig_thirdb_pf.Integral())
line8 = ROOT.TLine(0.24, 0, 0.24, bkg_thirdb_pf.GetMaximum()+0.001)
line8.SetLineColorAlpha(ROOT.kBlack, 0.7)
line8.SetLineStyle(2)
bkg_thirdb_pf.SetTitle("3rd BTag PF")
bkg_thirdb_pf.GetXaxis().SetTitle("btag score")
bkg_thirdb_pf.GetYaxis().SetTitle("Events")
bkg_thirdb_pf.Draw("hist")
line8.Draw("same")
sig_thirdb_pf.Draw("hist same")
leg.Draw()


c.Draw()
c.SaveAs("2bSum_variables.pdf")

