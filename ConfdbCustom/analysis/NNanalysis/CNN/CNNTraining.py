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

from tensorflow.keras import datasets, layers, models
from keras.layers import Dense, Input, Conv2D, Dropout, Flatten
from keras.layers import MaxPooling2D, BatchNormalization, Activation
from keras.utils import plot_model
from keras import backend as K
from keras import metrics
from keras.callbacks import EarlyStopping, ReduceLROnPlateau, TerminateOnNaN


from Utils import Utils
from ImageUtils import ImageUtils
from NNUtils import NNPlotter

f = ROOT.TFile("../ggMCRAW_GenJets.root")
tree = f.Get("SaveAllJets/Jets")

#create training for signal N=3000

images = []

counter = 0
for event in tqdm(tree):
    if(counter == 3000): break
    fig, ax, canvas = Utils.Figure()
    for pt, eta, phi, btag in zip(event.pf_pt, event.pf_eta, event.pf_phi, event.pf_btag):
        ax = ImageUtils.FillAx(pt, eta, phi, btag, ax)
    plt.gca().set_position([0, 0, 1, 1])
    canvas.draw()
    fig.add_axes(ax)
    w,h = canvas.get_width_height()
    s, (width, height) = canvas.print_to_buffer()
    images.append(rgb2gray(np.fromstring(s, np.uint8).reshape((height, width, 4))))
    plt.ioff()
    plt.close()
    counter+=1
    

f.close()


#building images for background N = 3000

f_bkg = ROOT.TFile("../324997.root")
tree_bkg = f_bkg.Get("SaveAllJets/Jets")

images_bkg = []

counter = 0
for event in tqdm(tree_bkg):
    if(counter==3000): break
    fig, ax, canvas = Utils.Figure()
    for pt, eta, phi, btag in zip(event.pf_pt, event.pf_eta, event.pf_phi, event.pf_btag):
        ax = ImageUtils.FillAx(pt, eta, phi, btag, ax)
    plt.gca().set_position([0, 0, 1, 1])
    canvas.draw()
    fig.add_axes(ax)
    w,h = canvas.get_width_height()
    s, (width, height) = canvas.print_to_buffer()
    images_bkg.append(rgb2gray(np.fromstring(s, np.uint8).reshape((height, width, 4))))
    plt.ioff()
    plt.close()
    counter+=1
    
f_bkg.Close()

# creating test validation and train

images = np.array(images)
images_bkg = np.array(images_bkg)

shape = min(images.shape[0], images_bkg.shape[0])
images = images[:shape]
images_bkg = images_bkg[:shape]


y_sig = [1]*images.shape[0]
y_bkg = [0]*images_bkg.shape[0]

y = y_sig + y_bkg
y = np.array(y)

X = np.concatenate((images, images_bkg), axis=0)


#random state for reproducibility
x_train, x_test, y_train, y_test = train_test_split(X, y, test_size=0.2, random_state=42)

x_train = x_train.reshape((x_train.shape[0], x_train.shape[1], x_train.shape[2], 1))
x_test = x_test.reshape((x_test.shape[0], x_test.shape[1], x_test.shape[2], 1))

img_rows = x_train.shape[1]
img_cols = x_train.shape[2]
dropoutRate = 0.1

#Creating convolutional network

image_shape = (img_rows, img_cols, 1)

model = models.Sequential()
model.add(layers.Conv2D(16, (3, 3), activation='relu', input_shape=image_shape))
#model.add(layers.Conv2D(32, (5, 5), activation='relu', input_shape=(397,250,1)))
model.add(layers.Dropout(0.2))
model.add(layers.MaxPooling2D((2, 2)))
model.add(layers.Conv2D(16, (3, 3), activation='relu'))
model.add(layers.Dropout(0.2))
model.add(layers.MaxPooling2D((2, 2)))
model.add(layers.Conv2D(4, (3, 3), activation='relu'))
model.add(layers.Dropout(0.2))
#Here the output as a 1xN vector of convolution
model.add(layers.Flatten())
#Here enters DNN
model.add(layers.Dense(32, activation='relu'))
model.add(layers.Dropout(0.2))
model.add(layers.Dense(1, activation='sigmoid'))

model.compile(loss='binary_crossentropy', optimizer='adam', metrics=['accuracy'])

#compiling with little batch size
batch_size = 16
n_epochs = 20

callback = tf.keras.callbacks.EarlyStopping(monitor='val_loss', patience=5, min_delta=0.001)
history = model.fit(x_train, y_train, epochs=20, batch_size=batch_size, validation_data=(x_test, y_test), callbacks = [callback])

#plot loss
NNPlotter.PlotLoss(history, "./Loss_CNN.png")

#saving model
model_json = model.to_json()
with open("model_cnn_pf.json", "w") as json_file:
    json_file.write(model_json)
# serialize weights to HDF5
model.save_weights("model_cnn_pf.h5")




