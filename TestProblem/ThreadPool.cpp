#include "pch.h"
#include "ThreadPool.h"
#include <stdexcept>

ThreadPool::Semaphore::Semaphore(const size_t n)
	: counter_{ n }, number_of_tasks_{ 0u }
{
}

void ThreadPool::Semaphore::wait()
{
	std::unique_lock<std::mutex> lock{ mutex_ };
	condition_.wait(lock, [&] { return counter_ > 0; });
	--counter_;
	number_of_tasks_++;
}

void ThreadPool::Semaphore::post()
{
	std::lock_guard<std::mutex> lock{ mutex_ };
	++counter_;
	condition_.notify_one();
	number_of_tasks_--;
}

ThreadPool::ThreadPool(const size_t number_of_threads)
	: number_of_threads_{ number_of_threads }
	, workers_(number_of_threads_)
	, has_job_{ 0u }
	, idle_threads_{ number_of_threads_ }
	, quit_{ 0u }
	, flag_{ 0u }
{
	if (!number_of_threads_)
	{
		//! care if one of member will not be constructed with an exception, there will rethrow and we wont join here
		throw std::runtime_error("Multithreading is not supported");
	}

	auto routine = [this] {
		while (!quit_)
		{
			if (!flag_)
				has_job_.wait();

			if (flag_)
			{
				has_job_.post();
				break;
			}
	
			if (quit_)
			{
				
				break;
			}

			idle_threads_.fetch_sub(1, std::memory_order_release);

			std::unique_lock<std::mutex> lock{ queue_mutex_ };
			const auto job = queue_.front();
			queue_.pop_front();
			lock.unlock();

			job();
			idle_threads_.fetch_add(1, std::memory_order_release);
		}
		return;
	};

	for (auto & worker : workers_) {
		worker = std::thread(routine);
	}


}

ThreadPool::~ThreadPool()
{

	for (;;)
	{
		if (queue_.empty())
		{
			flag_ = 1u;
			break;
		}
	}
	for (auto & worker : workers_)
		worker.join();
	//stop();
}

void ThreadPool::stop()
{
	quit_ = 1u;
}




