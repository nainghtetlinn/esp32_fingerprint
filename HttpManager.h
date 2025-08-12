#ifndef HTTP_MANAGER_H
#define HTTP_MANAGER_H

#include <Arduino.h>

int initHttp();

bool fetchNotRegisteredStudent();
bool sendRegisteration(int fpid);
bool sendAttendance(int fpid);

#endif