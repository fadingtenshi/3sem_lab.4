#include <iostream>
#include <string>
#include <vector>
#include <thread>
#include <mutex>
#include <semaphore>
#include <barrier>
#include <atomic>
#include <condition_variable>
#include <chrono>
#include <random>
#include <cmath>

#define ll long long
#define uf32 uint_fast32_t
#define uf16 uint_fast16_t
#define uf8 uint_fast8_t
#define HANDLE_INVALID_INPUT(){ \
    std::cin.clear(); \
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); \
    std::cout << "Invalid input" << std::endl; \
    } \

namespace task_1 {
    int race_distance = 5;

    std::vector<ll> times;

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
        std::cout << std::endl << thread_id << ": " << std::chrono::duration<double>(finish - start).count()
            << " seconds" << std::endl;
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
}

namespace task_2 {

    std::mutex mux;

    struct Car {

        std::string model = "";
        uf32 price = 0;
        uf32 mileage = 0;
        std::string body_type = "";
        uf16 year = 0;

    };

    std::ostream& operator<<(std::ostream& os, Car car) {

        os << "Model: " << car.model << std::endl;
        os << "Price: " << car.price << std::endl;
        os << "Mileage: " << car.mileage << std::endl;
        os << "Body type: " << car.body_type << std::endl;
        os << "Date of issue: " << car.year << std::endl;
        os << "--------------------" << std::endl;

        return os;

    }

    void randomCars(uf16 number_of_cars, std::vector<task_2::Car>& cars) {

        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<uint_fast32_t> price_dist(10000, 1000000);
        std::uniform_int_distribution<uint_fast32_t> mileage_dist(0, 100000);
        std::uniform_int_distribution<uint_fast16_t> year_dist(1967, 2023);
        std::uniform_int_distribution<uint_fast32_t> body_type_dist(0, 8);

        std::vector<std::string> body_type = { "Saloon", "Hatchback", "Estate",
            "Coupe", "Convertible", "SUV", "Pickup", "Crossover", "Van" };

        for (uf16 i = 0; i < number_of_cars; ++i) {

            Car car;
            car.model = "Model " + std::to_string(i + 1);
            car.price = price_dist(gen);
            car.mileage = mileage_dist(gen);
            car.year = year_dist(gen);
            car.body_type = body_type[body_type_dist(gen)];

            cars.push_back(car);

        }

    }

    void checker(uf16 start, uf16 end,
        std::vector<Car>& cars, std::vector<Car>& good_cars,
        uf32 min_price, uf32 max_price, uf32 max_mileage, uf16 min_year) {

        for (uf16 i = start; i < end && i < cars.size(); ++i) {

            if (cars[i].price > min_price && cars[i].price < max_price &&
                cars[i].mileage < max_mileage && cars[i].year > min_year) {

                std::lock_guard<std::mutex> lock(mux);
                good_cars.push_back(cars[i]);

            }

        }

    }

}

int main()
{
    uf8 task_number;
    uf16 threads_num, chunk, start = 0, end = 0;
    uf16 number_of_cars, min_year;
    uf32 min_price, max_price, max_mileage;
    std::vector<std::thread> threads;
    std::vector<task_2::Car> cars;
    std::vector<task_2::Car> good_cars;


    std::chrono::steady_clock::time_point start_time1;
    std::chrono::steady_clock::time_point start_time2;
    std::chrono::steady_clock::time_point finish_time1;
    std::chrono::steady_clock::time_point finish_time2;
    double single_threading;
    double multithreading;

    std::cout << "Enter task number (1 2 3): ";
    std::cin >> task_number;

    switch (task_number)
    {
    case '1':

        task_1::start_race(task_1::raceMutex, "Mutex");

        break;

    case '2':

        for (;;) {

            std::cout << "Enter number of threads: ";
            std::cin >> threads_num;
            if (std::cin.fail() || threads_num <= 0 || threads_num > 12) {
                HANDLE_INVALID_INPUT();
                continue;
            }

            std::cout << "Enter number of cars: ";
            std::cin >> number_of_cars;
            if (std::cin.fail()) {
                HANDLE_INVALID_INPUT();
                continue;
            }

            chunk = number_of_cars / threads_num;

            std::cout << "Enter min price (5000 - 1000000$): ";
            std::cin >> min_price;
            std::cout << "Enter max price (5000 - 1000000$): ";
            std::cin >> max_price;
            if (min_price > max_price) {

                HANDLE_INVALID_INPUT();
                continue;

            }

            std::cout << "Enter max milliage (0 - 100000 miles): ";
            std::cin >> max_mileage;
            if (max_mileage < 0 || max_mileage > 100000) {

                HANDLE_INVALID_INPUT();
                continue;

            }
            

            std::cout << "Enter min date of issue (1967 - 2023): ";
            std::cin >> min_year;
            if (min_year < 1967 || min_year > 2023) {

                HANDLE_INVALID_INPUT();
                continue;

            }

            break;

        }

        task_2::randomCars(number_of_cars, cars);

        start_time1 = std::chrono::high_resolution_clock::now();

        task_2::checker(start, end, cars,
            good_cars, min_price, max_price, max_mileage, min_year);

        finish_time1 = std::chrono::high_resolution_clock::now();

        single_threading = std::chrono::duration<double>
            (finish_time1 - start_time1).count();

        start_time2 = std::chrono::high_resolution_clock::now();

        for (uf16 i = 0; i < threads_num; ++i) {

            end = start + chunk;
            threads.emplace_back(task_2::checker, start, end, std::ref(cars),
                std::ref(good_cars), min_price, max_price, max_mileage, min_year);
            start = end;

        }

        for (auto& thread : threads) {

            thread.join();

        }

        finish_time2 = std::chrono::high_resolution_clock::now();
        multithreading = std::chrono::duration<double>
            (finish_time2 - start_time2).count();

        std::cout << "--------------------" << std::endl;

        /*for (const auto& car : good_cars) {

            std::cout << car;

        }*/

        std::cout << "Single threading: " << single_threading << " seconds" << std::endl;
        std::cout << "Multi threading: " << multithreading << " seconds" << std::endl;

        break;


    case '3':
        ;
    default:
        break;
    }

    return 0;
}