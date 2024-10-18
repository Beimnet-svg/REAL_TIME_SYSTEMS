#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <stdbool.h>
#include <string.h>
#include "scheduling.h"
#include "schedulers.h"

#define NUM_QUEUES 3
#define QUEUE_QUANTA {5,7,10}  //Example quanta for three priority levels

void set_task_state(struct Task *task, enum taskState taskNewState)
{
    pthread_mutex_lock(&taskStateMutex);
    task->state = taskNewState;
    pthread_mutex_unlock(&taskStateMutex);
}

void wait_for_rescheduling(int quantum, struct Task *task)
{
    int startTime;
    int waitTime;

    pthread_mutex_lock(&timeMutex);
    startTime = globalTime;
    pthread_mutex_unlock(&timeMutex);

    do
    {
        pthread_mutex_lock(&timeMutex);
        pthread_cond_wait(&timeCond, &timeMutex);
        waitTime = globalTime - startTime;
        pthread_mutex_unlock(&timeMutex);
    } while (task->state != finished && waitTime < quantum);

    usleep(timeUnitUs / 100);
}

void round_robin(struct Task **tasks, int taskCount, int timeout, int quantum)
{
    int taskIndex = 0;

    do
    {
        // Skip finished tasks or those that have not arrived yet
        if (tasks[taskIndex]->state == finished || tasks[taskIndex]->arrivalTime > globalTime)
        {
            taskIndex = (taskIndex + 1) % taskCount;
            continue;
        }

        // Set the task state to running
        if (tasks[taskIndex]->startTime == -1)
            tasks[taskIndex]->startTime = globalTime;
        set_task_state(tasks[taskIndex], running);

        // Wait for the quantum interval
        wait_for_rescheduling(quantum, tasks[taskIndex]);

        //  Check if the task is finished
        if (tasks[taskIndex]->state == finished)
        {
        }
        else
        {
            set_task_state(tasks[taskIndex], preempted);
        }

        // Find the next task to run
        taskIndex = (taskIndex + 1) % taskCount;

    } while (globalTime < timeout);
}

