// Copyright 2021 GHA Test Team
#include <gtest/gtest.h>
#include "task.h"

TEST(TestCaseName, NumberOfRequests) {
    double numberRegisters = 3;
    double numberRequests = 30;
    double speed = 0.2;
    double avgProducts = 5;
    double maxQueue = 10;

    Shop shop(numberRegisters, numberRequests, speed, avgProducts, maxQueue);
    shop.Work();
  
    int result = shop.servedСustomers + shop.unservedCustomers;
    EXPECT_EQ(result, numberRequests);
}

TEST(TestCaseName, HighSpeed) {
    double numberRegisters = 3;
    double numberRequests = 30;
    double speed = 0.5;
    double avgProducts = 5;
    double maxQueue = 10;

    Shop shop(numberRegisters, numberRequests, speed, avgProducts, maxQueue);
    shop.Work();

    EXPECT_EQ(shop.servedСustomers, numberRequests);
}

TEST(TestCaseName, TheQuickerTheBetter) {
    double numberRegisters = 3;
    double numberRequests = 30;
    double speed = 0.1;
    double avgProducts = 5;
    double maxQueue = 10;

    Shop shop1(numberRegisters, numberRequests, speed, avgProducts, maxQueue);
    shop1.Work();

    speed = 0.3;

    Shop shop2(numberRegisters, numberRequests, speed, avgProducts, maxQueue);
    shop2.Work();

    bool better = false;
    if (shop1.servedСustomers < shop2.servedСustomers) better = true;
    EXPECT_EQ(better, true);
}

TEST(TestCaseName, MoreCashRegistersLessQueue) {
    double numberRegisters = 2;
    double numberRequests = 30;
    double speed = 0.1;
    double avgProducts = 5;
    double maxQueue = 10;

    Shop shop1(numberRegisters, numberRequests, speed, avgProducts, maxQueue);
    shop1.Work();

    numberRegisters = 10;

    Shop shop2(numberRegisters, numberRequests, speed, avgProducts, maxQueue);
    shop2.Work();

    bool better = false;
    if (shop1.avgQueue > shop2.avgQueue) better = true;
    EXPECT_EQ(better, true);
}

TEST(TestCaseName, pProbabilityAdequacy) {
    double numberRegisters = 2;
    double numberRequests = 30;
    double speed = 0.1;
    double avgProducts = 5;
    double maxQueue = 10;

    Shop shop1(numberRegisters, numberRequests, speed, avgProducts, maxQueue);
    shop1.Work();
    shop1.Statistics();

    bool ok = false;
    if (shop1.pFailureProbability <= 1) ok = true;
    EXPECT_EQ(ok, true);
}

TEST(TestCaseName, tProbabilityAdequacy) {
    double numberRegisters = 2;
    double numberRequests = 30;
    double speed = 0.1;
    double avgProducts = 5;
    double maxQueue = 10;

    Shop shop1(numberRegisters, numberRequests, speed, avgProducts, maxQueue);
    shop1.Work();
    shop1.Statistics();

    bool ok = false;
    if (shop1.tFailureProbability <= 1) ok = true;
    EXPECT_EQ(ok, true);
}
