#include <iostream>
#include <thread>
#include <chrono>

using namespace std::chrono_literals;

void p1_locking();
void p1_lockFree();

void problem1() {
  std::cout << "-- locking --" << std::endl;
  void p1_locking();
  p1_locking();

  std::cout << std::endl << std::endl;
  std::this_thread::sleep_for(2s);

  std::cout << "-- lock free --" << std::endl;
  p1_lockFree();
}

int main() { 
    std::cout << "Problem 1" << std::endl;
    problem1(); 
}