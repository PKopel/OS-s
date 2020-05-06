#include "common.h"

int main(void){
    srand(time(NULL) ^ (getpid()<<16));
    init_worker(0);
    int max_n = 1000, mxn[3][2];
    while (1)
    {
        mxn[0][1] = INC;
        mxn[1][1] = GET;
        mxn[2][0] = 1 + rand()%max_n;
        mxn[2][1] = SET;
        mxn_interact(mxn);
        log_activity(getpid(), mxn[2][0], "Dodałem liczbę", mxn[0][0], mxn[1][0]);
        sleep(1);
    }
}