//========= Copyright © 1996-2005, Valve Corporation, All rights reserved. ============//
//
// Purpose: Implements visual effects entities: sprites, beams, bubbles, etc.
//
// $NoKeywords: $
//=============================================================================//
 
#include "cbase.h"
#include "tasks.h"
 
// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

void CC_AddTask(const CCommand& args)
{
    CBasePlayer* pPlayer = UTIL_GetCommandClient();

    int index = atoi(args[1]);
    int priority = atoi(args[2]);
    int count = atoi(args[3]);
    const char *target = args[4];

    if (pPlayer)
    {
        CTaskManager::GetTaskManager()->SendTaskData(index, priority, count, target, "#Task_Test");
    }
}
static ConCommand addtask("addtask", CC_AddTask, "Adds a test task\n", FCVAR_CHEAT);

BEGIN_DATADESC(CTaskManager)
END_DATADESC()

LINK_ENTITY_TO_CLASS(task_manager, CTaskManager)

CTaskManager* CTaskManager::pTaskManager;

CTaskManager* CTaskManager::GetTaskManager()
{
    if (pTaskManager == NULL)
    {
        pTaskManager = (CTaskManager*)gEntList.FindEntityByClassname(NULL, "task_manager");
        if (pTaskManager == NULL)
            pTaskManager = (CTaskManager*)CBaseEntity::Create("task_manager", vec3_origin, vec3_angle);
    }
    return pTaskManager;
}

bool CTaskManager::DoesTaskExistAtIndex(int index)
{
    FOR_EACH_VEC(GetTaskManager()->m_Tasks, i)
    {
        Task* t = GetTaskManager()->m_Tasks[i];

        if (t && t->index == index)
        {
            return true;
        }
    }
}

void CTaskManager::SendTaskData(int index, int urgency, int count, const char* target, const char* message, bool complete)
{
    // this hack is so stupid. i should never touch a keyboard again
    int iUrgency = urgency;

    if (complete)
    {
        iUrgency = TASK_COMPLETE;
    }

    CBasePlayer* pPlayer = NULL;

    // this will be fucking stupid for MP players.
    pPlayer = UTIL_GetLocalPlayer();

    if (pPlayer)
    {
        if (!pPlayer->IsNetClient())
        {
            return;
        }

        CSingleUserRecipientFilter user(pPlayer);
        user.MakeReliable();

        UserMessageBegin(user, "TaskList");
        WRITE_BYTE(index);
        WRITE_BYTE(iUrgency);
        WRITE_BYTE(count);
        WRITE_STRING(target);
        WRITE_STRING(message);
        MessageEnd();

        bool updatePriority = false;

        if (iUrgency > TASK_COMPLETE)
        {
            if (DoesTaskExistAtIndex(index))
            {
                //this task exists! don't do anything!
                updatePriority = true;
            }
        }

        if (!updatePriority)
        {
            if (iUrgency > TASK_COMPLETE)
            {
                Task* newTask = new Task(index);
                GetTaskManager()->m_Tasks.AddToHead(newTask);
                DevMsg("Added task %d to manager.\n", index);
            }
            else
            {
                FOR_EACH_VEC(GetTaskManager()->m_Tasks, i)
                {
                    Task* t = GetTaskManager()->m_Tasks[i];

                    if (t && t->index == index)
                    {
                        GetTaskManager()->m_Tasks.FindAndRemove(t);
                        DevMsg("Removed task %d from manager.\n", index);
                    }
                }

                if (iUrgency == TASK_COMPLETE)
                {
                    // reward us for completing the task
                    pPlayer->TaskCompleted();
                }
            }
        }
    }
}

void CTaskManager::Shutdown()
{
    pTaskManager = NULL;
}
 
LINK_ENTITY_TO_CLASS( env_hudtasklist, CEnvHudTasklist );
 
BEGIN_DATADESC( CEnvHudTasklist )
    DEFINE_KEYFIELD( m_iszTaskmsg[0], FIELD_STRING, "task1message" ),
    DEFINE_KEYFIELD( m_iUrgency[0], FIELD_INTEGER,  "task1urgency" ),
 
    DEFINE_KEYFIELD( m_iszTaskmsg[1], FIELD_STRING, "task2message" ),
    DEFINE_KEYFIELD( m_iUrgency[1], FIELD_INTEGER,  "task2urgency" ),
 
    DEFINE_KEYFIELD( m_iszTaskmsg[2], FIELD_STRING, "task3message" ),
    DEFINE_KEYFIELD( m_iUrgency[2], FIELD_INTEGER,  "task3urgency"),
 
    DEFINE_KEYFIELD( m_iszTaskmsg[3], FIELD_STRING, "task4message" ),
    DEFINE_KEYFIELD( m_iUrgency[3], FIELD_INTEGER,  "task4urgency" ),

    DEFINE_KEYFIELD(m_iszTaskmsg[3], FIELD_STRING, "task5message"),
    DEFINE_KEYFIELD(m_iUrgency[3], FIELD_INTEGER, "task5urgency"),
 
    // Set individual task list strings
    DEFINE_INPUTFUNC( FIELD_STRING, "Task1Message", InputTask1Message ),
    DEFINE_INPUTFUNC( FIELD_STRING, "Task2Message", InputTask2Message ),
    DEFINE_INPUTFUNC( FIELD_STRING, "Task3Message", InputTask3Message ),
    DEFINE_INPUTFUNC( FIELD_STRING, "Task4Message", InputTask4Message ),
    DEFINE_INPUTFUNC( FIELD_STRING, "Task5Message", InputTask5Message ),
 
    // Set individual task urgency values 
    DEFINE_INPUTFUNC( FIELD_INTEGER, "Task1Urgency", InputTask1Urgency ),
    DEFINE_INPUTFUNC( FIELD_INTEGER, "Task2Urgency", InputTask2Urgency ),
    DEFINE_INPUTFUNC( FIELD_INTEGER, "Task3Urgency", InputTask3Urgency ),
    DEFINE_INPUTFUNC( FIELD_INTEGER, "Task4Urgency", InputTask4Urgency ),
    DEFINE_INPUTFUNC( FIELD_INTEGER, "Task5Urgency", InputTask5Urgency ),
