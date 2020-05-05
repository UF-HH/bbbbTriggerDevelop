import ROOT
import tensorflow as tf
import numpy
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
import numpy as np

class Utils:
        
    def Figure():
        rate = 10./(2*mt.pi)
        fig = plt.figure(figsize=(5,5*rate), dpi=50)
        fig.tight_layout()
        plt.ylim(-5, 5)
        plt.xlim(-mt.pi, mt.pi)
        axes = fig.add_subplot(1, 1, 1)
        axes.set_axis_off()
        #axes.patch.set_facecolor('black')
        fig.patch.set_facecolor('black')
        canvas = FigureCanvasAgg(fig)
        return fig, axes, canvas
        
    def rotate_around_point_highperf(xy, cos, origin):
        """
            clockwise rotation given btag = cos
        """
        cos_rad = cos
        sin_rad = (cos/abs(cos))*mt.sqrt(1-pow(cos, 2))
        x, y = xy
        offset_x, offset_y = origin
        adjusted_x = (x - offset_x)
        adjusted_y = (y - offset_y)
        qx = offset_x + cos_rad * adjusted_x + sin_rad * adjusted_y
        qy = offset_y + -sin_rad * adjusted_x + cos_rad * adjusted_y

        return qx, qy
        

