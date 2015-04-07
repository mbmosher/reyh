
// Arduino code for Reyh

int rPin = 10;
int gPin = 9;
int bPin = 11;
int solenoidPin = 4;
int doorTog = 0;
int smoothNum = 100;
int lightSmooth[100];
int lightIndex = 0;
unsigned long lightTotal = 0;
unsigned long doorTimer = 0;

void setup() {
  // set pins
  pinMode(rPin, OUTPUT);
  pinMode(gPin, OUTPUT);
  pinMode(bPin, OUTPUT);
  pinMode(solenoidPin, OUTPUT);
  
  // for debugging
  Serial.begin(9600);
  
  // Zero light smoothing array
  for (int i = 0; i < smoothNum; i++) {
    lightSmooth[i] = 0;
  }
  
  Serial.println("Init complete");
  
}

void loop() {
  //read sensors
  int doorVal = analogRead(A0);
  int lightVal = analogRead(A2);
  if (doorVal < 500) {
    doorVal = 0;
  } else {
    doorVal = 1;
  }
  
  // smooth light
  lightTotal = lightTotal - lightSmooth[lightIndex];
  lightSmooth[lightIndex] = lightVal;
  lightTotal = lightTotal + lightSmooth[lightIndex];
  lightIndex++;
  if (lightIndex >= smoothNum) { lightIndex = 0; }
  int lightAve = lightTotal / smoothNum;
  
  
    // check door and trigger solenoid
  if(millis() - doorTimer > 7000) {
    Serial.println("door timer passed");
  if(doorVal != doorTog && doorVal == 0) {
    Serial.println("Door open");
    digitalWrite(solenoidPin, HIGH);
    Serial.println("solenoid triggered");
    delay(30);
    digitalWrite(solenoidPin, LOW);
    doorTog = doorVal;
    doorTimer = millis();
    for(int i = 0; i < 255; i++) {
      Serial.println("Door loop");
      analogWrite(bPin, 255-i);
      analogWrite(rPin, i);
      int g = map(i, 0, 255, 255, 35);
      analogWrite(gPin, g);
      delay(5);
    }
  }
  }
  
  if(doorVal != doorTog && doorVal == 1) {
    Serial.println("Door closed");
    for(int i = 0; i < 255; i++) {
      Serial.println("Door loop");
      analogWrite(rPin, 255-i);
      analogWrite(bPin, i);
      int g = map(i, 0, 255, 35, 255);
      analogWrite(gPin, g);
      delay(5);
    }
    doorTog = doorVal;
  }
  
  
  // control RGBLED lights
  if (lightAve > 10) {
    digitalWrite(rPin, LOW);
    digitalWrite(gPin, LOW);
    digitalWrite(bPin, LOW);
  } else if (!doorVal) {
    digitalWrite(rPin, HIGH);
    analogWrite(gPin, 35);
    digitalWrite(bPin, LOW);
  } else {
    digitalWrite(rPin, LOW);
    digitalWrite(gPin, HIGH);
    digitalWrite(bPin, HIGH);
  }
 
  
  Serial.print("door = ");
  Serial.print(doorVal);
  Serial.print(", light Val = ");
  Serial.print(lightVal);
  Serial.print(", light Ind = ");
  Serial.print(lightIndex);
  Serial.print(", light Tot = ");
  Serial.print(lightTotal);
  Serial.print(", light Ave = ");
  Serial.println(lightAve);
  
  //delay(100);
}
