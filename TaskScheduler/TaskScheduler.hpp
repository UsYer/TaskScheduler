#pragma once
#include <thread>
#include <mutex>
#include <vector>
#include <atomic>

#include "Task.hpp"

class TaskScheduler
{
public:
	TaskScheduler();
	~TaskScheduler();
	void schedule(const Task& task);
	std::int64_t task_count() const;
private:

	void schedule_task();
	void perform_standard_loop();
	void perform_long_computation_loop();
	void perform_io_loop();

	std::vector<Task> m_task_queue_standard;
	std::vector<Task> m_task_queue_long_computation;
	std::vector<Task> m_task_queue_io;

	std::mutex m_mutex_standard;
	std::mutex m_mutex_long_computation;
	std::mutex m_mutex_io;

	std::atomic_bool m_quit = false;

	std::condition_variable m_cv_schedule;
	std::mutex m_mutex_schedule;
	bool m_schedule = false;

	std::thread m_thread_schedule;
	std::thread m_thread_standard;
	std::thread m_thread_long_computation;
	std::thread m_thread_io;
};

