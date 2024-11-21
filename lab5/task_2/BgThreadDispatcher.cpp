#include "BgThreadDispatcher.h"
#include <iostream>

BgThreadDispatcher::BgThreadDispatcher()
    : m_thread(&BgThreadDispatcher::WorkerThread, this)
{
}

BgThreadDispatcher::~BgThreadDispatcher()
{
    //
    Stop();
}

void BgThreadDispatcher::Dispatch(Task task)
{
    // проверить stopped дол захвата и после(это уже сделал)
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        if (m_stopped)
        {
            return;
        }
        m_tasks.push(std::move(task));
    }
    m_condition.notify_one();
}

void BgThreadDispatcher::Wait()
{
    std::unique_lock<std::mutex> lock(m_mutex);
    if (m_stopped)
        return;

    m_waiting = true;
    // добавить ещё один condition
    m_condition.wait(lock, [this] 
        {
            return m_tasks.empty() || m_stopped;
        });
    m_waiting = false;
}

void BgThreadDispatcher::Stop()
{
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        m_stopped = true;
        // clear
        while (!m_tasks.empty())
        {
            m_tasks.pop();
        }
    }
    m_condition.notify_all();
}

void BgThreadDispatcher::WorkerThread()
{
    while (true)
    {
        Task task;
        {
            std::unique_lock<std::mutex> lock(m_mutex);

            m_condition.wait(lock, [this] 
                {
                    return !m_tasks.empty() || m_stopped;
                });

            if (m_stopped)
            {
                return;
            }
                
            task = std::move(m_tasks.front());
            m_tasks.pop();
        }

        try
        {
            task();
        }
        catch (const std::exception& ex)
        {
            std::cerr << "Task threw an exception: " << ex.what() << '\n';
        }
        catch (...)
        {
            std::cerr << "Task threw an unknown exception.\n";
        }

        if (m_waiting && m_tasks.empty())
        {
            m_condition.notify_all();
        }
    }
}
