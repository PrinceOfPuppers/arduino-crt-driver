// resistor values (R2 is across cap)
#define R1 1e3
#define R2 100e3

// capacitor value
#define CAP 1e-6

// #define BEAM_MOVE_CALCULATION_TIME 88

// characteristic time of RC circuit
#define TAU_TRIM 1
static float tau = TAU_TRIM*(R1*R2*CAP)/(R1 + R2);
// static float tau = (R1*CAP);

#define X_BIAS 0.965

#define MAX_SMOOTH_BEAM_SPEED 15

// #define BEAM_MOVE_CALCULATION_TIME 130
#define BEAM_MOVE_CALCULATION_TIME 85
static int beamSampleSpeed = 5;
static int beamStepDelay = (int)(1e6*tau/beamSampleSpeed) - BEAM_MOVE_CALCULATION_TIME;

static int currentX = 0;
static int currentY = 0;


// pwm bounds (lower bound needs to be enough to overcome diode)
#define LOWER_BOUND 80
#define UPPER_BOUND 185

#define DIFF ((float)(UPPER_BOUND-LOWER_BOUND))

#define toScreenVal(f) ((int)(DIFF*(f)) + LOWER_BOUND)
#define smoothMoveBeam(xf, yf, time) _smoothMoveBeam(toScreenVal(xf), toScreenVal(yf), time)

// time is factor of tau
// equation is $$ V(t) = {(V_i - V_{pwm}) e^{-t/\tau} + V_{pwm}} $$
void _smoothMoveBeam(float finalX, float finalY, float time){
    // internal units are pwmNumber (0-255) for potential and tau/beamSampleSpeed for time
    int steps = (int)(time*beamSampleSpeed);

    float velX = (float)(finalX - currentX)/( (float)beamSampleSpeed*time )*X_BIAS;
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

