#pragma once
#include <cstddef>
#include <functional>

enum class TaskPriority
{	
	Low,
	Standard,
	High
};

enum class TaskCategory {
	Standard,
	LongComputation,
	IO
};

class Task
{
public:
	Task();
	Task(TaskCategory category, TaskPriority prio, const std::function<void()>& target_callable);
	~Task();
	TaskPriority priority() const {
		return m_prio;
	}

	TaskCategory category() const {
		return m_category;
	}

	void operator() () {
		m_target();
	}
private:
	std::function<void()> m_target;
	TaskPriority m_prio;
	TaskCategory m_category;
};

