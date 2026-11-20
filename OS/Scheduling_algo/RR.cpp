#include <bits/stdc++.h>
using namespace std;

struct Process
{
    int id;
    int at;
    int bt;
    int rt;
    int ct = 0;
    int tat = 0;
    int wt = 0;
    int pri;

    Process (int pid, int at, int bt, int pri=0)
    : id(pid), at(at), bt(bt), rt(bt), pri(pri) {}
};

void displayMetrics(const string& algoName, const vector<Process>& processes) {
    double totalTAT = 0, totalWT = 0;
    
    // Create a local copy for sorting so we don't mutate original vector unexpectedly
    vector<Process> temp = processes;
    sort(temp.begin(), temp.end(), [](const Process& a, const Process& b) {
        return a.id < b.id;
    });

    cout << "\n=============================================\n";
    cout << "      " << algoName << "\n";
    cout << "=============================================\n";
    cout << "PID\tAT\tBT\tPrio\tCT\tTAT\tWT\n";
    cout << "---------------------------------------------\n";
    
    for (const auto& p : temp) 
    {
        totalTAT += p.tat;
        totalWT += p.wt;
        cout << p.id << "\t" 
             << p.at << "\t" 
             << p.bt << "\t" 
             << p.pri << "\t" 
             << p.ct << "\t" 
             << p.tat << "\t" 
             << p.wt << "\n";
    }
    
    cout << fixed << setprecision(2);
    cout << "---------------------------------------------\n";
    cout << "Average Turnaround Time: " << totalTAT / temp.size() << "\n";
    cout << "Average Waiting Time:    " << totalWT / temp.size() << "\n";
}

void RR(vector<Process>& processes, int quantum) // Must be pass-by-reference
{
    int n = processes.size();
    sort(processes.begin(), processes.end(), [](const Process &a, const Process &b)
    {
        return a.at < b.at;
    });

    vector<bool> visited(n, false); //Checks for element in queue
    queue<int> q;
    int completed = 0;
    //Initialisation
    int time = processes[0].at;
    q.push(0);
    visited[0]=true;
    
    while (!q.empty())
    {
        int idx=q.front();
        q.pop();

        int et=min(processes[idx].bt, quantum);
        processes[idx].rt-=et;
        time+=et;

        for (int i=0; i<n; i++)
        {
            if (processes[i].at <= time && !visited[i] && processes[i].rt > 0)
            {
                q.push(i);
                visited[i]=true;
            }
        }

        if (processes[idx].rt > 0)  //Current process re-enqueue
        {
            q.push(idx);
        }
        else
        {
            processes[idx].ct = time;
            processes[idx].tat = processes[idx].ct - processes[idx].at;
            processes[idx].wt = processes[idx].tat - processes[idx].bt;
            completed++;
        }

        if (q.empty() && completed < n)
        {
            for (int i = 0; i < n; i++)
            {
                if (processes[i].rt > 0)
                {
                    time = max(time, processes[i].at);
                    q.push(i);
                    visited[i] = true;
                    break;
                }
            }
        }
    }
    displayMetrics("Round-robin", processes);
}

int main()
{
    vector<Process> processes =
    {
        Process(1, 0, 5, 2),
        Process(2, 1, 3, 1),
        Process(3, 2, 8, 4),
        Process(4, 3, 6, 3)
    };
    RR(processes, 5);
    return 0;
}