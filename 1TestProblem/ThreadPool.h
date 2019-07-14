#pragma once

#include <thread>
#include <type_traits>
#include <future>
#include <deque>
#include <csignal>
#include <iostream>
class ThreadPool final
{
public:
	explicit ThreadPool(size_t number_of_threads = std::thread::hardware_concurrency());

	ThreadPool(const ThreadPool &) = delete;
	ThreadPool & operator=(const ThreadPool &) = delete;
	ThreadPool(ThreadPool &&) = delete;
	ThreadPool & operator=(ThreadPool &&) = delete;

	~ThreadPool();

	template<typename Function, typename ...Args>
	std::future<std::invoke_result_t<Function, Args...>> add_task(Function && function, Args && ...args);

	void stop();

private:
	class Semaphore final
	{
	public:
		explicit Semaphore(size_t n);

		Semaphore(const Semaphore &) = delete;
		Semaphore & operator=(const Semaphore &) = delete;
		Semaphore(Semaphore &&) = delete;
		Semaphore & operator=(Semaphore &&) = delete;


		void wait();
		void post();

	private:
		std::size_t counter_;
		std::mutex mutex_;
		std::condition_variable condition_;
	};

private:
	size_t number_of_threads_;
	std::deque<std::function<void()>> queue_;
	std::vector<std::thread> workers_;
	Semaphore has_job_;
	std::atomic_size_t idle_threads_;
	std::mutex queue_mutex_;
	std::sig_atomic_t quit_;
};

#include "ThreadPool.inl"
