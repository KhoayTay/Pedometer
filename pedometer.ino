#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);
const int x_pin = A1;
const int y_pin = A2;
const int z_pin = A3;
byte p[8] = {0x1F,  0x1F,  0x1F,  0x1F,  0x1F,  0x1F,  0x1F,  0x1F};

float threshold = 15;
float x_value[100] = {0};
float y_value[100] = {0};
float z_value[100] = {0};
float x_avg, y_avg, z_avg;
int steps, flag = 0;

// void turnLEDon() {
//   digitalWrite(3, HIGH);
// }

void setup() {
  Serial.begin(9600);
  // pinMode(3, OUTPUT);
  pinMode(13, OUTPUT);
  // attachInterrupt(digitalPinToInterrupt(2), turnLEDon, HIGH);
  lcd.begin(16, 2);
  lcd.backlight();
  // lcd.print("Test LCD");
  lcd.clear();
  calibrate();
}

void loop() {

  for (int w = 0; w < 16; w++) {
    lcd.write(byte(0));
    delay(500);
  }

  int accel = 0;  //set accelerate
  float total_vect[100] = {0}; //total acceleration vector
  float total_avg[100] = {0};  //average of max and min acceleration value
  float x_accel[100] = {0};
  float y_accel[100] = {0};
  float z_accel[100] = {0};

  for (int a = 0; a < 100; a++) {
    x_accel[a] = float(analogRead(x_pin));
    delay(1);
    y_accel[a] = float(analogRead(y_pin));
    delay(1);
    z_accel[a] = float(analogRead(z_pin));
    delay(1);

    total_vect[a] = sqrt(((x_accel[a] - x_avg) * (x_accel[a] - x_avg)) + ((y_accel[a] - y_avg) * (y_accel[a] - y_avg)) + ((z_accel[a] - z_avg) * (z_accel[a] - z_avg)));
    total_avg[a] = (total_vect[a] + total_vect[a - 1]) / 2 ;

    Serial.println("total_avg[a]");
    Serial.println(total_avg[a]);
    Serial.println("flag");
    Serial.println(flag);
    delay(100);

    if (total_avg[a] > threshold && flag == 0) {
      steps = steps + 1;
      flag = 1;
    }

    else if (total_avg[a] > threshold && flag == 1) {
      // Don't Count
    }

    if (total_avg[a] < threshold   && flag == 1) {
      flag = 0;
    }

    if (steps < 0) {
      steps = 0;
    }

    Serial.println('\n');
    Serial.print("steps: ");
    Serial.println(steps);
    lcd.print("Steps: ");
    lcd.print(steps);
    digitalWrite(13,HIGH);
    delay(500);
    digitalWrite(13,LOW);
    delay(500);
    lcd.clear();
  }
  delay(1000);
}

void calibrate() {
  float sum_x = 0;
  float sum_y = 0;
  float sum_z = 0;

  for (int i = 0; i < 100; i++) {
    x_value[i] = float(analogRead(x_pin));
    sum_x = x_value[i] + sum_x;
  }

  delay(100);
  x_avg = sum_x / 100.0;
  Serial.println(x_avg);

  for (int j = 0; j < 100; j++) {
    y_value[j] = float(analogRead(y_pin));
    sum_y = y_value[j] + sum_y;
  }

  y_avg = sum_y / 100.0;
  Serial.println(y_avg);
  delay(100);

  for (int q = 0; q < 100; q++) {
    z_value[q] = float(analogRead(z_pin));
    sum_z = z_value[q] + sum_z;
  }

  z_avg = sum_z / 100.0;
  delay(100);
  Serial.println(z_avg);

}