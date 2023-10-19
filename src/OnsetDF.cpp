#include <math.h>
#include "OnsetDF.h"

//=================================
OnsetDF::OnsetDF()
{
    initialised = false;
    pi = 3.14159265358979323846264338327950288;
    initialise();
}
//=================================
void OnsetDF::initialise()
{
    initialised = true;

    switch(windowType)
    {
        case Hanning:
            calculateHanningWindow();
            break;
        case Hamming:
            calculateHammingWindow();
            break;
    }

    
}
//=================================
float OnsetDF::calculateODFSample(const std::array<float, framesize> &buffer)
{
    if(!initialised)
    {
        return 0.0f;
    }

    switch(odfType)
    {
        case EnergyEnvelope:
            return energyEnvelope();
            break;
        case EnergyDifference:
            return energyDifference();
            break;
    }

    return 1.0f;
}
//=================================
void OnsetDF::setODFType(int type_)
{
    odfType = type_;
}
//=================================
float OnsetDF::energyEnvelope()
{
    float sum = 0.0f;
    for(int i = 0; i < framesize; i++)
    {
        sum += frame[i] * frame[i];
    }
    return sqrt(sum);
}
//=================================
float OnsetDF::energyDifference()
{
    float sum = 0.0f;
    for(int i = 0; i < framesize; i++)
    {
        sum += frame[i] * frame[i];
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
void OnsetDF::calculateHammingWindow()
{
    float N = framesize - 1.0f;
    float n_val = 0.0f;
    for(int i = 0; i < framesize; i++)
    {
        window[i] = 0.54f - 0.46f * cos(2.0f * pi * (n_val / N));
        n_val += 1.0f;
    }
}
//=================================
void OnsetDF::calculateHanningWindow()
{
    float N = framesize - 1.0f;
    for(int i = 0; i < framesize; i++)
    {
        window[i] = 0.5f * (1.0f - cos(2.0f * pi * (i / N)));
    }
}