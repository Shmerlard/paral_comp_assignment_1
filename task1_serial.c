#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int ARR_SIZE = 600000;
int MAX_NUMBER = 50;

int main(int argc, char *argv[])
{
    int first_arr[ARR_SIZE];
    int final_arr[ARR_SIZE];

    srand(time(NULL));
    for (int i = 0; i < ARR_SIZE; i++){
        first_arr[i] = (rand() % MAX_NUMBER);
        printf("%d ", first_arr[i]);

    }
    printf("\n");

    clock_t t = clock();
    int max_int = 0;
    for (int i = 0; i < MAX_NUMBER; i++){
        final_arr[i] = first_arr[i] + max_int;
        max_int = final_arr[i];
        printf("%d ", final_arr[i]);
    }
    printf("\n");
    t = clock() - t;
    double time_taken = ((double)t)/CLOCKS_PER_SEC;
    printf("the operation took %fs\n",time_taken);
    return 0;
}
