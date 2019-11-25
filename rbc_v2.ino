//#################################################
// Setup Pins

// Transistor Pins
const int LightBar1 = 3;        //Red - Left
const int LightBar2 = 5;        //Amber - Left
const int LightBar3 = 7;        //Red - Center/Left
const int LightBar4 = 9;        //Red - Center/Right
const int LightBar5 = 11;       //Amber - Right
const int LightBar6 = 13;       //Red - Right

const int BrakeRight = 6;       //Red
const int TurnRight = 10;        //Amber
const int FlasherRight = 2;    //Red

const int BrakeLeft = 8;        //Red
const int TurnLeft = 12;         //Amber
const int FlasherLeft = 4;     //Red

// Mosfet Pins
const int BackupAll = 44;     //White
const int Pump = 46;

// Relay Output Pins
const int TrailerRight = 29;    //Red
const int TrailerLeft = 26;     //Red
const int RunningLights = 28;
const int TBController = 27;

// Input Pins
const int Brake_IN = 69;
const int TurnRight_IN = 68;
const int TurnLeft_IN = 67;
const int Emergency_IN = 66;
const int Backup_IN = 65;
const int Running_IN = 64;

const int FuelLevel_IN = A0;
const int AirTankPSI_IN = A1;

//#################################################
// Setup Global Veriables
String SerData = "";
int AirPSI = 0;
int FuelLevel = 0;
String BrakeState = "OFF";
String EMLightState = "OFF";
String TurnLeftState = "OFF";
String TurnRightState = "OFF";
String TurnLeftBlinker = "OFF";
String TurnRightBlinker = "OFF";
String FuelPumpState = "ON";
String BackupState = "OFF";
String RunningState = "OFF";
int RunningLevel = 0;

//#################################################
// Setup Turn Signal Flashing
const int signalInterval = 600; //Turn signal interval in milliseconds, SAE states 90 times a minute (600-700ms)
long int previousMillis = 0;
long int currentMillis = millis();

