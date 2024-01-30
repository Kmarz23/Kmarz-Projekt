# Kmarz Projekt

Ten projekt jest tworzony przez jedną osobę. Ma na celu łatwy wybór opcji i ich obsługę. Dostępne opcje:

- Info
- Wersja
- Termometr
- Stoper
- Minutnik

## Wymagania

- Płytka Arduino
- Wyświetlacz OLED 128x64 ISP
- 3 przyciski
- Czujnik temperatury i wilgotności DHT22
- Buzzer

## Podłączenie

### Przyciski

- Przycisk UP: pin 2
- Przycisk DOWN: pin 3
- Przycisk SELECT: pin 4

### Buzzer

-pin + buzera: pin 6

### Czujnik DHT22

- Pin DHT22: pin 5

### Wyświetlacz OLED

- Pin VCC: 5V
- Pin GND: GND
- Pin NC: Brak podłączenia
- Pin DIN: pin 9
- Pin CLK: pin 10
- Pin CS: pin 12
- Pin D/C: pin 11
- Pin RES: pin 13

## Obsługa

Aby poruszać się po menu, użyj przycisków UP i DOWN. Aby wybrać opcję, naciśnij przycisk SELECT. Aby wyjść z wybranej opcji, przytrzymaj przycisk SELECT. Bardziej szczegółowa prezentacja znajduje się w wideu które jest do pobrania w tym repozytorium GitHub.

> Wideo nie jest bardzo dobrej jakości, ponieważ używam telefonu do nagrania z aparatem 13MP.
