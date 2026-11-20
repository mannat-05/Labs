#include <bits/stdc++.h>
using namespace std;

struct PageStep {
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

    double total = trace.size();
    cout << fixed << setprecision(2);
    cout << "------------------------------------------------------\n";
    cout << "Total Page Faults: " << faults << " | Total Hits: " << hits << "\n";
    cout << "Fault Ratio: " << (faults / total) * 100.0 << "% | Hit Ratio: " << (hits / total) * 100.0 << "%\n";
}

void LRU(const vector<int>& refs, int capacity) {
    vector<int> frames;
    unordered_map<int, int> lastUsed; // page -> most recent index
    vector<PageStep> trace;

    for (int i = 0; i < (int)refs.size(); i++) {
        int page = refs[i];
        auto it = find(frames.begin(), frames.end(), page);

        if (it != frames.end()) {
            lastUsed[page] = i;
            trace.push_back({page, frames, true});
        } else {
            if ((int)frames.size() < capacity) {
                frames.push_back(page);
            } else {
                int lruIdx = 0;
                for (int j = 1; j < (int)frames.size(); j++) {
                    if (lastUsed[frames[j]] < lastUsed[frames[lruIdx]]) {
                        lruIdx = j;
                    }
                }
                frames[lruIdx] = page;
            }
            lastUsed[page] = i;
            trace.push_back({page, frames, false});
        }
    }
    displayMetrics("LRU (Least Recently Used)", trace, capacity);
}

int main() {
    vector<int> pageReferences = {7, 0, 1, 2, 0, 3, 0, 4, 2, 3, 0, 3, 2, 1, 2, 0, 1, 7, 0, 1};
    int frameCapacity = 3;

    LRU(pageReferences, frameCapacity);

    return 0;
}