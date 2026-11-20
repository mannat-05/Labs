#include <bits/stdc++.h>
using namespace std;

struct Edge {
    string from;
    string to;
};

int getNodeId(const string& name, int numProcesses) {
    int idx = stoi(name.substr(1));
    return (name[0] == 'P') ? idx : numProcesses + idx;
}

string getNodeName(int id, int numProcesses) {
    if (id < numProcesses) return "P" + to_string(id);
    return "R" + to_string(id - numProcesses);
}

bool dfsCycle(int u, const vector<vector<int>>& adj, vector<int>& state, vector<int>& parent, vector<int>& cycleNodes) {
    state[u] = 1;
    for (int v : adj[u]) {
        if (state[v] == 1) {
            int curr = u;
            cycleNodes.push_back(v);
            while (curr != v) {
                cycleNodes.push_back(curr);
                curr = parent[curr];
            }
            cycleNodes.push_back(v);
            reverse(cycleNodes.begin(), cycleNodes.end());
            return true;
        }
        if (state[v] == 0) {
            parent[v] = u;
            if (dfsCycle(v, adj, state, parent, cycleNodes)) return true;
        }
    }
    state[u] = 2;
    return false;
}

void displayMetrics(int numProcesses, int numResources, const vector<Edge>& edges, bool hasDeadlock, const vector<int>& cycleNodes) {
    cout << "\n======================================================\n";
    cout << "      Resource Allocation Graph (Deadlock Detection)\n";
    cout << "======================================================\n";
    cout << "Processes: " << numProcesses << " | Single-Instance Resources: " << numResources << "\n";
    cout << "------------------------------------------------------\n";
    cout << "Directed Edges:\n";
    for (const auto& e : edges) {
        string type = (e.from[0] == 'P') ? "(Request Edge)" : "(Assignment Edge)";
        cout << "  " << e.from << " -> " << e.to << "  " << type << "\n";
    }
    cout << "------------------------------------------------------\n";

    if (hasDeadlock) {
        cout << "Status: DEADLOCK DETECTED (Cycle Found)\nCycle Path: ";
        for (int i = 0; i < (int)cycleNodes.size(); i++) {
            cout << getNodeName(cycleNodes[i], numProcesses) << (i + 1 < (int)cycleNodes.size() ? " -> " : "");
        }
        cout << "\n";
    } else {
        cout << "Status: NO DEADLOCK (Graph is Acyclic)\n";
    }
}

void runRAG(int numProcesses, int numResources, const vector<Edge>& edges) {
    int totalNodes = numProcesses + numResources;
    vector<vector<int>> adj(totalNodes);

    for (const auto& e : edges) {
        int u = getNodeId(e.from, numProcesses);
        int v = getNodeId(e.to, numProcesses);
        adj[u].push_back(v);
    }

    vector<int> state(totalNodes, 0);
    vector<int> parent(totalNodes, -1);
    vector<int> cycleNodes;
    bool hasDeadlock = false;

    for (int i = 0; i < totalNodes; i++) {
        if (state[i] == 0) {
            if (dfsCycle(i, adj, state, parent, cycleNodes)) {
                hasDeadlock = true;
                break;
            }
        }
    }

    displayMetrics(numProcesses, numResources, edges, hasDeadlock, cycleNodes);
}

int main() {
    int numProcesses = 3;
    int numResources = 3;

    vector<Edge> edges = {
        {"R0", "P0"},
        {"P0", "R1"},
        {"R1", "P1"},
        {"P1", "R2"},
        {"R2", "P2"},
        {"P2", "R0"}
    };

    runRAG(numProcesses, numResources, edges);

    return 0;
}
