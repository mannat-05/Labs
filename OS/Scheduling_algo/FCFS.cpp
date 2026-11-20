#include <bits/stdc++.h>
using namespace std;

struct Process
{
    int id;
    int at;
    int bt;
    int rt;
    int tat=0;
    int ct=0;
    int wt=0;
    int pri;

    Process (int pid, int at, int bt, int pri=0)    //Constructor to initialize
    : id(pid), at(at), bt(bt), rt(bt), pri(pri){}
};

void displayMetrics(const string& algoName, vector<Process>& processes) {
    double totalTAT = 0, totalWT = 0;
    
    // Sort by Process ID for clean presentation
    sort(processes.begin(), processes.end(), [](const Process& a, const Process& b) {
        return a.id < b.id;
    });

    cout << "\n=============================================\n";
    cout << "      " << algoName << "\n";
    cout << "=============================================\n";
    cout << "PID\tAT\tBT\tPrio\tCT\tTAT\tWT\n";
    cout << "---------------------------------------------\n";
    
    for (const auto& p : processes) 
    {
        totalTAT += p.tat;
        totalWT += p.wt;
        cout<< p.id << "\t" 
            << p.at << "\t" 
            << p.bt << "\t" 
            << p.pri << "\t" 
            << p.ct << "\t" 
            << p.tat << "\t" 
            << p.wt << "\n";
    }
    
    cout << fixed << setprecision(2);
    cout << "---------------------------------------------\n";
    cout << "Average Turnaround Time: " << totalTAT / processes.size() << "\n";
    cout << "Average Waiting Time:    " << totalWT / processes.size() << "\n";
}

void fcfs(vector<Process> processes)
{
    sort(processes.begin(), processes.end(), [](const Process& a, const Process& b)
    {
        return a.at < b.at;
    });

    int time=0;
    for (auto &p: processes)
    {
        if (time < p.at)
            time=p.at;
        p.ct=time+p.bt;
        p.tat=p.ct-p.at;
        p.wt=p.tat-p.bt;
        time=p.ct;
    }
    displayMetrics("FCFS", processes);
}


int main()
{
    vector<Process> processes = {
        Process(1, 0, 5, 2),
        Process(2, 1, 3, 1),
        Process(3, 2, 8, 4),
        Process(4, 3, 6, 3)
    };

    fcfs(processes);
    return 0;
}
