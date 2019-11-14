#include <pcd/fft/fft.h>

using namespace pcd;

template <class T>
fft<T>::fft()
{

}

template <class T>
void fft<T>::forward(const std::vector<T>& input, std::vector<std::complex<T>>& output)
{

}

template <class T>
void fft<T>::inverse(const std::vector<std::complex<T>>& input, std::vector<T>& output)
{

}

template <class T>
void fft<T>::fft_recurse()
{
    // Calculate size of the input vector using current starting index and step.
    uint32_t n = (fft<T>::input_size - fft<T>::input_start - 1) / fft<T>::step + 1;

    // Check size of the input vector.    
    if(n == 2)
    {
        // Single even and single odd. Calculate FFT directly from input vector.
        // Keep in mind that exp(-2*pi*k/N) = 1 when k = 0, so values are just even[start] + 1 * odd[start].
        fft<T>::complex_output[fft<T>::output_start] = fft<T>::real_input[fft<T>::input_start] + fft<T>::real_input[fft<T>::input_start + fft<T>::step_size];
        // This assumes that input is real only.  Complex conjugate of a real number is just the real number.
        fft<T>::complex_output[fft<T>::output_start + 1] = fft<T>::complex_output[fft<T>::output_start];
    }
    else
    {
        // n_even and n_odd both > 1.  Recurse to get FFTs of the odd/even vectors.
        // Precalculate n/2.
        uint32_t n2 = n/2;

        // Increment step size by factor of 2.
        fft<T>::step_size *= 2;
        // Run FFT on evens.
        // Even branch always starts at current input_start, and writes to current output_start.
        fft::fft_recurse();
        // Odd branch always starts at current input_start + 1, and writes to output after even outputs (n/2)
        fft<T>::input_start++;
        fft<T>::output_start = n2;
        fft::fft_recurse();
        // Reverse changes to global, incrementally used input start/step
        fft<T>::input_start--;
        fft<T>::step_size /= 2;

        // Calculate FFT from output vector.
        // Iterate over k = 0 to N/2-1
        for(uint32_t k = 0; k < n2; k++)
        {
            // output[k] = even[k] + exp(-2*pi*i*k/n) * odd[k]
            fft<T>::complex_output[k] = fft<T>::complex_output[k] + std::exp<std::complex<T>>(-2 * M_PI * std::complex_literals::i * k / n) * fft<T>::complex_output[k + n2];
            // output[k+n/2] = even[k] - w_nk * odd[k]
            // This is the same as the complex conjugate of output[k]
            // Using this method also allows us to overwrite complex_output[k] on the prior step and still calculate output[k+n/2]
            fft<T>::complex_output[k + n2] = std::conj(fft<T>::complex_output[k]);
        }
    }

}

template class fft<float>;
template class fft<double>;