#include "mocc.h"

#include <chrono>
#include <cstdlib>
#include <functional>
#include <iostream>
#include <vector>

static void check_mocc_error(mocc_error error, const char* operation)
{
    if (error != MOCC_OK)
    {
        std::cerr << "MOCC error while " << operation << ": " << error << "\n";
        std::exit(EXIT_FAILURE);
    }
}

static double measure(const std::function<void()>& callback)
{
    const auto start = std::chrono::steady_clock::now();
    callback();
    const auto end = std::chrono::steady_clock::now();
    return std::chrono::duration<double, std::milli>(end - start).count();
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
            check_mocc_error(mocc_ctor(sizeof(int), &container), "ctor");
            for (size_t i = 0; i < count; ++i)
            {
                const int value = static_cast<int>(i);
                check_mocc_error(mocc_push_back(container, &value), "push_back");
            }
            check_mocc_error(mocc_dtor(container), "dtor");
        });

    std::cout << "  std::vector: " << vector_time << " ms" << std::endl;
    std::cout << "  mocc:        " << mocc_time << " ms" << std::endl;
    std::cout << std::endl;
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
            check_mocc_error(mocc_ctor(sizeof(int), &container), "ctor");
            for (size_t i = 0; i < count; ++i)
            {
                const int value = static_cast<int>(i);
                check_mocc_error(mocc_push_back(container, &value), "push_back");
            }

            volatile int sum = 0;
            for (size_t i = 0; i < count; ++i)
            {
                int* pointer = nullptr;
                check_mocc_error(mocc_at(container, i, reinterpret_cast<void**>(&pointer)), "at");
                sum += *pointer;
            }
            (void)sum;

            check_mocc_error(mocc_dtor(container), "dtor");
        });

    std::cout << "  std::vector: " << vector_time << " ms" << std::endl;
    std::cout << "  mocc:        " << mocc_time << " ms" << std::endl;
    std::cout << std::endl;
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
            check_mocc_error(mocc_ctor(sizeof(int), &container), "ctor");
            for (size_t i = 0; i < count; ++i)
            {
                const int value = static_cast<int>(i);
                check_mocc_error(mocc_push_back(container, &value), "push_back");
            }

            for (int i = 0; i < 10; ++i)
            {
                const size_t index = (count - static_cast<size_t>(i)) / 2;
                check_mocc_error(mocc_erase(container, index), "erase");
            }

            check_mocc_error(mocc_dtor(container), "dtor");
        });

    std::cout << "  std::vector: " << vector_time << " ms" << std::endl;
    std::cout << "  mocc:        " << mocc_time << " ms" << std::endl;
    std::cout << std::endl;
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
            check_mocc_error(mocc_ctor(sizeof(int), &container), "ctor");
            check_mocc_error(mocc_reserve(container, count), "reserve");
            check_mocc_error(mocc_dtor(container), "dtor");
        });

    std::cout << "  std::vector: " << vector_time << " ms" << std::endl;
    std::cout << "  mocc:        " << mocc_time << " ms" << std::endl;
    std::cout << std::endl;
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
