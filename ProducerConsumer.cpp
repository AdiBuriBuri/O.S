#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <cstdlib>
#include <ctime>
using namespace std;

#define BUFFER_SIZE 5
#define MAX_ITEMS 20

int buffer[BUFFER_SIZE];
int in = 0, out = 0;
int item_count = 0;

mutex mtx;
condition_variable not_full;
condition_variable not_empty;
    
void producer()
{
    while (true)
    {
        int item = rand() % 100;

        unique_lock<mutex> lock(mtx);

        while (item_count == BUFFER_SIZE)
            not_full.wait(lock);

        buffer[in] = item;
        cout << "Produced: " << item << " at " << in << endl;

        in = (in + 1) % BUFFER_SIZE;
        item_count++;
        not_empty.notify_one();
        lock.unlock();
        this_thread::sleep_for(chrono::seconds(1));
    }
}

void consumer()
{
    while (true)
    {
        unique_lock<mutex> lock(mtx);

        while (item_count == 0)
        {
            not_empty.wait(lock);
        }

        int item = buffer[out];
        cout << "Consumed: " << item << " at " << out << endl;

        out = (out + 1) % BUFFER_SIZE;
        item_count--;

        not_full.notify_one();
        lock.unlock();

        this_thread::sleep_for(chrono::seconds(1));
    }
}

int main()
{
    //srand(time(0));
    thread prod(producer);
    thread cons(consumer);

    prod.join();
    cons.join();
}
