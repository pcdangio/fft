#include <pcd/fft/fft.h>
#include <iostream>
#include <chrono>

int main(int argc, char** argv)
{
    // Create FFT structure.
    pcd::fft<double> m_fft;

    // Specify input size and n runs.
    int input_size = 32768;
    int iterations = 100;

    // Output configuration for user.
    std::cout << "Running " << iterations << " iterations on input vector of " << input_size << " elements...";

    // Create input vector.
    std::vector<double> input;
    input.reserve(input_size);
    for(double k = 0; k < input_size; k++)
    {
        input.push_back(k);
    }

    // Create output vector.
    std::vector<std::complex<double>> output;

    // Capture start time.
    auto start = std::chrono::steady_clock::now();

    // Run iterations for averaging.
    for(int i = 0; i < iterations; i++)
    {
        // Run forward FFT.
        m_fft.forward(input, output);
    }

    // Capture end time.
    auto end = std::chrono::steady_clock::now();

    // Calculate average elapsed time.
    double avg_seconds = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() / 1000.0 / iterations;

    // Write output for user.
    std::cout << " done" << std::endl;
    std::cout << "Average runtime: " << avg_seconds << "s" << std::endl;
}

