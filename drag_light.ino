#define LOOP_DELAY 1000
#define PAUSE 6000
#define BUTTON_MS_TRESHOLD 500

#define YELLOW_1 3
#define YELLOW_2 4
#define GREEN 5

#define RC 7
#define BUTTON 2

bool buttonState = false;
unsigned long millisLastTriggerred = 0;
const int LIGHTS[] = { YELLOW_1, YELLOW_2, GREEN };
const int LIGHTS_COUNT = 3;

void buttonIsr() {
  unsigned long millisNow = millis();

  if (millisNow < BUTTON_MS_TRESHOLD * 2) {
    return;
  }

  buttonState = true;
}

void switchLight(int light, bool on) {
  digitalWrite(light, !on);
}

void oneLightOn(int light) {
  for (int i = 0; i < LIGHTS_COUNT; i++) {
    if (LIGHTS[i] == light) {
      switchLight(light, HIGH);
    } else {
      switchLight(LIGHTS[i], LOW);
    }
  }
}

void switchAllOn() {
    for (int i = 0; i < LIGHTS_COUNT; i++) {
    switchLight(LIGHTS[i], HIGH);
  }
}

void switchAllOff() {
  for (int i = 0; i < LIGHTS_COUNT; i++) {
    switchLight(LIGHTS[i], LOW);
  }
}

void setup() {
  Serial.begin(9600);           
  attachInterrupt(0, buttonIsr, FALLING);

  pinMode(BUTTON, INPUT_PULLUP);

  pinMode(RC, INPUT_PULLUP);

  pinMode(YELLOW_1, OUTPUT);
  pinMode(YELLOW_2, OUTPUT);
  pinMode(GREEN, OUTPUT);

  switchAllOff();
}

void blinkPrepare() {
  const int blinkDelay = 300;
  const int blinkCount = 3;

  for (int i = 0; i < blinkCount; i++) {
    switchLight(YELLOW_1, HIGH);
    switchLight(YELLOW_2, HIGH);
    delay(blinkDelay);

    switchLight(YELLOW_1, LOW);
    switchLight(YELLOW_2, LOW);
    delay(blinkDelay);
  }
}

void startRace() {
  if (millis() - millisLastTriggerred < PAUSE) {
    return;
  }

  const int preDelay = 500;
  const int betweenLightsDelay = 1000;
  const int afterDelay = 3000;

  blinkPrepare();
  delay(preDelay);

  switchLight(YELLOW_1, HIGH);
  delay(betweenLightsDelay);

  switchLight(YELLOW_2, HIGH);
  delay(betweenLightsDelay);

  oneLightOn(GREEN);
  delay(afterDelay);

  switchAllOff();

  millisLastTriggerred = millis();
}

void loop() {
  if (buttonState) {
    startRace();
    Serial.println("BTN");
    buttonState = false;
  }

  if (digitalRead(RC) == LOW) {
    Serial.println("RC");
    startRace();
  }

  delay(LOOP_DELAY);
}