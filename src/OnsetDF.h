#ifndef __ONSETDF_H
#define __ONSETDF_H

#include "kiss_fft.h"
#include <vector>

#define FFT_SIZE 512
#define FFT_SIZE2 256
#define KISSFFT_CPX_MEMNEEDED 5208

enum OnsetDFType
{
    EnergyEnvelope,
    EnergyDifference,
    SpectralDifference,
    PhaseDeviation,
    ComplexSpectralDifference,
    HighFrequencyContent,
    HighFrequencySpecDiff
};

enum WindowType
{
    Hamming,
    Hanning,
    Blackman,
    BlackmanHarris,
    Kaiser
};

class OnsetDF
{
public:
    OnsetDF();

    ~OnsetDF();

    void initialise();

    float calculateODFSample(std::vector<float> &buffer);

    void setODFType(int type_);
    
private:
    void performFFT(std::vector<float> &buffer);
    //================================================
    float energyEnvelope(std::vector<float> &buffer);
    float energyDifference(std::vector<float> &buffer);
    float spectralDifference(std::vector<float> &buffer);
    // double phaseDeviation();
    // double complexSpectralDifference();
    // double highFrequencyContent();
    // double highFrequencySpecDiff();
    //================================================
    void calculateHammingWindow(int size);
    void calculateHanningWindow(int size);
    // void calculateBlackmanWindow();
    // void calculateBlackmanHarrisWindow();
    // void calculateKaiserWindow();
    //================================================
    void initialiseFFT();
    void freeFFT();
    //================================================
    float pi;
    int odfType;
    int windowType;

    kiss_fft_cfg cfg;
    kiss_fft_cpx *fftIn;
    kiss_fft_cpx *fftOut;
    kiss_fft_scalar fft_alloc_mem[KISSFFT_CPX_MEMNEEDED];
    size_t fft_mem_len = KISSFFT_CPX_MEMNEEDED;
    std::vector< std::vector<float> > complexOut;

    bool initialised;
    float prevEnergySum;
    std::vector<float> window;

    std::vector<float> magSpec;
    std::vector<float> prevMagSpec;
};

#endif