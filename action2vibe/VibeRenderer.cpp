#include "VibeRenderer.h"

VibeRenderer::VibeRenderer() {}

VibeRenderer::VibeRenderer(OscGrain &grain, std::vector<float> &binSizes)
{
    this->grain = &grain;
    this->binSizes = binSizes;
    // binValues is cumulative sum of binSizes
    binValues.push_back(binSizes[0]);
    for (int i = 1; i < binSizes.size(); i++)
    {
        binValues.push_back(binValues[i - 1] + binSizes[i]);
    }
    currentBin = -1;
}

void VibeRenderer::Update(float sensorValue)
{
    // if (sensorValue >= 170.0 && sensorValue <= 171.0)
    // {
    //     grain->Play();
    // }

    // if currentBin is -1, find the bin that sensorValue falls into
    if (currentBin == -1)
    {
        for (int i = 0; i < binValues.size(); i++)
        {
            if (sensorValue < binValues[i])
            {
                currentBin = i;
                break;
            }
        }
    }
    // if currentBin is not -1, check if sensorValue is outside of current bin
    else
    {
        if (currentBin > 0 && sensorValue < binValues[currentBin - 1])
        {
            grain->Play();
            currentBin--;
        }
        else if (currentBin < binValues.size() - 1 && sensorValue >= binValues[currentBin])
        {
            grain->Play();
            currentBin++;
        }
    }
}
