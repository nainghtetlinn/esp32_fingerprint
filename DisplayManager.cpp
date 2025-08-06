#include "DisplayManager.h"

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

char menu_items[NUM_ITEMS][MAX_ITEM_LENGTH] = {
    {"Attendance"},
    {"Register"},
};

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

void displayText(const char *text)
{
    display.clearDisplay();
    display.setCursor(0, 0);
    display.println(text);
    display.display();
}

void updateMenuDisplay(int selected)
{
    display.clearDisplay();
    display.setCursor(0, 0);
    for (int i = 0; i < NUM_ITEMS; i++)
    {
        if (i == selected)
            display.setTextColor(BLACK, WHITE);
        else
            display.setTextColor(WHITE);
        display.println(menu_items[i]);
    }
    display.display();
    display.setTextColor(WHITE); // Reset to default
}