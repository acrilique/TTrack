#include <math.h>
#include "OnsetDF.h"

//=================================
OnsetDF::OnsetDF()
{
    initialised = false;
    initialise();
}
//=================================
void OnsetDF::initialise()
{
    initialised = true;

    switch(windowType)
    {
        case Hanning:
            // calculateHanningWindow();
            break;
        case Hamming:
            // calculateHammingWindow();
            break;
    }

    
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
    }

    return 1.0f;
}
//=================================
void OnsetDF::setODFType(int type_)
{
    odfType = type_;
}
//=================================
float OnsetDF::energyEnvelope(std::vector<float> &buffer)
{
    float sum = 0.0f;
    for(int i = 0; i < buffer.size(); i++)
    {
        sum += buffer[i] * buffer[i];
    }
    return sqrt(sum);
}
//=================================
float OnsetDF::energyDifference(std::vector<float> &buffer)
{
    float sum = 0.0f;
    for(int i = 0; i < buffer.size(); i++)
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