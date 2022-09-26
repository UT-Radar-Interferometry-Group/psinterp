#include <vector>
#include <complex>
#include <iostream>
#include <fstream>
#include <algorithm>
#include "sario.hpp"
#include <pybind11/pybind11.h>
#include <pybind11/complex.h>
#include <pybind11/stl.h>

std::vector<std::vector<int>> scan_array(
        const unsigned int rdmin,
        const unsigned int rdmax){
    // using the mid-point cirlce drawing algorithm to search for neighboring PS pixels
    // code adpated from "https://www.geeksforgeeks.org/mid-point-circle-drawing-algorithm/"
    int x,y,p,flag; //flag > 0 means there are holes between concentric circles
    std::vector<std::vector<bool>> visited(
            rdmax,
            std::vector<bool> (rdmax,false));
    std::vector<std::vector<int>> indices;
    visited[0][0] = true;
    for (int r = 1; r < (int)rdmax; ++r){
        x = r;
        y = 0;
        p = 1-r;
        if (r > (int)rdmin){
            indices.push_back(std::vector<int> {r,0});
            indices.push_back(std::vector<int> {-r,0});
            indices.push_back(std::vector<int> {0,r});
            indices.push_back(std::vector<int> {0,-r});
        }
        visited[r][0] = true;
        visited[0][r] = true;
        flag = 0;
        while (x>y){
            // do not need to fill holes
            if (flag == 0){
                y++;
                if (p <= 0){
                    // Mid-point is inside or on the perimeter
                    p += 2*y + 1;
                }else{
                    // Mid-point is outside the perimeter
                    x--;
                    p += 2*y - 2*x + 1;
                }
            }else{
                flag--;
            }

            // All the perimeter points have already been visited
            if (x<y){
                break;
            }
            while (!visited[x-1][y]){
                x--;
                flag++;
            }
            visited[x][y]=true;
            visited[y][x]=true;
            if (r > (int)rdmin){
                indices.push_back(std::vector<int> {x,y});
                indices.push_back(std::vector<int> {-x,-y});
                indices.push_back(std::vector<int> {x,-y});
                indices.push_back(std::vector<int> {-x,y});
                if (x != y){
                    indices.push_back(std::vector<int> {y,x});
                    indices.push_back(std::vector<int> {-y,-x});
                    indices.push_back(std::vector<int> {y,-x});
                    indices.push_back(std::vector<int> {-y,x});
                }
            }
            if (flag > 0){
                x++;
            }
        }
    }
    return indices;
}

std::vector<std::vector<std::complex<double>>> interp_(
        const std::vector<std::vector<std::complex<double>>> &ifg,
        const std::vector<std::vector<bool>> &ps,
        const unsigned int N,
        const unsigned int rdmax,
        const double alpha){
    unsigned int nrow, ncol, nindices;
    nrow = ps.size();
    ncol = ps[0].size();
    std::vector<std::vector<std::complex<double>>> ifg_interp(
            nrow,std::vector<std::complex<double>>(ncol,0.));
    std::vector<std::vector<int>> indices;
    indices = scan_array(0,rdmax);
    nindices = indices.size();
#pragma omp parallel for shared(ps,ifg,ifg_interp,indices,nindices)
    for (int r0 = 0; r0 < int(nrow); ++r0){
        for (int c0 = 0; c0 < int(ncol); ++c0){
            unsigned int counter = 0;
            int r,c; 
            std::complex<double> csum = 0;
            std::vector<double> r2(N,0.);
            std::vector<std::complex<double>> cphase(N,0.);
            for (std::size_t i = 0; i < nindices; ++i){
                r = r0 + indices[i][0];
                c = c0 + indices[i][1];
                if ((r >= 0) && (r < (int)nrow) && (c >= 0) && (c < (int)ncol) && ps[r][c]){
                    // calculate the square distance to the center pixel
                    r2[counter] = std::pow(indices[i][0],2)+std::pow(indices[i][1],2);
                    cphase[counter] = ifg[r][c]/(1e-12+std::abs(ifg[r][c]));
                    ++counter;
                    if (counter >= N){
                        break;
                    }
                }
            }
            for (std::size_t i = 0; i < counter; ++i){
                csum += std::exp(-r2[i]/2/std::pow(r2[counter-1],alpha))*cphase[i];
            }
            ifg_interp[r0][c0] = std::abs(ifg[r0][c0])*csum/(std::abs(csum)+1e-12);
        }
    }
    return ifg_interp;
}

void interp(
        const std::string &ifgfile,
        const std::string &psfile,
        const std::string &outputfile,
        const unsigned int nrow,
        const unsigned int ncol,
        const unsigned int N,
        const unsigned int rdmax,
        const double alpha){
    std::vector<std::vector<std::complex<double>>> ifg = read_ifg(
            ifgfile,nrow,ncol);
    std::vector<std::vector<bool>> ps = read_psfile(
            psfile,nrow,ncol);
    std::vector<std::vector<std::complex<double>>> ifg_interp = interp_(
            ifg,ps,N,rdmax,alpha);
    save_ifg(ifg_interp, outputfile);
    return;
}

int main(int argc, char *argv[]){
    const std::string ifgfile = "20180115_20191106.int";
    const std::string psfile = "psfile";
    const std::string outputfile = "20180115_20191106.interp.int";
    interp(ifgfile,psfile,outputfile,600,600,20,101,0.75);
    return 0;
}

#define STRINGIFY(x) #x
#define MACRO_STRINGIFY(x) STRINGIFY(x)

namespace py = pybind11;
PYBIND11_MODULE(cppinterp,m){
    m.doc() = R"pbdoc(
        PS interpolation package
        )pbdoc";

    m.def("interp",&interp,R"pbdoc(
        Read interferograms and ps data from files and do the interpolation
        )pbdoc");

    m.def("interp_",&interp_,R"pbdoc(
        PS interplation
        )pbdoc");

#ifdef VERSION_INFO
   m.attr("__version__") = MACRO_STRINGIFY(VERSION_INFO); 
#else
   m.attr("__version__") = "dev";
#endif
}

