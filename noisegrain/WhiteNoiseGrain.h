#ifndef WhiteNoiseGrain_h
#define WhiteNoiseGrain_h

#include <DaisyDuino.h>

class WhiteNoiseGrain {
    private:
        WhiteNoise nse;
        bool isPlaying;
        int duration;
        float amplitude;
        unsigned long startTime;
    public:
        WhiteNoiseGrain();
        float Process();
        void Play(int duration, float amplitude);
};

#endif