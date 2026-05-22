/**
 * @file stats.cpp
 * @brief Реализация функций расчета статистических метрик.
 */
#include "stats.h"
#include <cmath>
#include <algorithm>

Stats calculate_stats(const std::vector<double>& sample, double max_val) {
    size_t n = sample.size();
    Stats stats = {0.0, 0.0, 0.0, 0.0};

    // 1. Математическое ожидание
    for (double val : sample) stats.mean += val;
    stats.mean /= n;

    // 2. Дисперсия и среднеквадратичное отклонение
    double variance = 0;
    for (double val : sample) {
        variance += (val - stats.mean) * (val - stats.mean);
    }
    variance /= n;
    stats.stddev = std::sqrt(variance);

    // 3. Коэффициент вариации
    stats.cv = stats.stddev / stats.mean;

    // 4. Критерий Хи-квадрат (Формула Стерджеса для k)
    int k = std::max(1, static_cast<int>(1 + 3.322 * std::log(n))); 
    std::vector<int> frequencies(k, 0);
    
    for (double val : sample) {
        // Нормализуем значение в индекс корзины
        int idx = static_cast<int>((val / max_val) * k);
        if (idx >= k) idx = k - 1;
        if (idx < 0) idx = 0;
        frequencies[idx]++;
    }

    double expected_freq = static_cast<double>(n) / k;
    for (int freq : frequencies) {
        stats.chi_square += std::pow(freq - expected_freq, 2) / expected_freq;
    }

    return stats;
}