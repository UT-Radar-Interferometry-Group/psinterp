import os
import numpy as np
from psinterp import cppinterp

def interpf(ifgfile,
            psfile,
            nrow,
            ncol,
            outputfile = None,
            N = 20,
            rdmax = 101,
            alpha=0.75):
    """
    Persistent scatterer interpolation
    
    Parameters
    ----------
    ifgfile : string 
        wrapped interferogram file to interpolate
    psfile : string
        binary file storing the location of PS pixels. 'psfile' should be read
        as a 2D boolean array that has the same size as each interferogram. PS
        pixels are marked as True and non-PS pixels are marked as False.
    nrow : int
        number of rows of the interferogram
    ncol : int
        number of columns of the interferogram
    outputfile: string(optional)
        file to store the interpolated interferogram
        if outputfile is None, the interpolated interferogram will be stored in
        the current directory and be named as ifgfile+'.interp'
    N: int (optional)
        number of neareast PS pixels used for interpolation
        N = 20 by default
    rdmax : int (optional)
        maximum radius (in pixel) for PS searching
        rdmax = 101 by default
    alpha : float (optional)
        hyperparameter controlling the weight of PS in interpolation: smaller
        alpha means more weight is assigned to PS closer to the center pixel.
        alpha = 0.75 by default

    References
    ----------
    "A persistent scatterer interpolation for retrieving accurate ground
    deformation over InSAR‐decorrelated agricultural fields"
    Chen et al., 2015, https://doi.org/10.1002/2015GL065031
    """
    if outputfile is None:
        outputfile = os.path.split(ifgfile)[-1]+'.interp'

    if not os.path.exists(ifgfile):
        raise FileNotFoundError("Cannot find file: {}".format(ifgfile))
    if not os.path.exists(psfile):
        raise FileNotFoundError("Cannot find file: {}".format(psfile))

    cppinterp.interp(ifgfile,psfile,outputfile,
                     int(nrow),int(ncol),int(N),int(rdmax),float(alpha))

def interp(ifg,
           ps,
           N = 20,
           rdmax = 51,
           alpha=0.75):
    """
    Persistent scatterer interpolation
    
    Parameters
    ----------
    ifg : 2D complex array
        wrapped interferogram to interpolate
    ps : 2D boolean array
        ps[i,j] = True if radar pixel (i,j) is a PS
        ps[i,j] = False if radar pixel (i,j) is not a PS 
    N: int (optional)
        number of neareast PS pixels used for interpolation
        N = 20 by default
    rdmax : int (optional)
        maximum radius (in pixel) for PS searching
        rdmax = 51 by default
    alpha : float (optional)
        hyperparameter controlling the weight of PS in interpolation: smaller
        alpha means more weight is assigned to PS closer to the center pixel.
        alpha = 0.75 by default

    Returns
    -------
    ifg_interp : 2D complex array
        interplated interferogram

    References
    ----------
    "A persistent scatterer interpolation for retrieving accurate ground
    deformation over InSAR‐decorrelated agricultural fields"
    Chen et al., 2015, https://doi.org/10.1002/2015GL065031
    """
    ifg_interp = cppinterp.interp_(ifg,ps,N,rdmax,alpha)
    ifg_interp = np.array(ifg_interp)
    return ifg_interp
