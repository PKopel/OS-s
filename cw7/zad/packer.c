#include "common.h"

int main(void){
    init_worker(1);
    int mxn[3][2];
    while (1)
    {
        mxn[0][1] = DEC;
        mxn[1][1] = INC;
        mxn[2][1] = GET;
        mxn_interact(mxn);
        log_activity(getpid(), mxn[2][0]*2, "Przygotowałem zamówienie o wielkości", mxn[0][0], mxn[1][0]);
        sleep(1);
    }
}