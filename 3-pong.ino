#define PADDLE_RADIUS_Y 0.1

// only for collision
#define PADDLE_RADIUS_X 0.04
#define PADDLE_Y_HITBOX_FACTOR 1.3

#define PADDLE_SPEED 0.035

static size_t p1Score = 0;
static size_t p2Score = 0;

#define P1_X 0.05
static float p1Y = 0.5;

#define P2_X 0.95
static float p2Y = 0.5;

#define SCORE_SIZE 0.4
#define SCORE_RENDER_FRAMES 30
#define ROUND_START_FRAMES 35

#define DRAW_SPEED 15
#define MOVE_SPEED 15
#define BALL_TAIL 1.7

#define BALL_SPEED_INC 0.002
#define BALL_SPEED_CAP 0.04
static float ballSpeed = 0.025;

static float ball[2] = {0.5, 0.5};
static float ballVel[2] = {0.5, 0.5};

#define JS_PIN_1 A0
#define JS_PIN_2 A1

# define HALF_JS_RANGE 511.5

static int js_1_zero = 0;
static int js_2_zero = 0;

// input
#define readJoystick(pin, zero) ( (analogRead(pin) - zero)/HALF_JS_RANGE )

// setup
void zeroJoysticks(){
    js_1_zero = analogRead(JS_PIN_1);
    js_2_zero = analogRead(JS_PIN_2);
}

void newRound(int first){
    ball[0] = 0.5;
    ball[1] = 0.1 + 0.8*randFloat();

    p1Y = 0.5;
    p2Y = 0.5;

    float theta = M_PI*(0.5 + randFloat()/4);
    theta *= -2*random(2) +1;
    
    ballVel[0] = ballSpeed*sin(theta);
    ballVel[1] = ballSpeed*cos(theta);

    if (!first){
        ballSpeed += BALL_SPEED_INC;
        ballSpeed = ballSpeed > BALL_SPEED_CAP ? BALL_SPEED_CAP : ballSpeed;
        drawScore();
    }

    for(int i = 0; i < ROUND_START_FRAMES; i++){
        renderPong();
        // control
        p1Y+=PADDLE_SPEED*readJoystick(JS_PIN_1, js_1_zero);
        p2Y+=PADDLE_SPEED*readJoystick(JS_PIN_2, js_2_zero);
    }

}

void setupPong(){
    zeroJoysticks();
    newRound(1);
}

// rendering
#define moveBeam(x, y) smoothMoveBeamSpeed(x, y, MOVE_SPEED)
#define drawBeam(x, y) analogWrite(ALPHA_PIN, 0); smoothMoveBeamSpeed(x, y, DRAW_SPEED); analogWrite(ALPHA_PIN, 255)

void drawPaddle(float px, float py){
    moveBeam(px, py- PADDLE_RADIUS_Y);
    drawBeam(px, py+PADDLE_RADIUS_Y);
}


void drawBall(float px, float py, float vx, float vy){
    moveBeam(px, py);
    drawBeam(px-BALL_TAIL*vx, py-BALL_TAIL*vy);

}

void drawScore(){
    for (int i = 0; i < SCORE_RENDER_FRAMES; i++){
        drawNum(p1Score, SCORE_SIZE, 0.5-1.3*SCORE_SIZE, 1);
        drawNum(p2Score, SCORE_SIZE, 0.5+SCORE_SIZE, 1);
    }
}


void renderPong(){
    drawPaddle(P1_X, p1Y);
    // drawLine();
    drawBall(ball[0], ball[1], ballVel[0], ballVel[1]);
    drawPaddle(P2_X, p2Y);
}

void ballPaddleHit(int paddleNum, float px, float dy){
    int paddleFactor = -2*(paddleNum == 2) + 1;
    ball[0] = px + paddleFactor*PADDLE_RADIUS_X;
    
    float theta = M_PI*(0.5 - dy/(4.*PADDLE_RADIUS_Y));
    ballVel[0] = paddleFactor * ballSpeed*sin(theta);
    ballVel[1] = ballSpeed*cos(theta);
}

// runtime
void stepPong(){

    // ball movement
    ball[0] += ballVel[0];
    ball[1] += ballVel[1];

    // control
    p1Y+=PADDLE_SPEED*readJoystick(JS_PIN_1, js_1_zero);
    p2Y+=PADDLE_SPEED*readJoystick(JS_PIN_2, js_2_zero);

    // y wall hitting
    if(ball[0] < 0) {
        p2Score+=1;
        newRound(0);
        return;
    }

    if(ball[0] > 1){
        p1Score+=1;
        newRound(0);
        return;
    }

    // x wall hitting
    if(ball[1] > 1){
        ball[1] = 1;
        ballVel[1]*=-1;
    }

    if(ball[1] < 0){
        ball[1] = 0;
        ballVel[1]*=-1;
    }

    // paddle hitting
    int closeToPaddle1 = abs(ball[0] - P1_X) < PADDLE_RADIUS_X;
    int closeToPaddle2 = abs(ball[0] - P2_X) < PADDLE_RADIUS_X;

    if(closeToPaddle1){
        float dy = ball[1] - p1Y;

        if(abs(dy) < PADDLE_RADIUS_Y*PADDLE_Y_HITBOX_FACTOR){
            // paddle 1 hit!
            ballPaddleHit(1, P1_X, dy);
        }
    }

    if(closeToPaddle2){
        float dy = ball[1] - p2Y;

        if(abs(dy) < PADDLE_RADIUS_Y*PADDLE_Y_HITBOX_FACTOR){
            // paddle 2 hit!
            ballPaddleHit(2, P2_X, dy);
        }

    }
}

void playPong(){
    setupPong();

    while(1){
        stepPong();
        renderPong();
    }
}

