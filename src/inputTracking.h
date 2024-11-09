#ifndef INPUT_TRACKING
#define INPUT_TRACKING

typedef struct MousePosition
{
    int xPosition;
    int yPosition;
    int xPrevPosition;
    int yPrevPosition;
}
MousePosition;

typedef struct ScreenSize
{
    double width;
    double height;
}
ScreenSize;

typedef struct ScrollPosition
{
    double xOffset;
    double yOffset;
}
ScrollPosition;

#endif