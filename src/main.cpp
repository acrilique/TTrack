#include "daisysp.h"
#include "daisy_pod.h"
#include "TTrack.h" 
#include <array> 

// Set max delay time to 0.75 of samplerate.
#define MAX_DELAY static_cast<size_t>(48000 * 2.5f)
#define REV 0
#define DEL1 1
#define DEL2 2

using namespace daisysp;
using namespace daisy;

static DaisyPod pod;
static TTrack ttrack;  

static ReverbSc                                  rev;
static DelayLine<float, MAX_DELAY> DSY_SDRAM_BSS dell;
static DelayLine<float, MAX_DELAY> DSY_SDRAM_BSS delr;
static Tone                                      tone;
static Parameter deltime, cutoffParam;
int              mode = REV;

float currentDelay, feedback, delayTarget, cutoff, drywet;
float discDelay, currentTempo;  

std::vector<float> audioAnalysysBuffer;  
size_t bufferIndex = 0; 

//Helper functions
void Controls();

void GetReverbSample(float &outl, float &outr, float inl, float inr);

void GetDelaySample(float &outl, float &outr, float inl, float inr);

void AudioCallback(AudioHandle::InterleavingInputBuffer  in,
                   AudioHandle::InterleavingOutputBuffer out,
                   size_t                                size)
{
    float outl, outr, inl, inr;

    Controls();
	//audio
    for(size_t i = 0; i < size; i += 2)
    {
        inl = in[i];
        inr = in[i + 1];
        
        audioAnalysysBuffer[bufferIndex++] = (inl + inr) / 2.0f;
        if (bufferIndex == audioAnalysysBuffer.size())
        {
        
            bufferIndex = 0;
        	ttrack.processAudioFrame(audioAnalysysBuffer);  //============================= PROBLEM=====
            currentTempo = ttrack.getTempo();  
        } 
		switch(mode)
    	{
            case REV: GetReverbSample(outl, outr, inl, inr); break;
            case DEL1: GetDelaySample(outl, outr, inl, inr); break;
			case DEL2: GetDelaySample(outl, outr, inl, inr); break;
            default: outl = outr = 0;
        }
        // left out
        out[i] = outl;

        // right out
        out[i + 1] = outr;
     
    }


}

int main(void)
{
    // initialize pod hardware and oscillator daisysp module
    float sample_rate;

    //Inits and sample rate
    pod.Init();
    pod.SetAudioBlockSize(4);
    sample_rate = pod.AudioSampleRate();
    rev.Init(sample_rate);
    dell.Init();
    delr.Init();
    tone.Init(sample_rate);

    discDelay = 1.0f;
    currentTempo = 120.0f;
    audioAnalysysBuffer.resize(512);
    std::fill(audioAnalysysBuffer.begin(), audioAnalysysBuffer.end(), 0.0f); //optional
    ttrack.initialise();

    //set parameters
    deltime.Init(pod.knob1, sample_rate * .05, MAX_DELAY, deltime.LOGARITHMIC);
    cutoffParam.Init(pod.knob1, 500, 20000, cutoffParam.LOGARITHMIC);

    //reverb parameters
    rev.SetLpFreq(18000.0f);
    rev.SetFeedback(0.85f);

    //delay parameters
    currentDelay = delayTarget = sample_rate * 0.75f;
    dell.SetDelay(currentDelay);
    delr.SetDelay(currentDelay);

    // start callback
    pod.StartAdc();
    pod.StartAudio(AudioCallback);

    while(1) {

         
        if (mode == DEL2) 
        { 
            bool b1 = pod.button1.Pressed(); 
            bool b2 = pod.button2.Pressed(); 
            if (b1 && !b2) 
            { 
                if (discDelay > 0.125f) 
                { 
                    discDelay /= 2.0f; 
                    delayTarget = discDelay/currentTempo; 
                } 
            } 
            else if (b2 && !b1) 
            { 
                if (discDelay < 2.0f) 
                { 
                    discDelay *= 2.0f; 
                    delayTarget = discDelay/currentTempo; 
                } 
            } 
            
        } 
         
    }
}

void UpdateKnobs(float &k1, float &k2)
{
    k1 = pod.knob1.Process();
    k2 = pod.knob2.Process();

    switch(mode)
    {
        case REV:
            drywet = k1;
            rev.SetFeedback(k2);
            break;
        case DEL1:
            delayTarget = deltime.Process();
            feedback    = k2;
            break;
        case DEL2: 
            feedback = k2; 
            break; 
    }
}

void UpdateEncoder()
{
    mode = mode + pod.encoder.Increment();
    mode = (mode % 3 + 3) % 3;
}

void UpdateLeds(float k1, float k2)
{
    pod.led1.Set(
        k1 * (mode == 2), k1 * (mode == 1), k1 * (mode == 0 || mode == 2));
    pod.led2.Set(
        k2 * (mode == 2), k2 * (mode == 1), k2 * (mode == 0 || mode == 2));

    pod.UpdateLeds();
}

void Controls()
{
    float k1, k2;
    delayTarget = feedback = drywet = 0;

    pod.ProcessAnalogControls();
    pod.ProcessDigitalControls();

    UpdateKnobs(k1, k2);

    UpdateEncoder();

    UpdateLeds(k1, k2);
}

void GetReverbSample(float &outl, float &outr, float inl, float inr)
{   
    rev.Process(inl, inr, &outl, &outr);
    outl = drywet * outl + (1 - drywet) * inl;
    outr = drywet * outr + (1 - drywet) * inr;
}

void GetDelaySample(float &outl, float &outr, float inl, float inr)
{
    fonepole(currentDelay, delayTarget, .00007f);
    delr.SetDelay(currentDelay);
    dell.SetDelay(currentDelay);
    outl = dell.Read();
    outr = delr.Read();

    dell.Write((feedback * outl) + inl);
    outl = (feedback * outl) + ((1.0f - feedback) * inl);

    delr.Write((feedback * outr) + inr);
    outr = (feedback * outr) + ((1.0f - feedback) * inr);
}
