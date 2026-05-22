"""
Скрипт для построения графиков производительности ГПСЧ.
"""
import pandas as pd
import matplotlib.pyplot as plt

def plot_prng_times():
    df = pd.read_csv('prng_times.csv')

    plt.figure(figsize=(10, 6))
    
    plt.plot(df['Size'], df['LCG'], label='LCG', marker='o')
    plt.plot(df['Size'], df['XorShift'], label='XorShift', marker='s')
    plt.plot(df['Size'], df['LaggedFibonacci'], label='Lagged Fibonacci', marker='^')
    plt.plot(df['Size'], df['MT19937'], label='std::mt19937', marker='x', linestyle='--')

    plt.title('Сравнение скорости алгоритмов генерации псевдослучайных чисел')
    plt.xlabel('Объем выборки (N)')
    plt.ylabel('Время генерации (микросекунды)')
    
    plt.yscale('log') 
    plt.xscale('log')
    
    plt.grid(True, which="both", ls="--", alpha=0.5)
    plt.legend()
    plt.tight_layout()
    
    plt.savefig('prng_times_plot.png', dpi=300)
    print("График успешно сохранен в 'prng_times_plot.png'")

if __name__ == "__main__":
    plot_prng_times()