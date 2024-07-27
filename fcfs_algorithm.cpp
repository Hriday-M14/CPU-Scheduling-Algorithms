// FCFS CPU Scheduling Algorithm
#include <bits/stdc++.h>
using namespace std;

class Process{
    int process_id;
    int burst_time;
    int arrival_time;
    int turnaround_time;
    int completion_time;
    int waiting_time;

    public:
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

    void UpdateAfterCompletion()
    {
        turnaround_time = completion_time - arrival_time;
        waiting_time = turnaround_time - burst_time;
    }
    void display()
    {
        cout << process_id << "\t" << arrival_time << "\t" << burst_time << "\t" << completion_time << "\t" << turnaround_time << "\t" << waiting_time << endl;
    }
};

void CalculateAvgTime(vector<Process>& Processes)
{
    double AvgWaitingTime = 0.0, AvgTurnaroundTime = 0.0;
    for(int i = 0; i < Processes.size(); i++)
    {
        AvgWaitingTime += Processes[i].GetWaitingTime();
        AvgTurnaroundTime += Processes[i].GetTurnaroundTime();
    }
    cout << "Average Waiting Time: " << AvgWaitingTime / Processes.size() << endl;
    cout << "Average Turnaround Time: " << AvgTurnaroundTime / Processes.size() << endl;
}

int main()
{
    int n;
    cout << "Enter the Number of Processes to consider: ";
    cin >> n;
    vector<Process> Processes(n);
    for(int i = 0; i < n; i++)
    {
        int arrival_time, burst_time;
        cout << "Enter the Arrival Time of Process " << i+1 <<" : ";
        cin >> arrival_time;
        cout << "Enter the Burst Time of Process " << i+1 << " : ";
        cin >> burst_time;
        Processes[i].SetProcessId(i+1);
        Processes[i].SetArrivalTime(arrival_time);
        Processes[i].SetBurstTime(burst_time);
    }
    
    // Sort the Processes in the Ascending order in which they arrive in Ready Queue
    sort(Processes.begin(), Processes.end(), [](Process P1, Process P2){
        return P1.GetArrivalTime() < P2.GetArrivalTime();
    });
    // Now After Sorting Processes will get CPU in this order only

    cout << "\n\npid\tat\tbt\tct\ttat\twt" << endl;

    // Calculate the Completion Time of Each Process and then Update the Turnaround Time and Waiting Time of that Process
    Processes[0].SetCompletionTime(Processes[0].GetArrivalTime() + Processes[0].GetBurstTime());
    Processes[0].UpdateAfterCompletion();
    Processes[0].display();

    for(int i = 1; i < n; i++)
    {
        Process& CurrentProcess = Processes[i];
        if(CurrentProcess.GetArrivalTime() < Processes[i-1].GetCompletionTime())
        {
            CurrentProcess.SetCompletionTime(Processes[i-1].GetCompletionTime() + CurrentProcess.GetBurstTime());
        }
        else{
            CurrentProcess.SetCompletionTime(CurrentProcess.GetArrivalTime() + CurrentProcess.GetBurstTime());
        }
        CurrentProcess.UpdateAfterCompletion();
        CurrentProcess.display();
    }

    // Calculate Average Waiting Time and Average Turnaround Time
    CalculateAvgTime(Processes);
    return 0;
}

