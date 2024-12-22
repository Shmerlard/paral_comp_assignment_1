#include <stdio.h>
#include "mpi.h"
#include <stdlib.h>
#include <time.h>

int ARR_SIZE = 12;
int MAX_NUMBER = 10;

// assumptions:
// the number of numbers in the array is divisible by size
//
int main(int argc, char *argv[])
{
    // Initialization
    double prog_start = MPI_Wtime();
    int rank, size, type = 99;
    MPI_Status status;
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    int elements_per_process = ARR_SIZE / size;
    int first_arr[ARR_SIZE];  // the array to be summed
    int temp_arr[elements_per_process];  // the temporary array for calculating the partial sums
    int final_arr[ARR_SIZE];  // the array of the partial sums
    double start_t, end_t;
    double prog_end;
    // Generating The random array
    if (rank == 0)
    {
        start_t = MPI_Wtime();
        srand(time(NULL));

        // creating the random first array
        for (int i = 0; i < ARR_SIZE; i++){
            first_arr[i] = (rand() % MAX_NUMBER);
            printf("%d ", first_arr[i]);
        }
        printf("\n");
        double end_t = MPI_Wtime();

        printf("process 0 generated: %d elements with a maximum value of %d in %F\n\n", ARR_SIZE, MAX_NUMBER, end_t - start_t);  // to complete
    }
    start_t = MPI_Wtime();
    MPI_Scatter(first_arr, elements_per_process, MPI_INT, temp_arr, elements_per_process, MPI_INT, 0, MPI_COMM_WORLD);
    end_t = MPI_Wtime();
    MPI_Barrier(MPI_COMM_WORLD);

    // testing what everyone recieved
    for (int i = 0; i < size; i++) {
        MPI_Barrier(MPI_COMM_WORLD);
        if (rank == i) {
            printf("Process %d received in %Fs the array: ", rank, end_t - start_t);
            for (int j = 0; j < elements_per_process; j++) {
                printf("%d ", temp_arr[j]);
            }
            printf("\n");
        }
    }
    MPI_Barrier(MPI_COMM_WORLD);


    // making the partial sums of each subarray
    int max_int = 0;
    for (int i = 0; i < elements_per_process; i++){
        temp_arr[i] = temp_arr[i] + max_int;
        max_int = temp_arr[i];
    }

    // Testing the result of the partial sums of the subarray
    for (int i = 0; i < size; i++) {
        MPI_Barrier(MPI_COMM_WORLD);
        if (rank == i) {
            printf("Process %d calculated: ", rank);
            for (int j = 0; j < elements_per_process; j++) {
                printf("%d ", temp_arr[j]);
            }
            printf(" and the maximum is: %d \n", max_int);
        }
    }
    MPI_Barrier(MPI_COMM_WORLD);


    // Shifting each subarray by the maximum of the previous process
    int shift_amount = 0;
    MPI_Exscan(&max_int, &shift_amount, 1, MPI_INT, MPI_SUM, MPI_COMM_WORLD);
    MPI_Barrier(MPI_COMM_WORLD);
    printf("process %d: shift amount is: %d \n", rank, shift_amount);

    for (int i = 0; i < elements_per_process; i++){
        temp_arr[i] = temp_arr[i] + shift_amount;
    }


    // Gathering each subarray to the the final results.
    MPI_Gather(temp_arr, elements_per_process, MPI_INT, final_arr, elements_per_process, MPI_INT, 0, MPI_COMM_WORLD);
    prog_end = MPI_Wtime();
    MPI_Barrier(MPI_COMM_WORLD);
    // Printing the final results
    if (rank == 0){
        printf("\nthe final results are: ");
        for (int i = 0; i < ARR_SIZE; i++){
            printf("%d ", final_arr[i]);
        }
        printf("\nin %Fs\n", prog_end - prog_start);
    }


    MPI_Barrier(MPI_COMM_WORLD);
    MPI_Finalize();
    return 0;

}
