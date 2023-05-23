#define MAX_NUM_LEN 7

int fontLens[10] = {5, 3, 5, 5, 4, 5, 6, 3, 7, 5};
int font[10][MAX_NUM_LEN][2]={
   {{0, 0}, {2, 0}, {2, 4}, {0, 4}, {0, 0}},
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


#define numSpeed 12
#define numResetSpeed 15

#define fontX(digit, i, offsetf, scale) (font[digit][i][0]*scale*0.25 + offsetf)
#define fontY(digit, i, offsetf, scale) (-font[digit][i][1]*scale*0.25 + offsetf)

void drawNum(size_t n, float scale, float offsetX, float offsetY){
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
    analogWrite(ALPHA_PIN, 255);
}


