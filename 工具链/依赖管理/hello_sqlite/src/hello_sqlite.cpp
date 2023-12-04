#include <sqlite3.h>
#include <spdlog/spdlog.h>

int main() {
    spdlog::info(sqlite3_libversion());
    return 0;
}