// TaskSchedulerProgram.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "../TaskScheduler/TaskScheduler.hpp"

using namespace std::chrono_literals;

int main()
{
	TaskScheduler scheduler;
	for (size_t i = 0; i < 10; i++) {

		scheduler.schedule({ TaskCategory::Standard, TaskPriority::Standard, [&scheduler, i]() {
			std::cout << "[standard] hello from standard task " << i << "!\nscheduling new task\n";
			scheduler.schedule({ TaskCategory::IO, TaskPriority::Standard, [i] {
				std::cout << "[io] hello from io task " << i << "!\n[io] sleeping...\n";
				std::this_thread::sleep_for(7s); }
			});
			std::cout << "[standard] sleeping...\n";
			std::this_thread::sleep_for(5s);
			std::cout << "[standard] Done sleeping!\n"; }
		});
	}
	scheduler.schedule({ TaskCategory::LongComputation, TaskPriority::Standard, [] {
		std::cout << "[long_comp] hello from long_comp task!\n[long_comp] sleeping...\n";
		std::this_thread::sleep_for(3s);
		std::cout << "[long_comp] Done sleeping!\n";
		} 
	});
	std::cin.get();
    return 0;
}

