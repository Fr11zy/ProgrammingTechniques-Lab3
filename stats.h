/**
 * @file stats.h
 * @brief Объявление структур и функций для статистического анализа.
 */
#ifndef STATS_H
#define STATS_H

#include <vector>

/**
 * @brief Структура для хранения статистических данных выборки.
 */
struct Stats {
    double mean;       // Математическое ожидание
    double stddev;     // Среднеквадратичное отклонение
    double cv;         // Коэффициент вариации
    double chi_square; // Значение критерия Хи-квадрат
    bool is_passed; // Пройден ли тест на равномерность
    bool is_random; // Пройден ли тест на случайность
};

/**
 * @brief Функция вычисления статистики и проверки критерия Хи-квадрат.
 * @param sample Вектор с выборкой случайных чисел.
 * @param max_val Максимальное значение в выборке (для нормализации Хи-квадрат).
 * @return Структура Stats с результатами.
 */
Stats calculate_stats(const std::vector<double>& sample, double max_val);

#endif // STATCS_H