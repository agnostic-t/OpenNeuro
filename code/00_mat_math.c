#include "math/mat.h"
#include "math/watcher.h"
#include <stdio.h>

void mat_print(const matnn *m) {
    char *s = neu_mat_str(m);
    printf("%s\n", s);
    free(s);
}

int main(){

    matnn mat_a, mat_b;
    neu_magic_vec(3, 2.f, &mat_a);
    neu_magic_mat(3, 3, 3.f, &mat_b);

    mat_print(&mat_a);
    mat_print(&mat_b);

    matnn mat_c;
    neu_check(neu_mat_mul(&mat_a, &mat_b, &mat_c));
    mat_print(&mat_c);

    neu_mat_free(&mat_c);
}