END_DATADESC()
 
//-----------------------------------------------------------------------------
// Purpose: Spawn the new ent
//-----------------------------------------------------------------------------
void CEnvHudTasklist::Spawn( void )
{
    Precache();
    SetSolid( SOLID_NONE );
    SetMoveType( MOVETYPE_NONE );
}
 
//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CEnvHudTasklist::Precache( void )
{
}
 
//-----------------------------------------------------------------------------
// Send a task data message to the client.  This gets caught by the task HUD
// display element and shown.
//-----------------------------------------------------------------------------
void CEnvHudTasklist::SendTaskData(int index, bool dismiss)
{
    CTaskManager::GetTaskManager()->SendTaskData(index, m_iUrgency[index], 0, "", STRING(m_iszTaskmsg[index]), dismiss);
}

void CEnvHudTasklist::TaskMessage(int index, const char* message)
{
    m_iszTaskmsg[index] = MAKE_STRING(message);
    SendTaskData(index);
}

void CEnvHudTasklist::TaskUrgency(int index, int urgency)
{
    m_iUrgency[index] = urgency;
    SendTaskData(index);
}
 
//-----------------------------------------------------------------------------
// Purpose: Input handler for setting task 1 urgency
// DEFINE_INPUTFUNC( FIELD_STRING, "Task1Urgency", InputTask1Urgency ),
//-----------------------------------------------------------------------------
void CEnvHudTasklist::InputTask1Urgency ( inputdata_t &inputdata )
{
    TaskUrgency(0, inputdata.value.Int());
}
 
//-----------------------------------------------------------------------------
// Purpose: Input handler for setting task 2 urgency
// DEFINE_INPUTFUNC( FIELD_STRING, "Task2Urgency", InputTask2Urgency ),
//-----------------------------------------------------------------------------
void CEnvHudTasklist::InputTask2Urgency ( inputdata_t &inputdata )
{
    TaskUrgency(1, inputdata.value.Int());
}
 
//-----------------------------------------------------------------------------
// Purpose: Input handler for setting task 3 urgency
// DEFINE_INPUTFUNC( FIELD_STRING, "Task3Urgency", InputTask3Urgency ),
//-----------------------------------------------------------------------------
void CEnvHudTasklist::InputTask3Urgency ( inputdata_t &inputdata )
{
    TaskUrgency(2, inputdata.value.Int());
}
 
//-----------------------------------------------------------------------------
// Purpose: Input handler for setting task 4 urgency
// DEFINE_INPUTFUNC( FIELD_STRING, "Task4Urgency", InputTask4Urgency ),
//-----------------------------------------------------------------------------
void CEnvHudTasklist::InputTask4Urgency ( inputdata_t &inputdata )
{
    TaskUrgency(3, inputdata.value.Int());
}

//-----------------------------------------------------------------------------
// Purpose: Input handler for setting task 5 urgency
// DEFINE_INPUTFUNC( FIELD_STRING, "Task5Urgency", InputTask5Urgency ),
//-----------------------------------------------------------------------------
void CEnvHudTasklist::InputTask5Urgency(inputdata_t& inputdata)
{
    TaskUrgency(4, inputdata.value.Int());
}

//-----------------------------------------------------------------------------
 
//-----------------------------------------------------------------------------
// Purpose: Input handler for setting task 1 text
// DEFINE_INPUTFUNC( FIELD_STRING, "TaskMessage1", InputTaskMessage1 ),
//-----------------------------------------------------------------------------
void CEnvHudTasklist::InputTask1Message( inputdata_t &inputdata )
{
    TaskMessage(1, inputdata.value.String());
}

//-----------------------------------------------------------------------------
// Purpose: Input handler for setting task 2 text
// DEFINE_INPUTFUNC( FIELD_STRING, "TaskMessage2", InputTaskMessage2 ),
//-----------------------------------------------------------------------------
void CEnvHudTasklist::InputTask2Message( inputdata_t &inputdata )
{
    TaskMessage(2, inputdata.value.String());
}

//-----------------------------------------------------------------------------
// Purpose: Input handler for setting task 3 text
// DEFINE_INPUTFUNC( FIELD_STRING, "TaskMessage3", InputTaskMessage3 ),
//-----------------------------------------------------------------------------
void CEnvHudTasklist::InputTask3Message( inputdata_t &inputdata )
{
    TaskMessage(3, inputdata.value.String());
}

//-----------------------------------------------------------------------------
// Purpose: Input handler for setting task 4 text
// DEFINE_INPUTFUNC( FIELD_STRING, "TaskMessage4", InputTaskMessage4 ),
//-----------------------------------------------------------------------------
void CEnvHudTasklist::InputTask4Message( inputdata_t &inputdata )
{
    TaskMessage(3, inputdata.value.String());
}

//-----------------------------------------------------------------------------
// Purpose: Input handler for setting task 5 text
// DEFINE_INPUTFUNC( FIELD_STRING, "TaskMessage5", InputTaskMessage4 ),
//-----------------------------------------------------------------------------
void CEnvHudTasklist::InputTask5Message(inputdata_t& inputdata)
{
    TaskMessage(4, inputdata.value.String());
}