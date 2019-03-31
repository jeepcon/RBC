// Output Pins
const int LightBar1 = 2; //Red
const int LightBar2 = 3; //Amber
const int LightBar3 = 4; //Red
const int LightBar4 = 5; //Red
const int LightBar5 = 6; //Amber
const int LightBar6 = 7; //Red
const int BrakeRight = 22; //Red
const int BrakeLeft = 23; //Red
const int RunningRight = 24;
const int RunningLeft = 25;
const int TurnRight = 26; //Amber
const int TurnLeft = 27;  //Amber
const int FlasherRight = 28; //Red
const int FlasherLeft = 29;  //Red
const int BackupRight = 30; //White
const int BackupLeft = 31;  //White
const int TrailerRight = 32;
const int TrailerLeft = 33;
const int Pump = 37;

// Input Pins
const int FuelLevel_IN = A0;
const int AirTankPSI_IN = A1;
const int Brake_IN = 45;
const int TurnRight_IN = 44;
const int TurnLeft_IN = 43;

//#################################################
// Setup Global Veriables
String SerData = "";
int AirPSI = 0;
int FuelLevel = 0;
String BrakeState = "OFF";
String EMLightState = "OFF";
String TurnLeftState = "OFF";
String TurnRightState = "OFF";
String FuelPumpState = "ON";
String BackupState = "OFF";
String RunningState = "OFF";

//#################################################
// Setup EM Flashing Lights
const int numberOfLights = 12;
byte pins[] = {LightBar1, LightBar2, LightBar3, LightBar4, LightBar5, LightBar6, FlasherRight, FlasherLeft, BrakeRight, BrakeLeft, TurnRight, TurnLeft};
byte pinState[numberOfLights];
long int changeTime[numberOfLights];
int flashRate[numberOfLights];

//#################################################
// Setup Turn Signal Flashing
const int signalInterval = 600; //Turn signal interval in milliseconds, SAE states 90 times a minute (600-700ms)
unsigned long LeftpreviousMillis = 0;
unsigned long RightpreviousMillis = 0;
unsigned long LeftcurrentMillis = millis();
unsigned long RightcurrentMillis = millis();

//#################################################
// Main Setup
void setup() {

  Serial.begin(115200);
  Serial.setTimeout(5);

  pinMode(LightBar1, OUTPUT);
  pinMode(LightBar2, OUTPUT);
  pinMode(LightBar3, OUTPUT);
  pinMode(LightBar4, OUTPUT);
  pinMode(LightBar5, OUTPUT);
  pinMode(LightBar6, OUTPUT);
  pinMode(BrakeRight, OUTPUT);
  pinMode(BrakeLeft, OUTPUT);
  pinMode(RunningRight, OUTPUT);
  pinMode(RunningLeft, OUTPUT);
  pinMode(TurnRight, OUTPUT);
  pinMode(TurnLeft, OUTPUT);
  pinMode(FlasherRight, OUTPUT);
  pinMode(FlasherLeft, OUTPUT);
  pinMode(BackupRight, OUTPUT);
  pinMode(BackupLeft, OUTPUT);
  pinMode(TrailerRight, OUTPUT);
  pinMode(TrailerLeft, OUTPUT);

  pinMode(Pump, OUTPUT);

  pinMode(AirTankPSI_IN, INPUT);
  pinMode(FuelLevel_IN, INPUT);

  pinMode(TurnRight_IN, INPUT_PULLUP);
  pinMode(TurnLeft_IN, INPUT_PULLUP);
  pinMode(Brake_IN, INPUT_PULLUP);

  for (int i = 0; i < numberOfLights; i++) {
    changeTime[i] = millis() + random(500, 900);
    flashRate[i] = random(300, 500);
    pinState[i] = HIGH;
  }
}

