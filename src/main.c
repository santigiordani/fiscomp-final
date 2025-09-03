#include <stdio.h>
#include <mtran.h>


int main() {

    printf("Hello world!\n");

    for (int i = 0; i < 10; ++i) {
        for (int j = 0; j < 10; ++j) {
            printf("%4.2f  ", mtrandom());
        }
        printf("\n");
    }

    return 0;

}