#define ALPHA_PIN 3

// y has diode connected to alpha
#define X_PIN 6
#define Y_PIN 5

#define TIMING_PIN 10

// pwm bounds (lower bound needs to be enough to overcome diode)
#define LOWER_BOUND 80
#define UPPER_BOUND 185

#define DIFF ((float)(UPPER_BOUND-LOWER_BOUND))

#define toScreenVal(f) ((int)(DIFF*(f)) + LOWER_BOUND)
#define smoothMoveBeam(xf, yf, time) _smoothMoveBeam(toScreenVal(xf), toScreenVal(yf), time)

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

void testCircle(float phase1, float phase2, float mult1, float mult2){
    int steps = 60;
    float stepsInv = 1./steps;


    int x;
    int y;
    int gamma;

    float t;

    analogWrite(ALPHA_PIN, 0);

    for(int i=0; i<steps; i++){
        t = M_PI*2*(float)i*stepsInv ;
        x = toScreenVal( 0.5*(sin(mult1*t+phase1)+1) );
        y = toScreenVal( 0.5*(sin(mult2*t+phase2)+1) );
        // smoothMoveBeam(x, y, 1);


        // beam cutting test
        analogWrite(ALPHA_PIN, (int)(255*(i%2==0)));

        // beam fading test
        // analogWrite(ALPHA_PIN, (int)(255*(i*stepsInv)));
        
        analogWrite(X_PIN, x);
        analogWrite(Y_PIN, y);

    }
}

void testRotatingSinusoid(){
    for(float i = 0.; i < 2*M_PI; i+=0.05){
        testCircle(0, M_PI_2, 2, 3);
    }
}

void printFloat(float x){
    char buff[32];
    dtostre(x, buff, 5, 0);
    Serial.print(buff);
}
void printFloatln(float x){
    char buff[32];
    dtostre(x, buff, 7, 0);
    Serial.println(buff);
}

void testSmoothMoveBeam(int tauFactor){
    smoothMoveBeam(0, 0,     tauFactor);
    smoothMoveBeam(0.5, 0.5, tauFactor);
    smoothMoveBeam(1., 0.,   tauFactor);
    smoothMoveBeam(0., 1.,   tauFactor);
    smoothMoveBeam(1., 1.,   tauFactor);
}
void testSmoothMoveBeamSquare(int tauFactor){
    smoothMoveBeam(0, 0,     tauFactor);
    smoothMoveBeam(1., 0.,   tauFactor);
    smoothMoveBeam(1., 1.,   tauFactor);
    smoothMoveBeam(0., 1.,   tauFactor);
}

void testSmoothMoveBeamPong(){
    analogWrite(ALPHA_PIN, 255);
    smoothMoveBeam(0, 0.1, 2);
    delayMicroseconds(100);
    analogWrite(ALPHA_PIN, 0);
    smoothMoveBeam(0, 0.3, 2);


    analogWrite(ALPHA_PIN, 255);
    smoothMoveBeam(0.3, 0.4, 2);
    analogWrite(ALPHA_PIN, 0);
    smoothMoveBeam(0.32, 0.43, 2);

    analogWrite(ALPHA_PIN, 255);
    smoothMoveBeam(1, 0.7, 2);
    analogWrite(ALPHA_PIN, 0);
    smoothMoveBeam(1, 0.9, 2);
}


void loop() {
    // testSmoothMoveBeam(2);
    drawNum(12345, 0.2, 0, 0);
    // drawNum(6789, 0.2, 0, 0.3);
    //drawNum(67890, 0.2, 0, 0.3);
    return;

    testSmoothMoveBeamPong();
    //testFlickBeam();
    // testFlickBeamSquare();
    testSmoothMoveBeamSquare(2);
    return;

    for(int i = 2; i < 10; i++){
        testSmoothMoveBeamSquare(i);
        //testSmoothMoveBeam(i);
    }
    return;

    analogWrite(X_PIN, 0);
    analogWrite(Y_PIN, 0);
    delay(100000);
    analogWrite(X_PIN, 128);
    analogWrite(Y_PIN, 128);
    delay(100000);
    analogWrite(X_PIN, 255);
    analogWrite(Y_PIN, 255);
    delay(100000);
}
