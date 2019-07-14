#pragma once

#include "ThreadPool.h"

#include <type_traits>
#include <future>

#include <functional>

template<typename Function, typename ...Args>
inline std::future<std::invoke_result_t<Function, Args...>> ThreadPool::add_task(Function && function, Args && ...args)
{
	auto task = std::make_shared<std::packaged_task<std::invoke_result_t<Function, Args...>()>> ([function, args...]() { return function(args...); });

	{
		std::lock_guard<std::mutex> lock{ queue_mutex_ };
		queue_.emplace_back([task] { (*task)(); });
	}

	has_job_.post();
	return task->get_future();
}
