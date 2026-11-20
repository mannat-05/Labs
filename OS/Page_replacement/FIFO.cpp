#include <bits/stdc++.h>
using namespace std;

struct PageStep
{
    int page;
    vector<int> frames;
    bool isHit;
};

void displayMetrics(const string& algoName, const vector<PageStep>& trace, int capacity) 
{
    int hits = 0, faults = 0;

    cout << "\n======================================================\n";
    cout << "          " << algoName << " Page Replacement\n";
    cout << "======================================================\n";
    cout << "Ref\tFrames\t\t\tStatus\n";
    cout << "------------------------------------------------------\n";

    for (const auto& step : trace) {
        if (step.isHit) hits++;
        else faults++;

        cout << step.page << "\t[ ";
        for (int i = 0; i < capacity; i++) {
            if (i < (int)step.frames.size()) cout << step.frames[i] << " ";
            else cout << "- ";
        }
        cout << "]\t\t" << (step.isHit ? "HIT" : "FAULT") << "\n";
    }
}

void FIFO(const vector<int>& refs, int capacity) 
{
    vector<int> frames;
    queue<int> fifoOrder;
    vector<PageStep> trace;

    for (int page : refs) 
    {
        auto it = find(frames.begin(), frames.end(), page);
        if (it != frames.end()) 
        {
            trace.push_back({page, frames, true});
        } 
        else 
        {
            if ((int)frames.size() < capacity) 
            {
                frames.push_back(page);
                fifoOrder.push(page);
            }
            else 
            {
                int victim = fifoOrder.front();
                fifoOrder.pop();
                auto vit = find(frames.begin(), frames.end(), victim);
                *vit = page;
                fifoOrder.push(page);
            }
            trace.push_back({page, frames, false});
        }
    }
    displayMetrics("FIFO", trace, capacity);
}

int main()
{
    vector<int> pageReferences = {7, 0, 1, 2, 0, 3, 0, 4, 2, 3, 0, 3, 2, 1, 2, 0, 1, 7, 0, 1};  //Reference String
    int frameCapacity = 3;

    FIFO(pageReferences, frameCapacity);

    return 0;
}