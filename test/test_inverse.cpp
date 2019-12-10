#include <pcd/fft/fft.h>
#include <iostream>

int main(int argc, char** argv)
{
    // Create FFT structure.
    pcd::fft<double> m_fft;

    // Create input vector.
    using namespace std::complex_literals;
    std::vector<std::complex<double>> input = {28,
                                               -4. + 9.656854i,
                                               -4. + 4i,
                                               -4. + 1.656854i,
                                               -4,
                                               -4. - 1.656854i,
                                               -4. - 4i,
                                               -4. - 9.656854i};

    // Create output vector.
    std::vector<double> output;

    // Run inverse FFT.
    m_fft.inverse(input, output);

    // Write outputs for comparison.
    std::cout << "fft(input) = " << std::endl;
    for(auto i = 0; i < output.size(); i++)
    {
        std::cout << output.at(i) << std::endl;
    }
}

