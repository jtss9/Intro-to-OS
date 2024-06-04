#include <iostream>
#include <thread>
#include <mutex>

using namespace std;

int cnt=0;
void count(int index) {
  static mutex io_mutex;
  int num = 1000000;
  while(cnt!=index) {}
  io_mutex.lock();
  while (num--) {}
  {
    cout << "I'm thread " << index << ", local count: 1000000\n";
  }
  io_mutex.unlock();
  cnt++;
}

int main(void) {
  thread t[100];
  for (int i = 0; i < 100; i++)
    t[i] = thread(count, i);

  for (int i = 0; i < 100; i++)
    t[i].join();
}
