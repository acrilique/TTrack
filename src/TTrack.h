/*Based on Adam Stark's BTrack. TODO LICENSING*/
#ifndef __TTRACK_H
#define __TTRACK_H

#include "OnsetDF.h"
#include <vector>
#include <array>
#include "CircularBuffer.h"

class TTrack
{
public:
    TTrack();

    float getTempo();

    void processAudioFrame(std::vector<float> &buffer);

    void processODFSample(float sample);

    void initialise();

private:

    // void resampleODF();

    void calculateTempo();

    void adaptiveThreshold(std::vector<float> &buffer);

    void normaliseVector(std::array<float, 41> &buffer);

    float calculateMeanOfVector(std::vector<float> &buffer, int in, int out);

    void calculateBalancedACF(std::vector<float> &buffer);   

    void calculateCombFilterBankOutput(std::vector<float> &buffer);

    CircularBuffer onsetDF;

    std::array<float, 512> acf;
    std::vector<float> combFilterBankOutput;
    std::array<float, 128> weightingVector;
    std::array<float, 41> delta;
    std::array<float, 41> prevDelta;
    std::array<float, 41> tempoObservationVector;
    std::vector<float> onsetDFResampled;

    OnsetDF odf;

    int hopSize;

    float pi;
    float tempoTransitionMatrix[41][41];
    float beatPeriod;
    float estimatedTempo;
    int framesUntilTempoDisplay;

};  

#endif