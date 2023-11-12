#include <math.h>
#include "OnsetDF.h"

//=================================
OnsetDF::OnsetDF()
{
    initialised = false;
    initialise();
}
//=================================
OnsetDF::~OnsetDF()
{
    if (initialised)
    {
        freeFFT();
    }
}
//=================================
void OnsetDF::initialise()
{
    pi = 3.14159265358979;
    odfType = EnergyDifference;
    windowType = Hamming;
    magSpec.resize(512);
    prevMagSpec.resize(512);
    
    switch(windowType)
    {
        case Hanning:
            // calculateHanningWindow();
            break;
        case Hamming:
            calculateHammingWindow(512);
            break;
    }

    
}
//=================================
void OnsetDF::freeFFT()
{
    delete[] fftIn;
    delete[] fftOut;
    free(cfg);
}
//=================================
float OnsetDF::calculateODFSample(std::vector<float> &buffer)
{
    if(!initialised)
    {
        return 0.0f;
    }

    switch(odfType)
    {
        case EnergyEnvelope:
            return energyEnvelope(buffer);
            break;
        case EnergyDifference:
            return energyDifference(buffer);
            break;
        case SpectralDifference:
            return spectralDifference(buffer);
            break;
    }

    return 1.0f;
}
//=================================
void OnsetDF::initialiseFFT()
{
    if (initialised)
    {
        freeFFT();
    }

    complexOut.resize(512);

    for (int i = 0; i < 512; i++)
    {
        complexOut[i].resize(2);
    }

    fftIn = new kiss_fft_cpx[512];
    fftOut = new kiss_fft_cpx[512];
    cfg = kiss_fft_alloc(512, 0, fft_alloc_mem, &fft_mem_len);

    initialised = true;
}
//=================================
void OnsetDF::setODFType(int type_)
{
    odfType = type_;
}
//=================================
void OnsetDF::performFFT(std::vector<float> &buffer)
{
    int fsize2 = 256;

    for (int i = 0; i < fsize2; i++)
    {
        fftIn[i].r = buffer[i + fsize2] * window[i + fsize2];
        fftIn[i].i = 0.0f;
        fftIn[i + fsize2].r = buffer[i] * window[i];
        fftIn[i + fsize2].i = 0.0f;
    }

    kiss_fft(cfg, fftIn, fftOut);

    for (int i = 0; i < 512; i++)
    {
        complexOut[i][0] = fftOut[i].r;
        complexOut[i][1] = fftOut[i].i;
    }
}
//=================================
//=================================
float OnsetDF::energyEnvelope(std::vector<float> &buffer)
{
    float sum = 0.0f;
    for(size_t i = 0; i < buffer.size(); i++)
    {
        sum += buffer[i] * buffer[i];
    }
    return sqrt(sum);
}
//=================================
float OnsetDF::energyDifference(std::vector<float> &buffer)
{
    float sum = 0.0f;
    for(size_t i = 0; i < buffer.size(); i++)
    {
        sum += buffer[i] * buffer[i];
    }
    float sample = sum - prevEnergySum;
    prevEnergySum = sum;
    if (sample < 0.0f)
    {
        sample = 0.0f;
    }
    return sample;
}
//=================================
float OnsetDF::spectralDifference(std::vector<float> &buffer)
{
    float diff;
    float sum;

    performFFT(buffer);

    for (int i = 0; i < 256 + 1; i++)
    {
        magSpec[i] = sqrt( pow (complexOut[i][0], 2) + pow (complexOut[i][1], 2));
    }

    for (int i = 256 + 1; i < 512; i++)
    {
        magSpec[i] = magSpec[512 - i];
    }

    sum = 0.0f;

    for (int i = 0; i < 512; i++)
    {
        diff = magSpec[i] - prevMagSpec[i];

        if (diff < 0.0f)
        {
            diff = diff * -1.0f;
        }

        sum = sum + diff;

        prevMagSpec[i] = magSpec[i];
    }

    return sum;

}
//=================================
void OnsetDF::calculateHammingWindow(int size)
{
    float N = size - 1.0f;
    float n_val = 0.0f;
    for(int i = 0; i < size; i++)
    {
        window[i] = 0.54f - 0.46f * cos(2.0f * pi * (n_val / N));
        n_val += 1.0f;
    }
}
//=================================
void OnsetDF::calculateHanningWindow(int size)
{
    float N = size - 1.0f;
    for(int i = 0; i < size; i++)
    {
        window[i] = 0.5f * (1.0f - cos(2.0f * pi * (i / N)));
    }
}