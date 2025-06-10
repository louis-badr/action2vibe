#include "AccelRenderer.h"

float mapFloat(float x, float in_min, float in_max, float out_min, float out_max)
{
    return out_min + (out_max - out_min) * ((x - in_min) / (in_max - in_min));
}

AccelRenderer::AccelRenderer() {}

AccelRenderer::AccelRenderer(Grain &grain)
{
    this->grain = &grain;
    this->frequency = 0;
    this->previousTime = -1;
}

void AccelRenderer::Update(float sensorValue)
{
    if (previousTime == -1)
    {
        previousSensorValue = sensorValue;
        previousTime = micros();
    }
    float sensorValueDiff = fabs(sensorValue - previousSensorValue);
    frequency = mapFloat(sensorValueDiff, 0, 180, 0, 500);
    if (frequency != 0 && micros() - previousTime >= 1000000.0 / frequency)
    {
        grain->Play();
        previousTime = micros();
        Serial.print(sensorValue);
        Serial.print("\t");
        Serial.print(sensorValueDiff);
        Serial.print("\t");
        Serial.println(frequency);
    }
    previousSensorValue = sensorValue;
}