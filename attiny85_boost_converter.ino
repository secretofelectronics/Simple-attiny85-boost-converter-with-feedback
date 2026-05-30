#define PWM_PIN     0      // PB0
#define FB_PIN      A3     // PB3
#define POT_PIN     A2     // PB4

float VREF = 5.0;

uint8_t duty = 20;

float targetVoltage;
float measuredVoltage;

void setup()
{
  pinMode(PWM_PIN, OUTPUT);

  // Fast PWM on PB0
  TCCR0A = 0;
  TCCR0B = 0;

  TCCR0A |= (1 << COM0A1);
  TCCR0A |= (1 << WGM01);
  TCCR0A |= (1 << WGM00);

  TCCR0B |= (1 << CS00);

  OCR0A = duty;
}

void loop()
{
  //-------------------------------
  // Read Potentiometer
  //-------------------------------

  int potValue = analogRead(POT_PIN);

  targetVoltage = map(potValue,
                      0,
                      1023,
                      50,
                      300);

  targetVoltage /= 10.0;

  //-------------------------------
  // Read Output Voltage
  //-------------------------------

  int adc = analogRead(FB_PIN);

  float adcVoltage =
      (adc * VREF) / 1023.0;

  // Divider:
  // R1 = 100k
  // R2 = 20k

  measuredVoltage =
      adcVoltage *
      ((100.0 + 20.0) / 20.0);

  //-------------------------------
  // Regulation
  //-------------------------------

  float error =
      targetVoltage -
      measuredVoltage;

  if(error > 0.15)
  {
    duty++;
  }
  else if(error < -0.15)
  {
    duty--;
  }

  //-------------------------------
  // Duty Limits
  //-------------------------------

  if(duty < 10)
    duty = 10;

  if(duty > 240)
    duty = 240;

  OCR0A = duty;

  delay(2);
}