/*Based on Adam Stark's BTrack. TODO LICENSING*/
#ifndef __TTRACK_H
#define __TTRACK_H

#include "OnsetDF.h"
#include <etl/circular_buffer.h>
#include <vector>

class TTrack
{
public:
    /*Assuming hopsize=512 and framesize=1024*/
    TTrack();

    // /*Assuming framesize=2*hopsize*/
    // TTrack(int hopsize);

    // TTrack(int hopsize, int framesize);

    void processAudioFrame(float* buffer);

    void processODFSample(float sample);
    //======================================
    int getHopSize();
    //======================================

private:
    void initialise();

    void setHopSize(int hopsize);

    // void resampleODF();

    void calculateTempo();

    void adaptiveThreshold(float &buffer);

    void normaliseVector(float &buffer);

    float calculateMeanOfVector(float* vec, int in, int out);

    void calculateBalancedACF(float &odf);   

    void calculateCombFilterBankOutput();

    OnsetDF odf;

    etl::circular_buffer<float, 512> onsetDF;

    std::vector<float> acf;
    std::vector<float> combFilterBankOutput;
    std::vector<float> weightingVector;
    std::vector<float> delta;
    std::vector<float> prevDelta;
    std::vector<float> tempoObservationVector;
    std::vector<float> onsetDFResampled;

    int hopSize;
    int frameSize;

    float pi;

    float tempoTransitionMatrix[41][41];
    float beatPeriod;
    float estimatedTempo;
    int framesUntilTempoDisplay;

};  

#endif