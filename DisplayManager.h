#ifndef DISPLAY_MANAGER_H
#define DISPLAY_MANAGER_H

#include <Arduino.h>

int initDisplay();

void clearDisplay();
void displayText(const String &text);
void displayTextln(const String &text);

void changeSelectedItem();
int getSelectedItem();
void updateMenuDisplay();

#endif