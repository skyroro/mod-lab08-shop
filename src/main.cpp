// Copyright 2021 GHA Test Team
#include <iostream>
#include "task.h"

int main()
{
    double numberRegisters = 3;
    double intensity = 30;

    //double speed = 0.05;
    //double speed = 0.1;
    //double speed = 0.2;
    double speed = 0.4;

    double avgProducts = 5;
    double maxQueue = 10;

    Shop shop(numberRegisters, intensity, speed, avgProducts, maxQueue);
    shop.Work();
    shop.Statistics();
}
