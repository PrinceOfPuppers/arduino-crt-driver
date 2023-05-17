// resistor values (R2 is across cap)
#define R1 1e3
#define R2 100e3

// capacitor value
#define CAP 1e-6

// #define BEAM_MOVE_CALCULATION_TIME 88

// characteristic time of RC circuit
static float tau = (R1*R2*CAP)/(R1 + R2);
// static float tau = (R1*CAP);

#define xBias 0.97
#define BEAM_MOVE_CALCULATION_TIME 125
static int beamSampleSpeed = 5;
static int beamStepDelay = (int)(1e6*tau/beamSampleSpeed) - BEAM_MOVE_CALCULATION_TIME;

static int currentX = 0;
static int currentY = 0;
// time is factor of tau
// equation is V_pwm(t) = V_i + (t+tau)V_vel
void smoothMoveBeam(float xf, float yf, float time){
    int finalX = toScreenVal(xf);
    int finalY = toScreenVal(yf);
    
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

void flickBeam(float xf, float yf){
    int finalX = toScreenVal(xf);
    int finalY = toScreenVal(yf);

    analogWrite(X_PIN, finalX);
    analogWrite(Y_PIN, finalY);

    int deltaX = finalX - currentX;
    int deltaY = finalY - currentY;
    //Serial.print("x: ");
    //printFloat(deltaX);
    //Serial.print(" y: ");
    //printFloat(deltaY);

    float t;
    if(abs(deltaX) > abs(deltaY)){
        t = log(abs(deltaX))*tau*1e6;
        analogWrite(X_PIN, (deltaX>0)*255 );
        analogWrite(Y_PIN, currentY-finalY*exp(t/tau) );
    }
    else{
        t = log(abs(deltaY))*tau*1e6;
        analogWrite(Y_PIN, (deltaY>0)*255 );
        analogWrite(Y_PIN, currentX-finalX*exp(t/tau) );
    }
    delayMicroseconds((int)t);
    //Serial.print(" t: ");
    //printFloatln(t);
    analogWrite(X_PIN, finalX);
    analogWrite(Y_PIN, finalY);

    currentX = finalX;
    currentY = finalY;
}

