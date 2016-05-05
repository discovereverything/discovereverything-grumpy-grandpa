/**
 *  GrumpGrandpa
 *  
 *  David Janes
 *  Discover Everything
 *  2016-05-04
 *  
 *  Re-written version of Microduino's Grumpy Grandpa.
 */

/*
 *  Change this to a bigger number
 */
const int microphone_trigger_volume = 200;

/**
 *  These correspond to the pins that the sensors are plugged 
 *  into on the Microduino Sensor Hub
 */
const int microphone_pin = A0;  // slot A0/A1
const int buzzer_pin = 6;       // slot 6/7
const int key_pin = 4;          // slot 4/5

/**
 *  Used to track the last time we changed the tone
 */
long last_millis;

/**
 *  Will be true if the buzzer should be on, otherwise no.
 *  The code in loop() will modify this depending on the volume
 *  and the switch
 */
boolean buzzer_should_be_on;

/**
 *  This is the current tone of the buzzer, and the direction
 *  it should be changed in next, so we get an up-down
 *  buzzer sound
 */
int tone_buzzer;
int tone_increment;


/**
 *  This is called once
 */
void setup() {
  // in case you want to look at what's happening on the serial monitor
  Serial.begin(115200);

  // set up the pins - these are programmable in purpose, so this has to be done
  pinMode(microphone_pin, INPUT);
  pinMode(key_pin, INPUT);
  pinMode(buzzer_pin, OUTPUT);

  // initial values for our variables being tracked
  buzzer_should_be_on = false;
  tone_buzzer = 400;
  tone_increment = 3;

  last_millis = millis();
}

/**
 *  This is run over and over forever
 */
void loop() {
  // if the buzzer is off, should we turn it on (because it's loud)
  if (!buzzer_should_be_on) {
    int microphone_value = analogRead(microphone_pin);
    if (microphone_value > microphone_trigger_volume) {
      Serial.print("turning ON buzzer, volume is: ");
      Serial.println(microphone_value);
      buzzer_should_be_on = true;
    }
  }

  // if the buzzer is on, should we turn it off (because the switch is open)
  if (buzzer_should_be_on) {
    if (digitalRead(key_pin) == 0) {
      Serial.println("turning OFF buzzer");
      buzzer_should_be_on = false;
    }
  }

  // then do the appropriate thing
  if (buzzer_should_be_on) {
    buzzer_sound();
  } else {
    buzzer_off();
  }
}

/**
 *  Turn the buzzer off
 */
void buzzer_off()
{
    noTone(buzzer_pin);
}

/**
 *  This makes the buzzer sound, changing it over time
 */
void buzzer_sound()
{
  if ((millis() - last_millis) < 10) {
    return;
  }
  
  tone_buzzer += tone_increment;
  if ((tone_buzzer < 200) || (tone_buzzer > 800)) {
    tone_increment = -tone_increment;
  }
    
  tone(buzzer_pin, tone_buzzer);
  last_millis = millis();
}
