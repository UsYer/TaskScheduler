#include "stdafx.h"
#include "CppUnitTest.h"
#include "../TaskScheduler/TaskScheduler.hpp"
#include "../TaskScheduler/Task.hpp"

namespace Microsoft {
	namespace VisualStudio {
		namespace CppUnitTestFramework
		{
			template<> inline std::wstring ToString<std::int64_t>(const int64_t& t) {
				return std::to_wstring(t); 
			}
		}
	}
}



using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace TaskSchedulerTest
{		
	TEST_CLASS(UnitTest1)
	{
	public:
		
		TEST_METHOD(CreateTaskScheduler)
		{
			TaskScheduler tscheduler;
		}
		TEST_METHOD(CreateTask) {
			Task task{ TaskCategory::Standard, TaskPriority::High, []() {} };
		}

		TEST_METHOD(AddTaskToScheduler) {
			TaskScheduler tscheduler;
			Task task{ TaskCategory::Standard, TaskPriority::High, []() {} };
			tscheduler.schedule(task);
			Assert::AreEqual(1ll, tscheduler.task_count(), L"Number of scheduled tasks is not correct.");
		}
	};
}