//#################################################
// Setup EM Flashing Lights
const int numberOfLights = 12;
byte pins[] = {LightBar1, LightBar2, LightBar3, LightBar4, LightBar5, LightBar6, FlasherRight, FlasherLeft, BrakeRight, BrakeLeft, TurnRight, TurnLeft};
byte pinState[numberOfLights];
long int changeTime[numberOfLights];
int flashRate[numberOfLights];

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
  pinMode(TurnRight, OUTPUT);
  pinMode(TurnLeft, OUTPUT);
  pinMode(FlasherRight, OUTPUT);
  pinMode(FlasherLeft, OUTPUT);
  
  pinMode(BackupAll, OUTPUT);
  pinMode(Pump, OUTPUT);

  pinMode(RunningLights, OUTPUT);
  pinMode(TrailerRight, OUTPUT);
  pinMode(TrailerLeft, OUTPUT);
  pinMode(TBController, OUTPUT);


  pinMode(AirTankPSI_IN, INPUT);
  pinMode(FuelLevel_IN, INPUT);

  pinMode(TurnRight_IN, INPUT_PULLUP);
  pinMode(TurnLeft_IN, INPUT_PULLUP);
  pinMode(Brake_IN, INPUT_PULLUP);
  pinMode(Emergency_IN, INPUT_PULLUP);
  pinMode(Backup_IN, INPUT_PULLUP);
  pinMode(Running_IN, INPUT_PULLUP);

  for (int i = 0; i < numberOfLights; i++) {
    changeTime[i] = millis() + random(500, 900);
    flashRate[i] = random(300, 500);
    pinState[i] = LOW;
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
  if ((digitalRead(Emergency_IN) == LOW) || (SerData.indexOf("BackEMLightsON") > -1)) {
    EMLightState = "ON";
  }
  if ((digitalRead(Emergency_IN) == HIGH) || (SerData.indexOf("BackEMLightsOFF") > -1)) {
    if (EMLightState == "ON") {
      EMLightsOFF();
      EMLightState = "OFF";
    }
  }
  if (SerData.indexOf("FuelPumpON") > -1) {
    FuelPumpState = "ON";
  }
  if (SerData.indexOf("FuelPumpOFF") > -1) {
    FuelPumpState = "OFF";
  }
  if ((digitalRead(Backup_IN) == LOW) || (SerData.indexOf("BackupLightsON") > -1)) {
    BackupState = "ON";
  }
  if ((digitalRead(Backup_IN) == HIGH) || (SerData.indexOf("BackupLightsOFF") > -1)) {
    BackupState = "OFF";
  }
  if ((digitalRead(Running_IN) == LOW) || (SerData.indexOf("RunningLightsON") > -1)) {
    RunningState = "ON";
    RunningLevel = 30;
  }
  if ((digitalRead(Running_IN) == HIGH) || (SerData.indexOf("RunningLightsOFF") > -1)) {
    RunningState = "OFF";
    RunningLevel = 0;
  }
  if (digitalRead(TurnRight_IN) == LOW) {
    TurnRightState = "ON";
  }
  if (digitalRead(TurnRight_IN) == HIGH) {
    TurnRightState = "OFF";
  }
  if (digitalRead(TurnLeft_IN) == LOW) {
    TurnLeftState = "ON";
  }
  if (digitalRead(TurnLeft_IN) == HIGH) {
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
  digitalWrite(BrakeRight, HIGH);
  digitalWrite(BrakeLeft, HIGH);
  digitalWrite(FlasherRight, HIGH);
  digitalWrite(FlasherLeft, HIGH);
  digitalWrite(LightBar1, HIGH);
  digitalWrite(LightBar3, HIGH);
  digitalWrite(LightBar4, HIGH);
  digitalWrite(LightBar6, HIGH);
  digitalWrite(TrailerRight, LOW);
  digitalWrite(TrailerLeft, LOW);
  digitalWrite(TBController, LOW);
}

void BrakeOFF() {
  digitalWrite(TBController, HIGH);
  if (TurnRightState == "OFF") {
    digitalWrite(TrailerRight, HIGH);
  }
  if (TurnLeftState == "OFF") { 
    digitalWrite(TrailerLeft, HIGH);
  }
  
  if (EMLightState == "OFF") {
    analogWrite(BrakeRight, RunningLevel);
    analogWrite(BrakeLeft, RunningLevel);
    analogWrite(FlasherRight, RunningLevel);
    analogWrite(FlasherLeft, RunningLevel);
    analogWrite(LightBar1, RunningLevel);
    analogWrite(LightBar3, RunningLevel);
    analogWrite(LightBar4, RunningLevel);
    analogWrite(LightBar6, RunningLevel);
  }

}

//#################################################
void EMLightsON() {
  if (BrakeState == "OFF") {
    for (int i = 0; i < numberOfLights; i++) {
      if (changeTime[i] <= millis()) {
        Serial.println("Running");
        pinState[i] = ~pinState[i];
        digitalWrite(pins[i], pinState[i]);
        changeTime[i] = millis() + flashRate[i];
        flashRate[i] = random(300, 500);
      }
    }
  }
  if (BrakeState == "ON")  {
    analogWrite(TurnRight, RunningLevel);
    analogWrite(LightBar5, RunningLevel);
    analogWrite(TurnLeft, RunningLevel);
    analogWrite(LightBar2, RunningLevel);
  }
}

void EMLightsOFF() {
  analogWrite(LightBar1, RunningLevel);
  analogWrite(LightBar2, RunningLevel);
  analogWrite(LightBar3, RunningLevel);
  analogWrite(LightBar4, RunningLevel);
  analogWrite(LightBar5, RunningLevel);
  analogWrite(LightBar6, RunningLevel);
  analogWrite(FlasherRight, RunningLevel);
  analogWrite(FlasherLeft, RunningLevel);
  analogWrite(TurnRight, RunningLevel);
  analogWrite(TurnLeft, RunningLevel);
}

//#################################################
void FuelPumpON() {
  digitalWrite(Pump, HIGH);
}

void FuelPumpOFF() {
  digitalWrite(Pump, LOW);
}

//#################################################
void BackupLightsON() {
  digitalWrite(BackupAll, HIGH);
}

void BackupLightsOFF() {
  digitalWrite(BackupAll, LOW);
}

//#################################################
void RunningLightsON() {
  digitalWrite(RunningLights, LOW);
}

void RunningLightsOFF() {
  digitalWrite(RunningLights, HIGH);
}

//#################################################
void TurnRightON() {
  if (EMLightState == "OFF")  {
    currentMillis = millis();
    if (currentMillis - previousMillis >= signalInterval) { //if time elapsed is greater than the signal interval
      previousMillis = currentMillis; //then reset time
      
      if (TurnRightBlinker == "OFF") {
        digitalWrite(TurnRight, HIGH);
        digitalWrite(LightBar5, HIGH);
        digitalWrite(TrailerRight, LOW);
        TurnRightBlinker = "ON";
      }
      
      else {
        analogWrite(TurnRight, RunningLevel);
        analogWrite(LightBar5, RunningLevel);
        if (BrakeState == "OFF")  {
          digitalWrite(TrailerRight, HIGH);
        }
        TurnRightBlinker = "OFF";
      }
    }
  }
}

void TurnRightOFF() {
  if (EMLightState == "OFF") {
    analogWrite(TurnRight, RunningLevel);
    analogWrite(LightBar5, RunningLevel);
    if (BrakeState == "OFF")  {
      digitalWrite(TrailerRight, HIGH);
    }
    TurnRightBlinker = "OFF";
  }
}

//#################################################
void TurnLeftON() {
  if (EMLightState == "OFF") {
    currentMillis = millis();
    if (currentMillis - previousMillis >= signalInterval) { //if time elapsed is greater than the signal interval
      previousMillis = currentMillis; //then reset time
      
      if (TurnLeftBlinker == "OFF") {
        digitalWrite(TurnLeft, HIGH);
        digitalWrite(LightBar2, HIGH);
        digitalWrite(TrailerLeft, LOW);
        TurnLeftBlinker = "ON";
      }

      else {
        analogWrite(TurnLeft, RunningLevel);
        analogWrite(LightBar2, RunningLevel);
        if (BrakeState == "OFF")  {
          digitalWrite(TrailerLeft, HIGH);
        }
        TurnLeftBlinker = "OFF";
      }
    }
  }
}

void TurnLeftOFF() {
  if (EMLightState == "OFF") {
    analogWrite(TurnLeft, RunningLevel);
    analogWrite(LightBar2, RunningLevel);
    if (BrakeState == "OFF")  {
      digitalWrite(TrailerLeft, HIGH);
    }
      TurnLeftBlinker = "OFF";
  }
}

//#################################################
void Sensors() {

  AirPSI = analogRead(AirTankPSI_IN);
  FuelLevel = analogRead(FuelLevel_IN);

  Serial.println(AirPSI);
  Serial.println(FuelLevel);
}