//#################################################
// Primary Loop
void loop() {

  //#################################################
  // Read Serial Port
  SerData = Serial.readString();
  Serial.println(SerData);

  //#################################################
  // Set states of fuctions
  if (SerData.indexOf("BackEMLightsON") > -1) {
    EMLightState = "ON";
  }
  if (SerData.indexOf("BackEMLightsOFF") > -1) {
    EMLightState = "OFF";
    EMLightsOFF();
  }
  if (SerData.indexOf("FuelPumpON") > -1) {
    FuelPumpState = "ON";
  }
  if (SerData.indexOf("FuelPumpOFF") > -1) {
    FuelPumpState = "OFF";
  }
  if (SerData.indexOf("BackupLightsON") > -1) {
    BackupState = "ON";
  }
  if (SerData.indexOf("BackupLightsOFF") > -1) {
    BackupState = "OFF";
  }
  if (SerData.indexOf("RunningLightsON") > -1) {
    RunningState = "ON";
  }
  if (SerData.indexOf("RunningLightsOFF") > -1) {
    RunningState = "OFF";
  }
  if ((digitalRead(TurnRight_IN) == LOW) || (SerData.indexOf("RightTurnON") > -1)) {
    TurnRightState = "ON";
  }
  if ((digitalRead(TurnRight_IN) == HIGH) || (SerData.indexOf("RightTurnOFF") > -1)) {
    TurnRightState = "OFF";
  }
  if ((digitalRead(TurnLeft_IN) == LOW) || (SerData.indexOf("RightLeftON") > -1)) {
    TurnLeftState = "ON";
  }
  if ((digitalRead(TurnLeft_IN) == HIGH) || (SerData.indexOf("RightLeftOFF") > -1)) {
    TurnLeftState = "OFF";
  }
  if (digitalRead(Brake_IN) == LOW) {
    BrakeState = "ON";
  }
  if (digitalRead(Brake_IN) == HIGH) {
    BrakeState = "OFF";
  }

  //#################################################
  // Run the Sub rutines
  if (EMLightState == "ON") {
    EMLightsON();
  }
  if (FuelPumpState == "ON") {
    FuelPumpON();
  }
  if (FuelPumpState == "OFF") {
    FuelPumpOFF();
  }
  if (BackupState == "ON") {
    BackupLightsON();
  }
  if (BackupState == "OFF") {
    BackupLightsOFF();
  }
  if (RunningState == "ON") {
    RunningLightsON();
  }
  if (RunningState == "OFF") {
    RunningLightsOFF();
  }
  if (BrakeState == "ON") {
    BrakeON();
  }
  if (BrakeState == "OFF") {
    BrakeOFF();
  }
  if (TurnRightState == "ON") {
    TurnRightON();
  }
  if (TurnRightState == "OFF") {
    TurnRightOFF();
  }
  if (TurnLeftState == "ON") {
    TurnLeftON();
  }
  if (TurnLeftState == "OFF") {
    TurnLeftOFF();
  }

  Sensors();
}

//#################################################
//#################################################
void BrakeON() {
  digitalWrite(BrakeRight, LOW);
  digitalWrite(BrakeLeft, LOW);
  digitalWrite(LightBar1, LOW);
  digitalWrite(LightBar3, LOW);
  digitalWrite(LightBar4, LOW);
  digitalWrite(LightBar6, LOW);
  digitalWrite(FlasherRight, LOW);
  digitalWrite(FlasherLeft, LOW);
  digitalWrite(TrailerRight, LOW);
  digitalWrite(TrailerLeft, LOW);
}

void BrakeOFF() {
  if (EMLightState == "OFF") {
    digitalWrite(BrakeRight, HIGH);
    digitalWrite(BrakeLeft, HIGH);
    digitalWrite(LightBar1, HIGH);
    digitalWrite(LightBar3, HIGH);
    digitalWrite(LightBar4, HIGH);
    digitalWrite(LightBar6, HIGH);
    digitalWrite(FlasherRight, HIGH);
    digitalWrite(FlasherLeft, HIGH);
  }
  if (TurnRightState == "OFF") {
    digitalWrite(TrailerRight, HIGH);
  }
  if (TurnLeftState == "OFF") { 
    digitalWrite(TrailerLeft, HIGH);
  }
}

