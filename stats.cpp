/**
 * @file stats.cpp
 * @brief Реализация функций расчета статистических метрик.
 */
#include "stats.h"
#include <cmath>
#include <algorithm>
#include <stdexcept>

// функции для проверки Хи-квадрат
bool checkChiSquare(double chi_square, int df) {
    if (df <= 0) return false;

    // Таблица критических значений Хи-квадрат для alpha = 0.05
    static const std::vector<double> chi_square_critical_005 = {
        0.0,
        3.841,  5.991,  7.815,  9.488,  11.070, 12.592, 14.067, 15.507, 16.919, 18.307,
        19.675, 21.026, 22.362, 23.685, 24.996, 26.296, 27.587, 28.869, 30.144, 31.410,
        32.671, 33.924, 35.172, 36.415, 37.652, 38.885, 40.113, 41.337, 42.557, 43.773
    };
    
    double critical_value;
    if (df < chi_square_critical_005.size()) {
        critical_value = chi_square_critical_005[df];
    } else {
        double z = 1.645;
        critical_value = 0.5 * std::pow((z + std::sqrt(2.0 * df - 1.0)), 2);
    }

    return chi_square < critical_value;
}

bool checkSerialRandomness(const std::vector<double>& sample, double max_val) {
    int n = sample.size();
    const int k = 5;
    const int num_cells = k * k; 
    
    std::vector<int> observed(num_cells, 0);

    int pairs_count = n / 2;
    double expected = static_cast<double>(pairs_count) / num_cells;

    for (int i = 0; i < n - 1; i += 2) {
        double norm1 = sample[i] / max_val;
        double norm2 = sample[i+1] / max_val;

        int bin1 = static_cast<int>(norm1 * k);
        int bin2 = static_cast<int>(norm2 * k);

        if (bin1 >= k) bin1 = k - 1;
        if (bin2 >= k) bin2 = k - 1;
        if (bin1 < 0) bin1 = 0;
        if (bin2 < 0) bin2 = 0;

        int cell_index = bin1 * k + bin2;
        observed[cell_index]++;
    }

    double chi2_stat = 0.0;
    for (int i = 0; i < num_cells; ++i) {
        double diff = observed[i] - expected;
        chi2_stat += (diff * diff) / expected;
    }

    double chi2_crit_min = 13.848;
    double chi2_crit_max = 36.415;

    return (chi2_stat >= chi2_crit_min && chi2_stat <= chi2_crit_max);
}

Stats calculate_stats(const std::vector<double>& sample, double max_val) {
    size_t n = sample.size();
    Stats stats = {0.0, 0.0, 0.0, 0.0, false, false};

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

    // 4. Критерий Хи-квадрат
    int k = std::max(1, static_cast<int>(1 + std::log2(n))); 
    std::vector<int> frequencies(k, 0);
    
    for (double val : sample) {
        int idx = static_cast<int>((val / max_val) * k);
        if (idx >= k) idx = k - 1;
        if (idx < 0) idx = 0;
        frequencies[idx]++;
    }

    double expected_freq = static_cast<double>(n) / k;
    for (int freq : frequencies) {
        stats.chi_square += std::pow(freq - expected_freq, 2) / expected_freq;
    }
    
    // Проверка критерия Хи-квадрат
    int k_unif = std::max(1, static_cast<int>(1 + std::log2(n))); 
    stats.is_passed = checkChiSquare(stats.chi_square, k_unif - 1);

    stats.is_random = checkSerialRandomness(sample, max_val);

    return stats;
}