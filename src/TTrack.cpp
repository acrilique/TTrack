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
}