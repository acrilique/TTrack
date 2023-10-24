#ifndef __ONSETDF_H
#define __ONSETDF_H
#include <vector>

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

    void initialise();

    float calculateODFSample(std::vector<float> &buffer);

    void setODFType(int type_);
private:
    float energyEnvelope(std::vector<float> &buffer);
    float energyDifference(std::vector<float> &buffer);
    // double spectralDifference();
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
    double pi = 3.14159265358979323846;
    int odfType = 1;
    int windowType = 1;
    bool initialised;
    float prevEnergySum;
    std::vector<float> frame;
    std::vector<float> window;
};

#endif