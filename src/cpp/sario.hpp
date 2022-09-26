#ifndef SARIO
#define SARIO

#include<complex>
#include<vector>
#include<string>

std::vector<std::vector<std::complex<double>>> read_ifg(
        const std::string& fname, 
        const std::size_t nrow,
        const std::size_t ncol);

void save_ifg(
        const std::vector<std::vector<std::complex<double>>> ifg,
        const std::string& fname);

std::vector<std::vector<bool>> read_psfile(
        const std::string& fname, 
        const std::size_t nrow,
        const std::size_t ncol);

#endif

