#include <WiFi.h>       // Use <ESP8266WiFi.h> if using ESP8266
#include <WiFiUdp.h>
#include <Servo.h>

// Wi-Fi credentials
const char* ssid = "YOUR_WIFI_SSID";
const char* password = "YOUR_WIFI_PASSWORD";

// UDP settings
WiFiUDP udp;
const int udpPort = 8888;
char packetBuffer[255];

// Servo configurations
struct ServoConfig {
    Servo servo;
    int pin;
    int minAngle;
    int maxAngle;
};

// Define servos and their limits
ServoConfig servos[] = {
    {"Base",  23, -90, 90},
    {"Pitch", 24, -45, 45},
    {"Tilt",  25, -45, 45},
    {"Mouth", 18,  45, 80} // Mouth servo has a different range
};

// Number of servos
const int numServos = sizeof(servos) / sizeof(servos[0]);

void setup() {
    Serial.begin(115200);

    // Connect to Wi-Fi
    WiFi.begin(ssid, password);
    Serial.print("Connecting to Wi-Fi");
    while (WiFi.status() != WL_CONNECTED) {
        Serial.print(".");
        delay(500);
    }
    Serial.println("\nConnected to Wi-Fi!");
    Serial.print("IP Address: ");
    Serial.println(WiFi.localIP());

    // Attach servos
    for (int i = 0; i < numServos; i++) {
        servos[i].servo.attach(servos[i].pin);
        servos[i].servo.write(mapToServoRange(servos[i], 0)); // Initialize to center position
    }

    // Start UDP
    udp.begin(udpPort);
    Serial.println("UDP server started. Waiting for commands...");
}

void loop() {
    int packetSize = udp.parsePacket();
    if (packetSize) {
        // Read UDP packet
        int len = udp.read(packetBuffer, sizeof(packetBuffer) - 1);
        if (len > 0) {
            packetBuffer[len] = '\0'; // Null-terminate the received string
        }

        Serial.print("Received: ");
        Serial.println(packetBuffer);

        // Parse commands
        parseAndSetServos(packetBuffer);
    }
}

// Function to parse UDP commands and control servos
void parseAndSetServos(char* command) {
    char* token = strtok(command, " ");
    while (token != NULL) {
        char servoID = token[0];
        int angle = atoi(&token[1]);

        switch (servoID) {
            case 'B': setServoAngle(0, angle); break;
            case 'P': setServoAngle(1, angle); break;
            case 'T': setServoAngle(2, angle); break;
            case 'M': setServoAngle(3, angle); break;
            default: Serial.println("Invalid command");
        }

        token = strtok(NULL, " ");
    }
}

// Function to set a servo's angle within limits
void setServoAngle(int index, int angle) {
    if (index >= numServos) return;

    ServoConfig& config = servos[index];
    int clampedAngle = constrain(angle, config.minAngle, config.maxAngle);

    if (clampedAngle != angle) {
        Serial.printf("Warning: %s angle clamped from %d to %d\n", config.servo.servo, angle, clampedAngle);
    }

    int mappedValue = mapToServoRange(config, clampedAngle);
    config.servo.write(mappedValue);
}

// Maps the angle to the servo's 0-180 range
int mapToServoRange(ServoConfig& config, int angle) {
    return map(angle, config.minAngle, config.maxAngle, 0, 180);
}
