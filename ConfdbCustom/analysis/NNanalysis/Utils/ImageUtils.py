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

from Utils import *

class ImageUtils:

    def rgb2gray(rgb):
        """
            Conversion of image into Grayscale
        """

        r, g, b = rgb[:,:,0], rgb[:,:,1], rgb[:,:,2]
        gray = 0.2989 * r + 0.5870 * g + 0.1140 * b

        return gray
        
    
        
    def FillAxWithAllCircles(pt, eta, phi, btag, ax):
        """
            Btags gives rotation of figures
        """
        radius = pt/200
        circle = plt.Circle((phi, eta), radius, color='white', fill=False)
        ax.add_artist(circle)
        return ax
    
    def FillAxWithFake(pt, eta, phi, btag, ax):
        """
            Btags gives rotation of figures
        """
        if pt>= 100:
            radius = pt/200
            #the first segment is (lower_phi,lower_eta), (higher_phi,lower_eta) then 60 deg rotation
            lower_eta = eta-radius
            lower_phi = phi-radius
            higher_phi = phi+radius
            (first_phi, first_eta) = (lower_phi, lower_eta)
            (second_phi, second_eta) = Utils.rotate_around_point_highperf((first_phi,first_eta), 0.5, (phi, eta) )
            (third_phi, third_eta) = Utils.rotate_around_point_highperf((second_phi, second_eta), 0.5, (phi, eta) )
            (fourth_phi, fourth_eta) = Utils.rotate_around_point_highperf((third_phi, third_eta),0.5, (phi, eta) )
            (fifth_phi, fifth_eta) = Utils.rotate_around_point_highperf((fourth_phi, fourth_eta),0.5, (phi, eta) )
            (sixth_phi, sixth_eta) = Utils.rotate_around_point_highperf((fifth_phi, fifth_eta),0.5, (phi, eta) )
            
            if btag!=0:
                (first_phi, first_eta) = Utils.rotate_around_point_highperf((first_phi, first_eta), btag, (phi, eta) )
                (second_phi, second_eta) = Utilsrotate_around_point_highperf((second_phi, second_eta), btag, (phi, eta) )
                (third_phi, third_eta) = Utilsrotate_around_point_highperf((third_phi, third_eta), btag, (phi, eta) )
                (fourth_phi, fourth_eta) = Utilsrotate_around_point_highperf((fourth_phi, fourth_eta), btag, (phi, eta) )
                (fifth_phi, fifth_eta) = Utilsrotate_around_point_highperf((fifth_phi, fifth_eta), btag, (phi, eta) )
                (sixth_phi, sixth_eta) = Utils.rotate_around_point_highperf((sixth_phi, sixth_eta), btag, (phi, eta) )
                
            exagon = Polygon(((first_phi, first_eta), (second_phi, second_eta), (third_phi, third_eta), (fourth_phi, fourth_eta), (fifth_phi, fifth_eta), (sixth_phi, sixth_eta)), color='white', fill = True)
            ax.add_artist(exagon)
            
        if 100 > pt>= 70:
            radius = pt/150
            lower_eta = eta-radius
            lower_phi = phi-radius
            higher_phi = phi+radius
            higher_eta = eta+radius
            if btag!=0:
                (ll_phi, ll_eta) = Utils.rotate_around_point_highperf((lower_phi,lower_eta), btag, (phi, eta) )
                (lr_phi, lr_eta) = Utils.rotate_around_point_highperf((higher_phi,lower_eta), btag, (phi, eta) )
                (hc_phi, hc_eta) = Utils.rotate_around_point_highperf((phi, higher_eta), btag, (phi, eta) )
                triangle = Polygon(((ll_phi, ll_eta), (lr_phi, lr_eta), (hc_phi, hc_eta)), color='white', fill = True)
                
            else:
                triangle = Polygon(((lower_phi,lower_eta), (higher_phi,lower_eta), (phi, higher_eta)), color='white', fill = True)
            ax.add_artist(triangle)
            
        elif 70 > pt >= 30:
            radius = pt/100
            square = Rectangle((phi, eta), radius, radius, color='white', fill=True)
            ax.add_artist(square)
            
        elif pt<30: #no btag if pt < 30Gev
            radius = pt/100
            circle = plt.Circle((phi, eta), radius, color='white', fill=True)
            ax.add_artist(circle)
        return ax
        
    def FillAx(pt, eta, phi, btag, ax):
        """
            Btags gives rotation of figures
        """
        if pt>= 100:
            radius = pt/200
            #the first segment is (lower_phi,lower_eta), (higher_phi,lower_eta) then 60 deg rotation
            lower_eta = eta-radius
            lower_phi = phi-radius
            higher_phi = phi+radius
            (first_phi, first_eta) = (lower_phi, lower_eta)
            (second_phi, second_eta) = Utils.rotate_around_point_highperf((first_phi,first_eta), 0.5, (phi, eta) )
            (third_phi, third_eta) = Utils.rotate_around_point_highperf((second_phi, second_eta), 0.5, (phi, eta) )
            (fourth_phi, fourth_eta) = Utils.rotate_around_point_highperf((third_phi, third_eta),0.5, (phi, eta) )
            (fifth_phi, fifth_eta) = Utils.rotate_around_point_highperf((fourth_phi, fourth_eta),0.5, (phi, eta) )
            (sixth_phi, sixth_eta) = Utils.rotate_around_point_highperf((fifth_phi, fifth_eta),0.5, (phi, eta) )
            
            if btag!=0:
                (first_phi, first_eta) = Utils.rotate_around_point_highperf((first_phi, first_eta), btag, (phi, eta) )
                (second_phi, second_eta) = Utils.rotate_around_point_highperf((second_phi, second_eta), btag, (phi, eta) )
                (third_phi, third_eta) = Utils.rotate_around_point_highperf((third_phi, third_eta), btag, (phi, eta) )
                (fourth_phi, fourth_eta) = Utils.rotate_around_point_highperf((fourth_phi, fourth_eta), btag, (phi, eta) )
                (fifth_phi, fifth_eta) = Utils.rotate_around_point_highperf((fifth_phi, fifth_eta), btag, (phi, eta) )
                (sixth_phi, sixth_eta) = Utils.rotate_around_point_highperf((sixth_phi, sixth_eta), btag, (phi, eta) )
                
            exagon = Polygon(((first_phi, first_eta), (second_phi, second_eta), (third_phi, third_eta), (fourth_phi, fourth_eta), (fifth_phi, fifth_eta), (sixth_phi, sixth_eta)), color='white', fill = False)
            ax.add_artist(exagon)
            
        if 100 > pt>= 70:
            radius = pt/150
            lower_eta = eta-radius
            lower_phi = phi-radius
            higher_phi = phi+radius
            higher_eta = eta+radius
            if btag!=0:
                (ll_phi, ll_eta) = Utils.rotate_around_point_highperf((lower_phi,lower_eta), btag, (phi, eta) )
                (lr_phi, lr_eta) = Utils.rotate_around_point_highperf((higher_phi,lower_eta), btag, (phi, eta) )
                (hc_phi, hc_eta) = Utils.rotate_around_point_highperf((phi, higher_eta), btag, (phi, eta) )
                triangle = Polygon(((ll_phi, ll_eta), (lr_phi, lr_eta), (hc_phi, hc_eta)), color='white', fill = False)
                
            else:
                triangle = Polygon(((lower_phi,lower_eta), (higher_phi,lower_eta), (phi, higher_eta)), color='white', fill = False)
            ax.add_artist(triangle)
            
        elif 70 > pt >= 30:
            radius = pt/100
            square = Rectangle((phi, eta), radius, radius, color='white', fill=False)
            ax.add_artist(square)
            
        elif pt<30: #no btag if pt < 30Gev
            radius = pt/100
            circle = plt.Circle((phi, eta), radius, color='white', fill=False)
            ax.add_artist(circle)
        return ax
    
    
