#ifndef FFT_H
#define FFT_H

#include <vector>
#include <complex>

namespace pcd
{

template <class T>
class fft
{
public:
    fft();

    void forward(std::vector<T>& input, std::vector<std::complex<T>>& output);
    void inverse(const std::vector<std::complex<T>>& input, std::vector<T>& output);

private:
    const std::vector<T>* real_input;
    const std::vector<std::complex<T>>* complex_input;
    uint32_t input_size;
    uint32_t input_start;
    uint32_t step_size;
    std::vector<std::complex<T>>* complex_output;
    std::vector<T>* real_output;
    uint32_t output_start;

    void fft_recurse();


};
}

#endif