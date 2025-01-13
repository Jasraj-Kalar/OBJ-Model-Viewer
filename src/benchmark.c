#include "benchmark.h"

void benchmarkPrint(voidFunction function, const char* functionName)
{
    double elapsed = computeTime(function);

    printf("\rFrametime for %s: %.9f seconds, FPS: %d", functionName, elapsed, (int)(1 / elapsed));
    fflush(stdout);
}

double benchmark(voidFunction function)
{
    return computeTime(function);
}

double computeTime(voidFunction function)
{
    struct timespec start, end;

    // Measure function
    clock_gettime(CLOCK_MONOTONIC, &start);
    function();
    clock_gettime(CLOCK_MONOTONIC, &end);

    // Calculate time
    long seconds = end.tv_sec - start.tv_sec;
    long nanoseconds = end.tv_nsec - start.tv_nsec;
    
    return seconds + nanoseconds * 1e-9;
}