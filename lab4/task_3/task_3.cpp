﻿#include <algorithm>
#include <cassert>
#include <chrono>
#include <filesystem>
#include <fstream>
#include <functional>
#include <iostream>
#include <iterator>
#include <limits>
#include <optional>
#include <random>
#include <string>
#include <string_view>
#include <thread>
#include <variant>
#include <vector>

namespace
{
	namespace fs = std::filesystem;
	using namespace std::literals;

	namespace action
	{
		struct Generate
		{
			fs::path outputPath;
			size_t size;
		};

		struct Sort
		{
			fs::path inputPath;
			fs::path outputPath;
			unsigned numThreads;
		};

		struct Help
		{
		};
	} // namespace action

	using Action = std::variant<action::Generate, action::Sort, action::Help>;

	Action ParseCommandLine(int argc, const char* argv[])
	{
		if (argc == 1)
		{
			throw std::runtime_error("Invalid command line. Type sorter -h|--help for help");
		}
		if (argc == 2 && argv[1] == "--help"sv || argv[1] == "-h"sv)
		{
			return action::Help{};
		}
		if (argv[1] == "generate"sv)
		{
			if (argc != 4)
			{
				throw std::runtime_error("Invalid \"generate\" command arguments");
			}
			return action::Generate{ .outputPath = argv[2], .size = std::stoull(argv[3]) };
		}
		if (argv[1] == "sort"sv)
		{
			if (argc != 5)
			{
				throw std::runtime_error("Invalid \"sort\" command arguments");
			}
			return action::Sort{
				.inputPath = argv[2], .outputPath = argv[3], .numThreads = [argv] {
					int n = std::stoi(argv[4]);
					return (n >= 0)
						? static_cast<unsigned>(n)
						: throw std::runtime_error("Number of threads must not be negative");
				}()
			};
		}
		throw std::runtime_error("Invalid option "s + argv[1]);
	}

	template <typename... Ts>
	struct overloaded : Ts...
	{
		using Ts::operator()...;
	};

	void ShowHelp()
	{
		std::cout << R"(Usage:
sorter generate OUTPUT_FILE SIZE
    Generates OUTPUT_FILE containing SIZE pseudo-random integer numbers.

sorter sort INPUT_FILE OUTPUT_FILE MAX_THREADS
    Sort integer numbers in INPUT_FILE and writes result to OUTPUT_FILE.
    Numbers are sorted using up to MAX_THREADS
)";
	}

	using Number = int64_t;
	using Numbers = std::vector<Number>;

	/**
	 * Генерирует файл filePath, содержащий size псевдослучайных целых чисел
	 */
	void GenerateFile(const fs::path& filePath, size_t size)
	{
		std::ofstream out{ filePath.string() };
		std::mt19937 gen;
		std::uniform_int_distribution dist(
			std::numeric_limits<Number>::min(),
			std::numeric_limits<Number>::max());
		// Записываем в out size псевдослучайных чисел, вызывая для каждого dist(gen).
		// std::bind_front(dist, gen) аналогично [&dist, &gen]{ dist(gen); }
		std::generate_n(std::ostream_iterator<Number>(out, "\n"), size, std::bind_front(dist, gen));
	}

	Numbers ReadNumbers(const fs::path& fileName)
	{
		std::ifstream in{ fileName };
		Numbers numbers;
		// Самое большое 64-битное целое содержит примерно 18 десятичных знаков
		// Поэтому можно примерно оценить минимальное количество чисел, содержащихся в текстовом
		// файле заданного размера.
		numbers.reserve(static_cast<size_t>(fs::file_size(fileName) / 18));
		std::copy(std::istream_iterator<Number>(in),
			std::istream_iterator<Number>(),
			std::back_inserter(numbers));
		return numbers;
	}

	void WriteNumbers(const Numbers& numbers, const fs::path& outputFileName)
	{
		std::ofstream out(outputFileName);
		std::ranges::copy(numbers, std::ostream_iterator<Numbers::value_type>(out, "\n"));
	}

	/**
	 * Сортирует содержимое inputFileName в outputFileName последовательно перебирая
	 * количество потоков от 1 до numThreads и выводит время сортировки в stdout.
	 *
	 * Если numThreads равно 0, то количество потоков должно быть равно максимальному
	 * количеству потоков, которое поддерживает компьютер.
	 */
	void SortFile(const fs::path& inputFileName, const fs::path& outputFileName, unsigned numThreads)
	{
		numThreads = numThreads != 0 ? numThreads : std::thread::hardware_concurrency();

		auto numbers = ReadNumbers(inputFileName);

		// TODO: Реализуйте сортировку самостятельно

		WriteNumbers(numbers, outputFileName);
	}

} // namespace

int main(int argc, const char* argv[])
{
	try
	{
		auto action = ParseCommandLine(argc, argv);

		std::visit(overloaded{
					   [](const action::Generate& gen) {
						   GenerateFile(gen.outputPath, gen.size);
					   },
					   [](const action::Sort& sort) {
						   SortFile(sort.inputPath, sort.outputPath, sort.numThreads);
					   },
					   [](const action::Help&) {
						   ShowHelp();
					   },
			},
			action);

		return EXIT_SUCCESS;
	}
	catch (const std::exception& e)
	{
		std::cerr << e.what() << std::endl;
		return EXIT_FAILURE;
	}
}