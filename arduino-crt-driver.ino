#define ALPHA_PIN 3

// y has diode connected to alpha
#define X_PIN 6
#define Y_PIN 5

#define TIMING_PIN 10

// pwm bounds (lower bound needs to be enough to overcome diode)
#define LOWER_BOUND 100 
#define UPPER_BOUND 200

#define DIFF ((float)(UPPER_BOUND-LOWER_BOUND))

#define toScreenVal(f) ((int)(DIFF*(f)) + LOWER_BOUND)

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

int z[] = {
    0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,
    0,0,1,0,0,0,0,0,0,0,
    0,0,1,0,0,0,0,0,0,0,
    0,0,1,0,0,0,0,0,0,0,
    0,0,1,0,0,0,0,0,0,0,
    0,0,1,0,0,0,0,0,0,0,
    0,0,1,0,0,0,0,0,0,0,
    0,0,1,1,1,1,1,1,0,0,
    0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,
};

void testScan(){
    int steps = 15;
    float factor = 255./steps;
    for(int i = 0; i < steps; i++){

        analogWrite(ALPHA_PIN, 255);
        analogWrite(Y_PIN, LOWER_BOUND);
        analogWrite(X_PIN, toScreenVal((float)i/steps));
        delay(5);

        for(int j = 0; j < steps; j++){
            analogWrite(ALPHA_PIN, 255);
            analogWrite(Y_PIN, toScreenVal((float)j/steps));
            //if (z[i+steps*j] == 1){
                analogWrite(ALPHA_PIN, 0);
            //}
            //delay(10);
        }
    }

    analogWrite(ALPHA_PIN, 255);
    analogWrite(X_PIN, LOWER_BOUND);
    analogWrite(Y_PIN, LOWER_BOUND);
    delay(200);
}


void drawPixel(float x, float y){
    // analogWrite(ALPHA_PIN, 255);
    analogWrite(X_PIN, toScreenVal(x));
    analogWrite(Y_PIN, toScreenVal(y));
    delay(50);
    // delay(2);
    // analogWrite(ALPHA_PIN, 0);
}

void snakeScan(){
    // must be even and greater than 2
    int steps = 8;
    float stepsInv = 1./(steps-1);
    int i;
    int j;

    // first layer
    for (i = 0; i < steps; i++){
        drawPixel(i*stepsInv, 0);
    }


    // snake up
    for (j = 1; j < steps-1; j++){
        if(j%2==0){
            // forward
            for(i=steps/2; i<steps; i++){
                drawPixel(i*stepsInv, j*stepsInv);
            }
        }
        else {
            // backwards
            for(i=steps-1; i >= steps/2; i--){
                drawPixel(i*stepsInv, j*stepsInv);
            }
        }
    }


    // top layer
    for (i=steps-1; i >= 0; i--){
        drawPixel(i*stepsInv, 1);
    }

    // snake down
    for (j=steps-2; j >= 1; j--){
        if(j%2==0){
            // forward
            for(i=0; i < steps/2; i++){
                drawPixel(i*stepsInv, j*stepsInv);
            }
        }
        else {
            // backwards
            for(i = steps/2-1; i >= 0; i--){
                drawPixel(i*stepsInv, j*stepsInv);
            }
        }
    }
}

void testLine(){
    // reset to zero
    analogWrite(ALPHA_PIN, 255);
    analogWrite(Y_PIN, toScreenVal(0));
    delay(100);

    // draw line to 85
    analogWrite(ALPHA_PIN, 0);
    analogWrite(Y_PIN, toScreenVal(1./3.));
    delay(50);

    // move to 170
    analogWrite(ALPHA_PIN, 255);
    analogWrite(Y_PIN, toScreenVal(2./3.));
    delay(50);

    // draw line to 255
    analogWrite(ALPHA_PIN, 0);
    analogWrite(Y_PIN, toScreenVal(1));
    delay(50);
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


void loop() {
    testSmoothMoveBeam(3);
    return;

    flickBeam(0, 0);
    flickBeam(0.5, 0.5);
    flickBeam(1., 0.);
    flickBeam(0., 1.);
    flickBeam(1., 1.);
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
