
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

