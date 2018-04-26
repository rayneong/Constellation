//Input: PinA0 ~ PinA7 --> C ~ B
//Input: PinA9 ~ PinA11 --> 2, 4, 6
//Output: (note) --> 3 frequencies --> 3 buzzers
//Example: A_0 + A_11 --> C6 --> [100, 200, 300] --> Pin5, Pin11, Pin13

int note[][7][3] = {
  { // third octave
    {262, 131, 523}, // C3
    {294, 147, 587}, // D3
    {330, 165, 659}, // E3
    {349, 175, 698}, // F3
    {392, 196, 784}, // G3
    {440, 220, 880}, // A3
    {494, 247, 988} // B3 
  },
  { // fourth octave
    {523, 262, 1047}, // C4
    {587, 294, 1175}, // D4
    {659, 330, 1319}, // E4
    {698, 349, 1397}, // F4
    {784, 392, 1568}, // G4
    {880, 440, 1760}, // A4
    {988, 494, 1976} // B4
  },
  { // fifth ctave
    {1057, 523, 2093}, // C5
    {1175, 587, 2349}, // D5
    {1319, 659, 2637}, // E5
    {1397, 698, 2794}, // F5
    {1568, 784, 3136}, // G5
    {1760, 880, 3520}, // A5
    {1976, 988, 3951} // B5
  }
};

int sensorPin[] = {A2, A1, A0};
int octavePin[] = {A3};
int speakerPin[] = {5, 11, 13};
int sensorValues[] = {0, 0, 0};
int octaveValues[] = {0};
int sensorThreshold = 400;
int currOctave = 0;
 
void setup() {
  Serial.begin(9600);
}

void loop() {
  checkOctave();
  for (int i = 0; i < sizeof(sensorPin) / sizeof(int); i += 1) {
    sensorValues[i] = analogRead(sensorPin[i]);
    if (sensorValues[i] > sensorThreshold) { // CHANGE TO < WHEN USING LASER!
      play(note[currOctave][i]);
    } else {
      noTone(speakerPin[i]);
    }
    Serial.println("PORT: " + String(sensorPin[i])  + " VALUE: " + String(sensorValues[i]));
  }
  delay(50);
}

void checkOctave() {
  for (int i = 0; i < sizeof(octavePin) / sizeof(int); i += 1) {
    octaveValues[i] = analogRead(octavePin[i]);
    if (octaveValues[i] > sensorThreshold) { // CHANGE TO < WHEN USING LASER!
      currOctave = i + 1; //REMOVE + 1 after debugging
    }
  }
}

void play(int freq[]) {
  for (int i = 0; i < sizeof(speakerPin) / sizeof(int); i += 1) {
    tone(speakerPin[i], freq[i]);
    delay(50); //if want to play one freq (one speaker) at a time
    noTone(speakerPin[i]); //if want to play one freq (one speaker) at a time
    //delay(1000); //debug
  }
  //delay(50); //if want to play all speakers at once
//  for (int i = 0; i < sizeof(speakerPin) / sizeof(int); i += 1) {
//    noTone(speakerPin[i]);
    //delay(1000); //debug
  //}
  //delay(1000); //debug
}

