// Pinneoppsett for heis og skyvemotorer
const int stepPinElevator = 6;  // STEP (PULS) pin for heismotoren (PWM-signal)
const int dirPinElevator = 7;   // DIR pin for heismotoren
const int stepPinLeft = 9;      // STEP (PULS) pin for venstre skyvemotor (PWM-signal)
const int dirPinLeft = 10;      // DIR pin for venstre skyvemotor
const int stepPinRight = 11;    // STEP (PULS) pin for høyre skyvemotor (PWM-signal)
const int dirPinRight = 12;     // DIR pin for høyre skyvemotor

const int lockPin = 5;          // Pin for låsemekanisme (servo eller rele)

// Sensorpinner for parkeringsplasser
int sensors[12] = {A0, A1, A2, A3, A4, A5, A6, A7, A8, A9, A10, A11}; // 12 sensorer, én for hver plass

// Antall etasjer og plasser
const int totalFloors = 6;
const int totalSlots = 12;  // 2 plasser per etasje (venstre/høyre)

// Array for å lagre status for parkeringsplasser (0 = ledig, 1 = opptatt)
int parkingSlots[12];       

// Knapper for parkering og henting
const int parkButtonPin = 2;
const int retrieveButtonPin = 3;

// PWM innstillinger
int pwmDutyCycle = 127;  // Duty cycle for PWM (0-255, 127 = 50%)

// Spor for å holde styr på gjeldende etasje
int currentFloor = 0;  // Start med heisen på bakkenivå

// Debouncing-variabler
unsigned long lastDebounceTime = 0;  
const int debounceDelay = 50;  // 50ms forsinkelse for debouncing

// Variabler for å lagre forrige knappestatus
int lastParkButtonState = HIGH;
int lastRetrieveButtonState = HIGH;

// Variabel for å holde styr på om motoren kjører
bool motorRunning = false;

void setup() {
    // Sett opp pinner som utganger
    pinMode(stepPinElevator, OUTPUT);
    pinMode(dirPinElevator, OUTPUT);
    pinMode(stepPinLeft, OUTPUT);
    pinMode(dirPinLeft, OUTPUT);
    pinMode(stepPinRight, OUTPUT);
    pinMode(dirPinRight, OUTPUT);
    pinMode(lockPin, OUTPUT);
    
    // Sett opp knappene med interne pull-up motstander
    pinMode(parkButtonPin, INPUT_PULLUP);
    pinMode(retrieveButtonPin, INPUT_PULLUP);

    // Initialiser sensorene som innganger
    for (int i = 0; i < totalSlots; i++) {
        pinMode(sensors[i], INPUT);
    }

    // Start seriell kommunikasjon for feilsøking
    Serial.begin(9600);
}

void loop() {
    // Les statusen til knappene
    int parkButtonState = digitalRead(parkButtonPin);
    int retrieveButtonState = digitalRead(retrieveButtonPin);

    // Debouncing for parkering
    if (parkButtonState != lastParkButtonState) {
        lastDebounceTime = millis();  // Nullstill debounce-timeren
    }

    if ((millis() - lastDebounceTime) > debounceDelay) {
        if (parkButtonState == LOW && !motorRunning) {  // Hvis park-knappen er trykket og motoren er ikke allerede i gang
            motorRunning = true;
            parkCar();  // Utfør parkering
            motorRunning = false;  // Tillat neste kommando etter at parkeringen er ferdig
        }
    }

    // Debouncing for henting
    if (retrieveButtonState != lastRetrieveButtonState) {
        lastDebounceTime = millis();  // Nullstill debounce-timeren
    }

    if ((millis() - lastDebounceTime) > debounceDelay) {
        if (retrieveButtonState == LOW && !motorRunning) {  // Hvis hente-knappen er trykket og motoren er ikke allerede i gang
            motorRunning = true;
            retrieveCar();  // Utfør henting
            motorRunning = false;  // Tillat neste kommando etter at henting er ferdig
        }
    }

    // Oppdater knappestatus for neste loop
    lastParkButtonState = parkButtonState;
    lastRetrieveButtonState = retrieveButtonState;
}

