#include "Compass.h"

Compass compass;
int main()
{
    compass.setup();
    for(int i = 0; i < 10; i++)
    {
        compass.measure();
    }
}