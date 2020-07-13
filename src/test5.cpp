#include "core_pipeline.hpp"
#include "globaldef.hpp"
#include "gtest/gtest.h"
#include <iostream>
using namespace std;
#define G(AAA, b)                                \
    TEST(FileTest, AAA) {                        \
        core_session C("dataset/" #AAA ".data"); \
        EXPECT_EQ(C.run(), b);                   \
    }

G(sample, 94);
G(array_test1, 123);
G(array_test2, 43);
G(gcd, 178);
G(lvalue2, 175);
G(manyarguments, 40);
G(naive, 94);
G(tak, 186);
G(superloop, 134);
// G(magic, 106);
// //TIME
// G(basicopt1, 88);
// G(qsort, 105);
// G(bulgarian, 159);
// G(expr, 58);
// G(magic, 106);
// G(multiarray, 115);
// G(statement_test, 50);
// G(pi, 137);
// G(queens, 171);
//ERROR
// G(hanoi, 20);

int main(int argc, char** argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}