#include <GUnit/GMake.h>
#include <GUnit/GTest.h>

#include <array.h>
#include <operators.h>

GTEST("Addition Test") {
  SHOULD("have the expected values after constructed by initializer list ") {
    Array<double> x{1, 2, 3, 4, 5};
    EXPECT_EQ(x[0], 1);
    EXPECT_EQ(x[1], 2);
    EXPECT_EQ(x[2], 3);
    EXPECT_EQ(x[3], 4);
    EXPECT_EQ(x[4], 5);
  }

  SHOULD("do x = 5.5 + x correctly") {
    Array<double> x{1, 2, 3, 4, 5};
    x = 5.5 + x;
    Array<double> expected{6.5, 7.5, 8.5, 9.5, 10.5};
    EXPECT_EQ(x, expected);
  }

  SHOULD("do x = 2 * x correctly") {
    Array<double> x{1, 2, 3, 4, 5};
    x = 2.0 * x;
    Array<double> expected{2, 4, 6, 8, 10};
    EXPECT_EQ(x, expected);
  }

  SHOULD("do x = x + y correctly") {
    Array<double> x{1, 2, 3, 4, 5};
    Array<double> y{1, 2, 3, 4, 1};
    x = x + y;
    Array<double> expected{2, 4, 6, 8, 6};
    EXPECT_EQ(x, expected);
  }

  SHOULD("do x = x * y correctly") {
    Array<double> x{1, 2, 3, 4, 5};
    Array<double> y{1, 2, 3, 4, 1};
    x = x * y;
    Array<double> expected{1, 4, 9, 16, 5};
    EXPECT_EQ(x, expected);
  }
}
