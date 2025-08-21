#include "ContinuousRenderer.h"
#include <DaisyDuino.h>

ContinuousRenderer::ContinuousRenderer() {}

ContinuousRenderer::ContinuousRenderer(float sample_rate, float grainSize, float minFreq, float maxFreq, std::vector<float> &binSizes)
{
    this->minFreq = minFreq;
    this->maxFreq = maxFreq;
    this->frequency = 120.0f;
    this->amplitude = 0.0f;
    this->grainSize = grainSize;
    this->binSizes = binSizes;
    // binValues is cumulative sum of binSizes
    binValues.push_back(-180.0f);
    for (int i = 1; i < binSizes.size() + 1; i++)
    {
        binValues.push_back(binValues[i - 1] + binSizes[i - 1]);
    }
    Serial.print("Bin values: ");
    for (const auto &value : binValues)
    {
        Serial.print(value);
        Serial.print(" ");
    }
    Serial.println();
    currentBin = -1;
    previousSensorValue = 0.0f;
    currentTime = 0;
    previousTime = 0;
    speed = 0.0f;
    smoothedSpeed = 0.0f;
    osc.Init(sample_rate);
    osc.SetFreq(frequency);
    osc.SetAmp(0.0f);
    osc.SetWaveform(osc.WAVE_SIN);
}

void ContinuousRenderer::Update(float sensorValue)
{
    // find the current bin based on sensorValue
    for (int i = 0; i < binValues.size(); i++)
    {
        if (sensorValue < binValues[i])
        {
            currentBin = i;
            break;
        }
    }
    // calculate amplitude based on distance to next bin
    float distanceToNextBin = GetDistanceToClosestBin(sensorValue);
    if (distanceToNextBin > grainSize)
    {
        osc.SetAmp(0.0f);
    }
    else
    {
        float amplitude = sq(distanceToNextBin - grainSize) / sq(grainSize); // quadratic mapping of distance to amplitude
        // float amplitude = 1.0f - (distanceToNextBin / grainSize); // linear mapping of distance to amplitude
        // Serial.print("Amplitude: ");
        // Serial.print(amplitude);
        // Serial.print("| Sensor Value: ");
        // Serial.println(sensorValue);
        // Serial.print(" | Lower bin value: ");
        // Serial.print(binValues[currentBin - 1]);
        // Serial.print(" | Upper bin value: ");
        // Serial.print(binValues[currentBin]);
        // Serial.print(" | Distance to closest bin: ");
        // Serial.println(distanceToNextBin);
        osc.SetAmp(amplitude);
    }
    // calculate frequency based on speed of change in sensorValue
    currentTime = micros();                                             // Get current time in microseconds
    float deltaTime = (float)(currentTime - previousTime) / 1000000.0f; // convert to seconds
    float sensorValueDiff = fabs(fabs(sensorValue) - fabs(previousSensorValue));
    speed = sensorValueDiff / deltaTime;                       // speed of change in sensorValue
    smoothedSpeed = 0.04 * speed + (1 - 0.04) * smoothedSpeed; // simple low-pass filter for speed
    float freqq = 0.0f;
    float amplitude = 0.0f;
    if (smoothedSpeed > 4.0f && currentBin > 0)
    {
        freqq = maxFreq * (smoothedSpeed - 4.0f) / (180.0f - 4.0f); // linear mapping of speed to frequency

        // freqq = maxFreq * log(smoothedSpeed) / log(180.0f); // logarithmic - map speed to frequency
        // freqq = maxFreq * sqrt(speed) / sqrt(180.0f); // square root - map speed to frequency

        // try with amplitude modulation - map between 0 and 1
        // amplitude = 1.0f * (smoothedSpeed - 4.0f) / (180.0f - 4.0f);
    }
    Serial.print(0);
    Serial.print(", ");
    Serial.print(180);
    Serial.print(", ");
    Serial.print(smoothedSpeed);
    Serial.print(", ");
    Serial.println(freqq);
    osc.SetFreq(freqq);
    previousSensorValue = sensorValue;
    previousTime = micros();
}

float ContinuousRenderer::Process()
{
    return osc.Process();
}

float ContinuousRenderer::GetDistanceToNextBin(float sensorValue)
{
    return binValues[currentBin] - sensorValue;
}

float ContinuousRenderer::GetDistanceToPreviousBin(float sensorValue)
{
    return sensorValue - binValues[currentBin - 1];
}

float ContinuousRenderer::GetDistanceToClosestBin(float sensorValue)
{
    return min(GetDistanceToNextBin(sensorValue), GetDistanceToPreviousBin(sensorValue));
}