// Implement your schedulers here!
void first_come_first_served(struct Task **tasks, int taskCount, int timeout)
{
    int taskIndex = 0;
    while(globalTime < timeout)
    {
        //Find the next available task by arrival time
        while(taskIndex < taskCount && (tasks[taskIndex]->state == finished || tasks[taskIndex]->arrivalTime > globalTime))
        {
            taskIndex++;
        }
        
        //If no more tasks to schedule
        if(taskIndex >= taskCount)
            break;
        
        //Execute the selected task
        tasks[taskIndex] -> startTime = globalTime;
        set_task_state(tasks[taskIndex], running);
        
        //Simulate the task running for its entire burst time
        wait_for_rescheduling(tasks[taskIndex]->totalRuntime, tasks[taskIndex]);
        
        //Mark task as finished
        set_task_state(tasks[taskIndex], finished);
        taskIndex++;
    }
}
void shortest_process_next(struct Task **tasks, int taskCount, int timeout)
{
    int completedTasks = 0;
    do{
        int shortestTaskIndex = -1;
        int shortestBurst = 10000;
        
        //Find the task with the shortest burst time that has arrived and is not finished
        for(int i =0; i<taskCount; i++)
        {
            if(tasks[i] -> state != finished && tasks[i] -> arrivalTime <= globalTime && tasks[i] -> totalRuntime < shortestBurst)
            {
                shortestTaskIndex = i;
                shortestBurst = tasks[i] -> totalRuntime;
            }
        }
        
        
        //Set the task's start time if it hasn't started before
        if(tasks[shortestTaskIndex] -> startTime == -1)
            tasks[shortestTaskIndex] -> startTime = globalTime;
        set_task_state(tasks[shortestTaskIndex], running);
        
        //Run the task fro its entire burst time
        wait_for_rescheduling(tasks[shortestTaskIndex] -> totalRuntime, tasks[shortestTaskIndex]);
        
        
        //Mark the task as finished
        set_task_state(tasks[shortestTaskIndex], finished);
        completedTasks++;
    }while(globalTime<timeout && completedTasks < taskCount);
}
void highest_response_ratio_next(struct Task **tasks, int taskCount, int timeout)
{
    int completedTasks = 0;
    do
    {
        int selectedTaskIndex = -1;
        double highestResponseRatio = -1.0;
        
        //Find the task with the highest response ratio that has arrived and is not finished
        for(int i = 0; i< taskCount;i++)
        {
            if(tasks[i] -> state != finished && tasks[i]->arrivalTime <= globalTime)
            {
                //Calculating waiting time
                int waitingTime = globalTime - tasks[i] -> arrivalTime;
                
                //Calculate response ratio
                double responseRatio = (double)(waitingTime + tasks[i]->totalRuntime)/tasks[i]->totalRuntime;
                
                //Check if this task has the highest response ratio so far
                if(responseRatio > highestResponseRatio)
                {
                    highestResponseRatio = responseRatio;
                    selectedTaskIndex = i;
                }
            }
        }
        
        
        set_task_state(tasks[selectedTaskIndex], running);
        
        //Run the selected task index for it's entire burst time
        wait_for_rescheduling(tasks[selectedTaskIndex]->totalRuntime, tasks[selectedTaskIndex]);
        
        
        //Mark the task as finished
        set_task_state(tasks[selectedTaskIndex], finished);
        completedTasks++;
        
    }while(globalTime<timeout && completedTasks < taskCount);
}
void shortest_remaining_time(struct Task **tasks, int taskCount, int timeout, int quantum)
{
    int completedTasks = 0;
    do
    {
        int shortestTaskIndex = -1;
        int shortestRemainingTime = 10000;
        
        //Find the task with the shortest remaining time that has arrived and is not finished
        for(int i = 0; i < taskCount; i++)
        {
            if(tasks[i]->state!=finished & tasks[i] -> arrivalTime <= globalTime && (tasks[i]->totalRuntime-tasks[i]->currentRuntime) < shortestRemainingTime)
            {
                shortestTaskIndex = i;
                shortestRemainingTime = tasks[i]->totalRuntime-tasks[i]->currentRuntime;
            }
        }
        
        
        
        //Set the task's start time if it hasn't started before
        if(tasks[shortestTaskIndex] -> startTime == -1)
        {
            tasks[shortestTaskIndex] -> startTime = globalTime;
        }
        set_task_state(tasks[shortestTaskIndex], running);
        
        //Run the task for 10 time units(preemptive scheduling)
        wait_for_rescheduling(quantum, tasks[shortestTaskIndex]);
        
        
        //Check if the task has completed
        if(tasks[shortestTaskIndex] -> currentRuntime >= tasks[shortestTaskIndex]->totalRuntime)
        {
            set_task_state(tasks[shortestTaskIndex], finished);
            completedTasks++;
        }
        else
        {
            //Set the task to preempted if it hasn't finished
            set_task_state(tasks[shortestTaskIndex], preempted);
        }
    }while(completedTasks < taskCount && globalTime < timeout);
}
void feedback(struct Task **tasks, int taskCount, int timeout, int quantum)
{
    int completedTasks = 0;
    int queueQuanta[NUM_QUEUES] = QUEUE_QUANTA; //Define time quanta for each queue
    
    //Assign all tasks to the highest-priority queue initially
    for(int i=0; i<taskCount;i++)
    {
        tasks[i] -> priorityQueue =0;
    }
    do
    {
        int selectedTaskIndex = -1;
        int selectedQueueLevel = NUM_QUEUES; //Start with the lowest priority(highest number)
        
        //Find the task with the least priority queue level that has arrived and is not finished
        for(int i = 0; i < taskCount; i++)
        {
            if((tasks[i]->state!=finished) && (tasks[i] -> arrivalTime <= globalTime) && (tasks[i]->priorityQueue < selectedQueueLevel))
            {
                selectedTaskIndex = i;
                selectedQueueLevel = tasks[i]->priorityQueue;
            }
        }
        
        
        //Set the task's start time if it hasn't started before
        if(tasks[selectedTaskIndex] -> startTime == -1)
        {
            tasks[selectedTaskIndex] -> startTime = globalTime;
        }
        set_task_state(tasks[selectedTaskIndex], running);
        
        //Run the task for its assigned quantum based on its priority queue level
        int timeSlice = queueQuanta[tasks[selectedTaskIndex]->priorityQueue];
        int actualTimeRun = (tasks[selectedTaskIndex]->totalRuntime - tasks[selectedTaskIndex]->currentRuntime < timeSlice) ? 
                            (tasks[selectedTaskIndex]->totalRuntime - tasks[selectedTaskIndex]->currentRuntime < timeSlice): timeSlice;
        wait_for_rescheduling(actualTimeRun, tasks[selectedTaskIndex]);
        
        
        //Check if the task has completed
        if(tasks[selectedTaskIndex] -> currentRuntime >= tasks[selectedTaskIndex]->totalRuntime)
        {
            set_task_state(tasks[selectedTaskIndex], finished);
            completedTasks++;
        }
        else
        {
            //Move the task to a lower-priority queue if it hasn't completed
            if(tasks[selectedTaskIndex]-> priorityQueue < NUM_QUEUES-1)
                tasks[selectedTaskIndex] -> priorityQueue++;
            set_task_state(tasks[selectedTaskIndex], preempted);
        }
    }while(completedTasks < taskCount && globalTime < timeout);
}
