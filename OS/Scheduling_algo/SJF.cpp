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

    Process (int pid, int at, int bt, int pri = 0)
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

void SJF(vector<Process>& processes) // Must be pass-by-reference
{
    int n = processes.size();
    int completed = 0;
    int time = 0;
    vector<bool> visited(n, false);
    
    while (completed != n)
    {
        int idx = -1;
        int min_burst = INT_MAX;

        for (int i=0; i<n; i++)
        {
            if (processes[i].at <= time && !visited[i]) //If process has arrived and not served
            {
                if (processes[i].bt < min_burst)
                {
                    min_burst = processes[i].bt;
                    idx = i;
                }
                else if (processes[i].bt == min_burst)
                {
                    if (idx == -1 || processes[i].at < processes[idx].at)
                    {
                        idx = i;
                    }
                }
            }
        }

        if (idx != -1)
        {
            if (time < processes[idx].at) 
            {
                time = processes[idx].at;
            }
            processes[idx].ct = time+processes[idx].bt;
            processes[idx].tat = processes[idx].ct - processes[idx].at;
            processes[idx].wt = processes[idx].tat - processes[idx].bt;

            visited[idx] = true;
            completed++;
            time = processes[idx].ct;
        }
        else
        {
            time++; 
        }
    }
    displayMetrics("SJF", processes);
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
    SJF(processes);
    return 0;
}