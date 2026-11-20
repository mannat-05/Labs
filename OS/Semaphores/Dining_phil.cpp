#include <bits/stdc++.h>
#include <semaphore>
using namespace std;

const int N = 5;

binary_semaphore forks[N] = {
    binary_semaphore(1), binary_semaphore(1), binary_semaphore(1),
    binary_semaphore(1), binary_semaphore(1)
};

counting_semaphore<N - 1> room(N - 1);
mutex printMtx;

void displayState(int id, const string& state) {
    lock_guard<mutex> lk(printMtx);
    cout << "Philosopher " << id << " : " << state << "\n";
}

void philosopher(int id, int meals) {
    int leftFork = id;
    int rightFork = (id + 1) % N;

    for (int m = 1; m <= meals; m++) {
        displayState(id, "Thinking...");
        this_thread::sleep_for(chrono::milliseconds(100));

        room.acquire();
        forks[leftFork].acquire();
        forks[rightFork].acquire();

        displayState(id, "Eating (Meal " + to_string(m) + ") using forks " 
                         + to_string(leftFork) + " & " + to_string(rightFork));
        this_thread::sleep_for(chrono::milliseconds(150));

        forks[rightFork].release();
        forks[leftFork].release();
        room.release();
    }
    displayState(id, "Finished all meals.");
}

int main() {
    cout << "======================================================\n";
    cout << "          Dining Philosophers (Deadlock-Free)\n";
    cout << "======================================================\n";

    vector<thread> philosophers;
    for (int i = 0; i < N; i++) {
        philosophers.emplace_back(philosopher, i, 2);
    }

    for (auto& t : philosophers) t.join();

    return 0;
}