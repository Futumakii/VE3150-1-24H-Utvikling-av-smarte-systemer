// Pinneoppsett for heis og skyvemotorer
const int stepPinElevator = 6;  // STEP pin for heismotoren (pulssignal)
const int dirPinElevator = 7;   // DIR pin for heismotoren
const int stepPinLeft = 9;      // STEP pin for venstre skyvemotor
const int dirPinLeft = 10;      // DIR pin for venstre skyvemotor
const int stepPinRight = 11;    // STEP pin for høyre skyvemotor
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

// Spor for å holde styr på gjeldende etasje
int currentFloor = 0;  // Start med heisen på bakkenivå

// Variabler for å lagre forrige knappestatus
int lastParkButtonState = HIGH;
int lastRetrieveButtonState = HIGH;

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

    // Parkering
    if (parkButtonState == LOW && lastParkButtonState == HIGH) {  // Hvis park-knappen er trykket
        parkCar();
    }

    // Henting
    if (retrieveButtonState == LOW && lastRetrieveButtonState == HIGH) {  // Hvis hente-knappen er trykket
        retrieveCar();
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

// Flytt heisen til riktig etasje med digitalWrite
void moveElevatorToFloor(int targetFloor) {
    if (targetFloor > currentFloor) {
        digitalWrite(dirPinElevator, HIGH);  // Sett retning opp
    } else {
        digitalWrite(dirPinElevator, LOW);   // Sett retning ned
    }

    for (int i = 0; i < abs(targetFloor - currentFloor) * 200; i++) {  // Antall steg per etasje
        digitalWrite(stepPinElevator, HIGH);
        delay(1);  // Juster forsinkelsen for hastighet
        digitalWrite(stepPinElevator, LOW);
        delay(1);
    }

    currentFloor = targetFloor;  // Oppdater gjeldende etasje
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
