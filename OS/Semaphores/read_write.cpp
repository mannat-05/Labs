#include <bits/stdc++.h>
#include <semaphore>
using namespace std;

binary_semaphore rwMutex(1);
binary_semaphore rcMutex(1);
int readCount = 0;
int sharedData = 10;
mutex printMtx;

void displayMetrics(const string& actor, int id, const string& action, int val) {
    lock_guard<mutex> lk(printMtx);
    cout << actor << " " << id << "\t| " << action 
         << "\t| Data: " << val << " | Active Readers: " << readCount << "\n";
}

void reader(int id) {
    rcMutex.acquire();
    readCount++;
    if (readCount == 1) {
        rwMutex.acquire();
    }
    rcMutex.release();

    displayMetrics("Reader", id, "Reading ", sharedData);
    this_thread::sleep_for(chrono::milliseconds(150));

    rcMutex.acquire();
    readCount--;
    if (readCount == 0) {
        rwMutex.release();
    }
    rcMutex.release();
}

void writer(int id, int newVal) {
    rwMutex.acquire();

    sharedData = newVal;
    displayMetrics("Writer", id, "Writing ", sharedData);
    this_thread::sleep_for(chrono::milliseconds(200));

    rwMutex.release();
}

int main() {
    cout << "==============================================================\n";
    cout << "          Readers-Writers Problem (Semaphores)\n";
    cout << "==============================================================\n";

    vector<thread> workers;
    workers.emplace_back(reader, 1);
    workers.emplace_back(reader, 2);
    workers.emplace_back(writer, 1, 42);
    workers.emplace_back(reader, 3);
    workers.emplace_back(writer, 2, 99);
    workers.emplace_back(reader, 4);

    for (auto& t : workers) t.join();

    return 0;
}