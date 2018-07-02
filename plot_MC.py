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
Current_Folder = '/home/daniele/codes/sourcesMC/chains_R_8/'
Output_Folder = '/home/daniele/codes/sourcesMC/plots/'

#/*****************************************************************************
# Reading file and plotting
#/*****************************************************************************

N_samples = 100	

index = np.zeros(N_samples)

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
    index[i] = p[0]

    plt.plot(E_new, (sp_new*pow(E_new,2)), ls='-' , color="blue", lw=2)	
    
#/*****************************************************************************
# Finalizing plot
#/*****************************************************************************

print " "
print "Finalizing plot..."
print " "

data = Data_Folder + '/' + 'PAMELA_protons.dat'
Emin_pam,Emax_pam = np.loadtxt(data,skiprows=2,usecols=(1,2),unpack=True)
Emean_pam = np.loadtxt(data,skiprows=2,usecols=(0,),unpack=True)
Flux_pam  = np.loadtxt(data,skiprows=2,usecols=(3,),unpack=True)
#Flux_low  = np.loadtxt(data,skiprows=2,usecols=(8,),unpack=True)
#Flux_up   = np.loadtxt(data,skiprows=2,usecols=(9,),unpack=True)
Err_low_pam = np.loadtxt(data,skiprows=1,usecols=(8,),unpack=True)
Err_up_pam  = np.loadtxt(data,skiprows=1,usecols=(9,),unpack=True)
Flux_low_pam = Flux_pam - Err_low_pam
Flux_up_pam  = Flux_pam + Err_up_pam
 
E2xFlux_pam = ((Emean_pam)**2) * Flux_pam
Emin_pam_err = (Emean_pam - Emin_pam)
Emax_pam_err = (Emax_pam - Emean_pam)

pam, = plt.plot(Emean_pam,E2xFlux_pam,'ro')
plt.errorbar(Emean_pam,E2xFlux_pam,xerr=[Emin_pam_err, Emax_pam_err],
     yerr=[Err_low_pam*pow(Emean_pam,2),Err_up_pam*pow(Emean_pam,2)],fmt=None, ecolor='b')

plt.axis([1.e1,1.e4, 1.e0,1.e4],interpolation='none')
#plt.axis([5.e-2,5.e3, 1.e-5,3.e4],interpolation='none')
plt.xlabel('E [GeV]',fontsize=20)
plt.ylabel(r'E$^2$ J(E)[GeV m$^{-2}$ s$^{-1}$ sr$^{-1}$]',fontsize=20)
#plt.ylabel(r'J(E)[GeV$^{-1}$ m$^{-2}$ s$^{-1}$ sr$^{-1}$]',fontsize=20)
plt.xscale('log')
plt.yscale('log')

plt.legend([pam],[r'\textbf{\textit{PAMELA 2009}}'],loc='upper right')

plt.savefig(Output_Folder + '/' + 'protons.eps',format='eps',bbox_inches='tight', dpi=200)
plt.show()

exit ()
