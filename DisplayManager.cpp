#include "DisplayManager.h"
#include <Adafruit_SSD1306.h>
#include <Wire.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
#define SCREEN_ADDRESS 0x3C

const int NUM_ITEMS = 2;
const int MAX_ITEM_LENGTH = 20;

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

char menu_items[NUM_ITEMS][MAX_ITEM_LENGTH] = {
    {"Attendance"},
    {"Register"},
};

int item_selected = 1;

int initDisplay()
{
    if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS))
    {
        Serial.println(F("SSD1306 allocation failed"));
        return -1;
    }
    else
    {
        display.clearDisplay();
        display.setTextSize(1);
        display.setTextColor(WHITE);
        display.display();
        return 0;
    }
}

void clearDisplay()
{
    display.clearDisplay();
    display.setCursor(0, 0);
}
void displayTextln(const String &text)
{
    display.println(text);
    display.display();
}
void displayText(const String &text)
{
    display.print(text);
    display.display();
}

void changeSelectedItem()
{
    item_selected = item_selected + 1; // select next item
    if (item_selected >= NUM_ITEMS)
        item_selected = 0; // last item was selected, jump to first menu item

    updateMenuDisplay();
}

int getSelectedItem()
{
    return item_selected;
}

void updateMenuDisplay()
{
    clearDisplay();
    for (int i = 0; i < NUM_ITEMS; i++)
    {
        if (i == item_selected)
            display.setTextColor(BLACK, WHITE);
        else
            display.setTextColor(WHITE);
        display.println(menu_items[i]);
    }
    display.display();
    display.setTextColor(WHITE); // Reset to default
}