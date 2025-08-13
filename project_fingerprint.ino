#include "DisplayManager.h"
#include "FingerprintManager.h"
#include "HttpManager.h"
#include <OneButton.h>

#define BUTTON_PIN 18
#define BUZZER_PIN 25
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
    clearDisplay();
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

    pinMode(BUZZER_PIN, OUTPUT);
    digitalWrite(BUZZER_PIN, HIGH);

    delay(2000);
    updateMenuDisplay();
}

void beep()
{
    digitalWrite(BUZZER_PIN, LOW);
    delay(500);
    digitalWrite(BUZZER_PIN, HIGH);
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
    {
        int fpid = submitAttendance();
        delay(1000);
        if (fpid > 0)
        {
            clearDisplay();
            displayTextln("Submitting attendance ...");
            bool success = sendAttendance(fpid);
            delay(1000);
            clearDisplay();
            if (success)
            {
                displayTextln("Submitted successfully!");
            }
            else
            {
                displayTextln("Something went wrong.");
            }
        }
    }
    else if (item_selected == 1) // Register
    {
        displayTextln("Finding not registered student ...");
        bool success = fetchNotRegisteredStudent();
        if (success)
        {
            int fpid = enrollFingerprint();
            delay(1000);
            if (fpid > 0)
            {
                clearDisplay();
                displayTextln("Sending request ...");
                success = false;
                success = sendRegisteration(fpid);
                delay(1000);
                clearDisplay();
                if (success)
                {
                    displayTextln("Registered successfully!");
                }
                else
                {
                    deleteFingerprintWithId(fpid);
                    displayTextln("Something went wrong.");
                }
            }
        }
        else
        {
            clearDisplay();
            displayTextln("All students have been registered");
        }
    }
    else if (item_selected == 2) // Inspect
    {
        int level, count;
        printFingerprintSensorDetails(level, count);
        displayText("Security level - ");
        displayTextln(String(level));
        displayText("Templates - ");
        displayTextln(String(count));
    }

    delay(2000);
    updateMenuDisplay();
}

static void handleDoubleClick()
{
    changeSelectedItem();
}

static void handleLongPress()
{
    emptyFingerprintSensorDatabase();
}

int enrollFingerprint()
{
    clearDisplay();

    int id = findNextAvailableFingerprintId();
    if (id < 0)
    {
        displayTextln("No empty ID available");
        return -1;
    }

    Serial.print("Available ID# ");
    Serial.println(id);

    displayTextln("Place finger to register...");
    bool success = false;
    success = enrollFingerprintInBuffer(1);

    beep();

    if (!success)
    {
        clearDisplay();
        displayTextln("Something went wrong. Try again!");
        return -1;
    }

    clearDisplay();
    displayTextln("Remove finger");
    waitForFingerRemoved();
    delay(1000);
    clearDisplay();

    displayTextln("Place the same finger again for confirmation...");
    success = false;
    success = enrollFingerprintInBuffer(2);

    beep();

    if (!success)
    {
        clearDisplay();
        displayTextln("Something went wrong. Try again!");
        return -1;
    }

    bool matched = checkIfFingerprintMatchedInBuffers();
    if (!matched)
    {
        clearDisplay();
        displayTextln("Fingerprints didn't match!");
        return -1;
    }

    bool enrolled = checkIfFingerprintAlreadyEnrolled();
    if (enrolled)
    {
        clearDisplay();
        displayTextln("Fingerprint already enrolled!");
        return -1;
    }

    bool stored = enrollFingerprintInMemory(id);
    if (!stored)
    {
        clearDisplay();
        displayTextln("Failed to store!");
        return -1;
    }

    clearDisplay();
    displayTextln("Success!");
    displayText("ID# ");
    displayTextln(String(id));

    return id;
}

int submitAttendance()
{
    clearDisplay();

    displayTextln("Place finger for attendance...");
    int id = findFingerprintId();

    beep();

    if (id < 0)
    {
        clearDisplay();
        displayTextln("Fingerprint not found!");
        return -1;
    }

    clearDisplay();
    displayTextln("Found!");
    displayText("ID# ");
    displayTextln(String(id));

    return id;
}