// Funksjon for å parkere bilen
void parkCar() {
    int slot = findNextAvailableSlot();  // Sjekk etter en ledig plass ved hjelp av sensorene

    if (slot != -1) {  // Hvis en plass er ledig
        moveElevatorToFloor(slot / 2);  // Flytt heisen til riktig etasje
        if (slot % 2 == 0) {  // Hvis det er en partallsplass (venstre side)
            slideCarToLeft();  // Skyv bilen til venstre plass
        } else {  // Hvis det er en oddetallsplass (høyre side)
            slideCarToRight();  // Skyv bilen til høyre plass
        }
        lockCarInPlace();  // Lås bilen på plassen
        parkingSlots[slot] = 1;  // Marker plassen som opptatt
        Serial.println("Bil parkert på plass " + String(slot));
    } else {
        Serial.println("Ingen ledige plasser.");
    }
}

// Funksjon for å hente bilen
void retrieveCar() {
    int slot = findOccupiedSlot();  // Finn plassen hvor bilen er parkert

    if (slot != -1) {  // Hvis en plass er funnet
        moveElevatorToFloor(slot / 2);  // Flytt heisen til riktig etasje
        if (slot % 2 == 0) {  // Hvis det er en partallsplass (venstre side)
            slideCarToLeft();  // Skyv bilen fra venstre plass
        } else {  // Hvis det er en oddetallsplass (høyre side)
            slideCarToRight();  // Skyv bilen fra høyre plass
        }
        unlockCar();  // Lås opp bilen for henting
        parkingSlots[slot] = 0;  // Marker plassen som ledig igjen
        Serial.println("Bil hentet fra plass " + String(slot));
    } else {
        Serial.println("Ingen bil å hente.");
    }
}

// Støttefunksjoner

// Sjekk etter en ledig parkeringsplass ved hjelp av sensorene
int findNextAvailableSlot() {
    for (int i = 0; i < totalSlots; i++) {
        if (digitalRead(sensors[i]) == LOW) {  // LOW indikerer at plassen er ledig
            return i;
        }
    }
    return -1;  // Ingen ledige plasser
}

// Flytt heisen til riktig etasje med PWM
void moveElevatorToFloor(int targetFloor) {
    if (targetFloor > currentFloor) {
        digitalWrite(dirPinElevator, HIGH);  // Flytt oppover
    } else {
        digitalWrite(dirPinElevator, LOW);   // Flytt nedover
    }
    
    analogWrite(stepPinElevator, pwmDutyCycle);  // Start bevegelse
    delay(2000 * abs(targetFloor - currentFloor));  // Juster forsinkelse
    analogWrite(stepPinElevator, 0);  // Stopp motoren
    
    currentFloor = targetFloor;  // Oppdater nåværende etasje
}

// Skyv bilen til venstre
void slideCarToLeft() {
    digitalWrite(dirPinLeft, LOW);  // Sett retning til venstre
    analogWrite(stepPinLeft, pwmDutyCycle);
    delay(2000);
    analogWrite(stepPinLeft, 0);
}

// Skyv bilen til høyre
void slideCarToRight() {
    digitalWrite(dirPinRight, HIGH);  // Sett retning til høyre
    analogWrite(stepPinRight, pwmDutyCycle);
    delay(2000);
    analogWrite(stepPinRight, 0);
}

// Lås bilen på plassen
void lockCarInPlace() {
    digitalWrite(lockPin, HIGH);
    delay(500);
    digitalWrite(lockPin, LOW);
}

// Lås opp bilen
void unlockCar() {
    digitalWrite(lockPin, HIGH);
    delay(500);
    digitalWrite(lockPin, LOW);
}

// Sjekk hvilken plass som er opptatt
int findOccupiedSlot() {
    for (int i = 0; i < totalSlots; i++) {
        if (parkingSlots[i] == 1) {
            return i;
        }
    }
    return -1;
}
