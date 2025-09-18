#pragma once

#include <vector>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <functional>
#include <future>
#include <queue>
#include <deque>
#include <atomic>
#include <stdexcept>

namespace zidian{
    enum class ScheduleMode {
        FIFO,
        LIFO
    };

    class ThreadPool{
    public:
        ThreadPool(size_t numThreads, ScheduleMode mode = ScheduleMode::FIFO);
        ~ThreadPool();

        template<class F, class... Args>
        auto enqueue(F&& f, Args&&... args) 
            -> std::future<typename std::result_of<F(Args...)>::type> {
            using return_type = typename std::result_of<F(Args...)>::type;

            auto task = std::make_shared<std::packaged_task<return_type()> >(
                std::bind(std::forward<F>(f), std::forward<Args>(args)...)
            );

            std::future<return_type> res = task->get_future();
            {
                std::unique_lock<std::mutex> lock(queue_mutex);
                if (stop)
                    throw std::runtime_error("enqueue on stopped ThreadPool");

                if (mode == ScheduleMode::FIFO)
                    tasks.emplace_back([task]() { (*task)(); });
                else
                    tasks.emplace_front([task]() { (*task)(); });
            }
            condition.notify_one();
            return res;
        }

        void shutdown();   // 异步关闭，不阻塞
        void wait();       // 阻塞直到所有任务完成并线程退出

    private:
        std::vector<std::thread> workers;
        std::deque<std::function<void()>> tasks;
        std::mutex queue_mutex;
        std::condition_variable condition;
        std::atomic<bool> stop;

        ScheduleMode mode;

        void workerLoop();
    };
}



