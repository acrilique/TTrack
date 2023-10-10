/*Based on Adam Stark's BTrack. TODO LICENSING*/
#ifndef __TTRACK_H
#define __TTRACK_H

#include "OnsetDF.h"
#include <etl/array.h>
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

    float calculateMeanOfVector(float* vec, int in, int out);

    void calculateBalancedACF(float &odf);   

    void calculateCombFilterBankOutput();

    OnsetDF odf;

    etl::circular_buffer<float, 512> onsetDF;

    etl::array<float, 512> acf;
    etl::array<float, 128> combFilterBankOutput;
    etl::array<float, 128> weightingVector;
    etl::array<float, 41> delta;
    etl::array<float, 41> prevDelta;
    etl::array<float, 41> tempoObservationVector;
    etl::array<float, 512> onsetDFResampled;

    float tempoTransitionMatrix[41][41];
    float beatPeriod;
    float estimatedTempo;
    int framesUntilTempoDisplay;

};  

#endif