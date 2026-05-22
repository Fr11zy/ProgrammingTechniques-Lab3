/**
 * @file generators.cpp
 * @brief Реализация методов генерации псевдослучайных чисел.
 */
#include "generators.h"

//LCG
LCG::LCG(uint32_t seed) : state(seed) {}

uint32_t LCG::next() {
    state = a * state + c;
    return state;
}

double LCG::next_double() { return (double)next() / UINT32_MAX; }

//XorShift
XorShift::XorShift(uint32_t seed) : state(seed == 0 ? 1 : seed) {}

uint32_t XorShift::next() {
    state ^= state << 13;
    state ^= state >> 17;
    state ^= state << 5;
    return state;
}

double XorShift::next_double() { return (double)next() / UINT32_MAX; }

//Lagged Fibonacci
LaggedFibonacci::LaggedFibonacci(uint32_t seed) {
    state.resize(p);
    LCG init_gen(seed);
    for (size_t i = 0; i < p; ++i) {
        state[i] = init_gen.next();
    }
}

uint32_t LaggedFibonacci::next() {
    uint32_t result = state[(index + p - q) % p] + state[index];
    state[index] = result;
    index = (index + 1) % p;
    return result;
}

double LaggedFibonacci::next_double() { return (double)next() / UINT32_MAX; }

//MT19937 Wrapper
MT19937Wrapper::MT19937Wrapper(uint32_t seed) : gen(seed) {}

uint32_t MT19937Wrapper::next() { return gen(); }

double MT19937Wrapper::next_double() { return (double)gen() / gen.max(); }