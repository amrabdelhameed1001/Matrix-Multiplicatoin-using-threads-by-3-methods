#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define MAX_THREADS 100 // Maximum number of threads allowed

// Global variables for matrices and their dimensions
int **A, **B, **C;
int x, y, y2, z;

// Function to compute a single element in the output matrix C
void *compute_element(void *arg) {
    int *args = (int *) arg;
    int row = args[0], col = args[1];
    int sum = 0;
    for (int i = 0; i < y; i++) {
        sum += A[row][i] * B[i][col];
    }
    C[row][col] = sum;
    pthread_exit(NULL);
}

// Function to multiply matrices using multi-threading
void multiply_matrices(int num_threads) {
    pthread_t threads[MAX_THREADS];
    int thread_args[MAX_THREADS][2];
    int count = 0;

    // Create threads for computing each element in the output matrix C
    for (int i = 0; i < x; i++) {
        for (int j = 0; j < z; j++) {
            thread_args[count][0] = i;
            thread_args[count][1] = j;
            pthread_create(&threads[count], NULL, compute_element, (void *) &thread_args[count]);
            count++;
        }
    }

    // Wait for all threads to complete
    for (int i = 0; i < count; i++) {
        pthread_join(threads[i], NULL);
    }
}

// Function to read matrix from file
void read_matrix(char *filename, int **matrix, int rows, int cols) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        printf("Error opening file %s\n", filename);
        exit(1);
    }

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            fscanf(file, "%d", &matrix[i][j]);
        }
    }

    fclose(file);
}

// Function to write matrix to file
void write_matrix(char *filename, int **matrix, int rows, int cols) {
    FILE *file = fopen(filename, "w");
    if (file == NULL) {
        printf("Error opening file %s\n", filename);
        exit(1);
    }

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            fprintf(file, "%d ", matrix[i][j]);
        }
        fprintf(file, "\n");
    }

    fclose(file);
}

int main(int argc, char **argv) {
    // Read input matrices from files
    read_matrix(argv[1], A, x, y);
    read_matrix(argv[2], B, y2, z);

    // Allocate memory for output matrix C
    C = (int **) malloc(x * sizeof(int *));
    for (int i = 0; i < x; i++) {
        C[i] = (int *) malloc(z * sizeof(int));
    }

    // Multiply matrices using multi-threading
    multiply_matrices(atoi(argv[3]));

    // Write output matrix C to file
    write_matrix(argv[4], C, x, z);

    // Free memory for matrices
    for (int i = 0;i < x; i++) {
        free(A[i]);
        free(C[i]);
    }

    for (int i = 0; i < y2; i++) {
        free(B[i]);
    }

    free(A);
    free(B);
    free(C);

    return 0;

}
