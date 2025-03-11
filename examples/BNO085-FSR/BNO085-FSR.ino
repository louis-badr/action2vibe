#include <Action2Vibe.h>
#include <Adafruit_BNO08x.h>
#include <DaisyDuino.h>
#include <vector>

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
std::vector<Grain *> grains;           // store pointers to grains here
std::vector<BinRenderer> binRenderers; // store vibe renderers here

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
        // average the output of all grains
        signal = 0.0f;
        for (size_t j = 0; j < grains.size(); j++)
        {
            signal += grains[j]->Process();
        }
        signal /= grains.size();
        out[0][i] = signal;
    }
}

void setup(void)
{
    Serial.begin();
    delay(1000);
    pinMode(LED_BUILTIN, OUTPUT);
    digitalWrite(LED_BUILTIN, HIGH);

    // blinks until BNO085 is found
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

    // init Daisy seed at 48kHz
    hw = DAISY.init(DAISY_SEED, AUDIO_SR_48K);
    analogReadResolution(16);

    // init grains
    grains.push_back(new OscGrain(DAISY.get_samplerate(), 350.0f, 1.0f, 12));
    grains[0]->AdjustDuration();
    grains.push_back(new OscGrain(DAISY.get_samplerate(), 80.0f, 1.0f, 12));
    // grains.push_back(new WhiteNoiseGrain(DAISY.get_samplerate(), 1.0, 12));

    // init vibe renderers
    std::vector<float> binSizes1(24, 180.0f / 24.0f);                                                         // gyro value is set to 0-180 - 24 bins of 7.5 degrees
    std::vector<float> binSizes2 = {10000.0f, 8500.0f, 7200.0f, 6100.0f, 5200.0f, 4400.0f, 3800.0f, 3200.0f}; // fsr range is approx 5000-60000 - bins get smaller because the value is not linear with force
    binRenderers.push_back(BinRenderer(*grains[0], binSizes1));
    binRenderers.push_back(BinRenderer(*grains[1], binSizes2));

    delay(1000);

    // start audio callback
    DAISY.begin(AudioCallback);

    digitalWrite(LED_BUILTIN, LOW);
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
        // feeding sensor values to BinRenderer here
        binRenderers[0].Update(fabs(ypr.roll));
    }
    binRenderers[1].Update(analogRead(A0));
}
