// Priority (Non-Preemptive) CPU Scheduling Algorithm
#include <bits/stdc++.h>
using namespace std;

class Process{
    int process_id;
    int priority;
    int burst_time;
    int arrival_time;
    int turnaround_time;
    int completion_time;
    int waiting_time;
    bool executed;

    public:
    Process()
    {
        executed = false;
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
    }
    void SetCompletionTime(int tm)
    {
        this->completion_time = tm;
    }
    void SetPriority(int p)
    {
        this->priority = p;
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
    int GetPriority()
    {
        return priority;
    }

    void UpdateAfterCompletion()
    {
        turnaround_time = completion_time - arrival_time;
        waiting_time = turnaround_time - burst_time;
    }
    void display()
    {
        cout << process_id << "\t" << priority << "\t" << arrival_time << "\t" << burst_time << "\t" << completion_time << "\t" << turnaround_time << "\t" << waiting_time << endl;
    }
    bool IsExecuted()
    {
        return executed;
    }
    void DoneExecution()
    {
        executed = true;
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

vector<Process*> GetProcesses(int tm, vector<Process*>& AllProcesses)
{
    vector<Process*> InReadyQueue;
    for(int i = 0; i < AllProcesses.size(); i++)
    {
        Process* CurrentProc = AllProcesses[i];
        if(CurrentProc->GetArrivalTime() <= tm && !CurrentProc->IsExecuted())
            InReadyQueue.push_back(CurrentProc);
    }
    return InReadyQueue;
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
        int arrival_time, burst_time, p;
        cout << "Enter the Arrival Time of Process " << i+1 <<" : ";
        cin >> arrival_time;
        cout << "Enter the Burst Time of Process " << i+1 << " : ";
        cin >> burst_time;
        cout << "Enter the Priority of Process " << i+1 << " : ";
        cin >> p;
        CurrentProc->SetProcessId(i+1);
        CurrentProc->SetArrivalTime(arrival_time);
        CurrentProc->SetBurstTime(burst_time);
        CurrentProc->SetPriority(p);
        Processes.push_back(CurrentProc);
    }
    cout << "\n\npid\tp\tat\tbt\tct\ttat\twt" << endl;
    int CurrentTime = 0, ProcessesExecuted = 0;
    while(ProcessesExecuted < n)
    {
        // Checking and returning processes in Ready Queue at Current Instance of Time and yet to be Executed
        vector<Process*> InReadyQueue = GetProcesses(CurrentTime, Processes);
        if(InReadyQueue.size() == 0)
        {
            CurrentTime++;
            continue;
        }
        // Sorting Processes in Descending Order of their Priority
        // If the Priority for more than 1 Processes came out to be same then one that arrived in ready queue first will be scheduled first
        sort(InReadyQueue.begin(), InReadyQueue.end(), [](Process* P1, Process* P2){
            int priority1 = P1->GetPriority();
            int priority2 = P2->GetPriority();
            return (priority1 > priority2 || (priority1 == priority2 && P1->GetArrivalTime() <= P2->GetArrivalTime()));
        });

        // Now out of all the Processes in the Ready Queue, only first or the one with Highest priority will get the CPU
        // and it completes it's execution
        InReadyQueue[0]->SetCompletionTime(CurrentTime + InReadyQueue[0]->GetBurstTime());
        InReadyQueue[0]->UpdateAfterCompletion();
        ProcessesExecuted++;
        InReadyQueue[0]->DoneExecution();
        CurrentTime += InReadyQueue[0]->GetBurstTime();
        InReadyQueue[0]->display();
    }

    // Calculate Average Waiting Time and Average Turnaround Time
    CalculateAvgTime(Processes);
    return 0;
}

