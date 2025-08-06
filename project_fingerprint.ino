#include "DisplayManager.h"
#include "FingerprintManager.h"
#include <OneButton.h>

#define BUTTON_PIN 18
OneButton btn;

int item_selected = 1;

void setup()
{
    Serial.begin(115200);

    int result = initDisplay();
    if (result < 0)
        while (1)
            delay(10);

    displayText("Initializing ...");

    result = -1;
    result = initFingerprint();
    if (result < 0)
    {
        displayText("Fingerprint sensor not found!");
        while (1)
            delay(10);
    }

    btn.setup(BUTTON_PIN, INPUT_PULLUP, true);
    btn.attachClick(handleClick);
    btn.attachDoubleClick(handleDoubleClick);
    btn.attachLongPressStart(handleLongPress);

    updateMenuDisplay(item_selected);
}

void loop()
{
    btn.tick();
}

static void handleClick()
{
    if (item_selected == 0) // Attendance
        displayAttendance();
    else if (item_selected == 1) // Register
        displayRegister();

    delay(2000);
    updateMenuDisplay(item_selected);
}

static void handleDoubleClick()
{
    item_selected = item_selected + 1; // select next item
    if (item_selected >= NUM_ITEMS)
        item_selected = 0; // last item was selected, jump to first menu item

    updateMenuDisplay(item_selected);
}

static void handleLongPress()
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

    finger.emptyDatabase();
}

void displayRegister()
{
    int id = findNextAvailableId();

    if (id < 0)
    {
        displayText("No empty ID available");
        return;
    }

    displayText("Place finger to register...");
    int result1 = findFingerprintId();
    if (result1 > 0)
    {
        displayText("Already registered");
        display.print("ID# ");
        display.println(result1);
        display.display();
        return;
    }

    int buffer1 = enrollFingerprintInBuffer(1);
    if (buffer1 != 0)
    {
        displayText("Try again!");
        return;
    }

    displayText("Remove finger");
    waitForFingerRemoved();

    displayText("Place the same finger again for confirmation...");
    int buffer2 = enrollFingerprintInBuffer(2);
    if (buffer2 != 0)
    {
        displayText("Try again!");
        return;
    }

    int result2 = enrollFingerprintInMemory(id);
    if (result2 != 0)
    {
        displayText("Error");
        return;
    }

    displayText("Success");
    display.print("ID# ");
    display.println(id);
    display.display();

    // int fpid = enrollFingerprint(20);
    // Serial.print("Result: ");
    // Serial.println(fpid);
    // if (fpid >= 0)
    // {
    //     displayText("Fingerprint registered!");
    //     delay(2000);
    //     displayText("Submitting to server ...");
    //     // sendToServer(20);
    //     delay(2000);
    //     displayText("Submitted successfully!");
    // }
    // else
    // {
    //     delay(2000);
    //     displayText("Try again.");
    // }
}

void displayAttendance()
{
    displayText("Place finger for attendance...");
    int result = findFingerprintId();
    if (result < 0)
    {
        displayText("Not found!");
    }
    else
    {
        displayText("Found!");
        display.print("ID# ");
        display.println(result);
        display.display();
    }
}