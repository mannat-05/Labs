#include <bits/stdc++.h>
using namespace std;

struct BuddyBlock
{
    int startAdd;
    int size;
    int allocpid=-1;   //free
};

struct Process
{
    int id;
    int size;
    int startAdd=-1;
    int alloc=0;    //unallocated
};

int nextpow(int n)
{
    int p=1;
    while (p<n) p <<= 1;
    return p;
}

void displayMetrics(const string& algoName, const vector<Process>& processes, const vector<BuddyBlock>& memory) 
{
    cout << "\n==================================================================\n";
    cout << "                  " << algoName << " Allocation\n";
    cout << "==================================================================\n";
    cout << "PID\tReq Size\tAlloc Block\tStart Addr\tStatus\n";
    cout << "------------------------------------------------------------------\n";

    for (const auto& p : processes) 
    {
        cout << p.id << "\t" << p.size << "\t\t";
        if (p.startAdd != -1) {
            cout << p.alloc << "\t\t" << p.startAdd << "\t\tAllocated\n";
        } else {
            cout << "-\t\t-\t\tNot Allocated\n";
        }
    }

    cout << "\n--- Final Memory Partitions ---\n";
    for (const auto& b : memory) {
        cout << "[" << b.startAdd << " - " << (b.startAdd + b.size - 1) << "] : "
             << b.size << " KB -> "
             << (b.allocpid == -1 ? "FREE" : "P" + to_string(b.allocpid)) << "\n";
    }
}

void buddy(int total, vector<Process> processes)
{
    vector<BuddyBlock> mem={{0, nextpow(total), -1}};
    for (auto &p: processes)
    {
        int req=nextpow(p.size);
        int best=-1;
        int min_size=INT_MAX;

        for (int i=0; i<mem.size(); i++)
        {
            if (mem[i].allocpid == -1 && mem[i].size >= req) 
            {
                if (mem[i].size < min_size)
                {
                    min_size=mem[i].size;
                    best=i;
                }
            }
        }
        if (best != -1) //Finding the best fit
        {
            while (mem[best].size > req)
            {
                int half=mem[best].size/2;
                int add=mem[best].startAdd;
                mem[best].size=half;
                mem.insert(mem.begin(), best+1, {add+half, half, -1});  //Creating a new block
            }
            mem[best].allocpid=p.id;
            p.alloc=req;
            p.startAdd=mem[best].startAdd;
        }
        displayMetrics("Buddy System (" + to_string(total) + " KB Pool)", processes, mem);
    }
}

int main()
{
    int totalMemory = 2048;
    vector<Process> processes = {
        {1, 212}, {2, 417}, {3, 112}, {4, 426}
    };

    buddy(totalMemory, processes);

    return 0;
}