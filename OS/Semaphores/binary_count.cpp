#include <bits/stdc++.h>
#include <semaphore>
using namespace std;

binary_semaphore binSem(1);
counting_semaphore<3> countSem(3);
mutex printMtx;

void logEvent(const string& msg) {
    lock_guard<mutex> lk(printMtx);
    cout << msg << "\n";
}

void binaryWorker(int id) {
    logEvent("[Binary] Thread " + to_string(id) + " waiting...");
    binSem.acquire();
    logEvent("[Binary] Thread " + to_string(id) + " ENTERED critical section.");
    this_thread::sleep_for(chrono::milliseconds(200));
    logEvent("[Binary] Thread " + to_string(id) + " LEAVING critical section.");
    binSem.release();
}

void countingWorker(int id) {
    logEvent("[Counting] Thread " + to_string(id) + " requesting resource...");
    countSem.acquire();
    logEvent("[Counting] Thread " + to_string(id) + " ACQUIRED resource slot.");
    this_thread::sleep_for(chrono::milliseconds(300));
    logEvent("[Counting] Thread " + to_string(id) + " RELEASING resource slot.");
    countSem.release();
}

int main() {
    cout << "=== 1. Binary Semaphore (Mutex Behavior) ===\n";
    vector<thread> binThreads;
    for (int i = 1; i <= 3; i++) binThreads.emplace_back(binaryWorker, i);
    for (auto& t : binThreads) t.join();

    cout << "\n=== 2. Counting Semaphore (Max 3 Concurrent) ===\n";
    vector<thread> countThreads;
    for (int i = 1; i <= 5; i++) countThreads.emplace_back(countingWorker, i);
    for (auto& t : countThreads) t.join();

    return 0;
}