#define ANALOG_WRITE_SPEED 50

void flickBeam(float xf, float yf){
    int finalX = toScreenVal(xf);
    int finalY = toScreenVal(yf);

    int deltaX = finalX - currentX;
    int deltaY = finalY - currentY;

    int xpwm = (deltaX > 0)*255;
    int ypwm = (deltaY > 0)*255;

    float tx = (-tau*log((finalX-xpwm)/(float)(currentX-xpwm))*1e6*xBias);
    float ty = (-tau*log((finalY-ypwm)/(float)(currentY-ypwm))*1e6);

    float deltaT = abs(tx - ty) - 2*ANALOG_WRITE_SPEED;
    deltaT = deltaT*(deltaT>0);

    float txaw = tx - ANALOG_WRITE_SPEED;
    txaw = txaw*(txaw>0);
    float tyaw = ty - ANALOG_WRITE_SPEED;
    tyaw = tyaw*(tyaw>0);

    if(tx>ty){
        analogWrite(Y_PIN, ypwm);
        analogWrite(X_PIN, xpwm);
        delayMicroseconds(tyaw);

        analogWrite(Y_PIN, finalY);
        delayMicroseconds(deltaT);

        analogWrite(X_PIN, finalX);
    }
    else {
        analogWrite(X_PIN, xpwm);
        analogWrite(Y_PIN, ypwm);

        delayMicroseconds(txaw);

        analogWrite(X_PIN, finalX);
        delayMicroseconds(deltaT);

        analogWrite(Y_PIN, finalY);

    }
    delayMicroseconds(100);

    currentX = finalX;
    currentY = finalY;
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

void testFlickBeam(){
    flickBeam(0., 0.);
    flickBeam(0.5, 0.5);
    flickBeam(1., 0.);
    flickBeam(0., 1.);
    flickBeam(1., 1.);
}

void testFlickBeamSquare(){
    flickBeam(0, 0);
    flickBeam(1., 0.);
    flickBeam(1., 1.);
    flickBeam(0., 1.);
}

