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

void p1_locking() {
  std::thread first{printFirst};
  std::thread second{printSecond};
  std::thread third{printThird};

  first.join();
  second.join();
  third.join();
}
