#include "common.h"

int main(void){
    int m, x, n;
    while (1)
    {
        n = get(n)*2;
        m = get_m() - 1;
        x = get_x() + 1;
        log_activity(getpid(), n, "Przygotowałem zamówienie o wielkości", m, x);
        set_m(m);
        set_x(x);
    }
}