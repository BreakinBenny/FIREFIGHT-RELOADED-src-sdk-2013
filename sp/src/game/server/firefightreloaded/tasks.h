#ifndef TASKS_H
#define TASKS_H

#ifdef _WIN32
#pragma once
#endif

#include "cbase.h"
#include "engine/IEngineSound.h"
#include "baseentity.h"
#include "entityoutput.h"
#include "recipientfilter.h"
#include "tasks_shared.h"

class Task
{
public:
    Task(int iIndex, int iUrgency, int iCount, string_t szTarget, string_t szMessage)
    {
        index = iIndex;
        urgency = iUrgency;
        count = iCount;
        target = szTarget;
        message = szMessage;
    }

    int index;
    int urgency;
    int count;
    string_t target;
    string_t message;
};

class CTaskManager : public CBaseEntity
{
	DECLARE_CLASS(CTaskManager, CBaseEntity)
	DECLARE_DATADESC()

public:
	static CTaskManager* pTaskManager;
	static CTaskManager* GetTaskManager();

    static bool DoesTaskExistAtIndex(int index);
    static Task *GetTaskInfo(int index);

    static void Wipe();

	static void SendTaskData(int index, int urgency, int count, const char *target, const char* message, bool complete = false);
	static void Shutdown();

    CUtlVector<Task *> m_Tasks;
};

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
class CEnvHudTasklist : public CPointEntity
{
public:
    DECLARE_CLASS(CEnvHudTasklist, CPointEntity);

    void Spawn(void);
    void Precache(void);

private:

    void InputTask1Message(inputdata_t& inputdata);
    void InputTask2Message(inputdata_t& inputdata);
    void InputTask3Message(inputdata_t& inputdata);
    void InputTask4Message(inputdata_t& inputdata);
    void InputTask5Message(inputdata_t& inputdata);

    void InputTask1Urgency(inputdata_t& inputdata);
    void InputTask2Urgency(inputdata_t& inputdata);
    void InputTask3Urgency(inputdata_t& inputdata);
    void InputTask4Urgency(inputdata_t& inputdata);
    void InputTask5Urgency(inputdata_t& inputdata);

    void TaskMessage(int index, const char* message);
    void TaskUrgency(int index, int urgency);

    void SendTaskData(int index, bool dismiss = false);

    string_t m_iszTaskmsg[TASKLIST_MAX_TASKS];
    int m_iUrgency[TASKLIST_MAX_TASKS]; // 0=complete, 1=low, 2=medium, 3=high

    DECLARE_DATADESC();
};

#endif