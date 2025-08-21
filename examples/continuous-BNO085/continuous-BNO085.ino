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
ContinuousRenderer cRenderer;

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
        out[0][i] = cRenderer.Process();
    }
}

void setup(void)
{
    Serial.begin();
    delay(1000);
    pinMode(LED_BUILTIN, OUTPUT);
    digitalWrite(LED_BUILTIN, HIGH);

    // LED off when BNO08x is found
    if (!bno08x.begin_I2C())
    {
        Serial.println("Failed to find BNO08x chip");
    }
    Serial.println("BNO08x Found!");
    setReports(reportType, reportIntervalUs);

    // init Daisy seed at 48kHz
    hw = DAISY.init(DAISY_SEED, AUDIO_SR_48K);

    // init bin renderers
    std::vector<float> binSizes(12, 360.0f / 12.0f); // gyro value is set to 0-180 - 24 bins of 7.5 degrees
    cRenderer = ContinuousRenderer(DAISY.get_samplerate(), 15.0f, 65.0f, 170.0f, binSizes);

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
        // feeding sensor value to renderer
        cRenderer.Update(ypr.roll);
    }
}
