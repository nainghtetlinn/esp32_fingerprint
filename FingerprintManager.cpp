#include "FingerprintManager.h"
#include <Adafruit_Fingerprint.h>
#include <HardwareSerial.h>

HardwareSerial mySerial(2);
Adafruit_Fingerprint finger(&mySerial);

int initFingerprint()
{
    mySerial.begin(57600, SERIAL_8N1, 16, 17);
    delay(1000);
    if (!finger.verifyPassword())
    {
        Serial.println("Did not find fingerprint sensor :(");
        return -1;
    }
    else
    {
        return 0;
    }
}

int enrollFingerprintInBuffer(int id)
{
    int p = -1;

    while (p != FINGERPRINT_OK)
        p = finger.getImage();
    p = finger.image2Tz(id);
    if (p != FINGERPRINT_OK)
    {
        Serial.print("Failed to convert image to template :");
        Serial.println(p);
        return -1;
    }

    return 0;
}

int enrollFingerprintInMemory(int id)
{
    int p = -1;
    p = finger.createModel();
    if (p != FINGERPRINT_OK)
    {
        Serial.print("Fingerprints didn't match :");
        Serial.println(p);
        return -1;
    }

    p = finger.storeModel(id);
    if (p != FINGERPRINT_OK)
    {
        Serial.print("Failed to store :");
        Serial.println(p);
        return -1;
    }
    Serial.print("Stored fingerprint at ID# ");
    Serial.println(id);
    return 0;
}

void waitForFingerRemoved()
{
    int p = 0;
    while (p != FINGERPRINT_NOFINGER)
        p = finger.getImage();
}

int findFingerprintId()
{
    int p = -1;
    while (p != FINGERPRINT_OK)
        p = finger.getImage();
    p = finger.image2Tz();
    if (p != FINGERPRINT_OK)
    {
        Serial.print("Failed to convert image to template :");
        Serial.println(p);
        return -1;
    }

    p = finger.fingerFastSearch();
    if (p != FINGERPRINT_OK)
    {
        Serial.print("Fingerprint not found :");
        Serial.println(p);
        return -1;
    }

    Serial.print("Found fingerprint at ID# ");
    Serial.println(finger.fingerID);
    return finger.fingerID;
}

int findNextAvailableFingerprintId()
{
    for (int id = 1; id < 127; id++)
    {
        if (finger.loadModel(id) != FINGERPRINT_OK)
            return id;
    }
    return -1;
}

void printFingerprintSensorDetails()
{
    Serial.println(F("Reading sensor parameters"));
    finger.getParameters();
    Serial.print(F("Status: 0x"));
    Serial.println(finger.status_reg, HEX);
    Serial.print(F("Sys ID: 0x"));
    Serial.println(finger.system_id, HEX);
    Serial.print(F("Capacity: "));
    Serial.println(finger.capacity);
    Serial.print(F("Security level: "));
    Serial.println(finger.security_level);
    Serial.print(F("Device address: "));
    Serial.println(finger.device_addr, HEX);
    Serial.print(F("Packet len: "));
    Serial.println(finger.packet_len);
    Serial.print(F("Baud rate: "));
    Serial.println(finger.baud_rate);

    finger.getTemplateCount();
    Serial.print(finger.templateCount);
    Serial.println(" templates");
}

void emptyFingerprintSensorDatabase()
{
    finger.emptyDatabase();
}