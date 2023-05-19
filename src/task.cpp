// Copyright 2021 GHA Test Team
#include "task.h"
#include <iostream>

Shop::Shop(double _numberRegisters, double _intensity, double _speed, double _avgProducts, double _maxQueue) {
    numberRegisters = _numberRegisters;
    numberRequests = _intensity;
    speed = _speed;
    avgProducts = _avgProducts;
    maxQueue = _maxQueue;

    allTime = 0;

    servedСustomers = 0;
    unservedCustomers = 0;

    avgQueue = 0;//средняя длина очереди
    avgClientQueueTime = 0;//среднее время нахождение покупателя в очереди
    avgClientCheckoutTime = 0;//cреднее время на кассе

    queue = std::queue<Client>();
    cashRegisters = std::vector<CashRegister>();
    ts = std::vector<std::thread*>();

    for (int i = 0; i < numberRegisters; i++) {
        cashRegisters.push_back(CashRegister());
        cashRegisters[i].id = i;
        cashRegisters[i].workTime = 0;
        cashRegisters[i].downTime = 0;
        cashRegisters[i].free = true;
        cashRegisters[i].timeDownStart = std::chrono::system_clock::now();
    }
}

void Shop::WorkSup(int id, Client client, double speed) {
    std::unique_lock<std::mutex> mu(myMutex);

    cashRegisters[id].downTime += std::chrono::duration_cast<std::chrono::milliseconds>
        (std::chrono::system_clock::now() - cashRegisters[id].timeDownStart).count();
    cashRegisters[id].workTime += client.products / speed;
    client.checkoutTime = (int)(client.products / speed);

    std::this_thread::sleep_for(std::chrono::milliseconds((int)(client.products / speed)));

    cashRegisters[id].timeDownStart = std::chrono::system_clock::now();
    cashRegisters[id].free = true;

    mu.unlock();
}

void Shop::Work() {
    allTimeStart = std::chrono::system_clock::now();

    srand(time(0));
    int time = 0;
    int nextCustomerTime = rand() % 10 + 1;
    int numberClients = 0;

    while (true) {
        if (numberClients < numberRequests) {
            time++;
            if (time == nextCustomerTime) {
                std::this_thread::sleep_for(std::chrono::milliseconds(10));
                numberClients++;
                Client client;
                client.products = rand() % (2 * (int)avgProducts) + 1;
                client.timeQueueStart = std::chrono::system_clock::now();

                if (queue.size() < maxQueue) {
                    queue.push(client);
                }
                else {
                    unservedCustomers++;
                }
                nextCustomerTime = time + rand() % 10 + 1;
            }
        }

        for (int i = 0; i < numberRegisters; i++) {
            if ((queue.size() > 0) && (cashRegisters[i].free == true)) {
                cashRegisters[i].free = false;

                queue.front().queueTime = std::chrono::duration_cast<std::chrono::milliseconds>
                    (std::chrono::system_clock::now() - queue.front().timeQueueStart).count();

                ts.push_back(new std::thread(&Shop::WorkSup, this, i, queue.front(), speed));

                //cобираем статистику
                avgQueue += queue.size();//средняя длина очереди
                avgClientQueueTime += queue.front().queueTime;//среднее время нахождение покупателя в очереди
                avgClientCheckoutTime += queue.front().products / speed;//cреднее время на кассе

                servedСustomers++;
                queue.pop();
            }
        }
        if (queue.empty() && numberClients == numberRequests) break;
    }
    for (auto& th : ts)
        th->join();

    allTime = std::chrono::duration_cast<std::chrono::milliseconds>
        (std::chrono::system_clock::now() - allTimeStart).count();
}

double factorial(double n) {
    if (n == 0)
        return 1;
    else
        return n * factorial(n - 1);
}

void Shop::Statistics() {
    avgCassaWorkTime = 0;
    avgCassaDownTime = 0;

    for (int i = 0; i < numberRegisters; i++) {
        avgCassaWorkTime += cashRegisters[i].workTime;
        avgCassaDownTime += cashRegisters[i].downTime;
    }

    avgQueue = avgQueue / numberRequests;
    avgClientQueueTime = avgClientQueueTime / numberRequests;
    avgClientCheckoutTime = avgClientCheckoutTime / numberRequests;

    avgCassaWorkTime = avgCassaWorkTime / numberRegisters;
    avgCassaDownTime = avgCassaDownTime / numberRegisters;

    std::cout << "Customers served: " << servedСustomers << std::endl;
    std::cout << "No customers served: " << unservedCustomers << std::endl << std::endl;
    
    std::cout << "Average queue length: " << avgQueue << std::endl;
    std::cout << "Average time spent by a customer in a queue: " << avgClientQueueTime << std::endl;
    std::cout << "Average time spent by a customer at the checkout: " << (int)avgClientCheckoutTime << std::endl << std::endl;
    
    std::cout << "Average opening time of the cash register: " << avgCassaWorkTime << std::endl;
    std::cout << "Average downtime of the cash register: " << avgCassaDownTime << std::endl << std::endl;

    //На основании накопленной статистики подсчитать:
    double la = numberRequests / allTime;//интесивность входного потока заявок
    double mu = (double)servedСustomers / allTime;//интесивность потока обслуживания
    double p = la / mu;//интенсивность нагрузки канала.

    std::cout << "All Time: " << allTime << std::endl;
    std::cout << "la: " << la << std::endl;
    std::cout << "mu: " << mu << std::endl;
    std::cout << "p: " << p << std::endl << std::endl;

    //практические
    double P = unservedCustomers / numberRequests;//вероятность отказа
    double Q = 1.0 - P;//относительную пропускную способность
    double A = la * Q;//абсолютную пропускную способность

    std::cout << "Pp: " << P << std::endl;
    std::cout << "Qp: " << Q << std::endl;
    std::cout << "Ap: " << A << std::endl << std::endl;

    pFailureProbability = P;//вероятность отказа
    pRelativeStoreThroughput = Q;//относительную пропускную способность магазина
    pAbsoluteStoreThroughput = A;//абсолютную пропускную способность

    //теоретические
    double P0 = 1.0;
    for (int i = 1; i <= numberRegisters; i++)
        P0 = P0 + pow(p, i) / factorial(i);

    for (int i = numberRegisters + 1; i < numberRegisters + maxQueue; i++)
        P0 = P0 + pow(p, i) / (factorial(numberRegisters) * pow(numberRegisters, i - numberRegisters));

    P0 = 1.0 / P0;

    P = P0 * pow(p, (numberRegisters + maxQueue)) / (double)(pow(numberRegisters, maxQueue) * factorial(numberRegisters));
    Q = 1.0 - P;
    A = la * Q;

    //std::cout << "P0: " << P0 << std::endl;
    std::cout << "Pt: " << P << std::endl;
    std::cout << "Qt: " << Q << std::endl;
    std::cout << "At: " << A << std::endl << std::endl;

    tFailureProbability = P;
    tRelativeStoreThroughput = Q;
    tAbsoluteStoreThroughput = A;
}
