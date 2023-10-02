#ifndef __ONSETDF_H
#define __ONSETDF_H
#include <array>

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
    OnsetDF(int type_, int wtype_);

    void initialise(int type_, int wtype_);

    float calculateODFSample(float* buffer);

    void setODFType(int type_);
private:
    float energyEnvelope();
    float energyDifference();
    // double spectralDifference();
    // double phaseDeviation();
    // double complexSpectralDifference();
    // double highFrequencyContent();
    // double highFrequencySpecDiff();
    //================================================
    void calculateHammingWindow();
    void calculateHanningWindow();
    // void calculateBlackmanWindow();
    // void calculateBlackmanHarrisWindow();
    // void calculateKaiserWindow();
    //================================================
    double pi;
    int hopsize = 512;
    int framesize = 1024;
    int odfType;
    int windowType;
    bool initialised;
    float prevEnergySum;
    std::array<float, 1024> frame;
    std::array<float, 1024> window;
};

#endif