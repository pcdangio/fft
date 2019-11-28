#include <pcd/fft/fft.h>

using namespace pcd;

template <class T>
fft<T>::fft()
{

}

template <class T>
void fft<T>::forward(std::vector<T>& input, std::vector<std::complex<T>>& output)
{
    // Pad input array to nearest power of 2.
    // Store original length to restore at end.
    uint32_t original_length = input.size();
    // Find the nearest power of 2.
    uint32_t padded_length = std::pow(2, std::ceil(std::log2(input.size())));
    // Resize the vector to pad with zeros.
    input.resize(padded_length, 0);    

    // Initialize output array and fill with blanks.
    output.resize(padded_length, 0);

    // Store and setup local variables.
    real_input = &input;
    input_size = input.size();
    input_start = 0;
    step_size = 1;
    complex_output = &output;
    output_start = 0;

    // Begin recursion.
    fft<T>::fft_recurse();

    // Remove padded zeros.
    input.resize(original_length);   
}

template <class T>
void fft<T>::inverse(const std::vector<std::complex<T>>& input, std::vector<T>& output)
{

}

template <class T>
void fft<T>::fft_recurse()
{
    // Calculate size of the input vector using current starting index and step.
    uint32_t n = (fft<T>::input_size - fft<T>::input_start - 1) / fft<T>::step_size + 1;

    // Check size of the input vector.    
    if(n == 2)
    {
        // Single even and single odd. Calculate FFT directly from input vector.
        // Keep in mind that exp(-2*pi*k/N) = 1 when k = 0, so values are:
        // even[start] + 1 * odd[start]
        // even[start] - 1 * odd[start]
        fft<T>::complex_output->at(fft<T>::output_start) = fft<T>::real_input->at(fft<T>::input_start) + fft<T>::real_input->at(fft<T>::input_start + fft<T>::step_size);
        fft<T>::complex_output->at(fft<T>::output_start + 1) = fft<T>::real_input->at(fft<T>::input_start) - fft<T>::real_input->at(fft<T>::input_start + fft<T>::step_size);
    }
    else
    {
        // n_even and n_odd both > 1.  Recurse to get FFTs of the odd/even vectors.
        // Precalculate n/2.
        uint32_t n_2 = n/2;

        // Store original step size before modifying it so odd starting index can be calculated.
        uint32_t current_step_size = fft<T>::step_size;

        // Increment step size by factor of 2 for recursion.
        fft<T>::step_size *= 2;
        // Run FFT on evens.
        // Even branch always starts at current input_start, and writes to current output_start.
        fft::fft_recurse();
        // Odd branch always starts at current input_start + 1, and writes to output after even outputs (n/2)
        fft<T>::input_start += current_step_size;
        fft<T>::output_start += n_2;
        fft::fft_recurse();
        // Reverse changes to global, incrementally used input start/step
        fft<T>::output_start -= n_2;
        fft<T>::input_start -= current_step_size;
        fft<T>::step_size = current_step_size;

        // Calculate FFT from output vector.
        // Initialize w_n, which will save on arithmetic operations.
        // w_k = exp(-2*pi*i*k/n), and only k changes.  so exp(-2*pi*i/n)^k is equivalent
        // w_n = exp(-2*pi*i/n) and thus for each iteration k we can just multiply by w_n again.
        std::complex<T> w_n = std::exp(static_cast<T>(-2 * M_PI / n) * std::complex<T>(0, 1));
        // Initialize w_k for k=0, which evaluates to 1.
        std::complex<T> w_k = 1;
        // Preallocate storage for even component as it will be overwritten.
        std::complex<T> even_k;
        // Iterate over k = 0 to N/2-1
        // Keep in mind that k must start at output_start to read and replace in correct location.
        for(uint32_t k = fft<T>::output_start; k < fft<T>::output_start + n_2; k++)
        {
            // Store current_even = complex_output[k]
            even_k = fft<T>::complex_output->at(k);
            // output[k] = even[k] + exp(-2*pi*i*k/n) * odd[k]
            fft<T>::complex_output->at(k) = even_k + w_k * fft<T>::complex_output->at(k + n_2);
            // output[k+n/2] = even[k] - w_nk * odd[k]
            fft<T>::complex_output->at(k + n_2) = even_k - w_k * fft<T>::complex_output->at(k + n_2);
            // Increment w_k for next iteration.
            w_k *= w_n;
        }
    }

}

template class fft<float>;
template class fft<double>;