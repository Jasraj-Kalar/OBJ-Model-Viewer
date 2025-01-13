#ifndef BENCHMARK 
#define BENCHMARK

#include <time.h>
#include <stdio.h>

typedef void (*voidFunction)();

// Public method(s)
void benchmarkPrint(voidFunction function, const char* functionName);
double benchmark(voidFunction function);
double computeTime(voidFunction function);


#endif