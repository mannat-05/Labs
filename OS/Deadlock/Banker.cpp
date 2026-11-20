#include <bits/stdc++.h>
using namespace std;

struct Process {
    int id;
    vector<int> maxNeed;
    vector<int> allocated;
    vector<int> need;
};

void displayMetrics(const vector<Process>& processes, const vector<int>& available, bool isSafe, const vector<int>& safeSeq) {
    int m = available.size();
    cout << "\n==================================================================\n";
    cout << "               Banker's Algorithm (Deadlock Avoidance)\n";
    cout << "==================================================================\n";
    cout << "PID\tAllocation\tMax\t\tNeed\n";
    cout << "------------------------------------------------------------------\n";

    for (const auto& p : processes) {
        cout << "P" << p.id << "\t";
        for (int j = 0; j < m; j++) cout << p.allocated[j] << " ";
        cout << "\t\t";
        for (int j = 0; j < m; j++) cout << p.maxNeed[j] << " ";
        cout << "\t\t";
        for (int j = 0; j < m; j++) cout << p.need[j] << " ";
        cout << "\n";
    }

    cout << "------------------------------------------------------------------\n";
    cout << "Available Resources: [ ";
    for (int x : available) cout << x << " ";
    cout << "]\n";

    if (isSafe) {
        cout << "System State: SAFE\nSafe Sequence: ";
        for (int i = 0; i < (int)safeSeq.size(); i++) {
            cout << "P" << safeSeq[i] << (i + 1 < (int)safeSeq.size() ? " -> " : "");
        }
        cout << "\n";
    } else {
        cout << "System State: UNSAFE (Potential Deadlock)\n";
    }
}

void runBankers(vector<Process> processes, vector<int> available) {
    int n = processes.size();
    int m = available.size();

    for (auto& p : processes) {
        p.need.resize(m);
        for (int j = 0; j < m; j++) {
            p.need[j] = p.maxNeed[j] - p.allocated[j];
        }
    }

    vector<int> work = available;
    vector<bool> finish(n, false);
    vector<int> safeSeq;

    int completed = 0;
    while (completed < n) {
        bool found = false;
        for (int i = 0; i < n; i++) {
            if (!finish[i]) {
                bool canExecute = true;
                for (int j = 0; j < m; j++) {
                    if (processes[i].need[j] > work[j]) {
                        canExecute = false;
                        break;
                    }
                }
                if (canExecute) {
                    for (int j = 0; j < m; j++) {
                        work[j] += processes[i].allocated[j];
                    }
                    safeSeq.push_back(processes[i].id);
                    finish[i] = true;
                    completed++;
                    found = true;
                }
            }
        }
        if (!found) break;
    }

    displayMetrics(processes, available, completed == n, safeSeq);
}

int main() {
    vector<int> available = {3, 3, 2};

    vector<Process> processes = {
        {0, {7, 5, 3}, {0, 1, 0}, {}},
        {1, {3, 2, 2}, {2, 0, 0}, {}},
        {2, {9, 0, 2}, {3, 0, 2}, {}},
        {3, {2, 2, 2}, {2, 1, 1}, {}},
        {4, {4, 3, 3}, {0, 0, 2}, {}}
    };

    runBankers(processes, available);

    return 0;
}

