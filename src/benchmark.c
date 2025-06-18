#include "benchmark.h"

#ifdef _WIN32
    #include <windows.h>
#else
    #include <time.h>
#endif

typedef void (*voidFunction)();

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

double computeTime(voidFunction function) {
#ifdef _WIN32
    LARGE_INTEGER freq, start, end;
    QueryPerformanceFrequency(&freq);
    QueryPerformanceCounter(&start);

    function();

    QueryPerformanceCounter(&end);
    return (double)(end.QuadPart - start.QuadPart) / freq.QuadPart;
#else
    struct timespec start, end;
    clock_gettime(CLOCK_MONOTONIC, &start);

    function();

    clock_gettime(CLOCK_MONOTONIC, &end);
    long seconds = end.tv_sec - start.tv_sec;
    long nanoseconds = end.tv_nsec - start.tv_nsec;
    return seconds + nanoseconds * 1e-9;
#endif
}