#define numDigits(num) (size_t)( num == 0 ? 1 : (log10(num)+1))

#define R_MAX 5000
#define randFloat() ((float)random(R_MAX)/(R_MAX-1))

int getDigit(size_t num, int numDigits, int i){
    size_t mask = 1;
    while(numDigits-i-1>0){
        mask *= 10;
        i++;
    }

    return (num/mask)%10;
}

