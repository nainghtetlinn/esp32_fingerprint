#include "HttpManager.h"
#include <WiFi.h>
#include <HTTPClient.h>

String ssid = "";
String password = "";
String baseUrl = "";
String registerUrl = "/register";
String attendanceUrl = "/attendance";

HTTPClient http;

void readStringFromSerial(String &target, const char *prompt)
{
    Serial.print(prompt);
    while (Serial.available())
        Serial.read(); // flush input buffer
    while (true)
    {
        if (Serial.available())
        {
            target = Serial.readStringUntil('\n');
            target.trim();
            if (target.length() > 0)
                break;
        }
        delay(100);
    }
    Serial.println(target);
}

void readWiFiCredentials()
{
    readStringFromSerial(ssid, "Enter WiFi SSID: ");
    readStringFromSerial(password, "Enter WiFi Password: ");
}

void readServerUrl()
{
    readStringFromSerial(baseUrl, "Enter URL: ");
}

int initHttp()
{
    readWiFiCredentials();
    readServerUrl();

    WiFi.begin(ssid, password);

    unsigned long startAttemptTime = millis();
    const unsigned long wifiTimeout = 30000;

    while (WiFi.status() != WL_CONNECTED && millis() - startAttemptTime < wifiTimeout)
    {
        delay(500);
        Serial.print(".");
    }

    if (WiFi.status() != WL_CONNECTED)
    {
        Serial.println("Cannot connect to WiFi :(");
        return -1;
    }
    else
    {
        return 0;
    }
}

bool sendRequest(const String method, const String &url, const String &payload = "")
{
    if (WiFi.status() == WL_CONNECTED)
    {
        http.begin(baseUrl + url);
        http.addHeader("Content-Type", "application/json");

        int responseCode;
        if (method == "POST")
        {
            responseCode = http.POST(payload);
        }
        else
        {
            responseCode = http.GET();
        }

        Serial.print("HTTP Status Code: ");
        Serial.println(responseCode);
        String payload = http.getString();
        Serial.print("Response:");
        Serial.println(payload);

        http.end();

        if (responseCode == 200)
        {
            return true;
        }
    }
    return false;
}

bool fetchNotRegisteredStudent()
{
    return sendRequest("GET", registerUrl);
}

bool sendRegisteration(int fpid)
{
    String payload = "{\"fpid\":" + String(fpid) + "}";
    return sendRequest("POST", registerUrl, payload);
}

bool sendAttendance(int fpid)
{
    String payload = "{\"fpid\":" + String(fpid) + "}";
    return sendRequest("POST", attendanceUrl, payload);
}
