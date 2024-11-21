#include <functional>
#include <queue>
#include <vector>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <atomic>
#include <iostream>

class ThreadPool 
{
public:
    using Task = std::function<void()>;


    explicit ThreadPool(unsigned numThreads)
        : m_stopped(false)
    {
        for (unsigned i = 0; i < numThreads; ++i)
        {
            m_workers.emplace_back([this]() { WorkerThread(); });
        }
    }

    ~ThreadPool() 
    {
        Stop();
        for (auto& worker : m_workers) 
        {
            if (worker.joinable()) 
            {
                worker.join();
            }
        }
    }

    void Dispatch(Task task) 
    {
        {
            std::lock_guard<std::mutex> lock(m_mutex);
            if (m_stopped) 
            {
                return;
            }
            m_tasks.push(std::move(task));
        }
        m_cv.notify_one();
    }

    void Wait() 
    {
        std::unique_lock<std::mutex> lock(m_mutex);
        m_waitCv.wait(lock, [this]() { return m_tasks.empty() && m_activeTasks == 0; });
    }

    void Stop() 
    {
        {
            std::lock_guard<std::mutex> lock(m_mutex);
            m_stopped = true;
        }
        m_cv.notify_all();
    }

private:
    std::vector<std::thread> m_workers;
    std::queue<Task> m_tasks;
    std::mutex m_mutex;
    std::condition_variable m_cv;
    std::condition_variable m_waitCv;
    std::atomic<bool> m_stopped;
    std::atomic<int> m_activeTasks{ 0 };

    void NotifyTaskFinished() 
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        --m_activeTasks;
        if (m_tasks.empty() && m_activeTasks == 0) 
        {
            m_waitCv.notify_all();
        }
    }

    void WorkerThread()
    {
        while (true)
        {
            Task task;
            {
                std::unique_lock<std::mutex> lock(m_mutex);
                m_cv.wait(lock, [this]()
                    {
                        return m_stopped || !m_tasks.empty();
                    }
                );

                if (m_stopped && m_tasks.empty())
                {
                    return;
                }

                task = std::move(m_tasks.front());
                m_tasks.pop();
            }
            try
            {
                ++m_activeTasks;
                task();
            }
            catch (const std::exception& e)
            {
                std::cerr << "Exception caught: " << e.what() << std::endl;
            }
            NotifyTaskFinished();
        }
    }
}; 