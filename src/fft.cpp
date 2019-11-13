#include <pcd/fft/fft.h>

using namespace pcd;

fft::fft()
{

}

template <typename T>
void fft::forward(const std::vector<T>& input, std::vector<std::complex<T>>& output)
{

}

template void fft::forward<float>(const std::vector<float>& input, std::vector<std::complex<float>>& output);
template void fft::forward<double>(const std::vector<double>& input, std::vector<std::complex<double>>& output);

template <typename T>
void fft::inverse(const std::vector<std::complex<T>>& input, std::vector<T>& output)
{

}

template void fft::inverse<float>(const std::vector<std::complex<float>>& input, std::vector<float>& output);
template void fft::inverse<double>(const std::vector<std::complex<double>>& input, std::vector<double>& output);

template <typename T1, typename T2>
void fft::fft_recurse(const std::vector<T1>& input, std::vector<T2>& output, uint32_t start, uint32_t step)
{

}

template void fft::fft_recurse<float, std::complex<float>>(const std::vector<float>& input, std::vector<std::complex<float>>& output, uint32_t start, uint32_t step);
template void fft::fft_recurse<double, std::complex<double>>(const std::vector<double>& input, std::vector<std::complex<double>>& output, uint32_t start, uint32_t step);
template void fft::fft_recurse<std::complex<float>, float>(const std::vector<std::complex<float>>& input, std::vector<float>& output, uint32_t start, uint32_t step);
template void fft::fft_recurse<std::complex<double>, double>(const std::vector<std::complex<double>>& input, std::vector<double>& output, uint32_t start, uint32_t step);
