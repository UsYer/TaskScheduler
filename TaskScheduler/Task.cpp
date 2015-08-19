#include "Task.hpp"


Task::Task()
{
}

Task::Task(TaskCategory category, TaskPriority prio, const std::function<void()>& target_callable):
	m_category{category},
	m_prio{prio},
	m_target{target_callable}
{
}


Task::~Task()
{
}
