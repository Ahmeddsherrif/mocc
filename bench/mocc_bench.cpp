#include "mocc.h"

#include <chrono>
#include <cstdlib>
#include <functional>
#include <iostream>
#include <vector>

static double measure(const std::function<void()>& callback)
{
    const auto start = std::chrono::steady_clock::now();
    callback();
    const auto end = std::chrono::steady_clock::now();
    return std::chrono::duration<double, std::milli>(end - start).count();
}

static void print_benchmark_result(const double vector_time, const double mocc_time)
{
    
    auto diff = mocc_time - vector_time;
    auto percentage_diff = (diff / vector_time) * 100.0;


    if(vector_time < mocc_time)
    {
        std::cout << " +++ std::vector: " << vector_time << " ms" << std::endl;
        std::cout << "     mocc       : " << mocc_time   << " ms" << std::endl;
    }
    else
    {
        std::cout << "     std::vector: " << vector_time << " ms"<< std::endl;
        std::cout << " +++ mocc       : " << mocc_time   << " ms" << std::endl;
    }

        std::cout << "     Delta      : " << diff << " ms (" << percentage_diff << "%)" << std::endl;

    std::cout << std::endl;
}

static void benchmark_push_back(size_t count)
{
    std::cout << "Benchmark: push_back(" << count << ")" << std::endl;

    const double vector_time = measure(
        [&]()
        {
            std::vector<int> container;
            for (size_t i = 0; i < count; ++i)
            {
                container.push_back(static_cast<int>(i));
            }
        });

    const double mocc_time = measure(
        [&]()
        {
            mocc_object* container = nullptr;
            mocc_ctor(sizeof(int), 8, &container);
            for (size_t i = 0; i < count; ++i)
            {
                const int value = static_cast<int>(i);
                mocc_push_back(container, &value);
            }
            mocc_dtor(container);
        });

        
    print_benchmark_result(vector_time, mocc_time);

}

static void benchmark_random_access(size_t count)
{
    std::cout << "Benchmark: sequential access(" << count << ")" << std::endl;

    const double vector_time = measure(
        [&]()
        {
            std::vector<int> container;
            container.reserve(count);
            for (size_t i = 0; i < count; ++i)
            {
                container.push_back(static_cast<int>(i));
            }

            volatile int sum = 0;
            for (size_t i = 0; i < count; ++i)
            {
                sum += container[i];
            }
            (void)sum;
        });

    const double mocc_time = measure(
        [&]()
        {
            mocc_object* container = nullptr;
            mocc_ctor(sizeof(int), 8, &container);
            for (size_t i = 0; i < count; ++i)
            {
                const int value = static_cast<int>(i);
                mocc_push_back(container, &value);
            }

            volatile int sum = 0;
            for (size_t i = 0; i < count; ++i)
            {
                int* pointer = nullptr;
                mocc_at(container, i, reinterpret_cast<void**>(&pointer));
                sum += *pointer;
            }
            (void)sum;

            mocc_dtor(container);
        });

    print_benchmark_result(vector_time, mocc_time);
}

static void benchmark_erase_middle(size_t count)
{
    std::cout << "Benchmark: erase middle(" << count << ")" << std::endl;

    const double vector_time = measure(
        [&]()
        {
            std::vector<int> container;
            container.reserve(count);
            for (size_t i = 0; i < count; ++i)
            {
                container.push_back(static_cast<int>(i));
            }

            for (int i = 0; i < 10 && !container.empty(); ++i)
            {
                container.erase(container.begin() + container.size() / 2);
            }
        });

    const double mocc_time = measure(
        [&]()
        {
            mocc_object* container = nullptr;
            mocc_ctor(sizeof(int), 8, &container);
            for (size_t i = 0; i < count; ++i)
            {
                const int value = static_cast<int>(i);
                mocc_push_back(container, &value);
            }

            for (int i = 0; i < 10; ++i)
            {
                const size_t index = (count - static_cast<size_t>(i)) / 2;
                mocc_erase(container, index);
            }

            mocc_dtor(container);
        });

    print_benchmark_result(vector_time, mocc_time);
}

static void benchmark_reserve(size_t count)
{
    std::cout << "Benchmark: reserve(" << count << ")" << std::endl;

    const double vector_time = measure(
        [&]()
        {
            std::vector<int> container;
            container.reserve(count);
        });

    const double mocc_time = measure(
        [&]()
        {
            mocc_object* container = nullptr;
            mocc_ctor(sizeof(int), 8, &container);
            mocc_reserve(container, count);
            mocc_dtor(container);
        });

    print_benchmark_result(vector_time, mocc_time);
}

int main()
{
    const size_t count = 500000;

    std::cout << "mocc vs std::vector benchmark" << std::endl;
    std::cout << "--------------------------------" << std::endl;

    benchmark_reserve(count);
    benchmark_push_back(count);
    benchmark_random_access(count);
    benchmark_erase_middle(count);

    return EXIT_SUCCESS;
}
