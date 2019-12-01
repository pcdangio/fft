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
    // Methods
    void fft_recurse(const std::vector<T>* real_input, std::vector<std::complex<T>>* output, uint32_t input_start, uint32_t step_size, uint32_t output_start);


    template <typename I>
    void fft_recurse(const std::vector<I>* input, std::vector<std::complex<T>>* output, uint32_t input_start, uint32_t step_size, uint32_t output_start);
};
}

#endif