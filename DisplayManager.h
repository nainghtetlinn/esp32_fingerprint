#ifndef DISPLAY_MANAGER_H
#define DISPLAY_MANAGER_H

#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
#define SCREEN_ADDRESS 0x3C

const int NUM_ITEMS = 2;
const int MAX_ITEM_LENGTH = 20;

extern char menu_items[NUM_ITEMS][MAX_ITEM_LENGTH];

extern Adafruit_SSD1306 display;

int initDisplay();
void displayText(const char *text);
void updateMenuDisplay(int selected);

#endif