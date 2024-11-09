#include <stdio.h>

#include "application.h"

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        printf("Usage: ./model-viewer <model>.obj\n");
        return 1;
    }

    initialiseApplication(argv[1]);
    renderFrames();
    releaseResources();

    return 0;
}