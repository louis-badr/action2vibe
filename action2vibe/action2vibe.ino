#include <Adafruit_BNO08x.h>
#include <DaisyDuino.h>
#include <vector>

#include "OscGrain.h"
#include "VibeRenderer.h"
#include "WhiteNoiseGrain.h"

#define FAST_MODE
#define BNO08X_RESET -1

struct euler_t
{
    float yaw;
    float pitch;
    float roll;
} ypr;

Adafruit_BNO08x bno08x(BNO08X_RESET);
sh2_SensorValue_t sensorValue;
sh2_SensorId_t reportType = SH2_GYRO_INTEGRATED_RV;
long reportIntervalUs = 2000;

DaisyHardware hw;
OscGrain sineGrain1;
OscGrain sineGrain2;
WhiteNoiseGrain noiseGrain;
VibeRenderer vibeRenderer1;
VibeRenderer VibeRenderer2;

void setReports(sh2_SensorId_t reportType, long report_interval)
{
    Serial.println("Setting desired reports");
    if (!bno08x.enableReport(reportType, report_interval))
    {
        Serial.println("Could not enable stabilized remote vector");
    }
}

void quaternionToEuler(float qr, float qi, float qj, float qk, euler_t *ypr, bool degrees = false)
{
    float sqr = sq(qr);
    float sqi = sq(qi);
    float sqj = sq(qj);
    float sqk = sq(qk);

    ypr->yaw = atan2(2.0 * (qi * qj + qk * qr), (sqi - sqj - sqk + sqr));
    ypr->pitch = asin(-2.0 * (qi * qk - qj * qr) / (sqi + sqj + sqk + sqr));
    ypr->roll = atan2(2.0 * (qj * qk + qi * qr), (-sqi - sqj + sqk + sqr));

    if (degrees)
    {
        ypr->yaw *= RAD_TO_DEG;
        ypr->pitch *= RAD_TO_DEG;
        ypr->roll *= RAD_TO_DEG;
    }
}

void quaternionToEulerGI(sh2_GyroIntegratedRV_t *rotational_vector, euler_t *ypr, bool degrees = false)
{
    quaternionToEuler(rotational_vector->real, rotational_vector->i, rotational_vector->j, rotational_vector->k, ypr, degrees);
}

void AudioCallback(float **in, float **out, size_t size)
{
    float signal;
    for (size_t i = 0; i < size; i++)
    {
        signal = sineGrain1.Process() + sineGrain2.Process();
        signal /= 2;
        out[0][i] = signal;
    }
}

void setup(void)
{
    Serial.begin();
    delay(1000);
    pinMode(LED_BUILTIN, OUTPUT);
    digitalWrite(LED_BUILTIN, HIGH);

    if (!bno08x.begin_I2C())
    {
        Serial.println("Failed to find BNO08x chip");
        while (1)
        {
            digitalWrite(LED_BUILTIN, LOW);
            delay(500);
            digitalWrite(LED_BUILTIN, HIGH);
            delay(500);
        }
    }

    Serial.println("BNO08x Found!");
    setReports(reportType, reportIntervalUs);

    hw = DAISY.init(DAISY_SEED, AUDIO_SR_48K);
    std::vector<float> frequencies1 = {120.0f};
    std::vector<float> frequencies2 = {220.0f};
    std::vector<float> amplitudes = {0.5f};
    sineGrain1 = OscGrain(DAISY.get_samplerate(), frequencies1, amplitudes, 12);
    sineGrain2 = OscGrain(DAISY.get_samplerate(), frequencies2, amplitudes, 12);
    noiseGrain = WhiteNoiseGrain(0.5f, 20);
    std::vector<float> binSizes1(12, 15.0f);
    std::vector<float> binSizes2(24, 7.5f);
    vibeRenderer1 = VibeRenderer(sineGrain1, binSizes1);
    VibeRenderer2 = VibeRenderer(sineGrain2, binSizes2);

    delay(1000); // Ensures stable startup
    DAISY.begin(AudioCallback);

    digitalWrite(LED_BUILTIN, LOW); // Indicate setup completion
}

void loop()
{
    if (bno08x.wasReset())
    {
        setReports(reportType, reportIntervalUs);
    }

    if (bno08x.getSensorEvent(&sensorValue))
    {
        quaternionToEulerGI(&sensorValue.un.gyroIntegratedRV, &ypr, true);
        vibeRenderer1.Update(fabs(ypr.roll));
        VibeRenderer2.Update(fabs(ypr.yaw));
    }
}
