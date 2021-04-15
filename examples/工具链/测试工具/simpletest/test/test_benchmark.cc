#define CATCH_CONFIG_ENABLE_BENCHMARKING
#include <catch2/catch.hpp>
#include "binary_vector.hpp"
using vector::BinaryVector;
using Catch::Matchers::WithinRel;

TEST_CASE( "Test_Benchmark", "[vector][benchmark]" ) {
    BinaryVector A(1.0,2.0);
    BinaryVector B(3.0,4.0);
    BENCHMARK( "benchmark mod A" ) {
        return A.mod();
    };
    BENCHMARK( "benchmark mod B" ) {
        return B.mod();
    };
    BENCHMARK( "benchmark mul A B" ) {
        return A.mul(B);
    };
    BENCHMARK( "benchmark add A B" ) {
        return  A.add(B);
    };
}