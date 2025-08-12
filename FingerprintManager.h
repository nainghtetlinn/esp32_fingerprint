#ifndef FINGERPRINT_MANAGER_H
#define FINGERPRINT_MANAGER_H

int initFingerprint();

int findNextAvailableFingerprintId();
bool enrollFingerprintInBuffer(int id);
bool checkIfFingerprintMatchedInBuffers();
bool checkIfFingerprintAlreadyEnrolled();
bool enrollFingerprintInMemory(int id);

void waitForFingerRemoved();
int findFingerprintId();

void printFingerprintSensorDetails();
void deleteFingerprintWithId(int fpid);
void emptyFingerprintSensorDatabase();

#endif