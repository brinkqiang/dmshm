
#include <iostream>
#include <vector>
#include <list>
#include <sstream>
#include <chrono>

#include "dmranges.h"
#include "dmchrono.h"
#include "dmcolor.h"

int main()
{
    std::cout << fmt::format("{0},{1},{2}", "a", "b", "c") << std::endl;
    std::cout << fmt::format("{2},{1},{0}", "a", "b", "c") << std::endl;

	std::cout << fmt::format("{:%Y-%m-%d %H:%M:%S}", fmt::localtime(time(NULL))) << std::endl;

	std::vector<int> v = { 1, 2, 3 };
	fmt::print("{}\n", v);
    using namespace std::literals::chrono_literals;
	fmt::print("Default format: {} {}\n", 42s, 100ms);
	fmt::print("strftime-like format: {:%H:%M:%S}\n", 3h + 15min + 30s);

    std::cout << fmt::format("{}{}{}{}{}{}{}{}{}{}{}{}",
        1, 2, 3, 4, 5, 6, 'a', 'b', 'c', 'd', 'e', 'f') << std::endl;

    std::cout << fmt::format("{_1}/{a_}/{A_}", fmt::arg("a_", 'a'),
        fmt::arg("A_", "A"), fmt::arg("_1", 1)) << std::endl;

	fmt::print(fg(fmt::color::crimson) | fmt::emphasis::bold, "Hello, {}!\n", "world");
	fmt::print(fg(fmt::color::floral_white) | bg(fmt::color::slate_gray) |
		fmt::emphasis::underline, "Hello, {}!\n", "мир");
	fmt::print(fg(fmt::color::steel_blue) | fmt::emphasis::italic,
		"Hello, {}!\n", "世界");
    return 0;
}