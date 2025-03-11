#include "AccelRenderer.h"

AccelRenderer::AccelRenderer() {}

AccelRenderer::AccelRenderer(Grain &grain)
{
    this->grain = &grain;
}

void AccelRenderer::Update(float sensorValue)
{
    // MAGIC

}