//#################################################
void EMLightsON() {
  if (BrakeState == "OFF") {
    for (int i = 0; i < numberOfLights; i++) {
      if (changeTime[i] <= millis()) {
        pinState[i] = ~pinState[i];
        digitalWrite(pins[i], pinState[i]);
        changeTime[i] = millis() + flashRate[i];
        flashRate[i] = random(300, 500);
      }
    }
  }
  if (BrakeState == "ON")  {
    digitalWrite(TurnRight, HIGH);
    digitalWrite(LightBar5, HIGH);
    digitalWrite(TurnLeft, HIGH);
    digitalWrite(LightBar2, HIGH);
  }
}

void EMLightsOFF() {
  digitalWrite(LightBar1, HIGH);
  digitalWrite(LightBar2, HIGH);
  digitalWrite(LightBar3, HIGH);
  digitalWrite(LightBar4, HIGH);
  digitalWrite(LightBar5, HIGH);
  digitalWrite(LightBar6, HIGH);
  digitalWrite(FlasherRight, HIGH);
  digitalWrite(FlasherLeft, HIGH);
  digitalWrite(TurnRight, HIGH);
  digitalWrite(TurnLeft, HIGH);
}

//#################################################
void FuelPumpON() {
  digitalWrite(Pump, LOW);
}

void FuelPumpOFF() {
  digitalWrite(Pump, HIGH);
}

//#################################################
void BackupLightsON() {
  digitalWrite(BackupRight, LOW);
  digitalWrite(BackupLeft, LOW);
}

void BackupLightsOFF() {
  digitalWrite(BackupRight, HIGH);
  digitalWrite(BackupLeft, HIGH);
}

//#################################################
void RunningLightsON() {
  digitalWrite(RunningRight, LOW);
  digitalWrite(RunningLeft, LOW);
}

void RunningLightsOFF() {
  digitalWrite(RunningRight, HIGH);
  digitalWrite(RunningLeft, HIGH);
}

//#################################################
void TurnRightON() {
  if (EMLightState == "OFF")  {
    RightcurrentMillis = millis();
    if (RightcurrentMillis - RightpreviousMillis >= signalInterval) { //if time elapsed is greater than the signal interval
      RightpreviousMillis = RightcurrentMillis; //then reset time
      if (digitalRead(TurnRight) == HIGH) { //if right state was low
        digitalWrite(TurnRight, LOW);
        digitalWrite(LightBar5, LOW);
        digitalWrite(TrailerRight, LOW);
      }
      else {
        digitalWrite(TurnRight, HIGH);
        digitalWrite(LightBar5, HIGH);
        if (BrakeState == "OFF")  {
          digitalWrite(TrailerRight, HIGH);
        }
      }
    }
  }
}

void TurnRightOFF() {
  if (EMLightState == "OFF") {
    digitalWrite(TurnRight, HIGH);
    digitalWrite(LightBar5, HIGH);
    if (BrakeState == "OFF")  {
      digitalWrite(TrailerRight, HIGH);
    }
  }
}

//#################################################
void TurnLeftON() {
  if (EMLightState == "OFF") {
    LeftcurrentMillis = millis();
    if (LeftcurrentMillis - LeftpreviousMillis >= signalInterval) { //if time elapsed is greater than the signal interval
      LeftpreviousMillis = LeftcurrentMillis; //then reset time
      if (digitalRead(TurnLeft) == HIGH) {
        digitalWrite(TurnLeft, LOW);
        digitalWrite(LightBar2, LOW);
        digitalWrite(TrailerLeft, LOW);
      }
      else {
        digitalWrite(TurnLeft, HIGH);
        digitalWrite(LightBar2, HIGH);
        if (BrakeState == "OFF")  {
          digitalWrite(TrailerLeft, HIGH);
        }
      }
    }
  }
}

void TurnLeftOFF() {
  if (EMLightState == "OFF") {
    digitalWrite(TurnLeft, HIGH);
    digitalWrite(LightBar2, HIGH);
    if (BrakeState == "OFF")  {
      digitalWrite(TrailerLeft, HIGH);
    }
  }
}

//#################################################
void Sensors() {

  AirPSI = analogRead(AirTankPSI_IN);
  FuelLevel = analogRead(FuelLevel_IN);

  Serial.println(AirPSI);
  Serial.println(FuelLevel);
}
