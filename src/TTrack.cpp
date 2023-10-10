#include "TTrack.h"

TTrack::TTrack() : odf(EnergyDifference, Hanning)
{
    initialise();
}
//================================================
void TTrack::initialise()
{
    framesUntilTempoDisplay = 1024;
    beatPeriod = 0.0f;
    estimatedTempo = 0.0f;
    
    float rayleighParameter = 43;

    tightness = 5;
    alpha = 0.9f;
    estimatedTempo = 120.0f;

    for (int i = 0; i < 128; i++)
    {
        weightingVector[i] = ((float) n / pow(rayleighParameter, 2)) * exp(-1 * pow((float) -n, 2) / (2 * pow(rayleighParameter, 2)));
    }

    for (int i = 0; i < 41; i++)
    {
        prevDelta[i] = 1.0f;
    }

    float t_mu = 41/2;
    float m_sig = 41/8;
    float x;

    for (int i = 0; i < 41; i++)
    {
        for (int j = 0; j < 41; j++)
        {
            x = j + 1;
            t_mu = i + 1;
            tempoTransitionMatrix[i][j] = (1 / (m_sig * sqrt(2 * pi))) * exp(-1 * pow((x - t_mu), 2) / (2 * pow(m_sig, 2)));
        }
    }

}
//================================================
void TTrack::processAudioFrame(double* buffer)
{
    sample = odf.calculateODFSample(buffer);
    processODFSample(sample);
}
//================================================
void TTrack::processODFSample(double sample)
{
    onsetDF.push(sample);

    if (framesUntilTempoDisplay == 0)
    {
        calculateTempo();
        framesUntilTempoDisplay = 1024;
    }
    else
    {
        framesUntilTempoDisplay--;
    }
}
//================================================
void TTrack::calculateTempo()
{
    float tempoToLagFactor = 60.0f * 44100.0f / 512.0f;
    adaptiveThreshold(onsetDF);
    calculateBalancedACF(onsetDF);
    calculateCombFilterBankOutput();
    adaptiveThreshold(combFilterBankOutput);

    for (int i = 0; i < 41; i++)
    {
        int tempoIndex1 = (int) round (tempoToLagFactor / ((float) ((2*i) + 80)));
        int tempoIndex2 = (int) round (tempoToLagFactor / ((float) ((4*i) + 160)));
        tempoObservationVector = combFilterBankOutput[tempoIndex1 - 1] + combFilterBankOutput[tempoIndex2 - 1];
    }

    for (int j = 0; j < 41; j++)
    {
        float maxValue = -1.0f;

        for (int i = 0; i < 41; i++)
        {
            float currentValue = prevDelta[i] * tempoTransitionMatrix[i][j];
            if (currentValue > maxValue)
            {
                maxValue = currentValue;
            }
        }
        delta[j] = tempoObservationVector[j] * maxValue;
    }

    normaliseVector(delta);

    float maxIndex = -1;
    float maxValue = -1;

    for (int j = 0; j < 41; j++)
    {
        if (delta[j] > maxValue)
        {
            maxValue = delta[j];
            maxIndex = j;
        }

        prevDelta[j] = delta[j];
    }

    beatPeriod = ((60.0f * 44100.0f) / (((2*maxIndex) + 80)) * (float) hopSize);

    if (beatPeriod > 0) 
    estimatedTempo = 60.0 / ((((float) hopSize) / 44100.0f) * beatPeriod);
}
//================================================
void TTrack::adaptiveThreshold(float &buffer)
{
    N = buffer.size();

    int p_post = 7;
    int p_pre = 8;

    int t = min(N, p_post);

    for (int i = 0; i <= t; i++)
    {
        int k = min(N, i + p_pre);
        threshold[i] = calculateMeanOfVector(buffer, 1, k);
    }

    for (int i = t + 1; i < N - p_post; i++)
    {
        threshold[i] = calculateMeanOfVector(buffer, i - p_pre, i + p_post);
    }

    for (int i = N - p_post; i < N; i++)
    {
        int k = max(i - p_post, 1);
        threshold[i] = calculateMeanOfVector(buffer, k, N);
    }

    for (int i = 0; i < N; i++)
    {
        buffer[i] -= threshold[i];
        
        if (buffer[i] < 0)
        {
            buffer[i] = 0;
        } 
    }

}
//================================================
void TTrack::normaliseVector(float &buffer)
{
    float sum = 0.0f;

    for (int i = 0; i < buffer.size(); i++)
    {
        sum += buffer[i];
    }

    for (int i = 0; i < buffer.size(); i++)
    {
        buffer[i] /= sum;
    }
}
//================================================
float TTrack::calculateMeanOfVector(float* vec, int in, int out)
{
    int length = out - in;
    float sum = 0.0f;

    for (int i = in; i < out; i++)
    {
        sum += vec[i];
    }

    if (length > 0)
    {
        return (float) sum / length;
    }
    else
    {
        return 0.0f;
    }
}
//================================================
void TTrack::calculateBalancedACF(float &odf)
{
    int l, n = 0;
    float sum, tmp;

    for (l = 0; l < 512; l++)
    {
        sum = 0.0f;

        for (n = 0; n < 512 - l; n++)
        {
            tmp = odf[n] * odf[n + l];
            sum += tmp;
        }

        acf[l] = sum / (512 - l);
    }
}
//================================================
void TTrack::calculateCombFilterBankOutput()
{
    int numCombElements = 4;

    for (int i = 2; i <= 127; i++)
    {
        for (int a = 1; a <= numCombElements; a++)
        {
            for (int b = 1 - a; b <= a - 1; b++)
            {
                combFilterBankOutput[i - 1] += (acf[(a * i + b) - 1] * weightingVector[i - 1]) / (2 * a - 1);
            }
        }
    }
}
