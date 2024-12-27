#include <benchmark/benchmark.h>
#include <cstring>
#include <stdlib.h> // for rand()
#include <time.h>   // for srand()

void BM_BubbleSort(benchmark::State& state) {
    int n = state.range(0); // Input size
    int* arr = (int*)malloc(n * sizeof(int));

    // Generate random data
    for (int i = 0; i < n; i++) {
        arr[i] = rand() % 1000;
    }

    for (auto _ : state) {
        // Copy data for each run
        int* temp = (int*)malloc(n * sizeof(int));
        memcpy(temp, arr, n * sizeof(int));

        // Bubble Sort
        for (int i = 0; i < n - 1; i++) {
            for (int j = 0; j < n - i - 1; j++) {
                if (temp[j] > temp[j + 1]) {
                    int tempVal = temp[j];
                    temp[j] = temp[j + 1];
                    temp[j + 1] = tempVal;
                }
            }
        }
        free(temp);
    }

    free(arr); // Clean up memory
}

// Add benchmark ranges
BENCHMARK(BM_BubbleSort)->Range(8, 8<<10); // Test from 8 to 8192 elements

BENCHMARK_MAIN();

