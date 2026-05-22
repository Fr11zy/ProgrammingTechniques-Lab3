/**
 * @file utils.h
 * @brief Вспомогательные шаблонные функции (замер времени, экспорт данных).
 */
#ifndef UTILS_H
#define UTILS_H

#include <chrono>
#include <fstream>
#include <string>
#include "stats.h"

/**
 * @brief Замеряет время генерации N элементов.
 * @tparam PRNG Тип генератора
 * @param gen Ссылка на объект генератора.
 * @param elements Количество генерируемых элементов.
 * @return Время в микросекундах.
 */
template<typename PRNG>
double measure_time(PRNG& gen, int elements) {
    auto start = std::chrono::high_resolution_clock::now();
    volatile double dummy = 0; 
    for(int i = 0; i < elements; ++i) {
        dummy = gen.next_double();
    }
    auto stop = std::chrono::high_resolution_clock::now();
    return std::chrono::duration<double, std::micro>(stop - start).count();
}

/**
 * @brief Экспортирует сгенерированные числа в бинарный файл для утилиты Dieharder.
 * @tparam PRNG Тип генератора.
 * @param gen Ссылка на объект генератора.
 * @param filename Имя выходного файла.
 * @param megabytes Размер файла в мегабайтах.
 */
template<typename PRNG>
void export_to_binary(PRNG& gen, const std::string& filename, size_t megabytes) {
    std::ofstream out(filename, std::ios::binary);
    size_t count = (megabytes * 1024 * 1024) / sizeof(uint32_t);
    for(size_t i = 0; i < count; ++i) {
        uint32_t val = gen.next();
        out.write(reinterpret_cast<const char*>(&val), sizeof(val));
    }
    out.close();
}

/**
 * @brief Прогоняет заданное количество выборок, считает статистику, выводит в консоль и пишет в CSV.
 * @tparam PRNG Тип генератора
 * @param gen Ссылка на объект генератора
 * @param gen_name Название генератора для таблицы
 * @param csv_file Поток записи в файл статистики
 * @param num_samples Количество выборо
 * @param sample_size Размер одной выборки
 * @param range_max Диапазон чисел
 */
template<typename PRNG>
void run_and_save_statistics(PRNG& gen, const std::string& gen_name, std::ofstream& csv_file, 
                             int num_samples, int sample_size, double range_max) {
                             
    std::cout << "\nСтатистика " << num_samples << " выборок для " << gen_name 
              << " (Диапазон 0-" << range_max << ")\n";
    std::cout << std::setw(8) << "Sample" << std::setw(15) << "Mean" << std::setw(15) << "StdDev" 
              << std::setw(15) << "CV" << std::setw(15) << "Chi-Square" << "\n";
         
    for (int i = 0; i < num_samples; ++i) {
        std::vector<double> sample(sample_size);
        for (int j = 0; j < sample_size; ++j) {
            sample[j] = gen.next_double() * range_max; 
        }
        
        Stats s = calculate_stats(sample, range_max);
        
        // Вывод в консоль
        std::cout << std::setw(8) << i + 1 
                  << std::setw(15) << s.mean 
                  << std::setw(15) << s.stddev 
                  << std::setw(15) << s.cv 
                  << std::setw(15) << s.chi_square << "\n";
             
        // Запись в CSV файл
        csv_file << gen_name << "," << i + 1 << "," << s.mean << "," 
                 << s.stddev << "," << s.cv << "," << s.chi_square << "\n";
    }
}

#endif // UTILS_H