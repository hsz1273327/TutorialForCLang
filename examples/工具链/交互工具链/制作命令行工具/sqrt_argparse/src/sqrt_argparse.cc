#include <cstdlib>
#include <cstdio>
#include <cmath>
#include <exception>
#include <argparse/argparse.hpp>

int main(int argc, char **argv)  {
    argparse::ArgumentParser program("sqrt_argparse","0.0.0");
    program.add_argument("target")
      .help("计算开根号")
      .action([&](const std::string& value) { return std::stoi(value); });

    try {
        program.parse_args(argc, argv);
    } catch (const std::runtime_error& err) {
        std::cout << err.what() << std::endl;
        std::cout << program;
        exit(0);
    }
    auto input = program.get<int>("target");
    std::cout << std::sqrt(input) << std::endl;
    return 0;
}  