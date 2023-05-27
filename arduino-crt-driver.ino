#define ALPHA_PIN 3

// y has diode connected to alpha
#define X_PIN 6
#define Y_PIN 5

#define TIMING_PIN 10

void setup() {
    //pinMode(LED_BUILTIN, OUTPUT);
    Serial.begin(9600);

    // TCCR2B = TCCR2B & B11111000 | B00000001; // for PWM frequency of 31372.55 Hz, pin D3 & D11
    TCCR2B = TCCR2B & B11111000 | B00000010;    // for PWM frequency of 3921.16 Hz, pin D3 & D11
    // TCCR2B = TCCR2B & B11111000 | B00000011; // for PWM frequency of 980.39 Hz, pin D3 & D11
    
    TCCR0B = TCCR0B & B11111000 | B00000001; // for PWM frequency of 62500.00 Hz, pin D5 & D6

    pinMode(ALPHA_PIN, OUTPUT);
    pinMode(X_PIN, OUTPUT);
    pinMode(Y_PIN, OUTPUT);

    pinMode(TIMING_PIN, OUTPUT);

    analogWrite(ALPHA_PIN, 0);
    digitalWrite(TIMING_PIN, 0);
}


void loop() {
    playPong();
    return;
}
