#include <iostream>
#include <fstream>
#include <complex>
#include <vector>
#include <string>

#include "sario.hpp"

std::vector<std::vector<std::complex<double>>> read_ifg(
        const std::string& fname, 
        const std::size_t nrow,
        const std::size_t ncol){
    using comp_double = std::complex<double>;
    std::size_t re_idx, im_idx;
    std::vector<float> imgbuffer(nrow*ncol*2);
    std::ifstream fin(fname, std::ios::binary);
    fin.read((char *)imgbuffer.data(), nrow*ncol*2*sizeof(float));
    fin.close();
    std::vector<std::vector<comp_double>> ifg(nrow,std::vector<comp_double>(ncol,0));
    for (std::size_t r = 0; r < nrow; ++r){
        for (std::size_t c = 0; c < ncol; ++c){
            re_idx = ncol*2*r + c*2;
            im_idx = ncol*2*r + c*2+1;
            ifg[r][c] = 
                std::complex<double>(imgbuffer[re_idx],imgbuffer[im_idx]);
        }
    }
    return ifg;
}

void save_ifg(
        const std::vector<std::vector<std::complex<double>>> ifg,
        const std::string& fname){
    std::ofstream fout(fname, std::ios::out | std::ios::binary);
    if (!fout.is_open()){
        printf("Unable to open file %s\n",fname.c_str());
        return;
    }
    for (std::size_t i = 0; i < ifg.size(); ++i){
        std::vector<float> line(2*ifg[i].size(),0.);
        for (std::size_t j=0; j < ifg[i].size(); ++j){
            line[2*j] = float(std::real(ifg[i][j]));
            line[2*j+1] = float(std::imag(ifg[i][j]));
        }
        fout.write((char*)&line[0],line.size()*sizeof(float));
    }
    fout.close();
    return; 
}

std::vector<std::vector<bool>> read_psfile(
        const std::string& fname, 
        const std::size_t nrow,
        const std::size_t ncol){
    std::vector<char> imgbuffer(nrow*ncol);
    std::ifstream fin(fname, std::ios::in| std::ios::out| std::ios::binary);
    fin.read((char *)imgbuffer.data(), nrow*ncol);
    fin.close();
    std::vector<std::vector<bool>> ps(nrow,std::vector<bool>(ncol,false));
    int counter = 0;
    for (std::size_t i = 0; i < nrow; ++i){
        for (std::size_t j = 0; j < ncol; ++j){
            ps[i][j] = bool(imgbuffer[ncol*i + j]);
            if(ps[i][j]){
                counter++;
            }
        }
    }
    return ps;
}
