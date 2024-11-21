#include <iostream>
#include <unordered_set>
#include <vector>
#include <thread>
#include <mutex>
#include <shared_mutex>
#include <atomic>
#include <chrono>

template <typename T, typename H = std::hash<T>, typename Comp = std::equal_to<T>>
class ThreadsafeSet 
{
public:
    // Добавление элемента в множество
    void insert(const T& value) 
    {
        std::unique_lock lock(m_mutex);
        m_set.insert(value);
    }

    // Проверка наличия элемента в множестве
    bool contains(const T& value) const 
    {
        std::shared_lock lock(m_mutex);
        return m_set.find(value) != m_set.end();
    }

    // Получение до n элементов из множества
    std::vector<T> get_some_elements(size_t n) const 
    {
        std::shared_lock lock(m_mutex);
        std::vector<T> result;
        for (const auto& element : m_set) 
        {
            if (result.size() >= n)
            {
                break;
            }
            result.push_back(element);
        }
        return result;
    }

private:
    std::unordered_set<T, H, Comp> m_set;
    mutable std::shared_mutex m_mutex;
};

// Проверка простоты числа
bool IsPrimes(uint64_t number) 
{
    if (number < 2)
    {
        return false;
    }
    for (uint64_t i = 2; i * i <= number; ++i) 
    {
        if (number % i == 0)
        {
            return false;
        }
    }
    return true;
}

// Функция для многопоточного добавления простых чисел в потокобезопасное множество
static void FindPrimes(uint64_t start, uint64_t end, ThreadsafeSet<uint64_t>& primes)
{
    for (uint64_t i = start; i <= end; ++i) 
    {
        if (IsPrimes(i))
        {
            primes.insert(i);
        }
    }
}

int main() {
    uint64_t maxNumber;
    std::cout << "Enter max number for find simply numbers: ";
    std::cin >> maxNumber;

    ThreadsafeSet<uint64_t> primesMultithread;
    std::unordered_set<uint64_t> primesSinglethread;

    // Однопоточное решение
    auto startSingle = std::chrono::high_resolution_clock::now();
    for (uint64_t i = 0; i <= maxNumber; ++i) 
    {
        if (IsPrimes(i))
        {
            primesSinglethread.insert(i);
        }
    }

    auto endSingle = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> durationSingle = endSingle - startSingle;

    // Многопоточное решение
    const unsigned int numThreads = std::jthread::hardware_concurrency();
    std::vector<std::jthread> threads;
    uint64_t range = maxNumber / numThreads;
    std::cout << "numThreads " << numThreads << std::endl;

    auto startMulti = std::chrono::high_resolution_clock::now();
    for (unsigned int i = 0; i < numThreads; ++i) 
    {
        uint64_t start = i * range;
        uint64_t end = (i == numThreads - 1) ? maxNumber : (i + 1) * range - 1;
        threads.emplace_back(FindPrimes, start, end, std::ref(primesMultithread));
    }
    auto endMulti = std::chrono::high_resolution_clock::now();

    std::chrono::duration<double> durationMulti = endMulti - startMulti;

    // Вывод до 10 простых чисел для однопоточного и многопоточного решений
    std::cout << "Single-threaded(time: " << durationSingle.count() << " seconds):\n";
    size_t count = 0;
    for (const auto& prime : primesSinglethread) 
    {
        if (count++ >= 10)
        {
            break;
        }
        std::cout << prime << " ";
    }
    std::cout << "\n";

    std::cout << "Multiple-threaded(time: " << durationMulti.count() << " seconds):\n";
    auto multi_primes = primesMultithread.get_some_elements(10);
    for (const auto& prime : multi_primes) 
    {
        std::cout << prime << " ";
    }
    std::cout << "\n";

    return 0;
}
