/*Based on Adam Stark's BTrack. TODO LICENSING*/
#ifndef __TTRACK_H
#define __TTRACK_H

#include "OnsetDF.h"
#include "ringbuffer.h"
#include <vector>

class TTrack
{
public:
    /*Assuming hopsize=512 and framesize=1024*/
    TTrack();

    // /*Assuming framesize=2*hopsize*/
    // TTrack(int hopsize);

    // TTrack(int hopsize, int framesize);

    void processAudioFrame(double* buffer);

    void processODFSample(double sample);
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

    void calculateBalancedACF(float &odf);   

    void calculateCombFilterBankOutput();

    OnsetDF odf;

    //RingBuffer<float, 512> onsetDF;

    std::array<float, 512> acf;
    std::array<float, 128> combFilterBankOutput;
    std::array<float, 41> delta;
    std::array<float, 41> prevDelta;
    std::array<float, 41> tempoObservationVector;
    std::array<float, 512> onsetDFResampled;

    float tempoTransitionMatrix[41][41];
    float beatPeriod;
    float estimatedTempo;
    int framesUntilTempoDisplay;

};  

#endif