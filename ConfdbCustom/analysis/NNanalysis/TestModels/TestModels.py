import ROOT
import tensorflow as tf
import numpy as np
import pandas
import matplotlib.pyplot as plt
import math as mt
from tqdm import tqdm
import matplotlib.patches as mpatches
from matplotlib.collections import PatchCollection
from matplotlib.patches import Polygon, Circle, Rectangle
from matplotlib.backends.backend_agg import FigureCanvasAgg
import random
from sklearn.model_selection import train_test_split

from Utils import Utils
from ImageUtils import ImageUtils
from NN_Utils import NNplotter, PlotTest

import argparse

parser = argparse.ArgumentParser()

parser.add_argument('-m', '--modeljson', required=True, help="Model json")
parser.add_argument('-w', '--modelweight', required=True, help="Model weigths")
parser.add_argument('-t', '--type', required=True, help="Model type DNN CNN RNN")
parser.add_argument('-s', '--testsignal', required=True, help="ROOT file signal")
parser.add_argument('-b', '--testbackground', required=False, help="ROOT file background")

args = parser.parse_args()

if args.type == "DNN":
    json_file = open(args.modeljson, 'r')
    loaded_model_json = json_file.read()
    json_file.close()
    loaded_model = model_from_json(loaded_model_json)
    # load weights into new model
    loaded_model.load_weights(args.modelweight)
    print("Loaded model from disk")
     
    # evaluate loaded model on test data
    loaded_model.compile(loss='binary_crossentropy', optimizer='adam', metrics=[tf.keras.metrics.PrecisionAtRecall(0.99, num_thresholds=200)])
    print("...Building dataset for DNN model")
    
    
    f_sig = ROOT.TFile(args.testsignal)
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
    
    if args.testbackground:
        f_bkg = ROOT.TFile(args.testbackground)
        tree_ = f.Get("data")
        
        l = [i.GetName() for i in tree.GetListOfBranches()]
        df_bkg = pd.DataFrame(columns=l)
        
        j = 0
        for event in tqdm(tree_):
            if(j%1000 == 0):
                print(j)
            obj = []
            for b in l:
                if(getattr(event, b) == 'PF' or getattr(event, b) == 'CALO'):
                    obj.append(str(getattr(event, b)))
                else:
                    obj.append(getattr(event, b))
                    
            df_bkg = df_bkg.append(pd.Series(obj, index=df.columns ), ignore_index=True)
            j+=1
            
        df = df.append(df, df_bkg)
    
    #balancing between two classes
    df1 = df[df["class"]==1]
    df0 = df[df["class"]==0]
    minlen = min(len(df1), len(df0))
    df1 = df1[:minlen]
    df0 = df0[:minlen]
    
    df = df1.append(df0)
    
    #into np arrays
    class_ = df["class"].values
    val = df.drop(columns="class").values
    
    print("...Plotting model output")
    
    PlotTest(model, val, class_, output_path="./DNNDistOutputTest.png"):
    

if args.type == "CNN":

if args.type == "RNN":

else:
    print("Model type does not match any base constructors")
