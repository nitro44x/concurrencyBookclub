#include <atomic>
#include <chrono>
#include <condition_variable>
#include <iostream>
#include <mutex>
#include <string>
#include <thread>

using namespace std::chrono_literals;

void print(const std::string &value);

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

void p1_lockFree() {
  std::thread first{printFirst_lockfree};
  std::thread second{printSecond_lockfree};
  std::thread third{printThird_lockfree};

  first.join();
  second.join();
  third.join();
}