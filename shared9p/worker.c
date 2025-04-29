#include <stdio.h>
#include <time.h>
#include <stdlib.h>



void swap(int *a, int *b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}


void permute(int arr[], int l, int r) {
   

    for (int i = l; i <= r; i++) {
        swap(&arr[l], &arr[i]);
        permute(arr, l + 1, r);
        swap(&arr[l], &arr[i]);
    }
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Error input\n");
        return 1;
    }
    int input = atoi(argv[1]);
    clock_t start, end;
    double cpu_time_used;

    start = clock();

    int arr[input];

    srand(time(NULL));

    int min = 0;
    int max = 100;
    for (int i = 0; i < input; i++) {
        arr[i] = min + rand() % (max - min + 1);
    }

    permute(arr, 0, input - 1);

    

    end = clock();

    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;

    printf("Elapsed time: %f seconds\n", cpu_time_used);
    return 0;
}
