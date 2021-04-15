#include <catch2/catch.hpp>
#include "binary_vector.hpp"
using vector::BinaryVector;
using Catch::Matchers::WithinRel;

TEST_CASE( "Test_TDD", "[vector][unittest]" ) {
    BinaryVector A(1.0,2.0);
    BinaryVector B(3.0,4.0);
    SECTION( "check mod A" ) {
        REQUIRE_THAT(2.23607,WithinRel(A.mod()));
    }
    SECTION( "check mod B" ) {
        REQUIRE_THAT(5.000000,WithinRel( B.mod()));
    }
    SECTION( "check mul A B" ) {
        REQUIRE_THAT(11.000000,WithinRel( A.mul(B)));
    }
    SECTION( "check add A B" ) {
        auto C =  A.add(B);
        REQUIRE_THAT(4.000000,WithinRel(C.x));
        REQUIRE_THAT(6.000000,WithinRel(C.y));
    }
}