#ifndef FINGERPRINT_MANAGER_H
#define FINGERPRINT_MANAGER_H

int initFingerprint();

int enrollFingerprintInBuffer(int id);
int enrollFingerprintInMemory(int id);

void waitForFingerRemoved();

int findNextAvailableFingerprintId();
int findFingerprintId();

void printFingerprintSensorDetails();
void emptyFingerprintSensorDatabase();

#endif