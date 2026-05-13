#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <RTClib.h>
#include <DHT.h>

// Pins
#define PIR 2
#define BUZZER 9
#define MODE_BTN 4
#define START_BTN 5

#define DHTPIN 7
#define DHTTYPE DHT11

int LDR = A0;

int LAMP = 6;

// Objects
LiquidCrystal_I2C lcd(0x27, 16, 2);
RTC_DS3231 rtc;
DHT dht(DHTPIN, DHTTYPE);

// Modes
int mode = 0;
String modes[3] = {"Focus", "Normal", "Relax"};
int studyTime[3] = {50, 25, 15};
int breakTime[3] = {10, 5, 5};

// Timer
bool timerRunning = false;
bool breakMode = false;

unsigned long timerStart;
int duration;

// LCD Screens
unsigned long screenTimer = 0;
int screen = 0;

// Button debounce
unsigned long lastStartPress = 0;
unsigned long lastModePress = 0;


void setup()
{
  pinMode(PIR, INPUT);
  pinMode(BUZZER, OUTPUT);

  pinMode(LAMP, OUTPUT);
  

  pinMode(MODE_BTN, INPUT_PULLUP);
  pinMode(START_BTN, INPUT_PULLUP);

  Serial.begin(9600);

  lcd.init();
  lcd.backlight();

  rtc.begin();
   if (!rtc.begin())
  {
    Serial.println("RTC not found");
    while (1);
  }

  // Set RTC to computer time
  //rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));

  dht.begin();

  lcd.setCursor(0,0);
  lcd.print("Smart Study Desk");
  lcd.setCursor(0,1);
  lcd.print("System Start");

  delay(2000);
  lcd.clear();
}

void loop()
{
  presenceControl();
  modeControl();
  timerControl();
  displayScreens();
}

// ---------------------------
// PIR + LDR LIGHT CONTROL
// ---------------------------

void presenceControl()
{
  int motion = digitalRead(PIR);
  int lightValue = analogRead(LDR);

  if(motion == LOW)
  {
    digitalWrite(LED1, LOW);
    digitalWrite(LED2, LOW);
  }
  else
  {
    if(lightValue > 700) // DARK
    {
      digitalWrite(LED1, HIGH);
      digitalWrite(LED2, HIGH);
    }

    else if(lightValue > 300) // MEDIUM
    {
      digitalWrite(LED1, HIGH);
      digitalWrite(LED2, LOW);
    }

    else // BRIGHT
    {
      digitalWrite(LED1, LOW);
      digitalWrite(LED2, LOW);
    }
  }
}

// ---------------------------
// MODE BUTTON
// ---------------------------

void modeControl()
{
  if (digitalRead(MODE_BTN) == LOW && millis() - lastModePress > 400)
  {
    lastModePress = millis();

    mode++;

    if (mode > 2)
      mode = 0;

    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Mode Changed");
    lcd.setCursor(0,1);
    lcd.print(modes[mode]);
  }
}

// ---------------------------
// START / STOP TIMER
// ---------------------------

void timerControl()
{
  if (digitalRead(START_BTN) == LOW && millis() - lastStartPress > 400)
  {
    lastStartPress = millis();

    // TOGGLE TIMER
    if (!timerRunning)
    {
      timerRunning = true;
      breakMode = false;

      duration = studyTime[mode];
      timerStart = millis();

      lcd.clear();
      lcd.print("Study Started");
    }
    else
    {
      timerRunning = false;

      lcd.clear();
      lcd.print("Timer Stopped");
    }
  }

  if (timerRunning)
  {
    int elapsed = (millis() - timerStart) / 60000;
    int remain = duration - elapsed;

    if (remain <= 0)
    {
      tone(BUZZER, 1000, 2000);

      if (!breakMode)
      {
        breakMode = true;
        duration = breakTime[mode];
        timerStart = millis();

        lcd.clear();
        lcd.print("Break Time");
      }
      else
      {
        timerRunning = false;

        lcd.clear();
        lcd.print("Session Done");
      }
    }
  }
}

// ---------------------------
// LCD DISPLAY
// ---------------------------

void displayScreens()
{
  DateTime now = rtc.now();

  float temp = dht.readTemperature();
  float hum = dht.readHumidity();

  if (millis() - screenTimer > 4000)
  {
    screen++;

    if (screen > 2)
      screen = 0;

    lcd.clear();
    screenTimer = millis();
  }

  // Screen 1
  if (screen == 0)
  {
    lcd.setCursor(0,0);
    lcd.print("Temp:");
    lcd.print(temp);
    lcd.print("C");

    lcd.setCursor(0,1);
    lcd.print("Hum:");
    lcd.print(hum);
    lcd.print("%");
  }

  // Screen 2
  if (screen == 1)
  {
    lcd.setCursor(0,0);
    lcd.print("Time:");
    lcd.print(now.hour());
    lcd.print(":");
    lcd.print(now.minute());
    lcd.print(":");
    
    lcd.print(now.second());

    lcd.setCursor(0,1);
    lcd.print("Date:");
    lcd.print(now.day());
    lcd.print("/");
    lcd.print(now.month());
    lcd.print("/");
    lcd.print(now.year());
  }

  // Screen 3
  if (screen == 2)
  {
    lcd.setCursor(0,0);
    lcd.print("Mode:");
    lcd.print(modes[mode]);

    lcd.setCursor(0,1);

    if (timerRunning)
    {
      int elapsed = (millis() - timerStart) / 60000;
      int remain = duration - elapsed;

      lcd.print("Remain:");
      lcd.print(remain);
      lcd.print("m");
    }
    else
    {
      lcd.print("Press Start");
    }
  }
}
