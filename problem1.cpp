#include <atomic>
#include <chrono>
#include <condition_variable>
#include <iostream>
#include <mutex>
#include <string>
#include <thread>

using namespace std::chrono_literals;

void print(const std::string &value) { std::cout << value; }

std::mutex g_mux;
std::condition_variable g_cv;
int g_counter = 0;

void printFirst() {
  std::this_thread::sleep_for(.5s);

  std::unique_lock<std::mutex> lock(g_mux);
  g_cv.wait(lock, []() { return g_counter == 0; });
  print("first\n");
  g_counter++;
  g_cv.notify_all();
}

void printSecond() {
  std::this_thread::sleep_for(.25s);

  std::unique_lock<std::mutex> lock(g_mux);
  g_cv.wait(lock, []() { return g_counter == 1; });
  print("second\n");
  g_counter++;
  g_cv.notify_all();
}

void printThird() {
  std::unique_lock<std::mutex> lock(g_mux);
  g_cv.wait(lock, []() { return g_counter == 2; });
  print("third\n");
  g_counter++;
  g_cv.notify_all();
}

void locking() {
  std::thread first{printFirst};
  std::thread second{printSecond};
  std::thread third{printThird};

  first.join();
  second.join();
  third.join();
}

std::atomic_int g_atomicCounter(0);

void printFirst_lockfree() {
  std::this_thread::sleep_for(.5s);

  while (g_atomicCounter != 0) {
  }
  print("first\n");
  ++g_atomicCounter;
}

void printSecond_lockfree() {
  std::this_thread::sleep_for(.25s);
  while (g_atomicCounter != 1) {
  }
  print("second\n");
  ++g_atomicCounter;
}

void printThird_lockfree() {
  while (g_atomicCounter != 2) {
  }
  print("third\n");
}

void lockFree() {
  std::thread first{printFirst_lockfree};
  std::thread second{printSecond_lockfree};
  std::thread third{printThird_lockfree};

  first.join();
  second.join();
  third.join();
}

void problem1() {
  std::cout << "-- locking --" << std::endl;
  locking();

  std::cout << std::endl << std::endl;
  std::this_thread::sleep_for(2s);

  std::cout << "-- lock free --" << std::endl;
  lockFree();
}