// Pinneoppsett for heismotor
const int stepPinElevator = 6;  // STEP (PULS) pin for heismotoren
const int dirPinElevator = 7;   // DIR pin for heismotoren

// Knapper for parkering og henting
const int parkButtonPin = 2;
const int retrieveButtonPin = 3;

// Spor for å holde styr på gjeldende etasje
int currentFloor = 0;  // Start med heisen på bakkenivå
const int moveDelay = 2000;  // Tidsforsinkelse for å simulere bevegelse

void setup() {
    // Sett opp pinner som utganger
    pinMode(stepPinElevator, OUTPUT);
    pinMode(dirPinElevator, OUTPUT);

    // Sett opp knappene med interne pull-up motstander
    pinMode(parkButtonPin, INPUT_PULLUP);
    pinMode(retrieveButtonPin, INPUT_PULLUP);

    // Start seriell kommunikasjon for feilsøking
    Serial.begin(9600);
}

void loop() {
    // Les statusen til knappene
    int parkButtonState = digitalRead(parkButtonPin);
    int retrieveButtonState = digitalRead(retrieveButtonPin);

    // Hvis park-knappen er trykket
    if (parkButtonState == LOW) {
        Serial.println("Park-knapp trykket. Flytter oppover...");
        moveElevatorUp();  // Flytt heisen opp
    }

    // Hvis hente-knappen er trykket
    if (retrieveButtonState == LOW) {
        Serial.println("Hente-knapp trykket. Flytter nedover...");
        moveElevatorDown();  // Flytt heisen ned
    }
}

// Flytt heisen oppover
void moveElevatorUp() {
    digitalWrite(dirPinElevator, HIGH);  // Sett retning oppover
    digitalWrite(stepPinElevator, HIGH);
    delay(moveDelay);  // Vent i noen sekunder for å simulere bevegelse
    digitalWrite(stepPinElevator, LOW);  // Stopp motoren
    currentFloor++;
    Serial.println("Heisen er nå i etasje: " + String(currentFloor));
}

// Flytt heisen nedover
void moveElevatorDown() {
    digitalWrite(dirPinElevator, LOW);  // Sett retning nedover
    digitalWrite(stepPinElevator, HIGH);
    delay(moveDelay);  // Vent i noen sekunder for å simulere bevegelse
    digitalWrite(stepPinElevator, LOW);  // Stopp motoren
    currentFloor = max(0, currentFloor - 1);  // Unngå negative etasjer
    Serial.println("Heisen er nå i etasje: " + String(currentFloor));
}
