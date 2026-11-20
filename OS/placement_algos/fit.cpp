#include <bits/stdc++.h>
using namespace std;

struct Block
{
    int id;
    int size;
    int pid=-1; //free
};

struct Process
{
    int id;
    int size;
    int bid=-1; //unallocated
};

void displayMetrics(const string& algoName, const vector<Process>& processes, const vector<Block>& blocks) {
    cout << "\n======================================================\n";
    cout << "          " << algoName << " Allocation\n";
    cout << "======================================================\n";
    cout << "PID\tProcess Size\tBlock ID\tBlock Size\n";
    cout << "------------------------------------------------------\n";

    for (const auto& p : processes) {
        cout << p.id << "\t" << p.size << "\t\t";
        if (p.bid != -1) {
            int bSize = 0;
            for (const auto& b : blocks) {
                if (b.id == p.bid) {
                    bSize = b.size;
                    break;
                }
            }
            cout << p.bid << "\t\t" << bSize << "\n";
        } else {
            cout << "Not Allocated\t-\n";
        }
    }
}

void firstFit(vector<Block> blocks, vector<Process> processes)
{
    for (auto &p:processes)
    {
        for (auto &b: blocks)
        {
            if (b.pid == -1 && b.size >= p.size)
            {
                b.pid=p.id;
                p.bid=b.id;
                break;
            }
        }
    }
    displayMetrics("First fit", processes, blocks);
}

void bestFit(vector<Block> blocks, vector<Process> processes)
{
    for (auto &p:processes)
    {
        int best=-1;
        for (int i=0; i<blocks.size(); i++)
        {
            if (blocks[i].pid == -1 && blocks[i].size >= p.size)
            {
                if (best == -1 || blocks[i].size <= blocks[best].size)  best=i;
            }
        }
        if (best != -1)
        {
            blocks[best].pid=p.id;
            p.bid=blocks[best].id;
        }
    }
    displayMetrics("Best fit", processes, blocks);
}

void worstFit(vector<Block> blocks, vector<Process> processes)
{
    for (auto &p:processes)
    {
        int worst=-1;
        for (int i=0; i<blocks.size(); i++)
        {
            if (blocks[i].pid == -1 && blocks[i].size >= p.size)
            {
                if (worst == -1 || blocks[i].size > blocks[worst].size)  worst=i;
            }
        }
        if (worst != -1)
        {
            blocks[worst].pid=p.id;
            p.bid=blocks[worst].id;
        }
    }
    displayMetrics("Worst fit", processes, blocks);
}

void nextFit(vector<Block> blocks, vector<Process> processes)
{
    int n=blocks.size();
    int last=0;
    for (auto &p:processes)
    {
        int visited=0;
        while (visited < n)
        {
            int i=(last+visited)%n; //Assuming circular array
            if (blocks[i].pid == -1 && blocks[i].size >= p.size)
            {
                blocks[i].pid=p.id;
                p.bid=blocks[i].id;
                last=i;
                break;
            }
            visited++;
        }
    }
    displayMetrics("Next fit", processes, blocks);
}

int main() 
{
    vector<Block> blocks = 
    {
        {1, 100}, {2, 500}, {3, 200}, {4, 300}, {5, 600}
    };

    vector<Process> processes = 
    {
        {1, 212}, {2, 417}, {3, 112}, {4, 426}
    };

    firstFit(blocks, processes);
    bestFit(blocks, processes);
    worstFit(blocks, processes);
    nextFit(blocks, processes);

    return 0;
}