#include "widget/thread_pool.h"

namespace zidian{
    ThreadPool::ThreadPool(size_t numThreads, ScheduleMode mode_)
        : stop(false), mode(mode_){
        for (size_t i = 0; i < numThreads; ++i) {
            workers.emplace_back([this] { this->workerLoop(); });
        }//end for i
    }

    ThreadPool::~ThreadPool() {
        wait();
    }

    void ThreadPool::shutdown() {
        stop.store(true);
        condition.notify_all();
    }

    void ThreadPool::wait() {
        shutdown();
        for (std::thread &worker : workers) {
            if (worker.joinable())
                worker.join();
        }
    }

    void ThreadPool::workerLoop() {
        while (true) {
            std::function<void()> task;
            {
                std::unique_lock<std::mutex> lock(queue_mutex);
                condition.wait(lock, [this] {
                    return stop.load() || !tasks.empty();
                });

                if (stop && tasks.empty())
                    return;

                if (mode == ScheduleMode::FIFO) {
                    task = std::move(tasks.front());
                    tasks.pop_front();
                } else {
                    task = std::move(tasks.back());
                    tasks.pop_back();
                }
            }
            task();
        }//end while
    }

}

