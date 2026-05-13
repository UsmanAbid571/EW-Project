# Smart Study Desk

This repository contains the source code for a "Smart Study Desk," an Arduino-based project designed to create an optimized and productive study environment. The system automates lighting, manages study sessions using a Pomodoro-like timer, and monitors ambient environmental conditions.

## Features
-   **Automated Lighting:** A PIR sensor detects presence, and an LDR sensor measures ambient light to automatically adjust desk lamp brightness.
-   **Multiple Study Modes:** Three predefined modes (Focus, Normal, Relax) with different study and break time intervals.
-   **Session Timer:** A built-in timer to manage study and break periods, with buzzer alerts to signal transitions.
-   **Environment Monitoring:** A DHT11 sensor tracks real-time temperature and humidity.
-   **Information Hub:** A 16x2 LCD display cycles through screens showing time, date, environmental data, current mode, and timer status.

## Hardware Requirements
-   Arduino Uno or a compatible microcontroller
-   PIR Motion Sensor
-   DHT11 Temperature & Humidity Sensor
-   DS3231 Real-Time Clock (RTC) Module
-   16x2 I2C LCD Display
-   LDR (Light Dependent Resistor)
-   5V Buzzer
-   LED or Lamp for the desk light
-   2 x Push Buttons
-   Jumper wires
-   Breadboard
-   Resistors (e.g., for the LDR voltage divider)

## Pin Connections

| Component           | Arduino Pin       |
| ------------------- | ----------------- |
| PIR Sensor (OUT)    | D2                |
| Buzzer              | D9                |
| Mode Button         | D4                |
| Start Button        | D5                |
| DHT11 Sensor (DATA) | D7                |
| Lamp (Control)      | D6                |
| LDR                 | A0                |
| I2C Devices (SDA)   | A4                |
| I2C Devices (SCL)   | A5                |

*Note: The I2C LCD and DS3231 RTC Module both connect to the same I2C pins (SDA/A4 and SCL/A5).*

## Software and Installation

1.  **Clone the repository:**
    ```bash
    git clone https://github.com/usmanabid571/ew-project.git
    ```

2.  **Install Arduino Libraries:**
    Open the Arduino IDE and navigate to `Sketch > Include Library > Manage Libraries...`. Search for and install the following libraries:
    *   `LiquidCrystal I2C` by Frank de Brabander
    *   `RTClib` by Adafruit
    *   `DHT sensor library` by Adafruit

3.  **Hardware Setup:**
    Connect all the components to your Arduino according to the [Pin Connections](#pin-connections) table.

4.  **Set RTC Time (One-time setup):**
    To set the time on the RTC module for the first time, open the `study_desk/study_desk.ino` file and uncomment the following line in the `setup()` function:
    ```cpp
    //rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
    ```
    Upload the sketch to your Arduino. This sets the RTC to your computer's time at compilation. Afterward, comment out the line again and re-upload the sketch to prevent the time from being reset on every startup.

5.  **Upload Sketch:**
    Open `study_desk/study_desk.ino` in the Arduino IDE, select your board and port from the `Tools` menu, and click the Upload button.

## How It Works

### Lighting Control
The system intelligently manages the desk lamp. When the PIR sensor detects your presence, the lamp turns on. The LDR sensor then measures the ambient light in the room, and the Arduino adjusts the lamp's brightness accordingly: brighter in a dark room and dimmer in a moderately lit room. If you leave the desk, the PIR sensor detects the absence of motion, and the lamp automatically turns off.

### Study & Break Timer
The project incorporates a productivity timer inspired by the Pomodoro Technique. You can cycle through three predefined modes using the `MODE` button:

*   **Focus:** 50 minutes study, 10 minutes break.
*   **Normal:** 25 minutes study, 5 minutes break.
*   **Relax:** 15 minutes study, 5 minutes break.

Press the `START` button to begin a study session. The LCD will display the time remaining. When the study period ends, a buzzer will sound, and the timer will automatically begin the corresponding break period. When the break is over, the buzzer will sound again to signal the end of the session.

### LCD Display
The 16x2 LCD acts as an information dashboard, automatically cycling through three screens every four seconds:

1.  **Environment:** Displays the current temperature (°C) and humidity (%).
2.  **Clock:** Shows the current time and date, fetched from the RTC module.
3.  **Session Status:** Shows the currently selected study mode and the time remaining in the current session. If no timer is running, it prompts you to "Press Start".
