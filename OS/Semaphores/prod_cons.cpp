#include <bits/stdc++.h>
#include <semaphore>
using namespace std;

const int BUFFER_SIZE = 3;
queue<int> buffer;

counting_semaphore<BUFFER_SIZE> emptySlots(BUFFER_SIZE);
counting_semaphore<BUFFER_SIZE> fullSlots(0);
binary_semaphore bufMutex(1);
mutex printMtx;

void displayState(const string& role, int id, int item, int currentSize) {
    lock_guard<mutex> lk(printMtx);
    cout << role << " " << id << " | Item: " << item 
         << " | Buffer Count: " << currentSize << "/" << BUFFER_SIZE << "\n";
}

void producer(int id, int itemsToProduce) {
    for (int i = 1; i <= itemsToProduce; i++) {
        int item = id * 100 + i;
        this_thread::sleep_for(chrono::milliseconds(100));

        emptySlots.acquire();
        bufMutex.acquire();

        buffer.push(item);
        displayState("Producer", id, item, buffer.size());

        bufMutex.release();
        fullSlots.release();
    }
}

void consumer(int id, int itemsToConsume) {
    for (int i = 1; i <= itemsToConsume; i++) {
        fullSlots.acquire();
        bufMutex.acquire();

        int item = buffer.front();
        buffer.pop();
        displayState("Consumer", id, item, buffer.size());

        bufMutex.release();
        emptySlots.release();

        this_thread::sleep_for(chrono::milliseconds(150));
    }
}

int main() {
    cout << "======================================================\n";
    cout << "          Producer-Consumer (Buffer Size = " << BUFFER_SIZE << ")\n";
    cout << "======================================================\n";

    thread p1(producer, 1, 4);
    thread p2(producer, 2, 4);
    thread c1(consumer, 1, 4);
    thread c2(consumer, 2, 4);

    p1.join(); p2.join();
    c1.join(); c2.join();

    return 0;
}