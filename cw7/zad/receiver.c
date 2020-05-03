#include "common.h"

int main(void){
    srandom(time(NULL));
    int m, x, n, max_n = 1000;
    while (1)
    {
        n = random()%max_n;
        set_n(n);
        m = get_m() + 1;
        x = get_x();
        log_activity(getpid(), n, "Dodałem liczbę", m, x);
        set_m(m);
    }
}