#include <pcd/fft/fft.h>
#include <iostream>

int main(int argc, char** argv)
{
    // Create FFT structure.
    pcd::fft<double> m_fft;

    // Create input vector.
    std::vector<double> input = {0, 1, 2, 3, 4, 5, 6, 7};

    // Create output vector.
    std::vector<std::complex<double>> output;

    // Run forward FFT.
    m_fft.forward(input, output);

    // Write outputs for comparison.
    std::cout << "fft(input) = " << std::endl;
    for(auto i = 0; i < output.size(); i++)
    {
        std::cout << output.at(i) << std::endl;
    }
}

