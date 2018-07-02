#!/bin/bash/python

import os
import numpy as np
import math
import random
from matplotlib import pyplot as plt
from matplotlib import rc, rcParams 
from scipy import interpolate
#from scipy.interpolate import interp1d  
import pylab
import pyfits
#from joblib import Parallel, delayed  
#import multiprocessing

# activate latex text rendering 
rc('text', usetex=True)
# Change all fonts to 'Computer Modern'
rc('font',**{'family':'serif','serif':['Computer Modern Roman']})
rc('xtick', labelsize=18)
rc('ytick', labelsize=18)
rcParams['legend.numpoints'] = 1

#/*****************************************************************************
# INPUT / OUTPUT DIRECTORIES
#/*****************************************************************************
Data_Folder = '/home/daniele/codes/CRPROJ/dragon-dev/branch/dragon-3.0.2_clean/plots/data_py/'
Current_Folder = '/home/daniele/codes/sourcesMC/chains_final_R5/'
Output_Folder = '/home/daniele/codes/sourcesMC/plots/'

#/*****************************************************************************
# Reading file and plotting
#/*****************************************************************************

N_samples = 100	

slopes = np.zeros(N_samples)

for i in range(N_samples):

    filename = Current_Folder + "final_spectrum_" + str(i) + ".dat"

    print "Reading ", filename

    data = np.loadtxt(filename, delimiter='\t', usecols=(0, 1), unpack=True)

    print data.shape
    E_vec = data[0,:]
    spectrum = data[1,:]

    sp_new = spectrum[spectrum>0.]
    E_new = E_vec[spectrum>0.]

    #for ie in range(len(E_new)):
	# print E_new[ie], " ", sp_new[ie]

    log_E = np.log10(E_new)     
    log_sp = np.log10(sp_new)

    p = np.polyfit(log_E,log_sp,1)  
    slopes[i] = p[0]
   
n, bins, patches = plt.hist(slopes, 10, normed=1, histtype='stepfilled') #, alpha=0.3
plt.setp(patches, 'FaceColor','r','EdgeColor','w')
plt.axis([-3.0, -2.6, 0, 50])
plt.grid(True)
plt.show()

"""
plt.axis([1.e1,1.e4, 1.e0,1.e4],interpolation='none')
plt.xlabel('E [GeV]',fontsize=20)
plt.ylabel(r'E$^2$ J(E)[GeV m$^{-2}$ s$^{-1}$ sr$^{-1}$]',fontsize=20)
plt.xscale('log')
plt.yscale('log')
plt.legend([pam],[r'\textbf{\textit{PAMELA 2009}}'],loc='upper right')
plt.savefig(Output_Folder + '/' + 'protons.eps',format='eps',bbox_inches='tight', dpi=200)
plt.show()
"""

exit ()
