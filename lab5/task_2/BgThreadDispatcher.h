#include <functional>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <queue>
#include <atomic>

class BgThreadDispatcher
{
public:
    using Task = std::function<void()>;

    BgThreadDispatcher();
    ~BgThreadDispatcher();

    BgThreadDispatcher(const BgThreadDispatcher&) = delete;
    BgThreadDispatcher& operator=(const BgThreadDispatcher&) = delete;

    void Dispatch(Task task);
    void Wait();
    void Stop();

private:
    void WorkerThread();

    // проблема, сейчас сначала всё разрушаются а потом только join потоков
    std::jthread m_thread; 
    std::queue<Task> m_tasks;
    std::mutex m_mutex; 
    // более говорящее значение
    std::condition_variable m_condition; 
    std::atomic<bool> m_stopped{ false };
    bool m_waiting{ false }; 
};
