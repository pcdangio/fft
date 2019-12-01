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
    void forward(std::vector<std::complex<T>>& input, std::vector<std::complex<T>>& output);

    void inverse(std::vector<std::complex<T>>& input, std::vector<T>& output);
    void inverse(std::vector<std::complex<T>>& input, std::vector<std::complex<T>>& output);

private:
    // METHODS
    template <typename V>
    void zero_pad(std::vector<V>& input);

    template <typename I>
    void fft_recurse(const std::vector<I>* input, std::vector<std::complex<T>>* output, uint32_t input_start, uint32_t step_size, uint32_t output_start);
};
}

#endif