#include <pcd/fft/fft.h>

using namespace pcd;

template <class T>
fft<T>::fft()
{

}

// FORWARD METHODS
template <class T>
void fft<T>::forward(std::vector<T>& input, std::vector<std::complex<T>>& output)
{
    // Pad input array to nearest power of 2.
    // Store original length to restore at end.
    uint32_t original_length = input.size();
    fft<T>::zero_pad<T>(input);

    // Initialize output array and fill with blanks.
    output.resize(input.size(), 0);
    // Begin recursion.
    fft<T>::fft_recurse<T>(&input, &output, true, 0, 1, 0);

    // Remove padded zeros.
    input.resize(original_length);   
}
template <class T>
void fft<T>::forward(std::vector<std::complex<T>>& input, std::vector<std::complex<T>>& output)
{
    // Pad input array to nearest power of 2.
    // Store original length to restore at end.
    uint32_t original_length = input.size();
    fft<T>::zero_pad<std::complex<T>>(input); 

    // Initialize output array and fill with blanks.
    output.resize(input.size(), 0);
    // Begin recursion.
    fft<T>::fft_recurse<std::complex<T>>(&input, &output, true, 0, 1, 0);

    // Remove padded zeros.
    input.resize(original_length);  
}

// INVERSE METHODS
template <class T>
void fft<T>::inverse(std::vector<std::complex<T>>& input, std::vector<T>& output)
{
    // Pad input array to nearest power of 2.
    // Store original length to restore at end.
    uint32_t original_length = input.size();
    fft<T>::zero_pad<std::complex<T>>(input);

    // To get a real output, we must create a temporary complex output vector
    // fft_recurse operates on the output in place and must be complex.
    std::vector<std::complex<T>> output_complex;

    // Initialize output array and fill with blanks.
    output_complex.resize(input.size(), 0);
    // Begin recursion.
    fft<T>::fft_recurse<std::complex<T>>(&input, &output_complex, false, 0, 1, 0);

    // Must scale down by N and copy to real array.
    output.clear();
    output.reserve(output_complex.size());
    for(uint32_t i = 0; i < output_complex.size(); i++)
    {
        output.push_back(output_complex.at(i).real() / output_complex.size());
    }

    // Remove padded zeros.
    input.resize(original_length);
}
template <class T>
void fft<T>::inverse(std::vector<std::complex<T>>& input, std::vector<std::complex<T>>& output)
{
    // Pad input array to nearest power of 2.
    // Store original length to restore at end.
    uint32_t original_length = input.size();
    fft<T>::zero_pad<std::complex<T>>(input); 

    // Initialize output array and fill with blanks.
    output.resize(input.size(), 0);
    // Begin recursion.
    fft<T>::fft_recurse<std::complex<T>>(&input, &output, false, 0, 1, 0);

    // Must scale down by N.
    for(uint32_t i = 0; i < output.size(); i++)
    {
        output[i] /= output.size();
    }

    // Remove padded zeros.
    input.resize(original_length);
}

// FREQUENCY BIN METHODS
template <class T>
void fft<T>::frequency_bins(uint32_t fft_size, T sample_frequency, std::vector<T>& frequency_bins)
{
    // Set up output vector.
    frequency_bins.clear();
    frequency_bins.reserve(fft_size);

    // Convert fft_size to T type for loop efficiency.
    T fft_size_t = static_cast<T>(fft_size);

    // Calculate and output frequency bins using f(i) = i * Fs / N
    for(T i = 0; i < fft_size_t; i++)
    {
        frequency_bins.push_back(i * sample_frequency / fft_size_t);
    }
}

// PRIVATE METHODS
template <class T>
template <typename V>
void fft<T>::zero_pad(std::vector<V>& input)
{
    // Pad input array to nearest power of 2.
    // Find the nearest power of 2.
    uint32_t padded_length = std::pow(2, std::ceil(std::log2(input.size())));
    // Resize the vector to pad with zeros.
    // This assumes that the default constructor uses zeros.
    input.resize(padded_length, 0);   
}

template <class T>
template <typename I>
void fft<T>::fft_recurse(const std::vector<I>* input, std::vector<std::complex<T>>* output, bool forward, uint32_t input_start, uint32_t step_size, uint32_t output_start)
{
    // Calculate size of the input vector using current starting index and step.
    uint32_t n = (input->size() - input_start - 1) / step_size + 1;

    // Check size of the input vector.    
    if(n == 2)
    {
        // Single even and single odd. Calculate FFT directly from input vector.
        // Keep in mind that exp(-2*pi*k/N) = 1 when k = 0, so values are:
        // even[start] + 1 * odd[start]
        // even[start] - 1 * odd[start]
        output->at(output_start) = input->at(input_start) + input->at(input_start + step_size);
        output->at(output_start + 1) = input->at(input_start) - input->at(input_start + step_size);
    }
    else
    {
        // n_even and n_odd both > 1.  Recurse to get FFTs of the odd/even vectors.
        // Precalculate n/2.
        uint32_t n_2 = n / 2;

        // Run FFT on evens.
        // Even branch always starts at current input_start, and writes to current output_start.
        fft::fft_recurse<I>(input, output, forward, input_start, step_size*2, output_start);
        // Odd branch always starts at current input_start + 1, and writes to output after even outputs (n/2)
        fft::fft_recurse<I>(input, output, forward, input_start + step_size, step_size*2, output_start + n_2);

        // Calculate FFT from output vector.
        // Initialize w_n, which will save on arithmetic operations.
        // w_k = exp(-2*pi*i*k/n), and only k changes.  so exp(-2*pi*i/n)^k is equivalent to
        // w_n = exp(-2*pi*i/n) and thus for each iteration k we can just multiply by w_n again.
        std::complex<T> w_n;
        if(forward)
        {
            w_n = std::exp(static_cast<T>(-2 * M_PI / n) * std::complex<T>(0, 1));
        }
        else
        {
            // For inverse DFTs, w_k = exp(2*pi*i*k/n)
            w_n = std::exp(static_cast<T>(2 * M_PI / n) * std::complex<T>(0, 1));
        }        
        // Initialize w_k for k=0, which evaluates to 1.
        std::complex<T> w_k = 1;
        // Preinitialize even_k
        std::complex<T> even_k;
        // Iterate over k = 0 to N/2-1
        // Keep in mind that k must start at output_start to read and replace in correct location.
        for(uint32_t k = output_start; k < output_start + n_2; k++)
        {
            // Store current_even = complex_output[k]
            even_k = output->at(k);
            // output[k] = even[k] + exp(-2*pi*i*k/n) * odd[k]
            output->at(k) = even_k + w_k * output->at(k + n_2);
            // output[k+n/2] = even[k] - w_nk * odd[k]
            output->at(k + n_2) = even_k - w_k * output->at(k + n_2);
            // Increment w_k for next iteration.
            w_k *= w_n;
        }
    }
}

template class fft<float>;
template class fft<double>;