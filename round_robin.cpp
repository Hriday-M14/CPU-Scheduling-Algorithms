// Round Robin CPU Scheduling Algorithm
#include <bits/stdc++.h>
using namespace std;

class Process{
    int process_id;
    int burst_time;
    int remaining_burst;
    int arrival_time;
    int turnaround_time;
    int completion_time;
    int waiting_time;
    bool executed;
    bool in_readyqueue;

    public:
    Process()
    {
        executed = false;
        in_readyqueue = false;
    }
    // Setters
    void SetProcessId(int i)
    {
        this->process_id = i;
    }
    void SetArrivalTime(int tm)
    {
        this->arrival_time = tm;
    }
    void SetBurstTime(int tm)
    {
        this->burst_time = tm;
        this->remaining_burst = tm;
    }
    void SetCompletionTime(int tm)
    {
        this->completion_time = tm;
    }

    //Getters
    int GetArrivalTime()
    {
        return arrival_time;
    }
    int GetBurstTime()
    {
        return burst_time;
    }
    int GetRemBurstTime()
    {
        return remaining_burst;
    }
    int GetCompletionTime()
    {
        return completion_time;
    }
    int GetWaitingTime()
    {
        return waiting_time;
    }
    int GetTurnaroundTime()
    {
        return turnaround_time;
    }

    void UpdateAfterCompletion()
    {
        turnaround_time = completion_time - arrival_time;
        waiting_time = turnaround_time - burst_time;
    }
    void display()
    {
        cout << process_id << "\t" << arrival_time << "\t" << burst_time << "\t" << completion_time << "\t" << turnaround_time << "\t" << waiting_time << endl;
    }
    bool IsExecuted()
    {
        return executed;
    }
    void DoneExecution()
    {
        executed = true;
    }
    void UpdateRemBurstTime()
    {
        remaining_burst--;
        if(remaining_burst == 0)
            DoneExecution();
    }
    bool InReadyQueue()
    {
        return in_readyqueue;
    }
    void AddInReadyQueue()
    {
        in_readyqueue = true;
    }
    void RemFromReadyQueue()
    {
        in_readyqueue = false;
    }
};

void CalculateAvgTime(vector<Process*>& Processes)
{
    double AvgWaitingTime = 0.0, AvgTurnaroundTime = 0.0;
    for(int i = 0; i < Processes.size(); i++)
    {
        AvgWaitingTime += Processes[i]->GetWaitingTime();
        AvgTurnaroundTime += Processes[i]->GetTurnaroundTime();
    }
    cout << "Average Waiting Time: " << AvgWaitingTime / Processes.size() << endl;
    cout << "Average Turnaround Time: " << AvgTurnaroundTime / Processes.size() << endl;
}

void GetProcesses(int tm, vector<Process*>& AllProcesses, queue<Process*>& ReadyQueue)
{
    for(int i = 0; i < AllProcesses.size(); i++)
    {
        Process* CurrentProc = AllProcesses[i];
        if(CurrentProc->GetArrivalTime() <= tm && !CurrentProc->IsExecuted() && !CurrentProc->InReadyQueue())
        {
            ReadyQueue.push(CurrentProc);
            CurrentProc->AddInReadyQueue();
        }   
    }
}

int main()
{
    int n;
    cout << "Enter the Number of Processes to consider: ";
    cin >> n;
    vector<Process*> Processes;
    for(int i = 0; i < n; i++)
    {
        Process* CurrentProc = new Process();
        int arrival_time, burst_time;
        cout << "Enter the Arrival Time of Process " << i+1 <<" : ";
        cin >> arrival_time;
        cout << "Enter the Burst Time of Process " << i+1 << " : ";
        cin >> burst_time;
        CurrentProc->SetProcessId(i+1);
        CurrentProc->SetArrivalTime(arrival_time);
        CurrentProc->SetBurstTime(burst_time);
        Processes.push_back(CurrentProc);
    }
    int time_quantum;
    cout << "Enter the Time Quantum of the Algorithm: ";
    cin >> time_quantum;

    // Sort the Processes in the Ascending order in which they arrive in Ready Queue
    sort(Processes.begin(), Processes.end(), [](Process* P1, Process* P2){
        return P1->GetArrivalTime() < P2->GetArrivalTime();
    });

    cout << "\n\npid\tat\tbt\tct\ttat\twt" << endl;
    
    int CurrentTime = 0, ProcessesExecuted = 0;
    queue<Process*> ReadyQueue;
    while(ProcessesExecuted < n)
    {
        // Checking and returning processes in Ready Queue at Current Instance of Time and yet to be Executed completely in order of their
        // arrival in the Ready Queue
        GetProcesses(CurrentTime, Processes, ReadyQueue);
        if(ReadyQueue.size() == 0)
        {
            CurrentTime++;
            continue;
        }

        // Now out of all the Processes in the Ready Queue, only first will get the CPU 
        // If the Remaining Burst Time for it is less than Time Quantum then it gets executed completely and goes to termination state
        // else it gets executed for the Time Quantum unit and after all the Processes that should be again in the ready queue 
        // at this instance of time are added, current process gets preempted and pushed back in the ready queue so that it can be executed
        // again later
        
        Process* ToBeScheduled = ReadyQueue.front();
        ReadyQueue.pop();

        if(ToBeScheduled->GetRemBurstTime() <= time_quantum)
        {
            while(!ToBeScheduled->IsExecuted())
            {
                ToBeScheduled->UpdateRemBurstTime();
                CurrentTime++;
            } 
            ProcessesExecuted++;
            ToBeScheduled->SetCompletionTime(CurrentTime);
            ToBeScheduled->UpdateAfterCompletion();
            ToBeScheduled->display();
        }
        else
        {
            for(int i = 0; i < time_quantum; i++)
            {
                ToBeScheduled->UpdateRemBurstTime();
                CurrentTime++;
            }
        }
        GetProcesses(CurrentTime, Processes, ReadyQueue);
        // Reason to remove Current Process later from the Ready Queue or changing it's status later is because if we 
        // change it's status earlier before calling GetProcesses function then this function will again add current process in the ready
        // queue and at the front of the ready queue resulting in same process to get CPU again after it's execution for the first 
        // time
        ToBeScheduled->RemFromReadyQueue();
        if(!ToBeScheduled->IsExecuted())
        {
            ReadyQueue.push(ToBeScheduled);
            ToBeScheduled->AddInReadyQueue();
        }
    }

    // Calculate Average Waiting Time and Average Turnaround Time
    CalculateAvgTime(Processes);
    return 0;
}

