from Utils import Utils
from ImageUtils import ImageUtils
import ROOT
import matplotlib.pyplot as plt
from tqdm import tqdm
import cv2
import numpy as np

import tensorflow as tf
from tensorflow.keras import datasets, layers, models
from keras.layers import Dense, Input, Conv2D, Dropout, Flatten
from keras.layers import MaxPooling2D, BatchNormalization, Activation
from keras.utils import plot_model
from tensorflow.keras.models import model_from_json
import tensorflow.keras.backend as K
from keras import metrics
from keras.callbacks import EarlyStopping, ReduceLROnPlateau, TerminateOnNaN

from ActivationMap import ActivationMap

# Load the model

json_file = open('model_pf.json', 'r')
loaded_model_json = json_file.read()
json_file.close()
loaded_model = model_from_json(loaded_model_json)
# load weights into new model
loaded_model.load_weights("model_pf.h5")
print("Loaded model from disk")
 
# compile model
loaded_model.compile(loss='binary_crossentropy', optimizer='adam', metrics=['accuracy'])


#create independent test set fro signal
f = ROOT.TFile("../ggMCRAW_GenJets.root")
tree = f.Get("SaveAllJets/Jets")

images = []

counter = 0
for event in tqdm(tree):
    if(counter == 3010): break
    if(counter > 3000):
        fig, ax, canvas = Utils.Figure()
        for pt, eta, phi, btag in zip(event.pf_pt, event.pf_eta, event.pf_phi, event.pf_btag):
            ax = ImageUtils.FillAx(pt, eta, phi, btag, ax)
        plt.gca().set_position([0, 0, 1, 1])
        canvas.draw()
        fig.add_axes(ax)
        w,h = canvas.get_width_height()
        s, (width, height) = canvas.print_to_buffer()
        images.append(ImageUtils.rgb2gray(np.fromstring(s, np.uint8).reshape((height, width, 4))))
        plt.ioff()
        plt.close()
    counter+=1
    
f.Close()
    
f_bkg = ROOT.TFile("../323725_37_57_PU50.root")
tree_bkg = f_bkg.Get("SaveAllJets/Jets")

images_bkg = []

counter = 0
for event in tqdm(tree_bkg):
    if(counter==10): break
    fig, ax, canvas = Utils.Figure()
    for pt, eta, phi, btag in zip(event.pf_pt, event.pf_eta, event.pf_phi, event.pf_btag):
        ax = ImageUtils.FillAx(pt, eta, phi, btag, ax)
    plt.gca().set_position([0, 0, 1, 1])
    canvas.draw()
    fig.add_axes(ax)
    w,h = canvas.get_width_height()
    s, (width, height) = canvas.print_to_buffer()
    images_bkg.append(ImageUtils.rgb2gray(np.fromstring(s, np.uint8).reshape((height, width, 4))))
    plt.ioff()
    plt.close()
    counter+=1
    
f_bkg.Close()

#reshaping the arrays to match desired tensor dimension for input to cnn (n_samples, y, x, GreyScale=1/RGB=3)

images_bkg = np.array(images_bkg)
images_bkg = images_bkg.reshape((images_bkg.shape[0], images_bkg.shape[1], images_bkg.shape[2], 1))
images = np.array(images)
images = images.reshape((images.shape[0], images.shape[1], images.shape[2], 1))


#inference with model
y_pred_bkg = loaded_model.predict(images_bkg)
y_pred_sig = loaded_model.predict(images)

#extract info about most important area for prediction in the image
#Grad-CAM process

for i in range(len(images)):

    ActivationMap(loaded_model, "conv2d_12", images[i], "./ActivationMap/signal_{}.png".format(i))
    
for i in range(len(images_bkg)):

    ActivationMap(loaded_model, "conv2d_12", images_bkg[i], "./ActivationMap/background_{}.png".format(i))

