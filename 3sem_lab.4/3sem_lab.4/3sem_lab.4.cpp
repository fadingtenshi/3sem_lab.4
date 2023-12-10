#include <iostream>
#include <vector>
#include <thread>
#include <mutex> // mutex
#include <semaphore> // semaphore
#include <barrier> // barrier
#include <atomic> // spin
#include <condition_variable> // monitor
#include <chrono>
#define lox long long

int race_distance = 5;

std::vector<lox> times;

std::mutex race_mutex;

void raceMutex(char thread_id) {
    auto start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < race_distance; ++i) {
        std::lock_guard<std::mutex> lock(race_mutex);
        for (int j = 0; j < i; ++j)
            std::cout << ' ';
        std::cout << thread_id << std::endl;
        //std::this_thread::sleep_for(std::chrono::microseconds(700));
    }
    auto finish = std::chrono::high_resolution_clock::now();
    std::lock_guard<std::mutex> lock(race_mutex);
    std::cout << std::endl << thread_id << ": " << std::chrono::duration<double>(finish - start).count() << " seconds" << std::endl;
}
void start_race(void race_func(char), const std::string& name) {

    int threads_num;
    std::vector<std::thread> threads;

    std::cout << "Enter number of threads: ";
    std::cin >> threads_num;
    for (short i = 0; i < threads_num; i++) {

        threads.emplace_back(race_func, 'A' + i);

    }

    for (auto& thread : threads) {

        thread.join();

    }

}

int main()
{

    start_race(raceMutex, "Mutex");

    return 0;
}