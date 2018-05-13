//Input: PinA0 ~ PinA7 --> C ~ B
//Input: PinA9 ~ PinA11 --> 2, 4, 6
//Output: (note) --> 3 frequencies --> 3 buzzers
//Example: A_0 + A_11 --> C6 --> [100, 200, 300] --> Pin5, Pin11, Pin13

int note[][7][3] = {
  { // second octave
    {130, 65, 260}, // C2
    {146, 73, 292}, // D2
    {164, 82, 328}, // E2
    {174, 87, 348}, // F2
    {196, 98, 392}, // G2
    {220, 110, 440}, // A2
    {246, 123, 492} // B2
  },
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

// relayNum range: 0-2
#define relayNum 2
#define speakerPin 11
#define scale 7
#define octave 3

int sensorPin[] = {A0, A1, A2, A3, A4, A5, A7};
int octavePin[] = {A9, A10, A11};
int sensorValues[] = {0, 0, 0, 0, 0, 0, 0};
int octaveValues[] = {0, 0, 0};
int sensorThreshold = 150; // based on regular resistors
int currOctave = 0;
int counter = 0;
int index = 0;
int beat = 14;

int arpeggio[] = {0, 0, 0, 0, 0, 0, 0}; // supports up to seven notes, a full scale
 
void setup() {
  Serial.begin(9600);
}

void loop() {
  bool safe = checkOctave();
  for (int i = 0; i < scale; i += 1) {
    sensorValues[i] = analogRead(sensorPin[i]);
    if (sensorValues[i] < sensorThreshold && safe) { // CHANGE TO < WHEN USING LASER! May add condition so that it only plays if octave sensor is active. 
      arpeggio[i] = note[currOctave][i][relayNum];
      Serial.println("PORT: " + String(sensorPin[i])  + " FREQ: " + String(note[currOctave][i][relayNum]));
    } else {
      arpeggio[i] = 0;
    }
    Serial.println("PORT: " + String(sensorPin[i])  + " VALUE: " + String(sensorValues[i]));
  }
  play();
//  delay(50);
  for (int i = 0; i < scale; i += 1) {
    Serial.println(String(i) + ": " + String(arpeggio[i]));
  }
}

bool checkOctave() {
  bool onOctave = false;
  for (int i = 0; i < octave; i += 1) {
    octaveValues[i] = analogRead(octavePin[i]);
    if (octaveValues[i] < sensorThreshold) { // CHANGE TO < WHEN USING LASER!
      currOctave = i; // REMOVE + 1 after debugging
      onOctave = true;
      Serial.println(String(currOctave));
      Serial.println(String(onOctave));
    }
    Serial.println("OCTAVE: " + String(octavePin[i])  + " VALUE: " + String(octaveValues[i]));
  }
  return onOctave;
}

void play() {
  index = index % 7;
  while (index < 7 && arpeggio[index] == 0) { // finds next index
    index += 1;
  }
  if (index < 7) { // plays valid index, increments counter
    tone(speakerPin, arpeggio[index], 20);
    Serial.println(arpeggio[index]);
    counter += 1;
  } else { // resets counter
    counter = 0;
  }
  if (counter >= beat) { // plays next note in arpeggio
    index += 1;
    counter = 0;
  }
  //delay(10); // if want to play one freq (one speaker) at a time
  //noTone(speakerPin[i]); // if want to play one freq (one speaker) at a time
}

