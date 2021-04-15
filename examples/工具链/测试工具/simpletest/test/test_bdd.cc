#include <catch2/catch.hpp>
#include "binary_vector.hpp"
using vector::BinaryVector;
using Catch::Matchers::WithinRel;

SCENARIO("Test_BDD", "[vector][unittest]" ) {
    GIVEN( "2 vector A(1.0,2.0) and B(3.0,4.0)" ) {
        BinaryVector A(1.0,2.0);
        BinaryVector B(3.0,4.0);
        WHEN( "check mod A" ){
            THEN( "A mod is not far-off 2.23607" ){
                REQUIRE_THAT(2.23607,WithinRel(A.mod()));
            }
        }
        WHEN( "check mod B" ) {
            THEN( "B mod is not far-off 5.000000" ){
                REQUIRE_THAT(5.000000,WithinRel( B.mod()));
            }
        }
        WHEN( "check mul A B" ) {
            THEN( "A mul B is not far-off 11.000000" ){
                REQUIRE_THAT(11.000000,WithinRel( A.mul(B)));
            }
        }
        WHEN( "check add A B" ) {
            auto C =  A.add(B);
            THEN( "A add B is not far-off {4.000000 , 6.000000}" ){
                REQUIRE_THAT(4.000000,WithinRel(C.x));
                REQUIRE_THAT(6.000000,WithinRel(C.y));
            }
        }
    }
}