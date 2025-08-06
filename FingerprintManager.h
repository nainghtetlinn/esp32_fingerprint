#ifndef FINGERPRINT_MANAGER_H
#define FINGERPRINT_MANAGER_H

#include <Adafruit_Fingerprint.h>
#include <HardwareSerial.h>

extern Adafruit_Fingerprint finger;

int initFingerprint();
int enrollFingerprintInBuffer(int id);
int enrollFingerprintInMemory(int id);
void waitForFingerRemoved();
int findFingerprintId();
int findNextAvailableId();

#endif