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

def ActivationMap(model, layer_name, image, output_path, clas = 0, ret = False):
    """
        Input: Conv2D layer that we want to study with CAM method.
        an image intended as a greyscale (1,M,N,1) numpy array.
        Output path to save the original image and overlayed activation map for such layer.
        clas: for multi class classification, check gradients for the desired class.
        
        For theory check: https://arxiv.org/pdf/1610.02391.pdf
    """
    
    #reshaping example image to tensor (1,x,y,1)
    img = tf.keras.preprocessing.image.img_to_array(image)
    img = np.array([img]) #adding (1,...)

    grad_model = tf.keras.models.Model([model.inputs], [model.get_layer(layer_name).output, model.output])# Load initial model


    # Get the score for target class
    with tf.GradientTape() as tape:
        conv_outputs, predictions = grad_model(img)
        loss = predictions[:, clas]

    # Extract filters and gradients
    output = conv_outputs[0]
    grads = tape.gradient(loss, conv_outputs)[0]

    # Average gradients spatially
    weights = tf.reduce_mean(grads, axis=(0, 1))

    # Build a ponderated map of filters according to gradients importance
    cam = np.ones(output.shape[0:2], dtype=np.float32)

    for index, w in enumerate(weights):
        cam += w * output[:, :, index]
        
    # Heatmap visualization

    cam = cv2.resize(cam.numpy(), (img.shape[2], img.shape[1]))
    cam = np.maximum(cam, 0)
    heatmap = (cam - cam.min()) / (cam.max() - cam.min())

    cam = cv2.applyColorMap(np.uint8(255*heatmap), cv2.COLORMAP_JET)

    img = img[0]

    img_3D= cv2.merge([img,img,img])

    output_image = cv2.addWeighted(cv2.cvtColor(img_3D.astype('uint8'), cv2.COLOR_RGB2BGR), 1, cam, 0.5, 0)

    plt.imsave(output_path, output_image, format = "png")
    
    if ret:
        return output_image
    else:
        return
