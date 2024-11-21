#include <iostream>
#include <unordered_set>
#include <vector>
#include <thread>
#include <mutex>
#include <shared_mutex>
#include <atomic>
#include <chrono>

// template
template <typename T, typename H = std::hash<T>, typename Comp = std::equal_to<T>>
class ThreadsafeSet
{
public:
    void Insert(const uint64_t& value)
    {
        std::unique_lock lock(m_mutex);
        m_set.insert(value);
    }

    bool Contains(const uint64_t& value) const
    {
        std::shared_lock lock(m_mutex);
        return m_set.find(value) != m_set.end();
    }

    std::vector<uint64_t> GetElements(size_t n) const
    {
        // что делает shared_lock
        std::shared_lock lock(m_mutex);
        std::vector<uint64_t> result;
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
    // что такое mutex. Какие методы у него есть?
    mutable std::shared_mutex m_mutex;
};

bool IsPrimes(uint64_t number)
{
    if (number < 2)
    {
        return false;
    }
    //исправить переполнение
    for (uint64_t i = 2; i * i <= number; ++i)
    {
        if (number % i == 0)
        {
            return false;
        }
    }
    return true;
}

void FindPrimes(uint64_t start, uint64_t end, ThreadsafeSet<uint64_t>& prime_set) 
{
    // бесконечный цикл
    for (uint64_t i = start; i <= end; ++i)
    {
        if (IsPrimes(i))
        {
            prime_set.Insert(i);
        }
    }
}

int main() {
    uint64_t maxNumber;
    std::cout << "Insert max number: ";
    std::cin >> maxNumber;

    auto startTime = std::chrono::high_resolution_clock::now();
    std::unordered_set<uint64_t> singleThreadPrimes;
    for (uint64_t i = 0; i <= maxNumber; ++i) {
        if (IsPrimes(i)) {
            singleThreadPrimes.insert(i);
        }
    }
    auto endTime = std::chrono::high_resolution_clock::now();
    auto singleThreadDuration = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime).count();

    // Многопоточный вариант
    const size_t threadCount = std::jthread::hardware_concurrency();
    ThreadsafeSet<uint64_t> multiThreadPrimes;
    std::vector<std::jthread> threads;
    uint64_t range = maxNumber / threadCount;

    startTime = std::chrono::high_resolution_clock::now();
    for (size_t t = 0; t < threadCount; ++t) {
        uint64_t start = t * range;
        uint64_t end = (t == threadCount - 1) ? maxNumber : start + range - 1;
        threads.emplace_back(FindPrimes, start, end, std::ref(multiThreadPrimes));
    }
    // Потоки завершатся автоматически при выходе из области видимости.

    endTime = std::chrono::high_resolution_clock::now();
    auto multiThreadDuration = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime).count();

    // Вывод результатов
    std::cout << "Single thread search" << singleThreadDuration << " mc.\n";
    std::cout << "Multi thread search" << multiThreadDuration << " mc.\n";

    std::cout << "Simple numbers (multi thread): ";
    int count = 0;
    for (auto prime : multiThreadPrimes.GetElements(10)) {
        std::cout << prime << " ";
    }
    std::cout << "\n";

    std::cout << "Simple numbers (one thread): ";
    count = 0;
    for (auto it = singleThreadPrimes.begin(); it != singleThreadPrimes.end() && count < 10; ++it, ++count) {
        std::cout << *it << " ";
    }
    std::cout << "\n";



    return 0;
}