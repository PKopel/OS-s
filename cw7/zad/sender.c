#include "common.h"

int main(void){
    int mxn[3][2];
    while (1)
    {
        mxn[0][1] = GET;
        mxn[1][1] = DEC;
        mxn[2][1] = GET;
        mxn_interact(mxn);
        log_activity(getpid(), mxn[2][0]*3,  "Wysłałem zamówienie o wielkości", mxn[0][0], mxn[1][0]);
    }
}