/**
 * @file main.cpp
 * @brief Тестирование ГПСЧ, анализ, запись статистики и замер времени.
 */
#include <iostream>
#include <vector>
#include <iomanip>
#include <fstream>
#include <string>
#include "generators.h"
#include "stats.h" 
#include "utils.h"

using namespace std;

int main() {
    uint32_t seed = 123456789;
    
    // Инициализация генераторов
    LCG lcg(seed);
    XorShift xor_gen(seed);
    LaggedFibonacci fib_gen(seed);
    MT19937Wrapper mt(seed);

    //Получение 20 выборок и расчет статистики
    const int num_samples = 20;
    const int sample_size = 1000;
    const double range_max = 5000.0;

    ofstream stats_results("prng_stats.csv");
    stats_results << "Generator,Sample,Mean,StdDev,CV,ChiSquare,Uniform_Passed,Random_Passed\n";

    run_and_save_statistics(lcg, "LCG", stats_results, num_samples, sample_size, range_max);
    run_and_save_statistics(xor_gen, "XorShift", stats_results, num_samples, sample_size, range_max);
    run_and_save_statistics(fib_gen, "LaggedFibonacci", stats_results, num_samples, sample_size, range_max);
    
    stats_results.close();
    cout << "\n Cтатистика по всем выборкам сохранена в prng_stats.csv\n";
    

    // Diehard
    cout << "\nГенерация бинарных данных для тестов Diehard\n";
    
    export_to_binary(lcg, "lcg_diehard.bin", 50);
    export_to_binary(xor_gen, "xorshift_diehard.bin", 50);
    export_to_binary(fib_gen, "fibonacci_diehard.bin", 50);
    cout << "Готово.\n";


    //Замер времени генерации
    cout << "\nБенчмарк времени генерации (микросекунды)\n";
    vector<int> sizes = {1000, 5000, 10000, 25000, 50000, 100000, 250000, 500000, 750000, 1000000};
    
    ofstream time_results("prng_times.csv");
    time_results << "Size,LCG,XorShift,LaggedFibonacci,MT19937\n";

    cout << setw(10) << "Size" << setw(12) << "LCG" << setw(12) << "XorShift" 
         << setw(16) << "Fibonacci" << setw(12) << "MT19937" << endl;

    for (int n : sizes) {
        double timeLCG = measure_time(lcg, n);
        double timeXor = measure_time(xor_gen, n);
        double timeFib = measure_time(fib_gen, n);
        double timeMT  = measure_time(mt, n);

        cout << setw(10) << n << setw(12) << timeLCG << setw(12) << timeXor 
             << setw(16) << timeFib << setw(12) << timeMT << endl;
             
        time_results << n << "," << timeLCG << "," << timeXor << "," << timeFib << "," << timeMT << "\n";
    }

    time_results.close();
    cout << "\nРезультаты бенчмарка сохранены в prng_times.csv\n";
    
    return 0;
}