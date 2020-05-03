#include "common.h"

int main(void){
    int m, x, n;
    while (1)
    {
        n = get_n()*3;
        m = get_m();
        x = get_x() - 1;
        log_activity(getpid(), n, "Wysłałem zamówienie o wielkości", m, x);
        set_x(x);
    }
}