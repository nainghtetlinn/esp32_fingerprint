#include "DisplayManager.h"
#include "FingerprintManager.h"
#include "HttpManager.h"
#include <OneButton.h>

#define BUTTON_PIN 18
OneButton btn;

void setup()
{
    Serial.begin(115200);

    int result = initDisplay();
    if (result < 0)
        while (1)
            delay(10);

    clearDisplay();
    displayTextln("Initializing ...");

    result = -1;
    result = initFingerprint();
    if (result < 0)
    {
        displayTextln("Fingerprint sensor not found!");
        while (1)
            delay(10);
    }

    clearDisplay();
    displayTextln("Connecting to WiFi");
    displayTextln("...");
    result = -1;
    result = initHttp();
    if (result < 0)
    {
        displayTextln("Failed!");
        while (1)
            delay(10);
    }
    displayTextln("Connected!");

    btn.setup(BUTTON_PIN, INPUT_PULLUP, true);
    btn.attachClick(handleClick);
    btn.attachDoubleClick(handleDoubleClick);
    btn.attachLongPressStart(handleLongPress);

    delay(2000);
    updateMenuDisplay();
}

void loop()
{
    btn.tick();
}

static void handleClick()
{
    int item_selected = getSelectedItem();

    clearDisplay();

    if (item_selected == 0) // Attendance
        displayAttendance();
    else if (item_selected == 1) // Register
        displayRegister();

    delay(2000);
    updateMenuDisplay();
}

static void handleDoubleClick()
{
    changeSelectedItem();
}

static void handleLongPress()
{
    printFingerprintSensorDetails();
    emptyFingerprintSensorDatabase();
}

void displayRegister()
{
    int id = findNextAvailableFingerprintId();

    if (id < 0)
    {
        displayTextln("No empty ID available");
        return;
    }

    displayTextln("Place finger to register...");
    int result1 = findFingerprintId();
    if (result1 > 0)
    {
        clearDisplay();
        displayTextln("Already registered");
        displayText("ID# ");
        displayTextln(String(result1));
        return;
    }

    int buffer1 = enrollFingerprintInBuffer(1);
    if (buffer1 != 0)
    {
        displayTextln("Try again!");
        return;
    }

    clearDisplay();
    displayTextln("Remove finger");
    waitForFingerRemoved();

    clearDisplay();
    displayTextln("Place the same finger again for confirmation...");
    int buffer2 = enrollFingerprintInBuffer(2);
    if (buffer2 != 0)
    {
        clearDisplay();
        displayTextln("Try again!");
        return;
    }

    int result2 = enrollFingerprintInMemory(id);
    if (result2 != 0)
    {
        clearDisplay();
        displayTextln("Error");
        return;
    }

    clearDisplay();
    displayTextln("Success");
    displayText("ID# ");
    displayTextln(String(id));
}

void displayAttendance()
{
    displayTextln("Place finger for attendance...");
    int result = findFingerprintId();
    if (result < 0)
    {
        displayTextln("Not found!");
        return;
    }

    clearDisplay();
    displayTextln("Found!");
    displayText("ID# ");
    displayTextln(String(result));
}