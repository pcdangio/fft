#ifndef FFT_H
#define FFT_H

#include <vector>
#include <complex>

namespace pcd
{

class fft
{
public:
    fft();

    template <typename T>
    void forward(const std::vector<T>& input, std::vector<std::complex<T>>& output);

    template <typename T>
    void inverse(const std::vector<std::complex<T>>& input, std::vector<T>& output);

private:
    template <typename T1, typename T2>
    void fft_recurse(const std::vector<T1>& input, std::vector<T2>& output, uint32_t start, uint32_t step);
};
}

#endif