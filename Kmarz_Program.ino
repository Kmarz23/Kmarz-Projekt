#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// Definicja opcji menu
enum MenuOption { INFO, WERSJA, TERMOMETR, STOPER, MINUTNIK };
const char* opcje[] = {"Info", "Wersja", "Termomet", "Stoper", "Minutnik", ""};

// Pin konfiguracyjny dla przycisków
const byte przyciskUP = 2;
const byte przyciskDOWN = 3;
const byte przyciskSELECT = 4;
const byte buzzer = 6;

#include "DHT.h"
#define DHTPIN 5
#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE);

// Konfiguracja wyświetlacza OLED
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_MOSI   9
#define OLED_CLK   10
#define OLED_DC    11
#define OLED_CS    12
#define OLED_RESET 13
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, OLED_MOSI, OLED_CLK, OLED_DC, OLED_RESET, OLED_CS);

int selectedOption = -1;

void setup() {
    // Inicjalizacja czujnika DHT
    dht.begin();
    // Inicjalizacja komunikacji szeregowej
    Serial.begin(9600);
    // Konfiguracja pinów przycisków
    pinMode(przyciskUP, INPUT_PULLUP);
    pinMode(przyciskDOWN, INPUT_PULLUP);
    pinMode(przyciskSELECT, INPUT_PULLUP);

    // Inicjalizacja wyświetlacza OLED
    if (!display.begin(SSD1306_SWITCHCAPVCC)) {
        Serial.println(F("Błąd alokacji SSD1306"));
        for (;;) ;
    }

    // Wyświetlenie ekranu powitalnego
    display.clearDisplay();
    display.setTextSize(3);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(0, 0);
    display.println(F("Kmarz"));
    display.println(F("Program"));
    display.display();
    delay(2000);
    // Rozpoczęcie pracy w menu
    startMenu();
}

void loop() {
    // Obsługa poruszania się po menu
    poruszaniePoMenu();
}

// Funkcja inicjująca menu
void startMenu() {
    selectedOption = 0;
    updateMenu(0);
}

// Funkcja obsługująca poruszanie się po menu
void poruszaniePoMenu() {
    if (digitalRead(przyciskDOWN) == LOW && selectedOption == -1) {
        updateMenu(1);
    }

    if (digitalRead(przyciskUP) == LOW && selectedOption == 1) {
        updateMenu(-1);
    }

    if (digitalRead(przyciskDOWN) == LOW && selectedOption > -1 && selectedOption < 4) {
        updateMenu(1);
    }

    if (digitalRead(przyciskUP) == LOW && selectedOption > 0) {
        updateMenu(-1);
    }

    if (digitalRead(przyciskSELECT) == LOW) {
        delay(200);
        menuSELECT();
    }
}

// Funkcja aktualizująca menu
void updateMenu(int step) {
    selectedOption += step;
    display.clearDisplay();
    display.setTextSize(2);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(0, 0);
    display.print(opcje[selectedOption]);
    display.println(" #");
    display.println(opcje[selectedOption + 1]);
    display.display();
    delay(200);
}

// Funkcja obsługująca wybór w menu
void menuSELECT() {
    switch (selectedOption) {
        case INFO:
            info();
            break;
        case WERSJA:
            wersja();
            break;
        case TERMOMETR:
            termometr();
            break;
        case STOPER:
            stoper();
            break;
        case MINUTNIK:
            minutnik();
            break;
    }
    // Oczekiwanie na zwolnienie przycisku SELECT
    while (digitalRead(przyciskSELECT) == LOW) {
        delay(20);
        // Powrót do głównego menu
        startMenu();
    }
}

// Funkcja wyświetlająca informacje o programie
void info() {
    display.clearDisplay();
    display.setTextSize(2);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(0, 0);
    display.println("By KmarzPL");
    display.println("29.01.2024");
    display.println("Testing:");
    display.println("Lukipuki");
    display.display();
}

// Funkcja wyświetlająca numer wersji programu
void wersja() {
    display.clearDisplay();
    display.setTextSize(2);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(0, 0);
    display.println("Wersja:");
    display.println("1.3.3");
    display.println("Data:");
    display.println("30.01.2024");
    display.display();
}

// Funkcja wyświetlająca dane z termometru
void termometr() {
    float h = dht.readHumidity();
    float t = dht.readTemperature();
    display.clearDisplay();
    display.setTextSize(2);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(0, 0);
    display.print("T: ");
    display.print(t);
    display.println("C");
    display.print("W: ");
    display.print(h);
    display.println("%");
    display.display();
}

// Funkcja obsługująca stoper
void stoper() {
    unsigned long startTime = millis();
    byte ms = 0;
    byte s = 0;
    byte m = 0;

    while (true) {
        unsigned long currentTime = millis() - startTime;
        ms = currentTime % 1000 / 10;
        s = currentTime / 1000 % 60;
        m = currentTime / 60000 % 60;

        display.clearDisplay();
        display.setTextSize(2);
        display.setTextColor(SSD1306_WHITE);
        display.setCursor(0, 0);

        if (m < 10) display.print("0");
        display.print(m);
        display.print(":");
        if (s < 10) display.print("0");
        display.print(s);
        display.print(":");
        if (ms < 10) display.print("0");
        display.print(ms);

        display.display();
        delay(1);

        if (digitalRead(przyciskSELECT) == LOW) {
            delay(200);
            break;
        }
    }
}

// Deklaracja funkcji displayTime
void displayTime(int hours, int minutes, int seconds = 0);

// Funkcja minutnik
void minutnik() {
    int h = 0;
    int m = 0;
    int s = 0;

    unsigned long lastTime = millis();

    while (true) {
        if (digitalRead(przyciskUP) == LOW) {
            delay(100);
            m++;
            if (m == 60) {
                h++;
                m = 0;
                if (h == 24) h = 0;
            }
        }

        if (digitalRead(przyciskSELECT) == LOW) {
            delay(200);
            break;
        }

        // Użycie funkcji displayTime
        displayTime(h, m);
    }

    while (h > 0 || m > 0 || s > 0) {
        if (digitalRead(przyciskSELECT) == LOW) {
            delay(200);
            break;
        }

        if (millis() - lastTime >= 1000) {
            lastTime = millis();

            // Użycie funkcji displayTime
            displayTime(h, m, s);

            if (--s < 0) {
                s = 59;
                if (--m < 0) {
                    m = 59;
                    if (--h < 0) h = 0;
                }
            }
        }
    }

    // Sygnał dźwiękowy po zakończeniu odliczania
    if (h == 0 && m == 0 && s == 0) {
        tone(buzzer, 500);
        delay(3000);
        noTone(buzzer);
    }
}

// Implementacja funkcji displayTime
void displayTime(int hours, int minutes, int seconds) {
    display.clearDisplay();
    display.setTextSize(2);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(0, 0);
    display.print(hours);
    display.print(":");
    if (minutes < 10) display.print("0");
    display.print(minutes);
    display.print(":");
    if (seconds < 10) display.print("0");
    display.print(seconds);
    display.display();
}