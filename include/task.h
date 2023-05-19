// Copyright 2021 GHA Test Team
#pragma once
#include <queue>
#include <deque>
#include <vector>
#include <chrono>
#include <thread>
#include <string>
#include <mutex>
#include <future>
#include <cmath>

class Client
{
    public:
        int products;
        int queueTime;
        int checkoutTime;
        std::chrono::system_clock::time_point timeQueueStart;
};

class CashRegister
{
    public:
        int id;
        double workTime;
        double downTime;
        std::chrono::system_clock::time_point timeDownStart;
        bool free;
};

class Shop
{
    public:
        //Входные параметры модели:
        double numberRegisters;//количество касс(каналов обслуживания)
        double numberRequests;//(заявки на обслуживание)
        double speed;//скорость обработки товара на кассе
        double avgProducts;//среднее количество товаров в тележке
        double maxQueue;//максимальная длина очереди

        std::mutex myMutex;

        std::queue<Client> queue;
        std::vector<CashRegister> cashRegisters;
        std::vector<std::thread*> ts;

        Shop(double numberRegisters, double intensity,
        double speed, double avgProducts, double maxQueue);
        void WorkSup(int id, Client client, double speed);//имитация раоты кассы
        void Work();
        void Statistics();
        void Print();

        //Статистика
        std::chrono::system_clock::time_point allTimeStart;
        int allTime;

        int servedСustomers;
        int unservedCustomers;

        double avgQueue;//средняя длина очереди
        double avgClientQueueTime;//среднее время нахождение покупателя в очереди
        double avgClientCheckoutTime;//cреднее время на кассе

        double avgCassaWorkTime;//среднее время работы кассы
        double avgCassaDownTime;//среднее время простоя кассы

        //На основании накопленной статистики подсчитать:
        double pFailureProbability;//вероятность отказа
        double pRelativeStoreThroughput;//относительную пропускную способность магазина
        double pAbsoluteStoreThroughput;//абсолютную пропускную способность

        //Сравнить рассчитанные на основе накопленных данных значения с теоретическими
        double tFailureProbability;//вероятность отказа
        double tRelativeStoreThroughput;//относительную пропускную способность магазина
        double tAbsoluteStoreThroughput;//абсолютную пропускную способность
};
