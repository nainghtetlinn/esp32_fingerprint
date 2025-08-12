#include "FingerprintManager.h"
#include "DisplayManager.h"
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

bool enrollFingerprintInBuffer(int id)
{
    Serial.println("Place finger");
    int p = -1;
    while (p != FINGERPRINT_OK)
        p = finger.getImage();
    p = finger.image2Tz(id);
    if (p == FINGERPRINT_OK)
    {
        Serial.print("Stored fingerprint at buffer# ");
        Serial.println(id);
        return true;
    }
    Serial.print("Failed to store:");
    Serial.println(p);
    return false;
}

bool checkIfFingerprintMatchedInBuffers()
{
    Serial.print("Checking if fingerprints in buffer 1 and 2 matched: ");
    if (finger.createModel() == FINGERPRINT_OK)
    {
        Serial.println("MATCHED");
        return true;
    }
    Serial.println("NOT MATCHED");
    return false;
}

bool checkIfFingerprintAlreadyEnrolled()
{
    Serial.print("Checking if merged fingerprint already enrolled: ");
    if (finger.fingerFastSearch() == FINGERPRINT_OK)
    {
        Serial.println("ENROLLED");
        return true;
    }
    Serial.println("NOT ENROLLED");
    return false;
}

bool enrollFingerprintInMemory(int id)
{
    int p = -1;
    p = finger.storeModel(id);
    if (p == FINGERPRINT_OK)
    {
        Serial.print("Stored fingerprint at ID# ");
        Serial.println(id);
        return true;
    }
    Serial.print("Failed to store :");
    Serial.println(p);
    return false;
}

void waitForFingerRemoved()
{
    int p = 0;
    while (p != FINGERPRINT_NOFINGER)
        p = finger.getImage();
}

int findFingerprintId()
{
    if (enrollFingerprintInBuffer(1) && checkIfFingerprintAlreadyEnrolled())
    {
        Serial.print("Found fingerprint at ID# ");
        Serial.println(finger.fingerID);
        return finger.fingerID;
    }
    return -1;
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

void deleteFingerprintWithId(int fpid)
{
    int p = finger.deleteModel(fpid);
    if (p == FINGERPRINT_OK)
    {
        Serial.print("Deleted fingerprint ID# ");
        Serial.println(fpid);
    }
    else
    {
        Serial.println("Deleting failed");
    }
}

void emptyFingerprintSensorDatabase()
{
    finger.emptyDatabase();
}