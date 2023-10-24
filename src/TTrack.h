/*Based on Adam Stark's BTrack. TODO LICENSING*/
#ifndef __TTRACK_H
#define __TTRACK_H

#include "OnsetDF.h"
#include <etl/circular_buffer.h>
#include <vector>
#include <span>


class TTrack
{
public:
    /*Assuming hopsize=512 and framesize=1024*/
    TTrack();

    // /*Assuming framesize=2*hopsize*/
    // TTrack(int hopsize);

    // TTrack(int hopsize, int framesize);
    float getTempo();

    void processAudioFrame(std::vector<float> &buffer);

    void processODFSample(float sample);
    //======================================
    int getHopSize();
    //======================================
    void initialise();

private:

    void setHopSize(int hopsize);

    // void resampleODF();

    void calculateTempo();

    void adaptiveThreshold(std::vector<float> &buffer);

    void normaliseVector(std::array<float, 41> &buffer);

    float calculateMeanOfVector(std::vector<float> &buffer, int in, int out);

    void calculateBalancedACF(std::vector<float> &buffer);   


    void calculateCombFilterBankOutput();

    etl::circular_buffer<float, 512> onsetDF;

    std::array<float, 512> acf;
    std::vector<float> combFilterBankOutput;
    std::array<float, 128> weightingVector;
    std::array<float, 41> delta;
    std::array<float, 41> prevDelta;
    std::array<float, 41> tempoObservationVector;
    std::vector<float> onsetDFResampled;

    OnsetDF odf;

    int hopSize;
    int frameSize;

    float pi = 3.14159265358979323846;

    float tempoTransitionMatrix[41][41];
    float beatPeriod;
    float estimatedTempo;
    int framesUntilTempoDisplay;

};  

#endif