// resistor values (R2 is across cap)
#define R1 1e3
#define R2 100e3

// capacitor value
#define CAP 1e-6

// #define BEAM_MOVE_CALCULATION_TIME 88

// characteristic time of RC circuit
#define TAU_TRIM 0.80
static float tau = TAU_TRIM*(R1*R2*CAP)/(R1 + R2);
// static float tau = (R1*CAP);

#define xBias 0.965

// #define BEAM_MOVE_CALCULATION_TIME 130
#define BEAM_MOVE_CALCULATION_TIME 85
static int beamSampleSpeed = 5;
static int beamStepDelay = (int)(1e6*tau/beamSampleSpeed) - BEAM_MOVE_CALCULATION_TIME;

static int currentX = 0;
static int currentY = 0;


// time is factor of tau
// equation is V_pwm(t) = V_i + (t+tau)V_vel
void _smoothMoveBeam(float finalX, float finalY, float time){
    // internal units are pwmNumber (0-255) for potential and tau/beamSampleSpeed for time
    int steps = (int)(time*beamSampleSpeed);

    float velX = (float)(finalX - currentX)/( (float)beamSampleSpeed*time )*xBias;
    float velY = (float)(finalY - currentY)/( (float)beamSampleSpeed*time );
    
    float x = currentX + velX*beamSampleSpeed;
    float y = currentY + velY*beamSampleSpeed;

    for(int i = 0; i < steps; i ++){
        x += velX;
        y += velY;

        analogWrite(X_PIN, 
            (x > 255 ? 255 : x)
        );
        analogWrite(Y_PIN, 
            (y > 255 ? 255 : y)
        );
        
        delayMicroseconds(beamStepDelay);
    }
    analogWrite(X_PIN, finalX);
    analogWrite(Y_PIN, finalY);

    currentX = finalX;
    currentY = finalY;
}


// pwm volt units per tau
void smoothMoveBeamSpeed(float xf, float yf, float speed){
    int finalX = toScreenVal(xf);
    int finalY = toScreenVal(yf);

    int deltaX = finalX - currentX;
    int deltaY = finalY - currentY;

    float dist = sqrt(deltaX*deltaX + deltaY*deltaY);
    float time = dist/speed;
    _smoothMoveBeam(finalX, finalY, time);
}

#define MAX_NUM_LEN 7

int fontLens[10] = {7, 3, 5, 5, 4, 5, 6, 3, 7, 5};
int font[10][MAX_NUM_LEN][2]={
   {{0, 0}, {2, 0}, {2, 0}, {2, 4}, {0, 4}, {0, 0}, {2, 4}},
   {{0, 1}, {1, 0}, {1, 4}}, 
   {{0, 1}, {1, 0}, {2, 1}, {0, 4}, {2, 4}},
   {{0, 0}, {2, 1}, {0, 2}, {2, 3}, {0, 4}}, 
   {{2, 4}, {2, 0}, {0, 2}, {2, 2}}, 
   {{2, 0}, {0, 0}, {0, 1}, {2, 2}, {0, 4}}, 
   {{2, 0}, {0, 0}, {0, 4}, {2, 4}, {2, 2}, {0, 2}}, 
   {{0, 0}, {2, 0}, {0, 4}},
   {{0, 2}, {2, 2}, {2, 0}, {0, 0}, {0, 4}, {2, 4}, {2, 0}}, 
   {{2, 4}, {2, 0}, {0, 0}, {0, 2}, {2, 2}}, 
};

#define numDigits(num) (int)(log10(num)+1)

int getDigit(int num, int numDigits, int i){
    size_t mask = 1;
    while(numDigits-i-1>0){
        mask *= 10;
        i++;
    }

    return (num/mask)%10;
}

#define numSpeed 12
#define numResetSpeed 15

#define fontX(digit, i, offsetf, scale) (font[digit][i][0]*scale*0.25 + offsetf)
#define fontY(digit, i, offsetf, scale) (-font[digit][i][1]*scale*0.25 + offsetf)

void drawNum(int n, float scale, float offsetX, float offsetY){
    int ds = numDigits(n);

    float scaledSpeed = numSpeed*scale;

    int digit, i, j;
    for(j = 0; j < ds; j++){
        digit = getDigit(n, ds, j);

        analogWrite(ALPHA_PIN, 255);
        smoothMoveBeamSpeed(fontX(digit, 0, offsetX, scale), fontY(digit, 0, offsetY, scale), numResetSpeed);
        analogWrite(ALPHA_PIN, 0);

        for(i = 1; i < fontLens[digit]; i++){
            smoothMoveBeamSpeed(fontX(digit, i, offsetX, scale), fontY(digit, i, offsetY, scale), numSpeed);
        }

        offsetX += 0.9*scale;
    }
}


