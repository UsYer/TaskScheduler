
#include <iostream>
#include <chrono>

#include "TaskScheduler.hpp"

using namespace std::chrono_literals;

TaskScheduler::TaskScheduler() :
	m_thread_schedule{ &TaskScheduler::schedule_task, this },
	m_thread_standard{ &TaskScheduler::perform_standard_loop, this },
	m_thread_long_computation{ &TaskScheduler::perform_long_computation_loop, this },
	m_thread_io{ &TaskScheduler::perform_io_loop, this }
{
}


TaskScheduler::~TaskScheduler()
{
	m_quit.store(true);
	m_thread_schedule.join();
	m_thread_standard.join();
	m_thread_long_computation.join();
	m_thread_io.join();
}

void TaskScheduler::schedule(const Task & task){
	switch (task.category())	{

		case TaskCategory::Standard:
			{ // intentionally introduced a scope here to keep the lock closed as shortly as possible
			std::lock_guard<std::mutex>{ m_mutex_standard };
			m_task_queue_standard.push_back(task);
			}
			break;

		case TaskCategory::LongComputation:
			{
			std::lock_guard<std::mutex>{ m_mutex_long_computation };
			m_task_queue_long_computation.push_back(task);
			}
			break;
		
		case TaskCategory::IO:
			{
			std::lock_guard<std::mutex>{ m_mutex_io };
			m_task_queue_io.push_back(task);
			}
			break;
		default:
			break;
	}

	std::lock_guard<std::mutex>{ m_mutex_schedule };
	m_schedule = true;
	m_cv_schedule.notify_one();
}


std::int64_t TaskScheduler::task_count() const {
	return m_task_queue_io.size() + m_task_queue_long_computation.size() + m_task_queue_standard.size();
}

void TaskScheduler::schedule_task(){
	while (!m_quit)	{
		// Wait until schedule() sends data
		std::unique_lock<std::mutex> lk{ m_mutex_schedule };
		if (m_cv_schedule.wait_for(lk, 200ms, [this] {return this->m_schedule; })) {

			// after the wait, we own the lock.
			std::cout << "[scheduler] Schedule thread is active\n";

			/* scheduling algorithm:
			* for each category:
			*	if category thread available:
			if high prio task available
			if scheduled_high_prio == 0 and scheduled_standard_prio == 0 and scheduled_low_prio == 0:
			schedule task
			scheduled_high_prio = 1
			else if scheduled_high_prio == 1 and scheduled_standard_prio = 1 and scheduled_low_prio == 0
			schedule task
			scheduled_high_prio = 2
			else if scheduled_high_prio == 2 and scheduled_standard_prio = 2 and scheduled_low_prio == 0
			schedule task
			scheduled_high_prio = 3

			if standard prio task available
			if scheduled_high_prio == 0 and scheduled_standard_prio == 0 and scheduled_low_prio == 0:
			schedule task
			scheduled_standard_prio = 1;
			else if

			*/
			int scheduled_high_prio = 0;
			int scheduled_standard_prio = 0;



			// Send data back to main()
			m_schedule = false;
			std::cout << "[scheduler] scheduling completed\n";

			// Manual unlocking is done before notifying, to avoid waking up
			// the waiting thread only to block again (see notify_one for details)
			lk.unlock();
			m_cv_schedule.notify_one();
		}
	}
}

void TaskScheduler::perform_standard_loop(){
	while (!m_quit)	{
		if (m_task_queue_standard.empty())	{
			std::this_thread::sleep_for(200ms);
		} else {
			Task task;
			{
				std::lock_guard<std::mutex>{ m_mutex_standard };
				task = m_task_queue_standard.back();
				m_task_queue_standard.pop_back();
			}
			task();
		}
	}
}

void TaskScheduler::perform_long_computation_loop(){
	while (!m_quit) {
		if (m_task_queue_long_computation.empty()) {
			std::this_thread::sleep_for(200ms);
		}
		else {
			Task task;
			{
				std::lock_guard<std::mutex>{ m_mutex_long_computation };
				task = m_task_queue_long_computation.back();
				m_task_queue_long_computation.pop_back();
			}
			task();
		}
	}
}

void TaskScheduler::perform_io_loop(){
	while (!m_quit) {
		if (m_task_queue_io.empty()) {
			std::this_thread::sleep_for(200ms);
		}
		else {
			Task task;
			{
				std::lock_guard<std::mutex>{ m_mutex_io };
				task = m_task_queue_io.back();
				m_task_queue_io.pop_back();
			}
			task();
		}
	}
}
