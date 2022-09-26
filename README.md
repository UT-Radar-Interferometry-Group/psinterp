# psinterp
## _Phase interpolation between PS pixels_
PS interplation (psinterp) is an open-source package for Interferometric Synthetic Aperture Radar (InSAR) phase reconstruction. It reads a wrapped interferogram and a set of PS pixels, and produces an interpolated interferogram by interpolate InSAR phases between PS pixels.

This is research code provided to you "as is" with NO WARRANTIES OF CORRECTNESS. Use at your own risk.

## Installation
PSPS requires Python3.6+ to run.
Clone to build/install
```
git clone https://github.com/UT-Radar-Interferometry-Group/psinterp.git
cd psinterp
git submodule update --init --recursive
pip install -e .
```
## Command Line Tool

```
$ psinterp -h
usage: psinterp [-h] [--outputfile OUTPUTFILE] [--N N] [--rdmax RDMAX] [--alpha ALPHA] ifgfile psfile nrow ncol

Perform phase interpolation between PS pixels

positional arguments:
  ifgfile               Filename of the interferogram to interpolate
  psfile                Filename of the PS file
  nrow                  number of rows of the interferogram
  ncol                  number of columns of the interferogram

  options:
  -h, --help            show this help message and exit
  --outputfile OUTPUTFILE 
                        Filename of the interpolated interferogram
  --N N                 Number of neighboring PS pixels used for interpolation
  --rdmax RDMAX         Maximum search radius of the PS search [in pixels]
  --alpha ALPHA         Parameter controlling the weight of each PS pixel during the interpolation: smaller alpha means more weight is assigned to PS closer to the center pixel.
```

## Example
```
Read the Sentinel-1 interferogram (January 15, 2018 - November 06, 2019) and the PS location file, perform the interpolation and store the interpolated interferogram to interp.int 

$ cd test
$ psinterp --outputfie interp.int 20180115_20191106.int psfile 600 600
```

