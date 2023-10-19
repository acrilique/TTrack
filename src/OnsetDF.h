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

const int hopsize = 512;
const int framesize = 1024;

class OnsetDF
{
public:
    OnsetDF();

    void initialise();

    float calculateODFSample(const std::array<float, framesize> &buffer);

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
    int odfType = 1;
    int windowType = 1;
    bool initialised;
    float prevEnergySum;
    std::array<float, framesize> frame;
    std::array<float, framesize> window;
};

#endif