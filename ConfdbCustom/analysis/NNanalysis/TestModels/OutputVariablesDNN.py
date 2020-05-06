import tensorflow as tf
import numpy as np
import os 
import sys
import pandas
import matplotlib.pyplot as plt
import math as mt
from tqdm import tqdm
import random

from Utils import Utils
from ImageUtils import ImageUtils
from NN_Utils import NNplotter, PlotTest

import argparse

parser = argparse.ArgumentParser()

parser.add_argument('-m', '--modeljson', required=True, help="Model json")
parser.add_argument('-w', '--modelweight', required=True, help="Model weigths")
parser.add_argument('-t', '--type', required=True, help="Model type DNN CNN RNN")
parser.add_argument('-s', '--test', required=True, help="ROOT file signal")

args = parser.parse_args()

json_file = open(args.modeljson, 'r')
loaded_model_json = json_file.read()
json_file.close()
loaded_model = model_from_json(loaded_model_json)
# load weights into new model
loaded_model.load_weights(args.modelweight)
print("Loaded model from disk")
    
# evaluate loaded model on test data
loaded_model.compile(loss='binary_crossentropy', optimizer='adam', metrics=[tf.keras.metrics.PrecisionAtRecall(0.99, num_thresholds=200)])

print("...Reading Trees")

f = ROOT.TFile(args.testsignal)
tree = f.Get("data")

l = [i.GetName() for i in tree.GetListOfBranches()]
df = pd.DataFrame(columns=l)

j = 0
for event in tqdm(tree):
    if(j%1000 == 0):
        print(j)
    obj = []
    for b in l:
        if(getattr(event, b) == 'PF' or getattr(event, b) == 'CALO'):
            obj.append(str(getattr(event, b)))
        else:
            obj.append(getattr(event, b))
            
    df = df.append(pd.Series(obj, index=df.columns ), ignore_index=True)  
    j+=1

calo = df[df["JetsType"] == "CALO"].drop(columns=["JetsType"])
pf = df[df["JetsType"] == "PF"].drop(columns=["JetsType"])

class_ = pf["class"].values
val_ = pf.drop(columns="class").values

check_dist = ["leading_pt", "leading_btag", "2leading_pt", "2leading_btag", "3leading_pt", "3leading_btag", "4leading_pt", "4leading_btag"]



