#ifndef TASKS_SHARED_H
#define TASKS_SHARED_H
#ifdef _WIN32
#pragma once
#endif

#define TASKLIST_MAX_TASKS 5

enum eTaskPriority
{
	TASK_INACTIVE = 0,
	TASK_COMPLETE,
	TASK_LOW,
	TASK_MEDIUM,
	TASK_HIGH
};

#endif