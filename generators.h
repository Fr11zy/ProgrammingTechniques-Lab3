/**
 * @file generators.h
 * @brief Объявление классов генераторов псевдослучайных чисел.
 */
#ifndef GENERATORS_H
#define GENERATORS_H

#include <cstdint>
#include <vector>
#include <random>

/**
 * @brief Линейный конгруэнтный генератор.
 */
class LCG {
    uint32_t state;
    const uint32_t a = 1664525;
    const uint32_t c = 1013904223;
public:
    LCG(uint32_t seed);
    uint32_t next();
    double next_double();
};

/**
 * @brief Метод сдвигового регистра.
 */
class XorShift {
    uint32_t state;
public:
    XorShift(uint32_t seed);
    uint32_t next();
    double next_double();
};

/**
 * @brief Алгоритм Фибоначчи с запаздыванием.
 */
class LaggedFibonacci {
    std::vector<uint32_t> state;
    size_t p = 55, q = 24;
    size_t index = 0;
public:
    LaggedFibonacci(uint32_t seed);
    uint32_t next();
    double next_double();
};

/**
 * @brief Обертка для стандартного вихря Мерсенна (std::mt19937).
 */
class MT19937Wrapper {
    std::mt19937 gen;
public:
    MT19937Wrapper(uint32_t seed);
    uint32_t next();
    double next_double();
};

#endif // GENERATORS_H