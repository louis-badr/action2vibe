#ifndef Grain_h
#define Grain_h

class Grain
{
private:
    bool isPlaying;
    unsigned long endTime;
    float duration;

public:
    virtual float Process() = 0;
    virtual void Play() = 0;
};

#endif