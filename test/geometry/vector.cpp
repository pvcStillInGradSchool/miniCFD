// Copyright 2019 Weicheng Pei and Minghao Yang
#include "mini/geometry/vector.hpp"
#include "mini/geometry/point.hpp"

#include <vector>

#include "gtest/gtest.h"

namespace mini {
namespace geometry {

class VectorTest : public ::testing::Test {
 public:
  using Real = double;
  using P1 = Point<Real, 1>;
  using P2 = Point<Real, 2>;
  using P3 = Point<Real, 3>;
  using V1 = Vector<Real, 1>;
  using V2 = Vector<Real, 2>;
  using V3 = Vector<Real, 3>;
  const Real x{1.0}, y{2.0}, z{3.0};
};
TEST_F(VectorTest, Constructors) {
  // Test V1(std::initializer_list<Real>):
  auto v1 = V1{x};
  EXPECT_EQ(v1.X(), x);
  // Test V2(std::initializer_list<Real>):
  auto v2 = V2{x, y};
  EXPECT_EQ(v2.X(), x);
  EXPECT_EQ(v2.Y(), y);
  // Test P3(std::initializer_list<Real>):
  auto v3 = V3{x, y, z};
  EXPECT_EQ(v3.X(), x);
  EXPECT_EQ(v3.Y(), y);
  EXPECT_EQ(v3.Z(), z);
}
TEST_F(VectorTest, OperatorsForV3) {
  auto v = V3{x, y, z};
  // Test v + v:
  auto sum = v + v;
  EXPECT_EQ(sum.X(), x + x);
  EXPECT_EQ(sum.Y(), y + y);
  EXPECT_EQ(sum.Z(), z + z);
  // Test v - v:
  auto diff = v - v;
  EXPECT_EQ(diff.X(), 0);
  EXPECT_EQ(diff.Y(), 0);
  EXPECT_EQ(diff.Z(), 0);
  // Test v * r and v / r:
  auto r = Real{3.14};
  auto prod = v * r;
  EXPECT_EQ(prod.X(), x * r);
  EXPECT_EQ(prod.Y(), y * r);
  EXPECT_EQ(prod.Z(), z * r);
  auto div = v / r;
  EXPECT_EQ(div.X(), x / r);
  EXPECT_EQ(div.Y(), y / r);
  EXPECT_EQ(div.Z(), z / r);
  // Test v.Dot():
  EXPECT_EQ(v.Dot(v), x*x + y*y + z*z);
  // Test v.Cross():
  auto cross = v.Cross(v);
  EXPECT_EQ(cross.X(), 0);
  EXPECT_EQ(cross.Y(), 0);
  EXPECT_EQ(cross.Z(), 0);
}
TEST_F(VectorTest, OperatorsForV2) {
  auto v = V2{x, y};
  // Test v + v:
  auto sum = v + v;
  EXPECT_EQ(sum.X(), x + x);
  EXPECT_EQ(sum.Y(), y + y);
  // Test v - v:
  auto diff = v - v;
  EXPECT_EQ(diff.X(), 0);
  EXPECT_EQ(diff.Y(), 0);
  // Test v * r and v / r:
  auto r = Real{3.14};
  auto prod = v * r;
  EXPECT_EQ(prod.X(), x * r);
  EXPECT_EQ(prod.Y(), y * r);
  auto div = v / r;
  EXPECT_EQ(div.X(), x / r);
  EXPECT_EQ(div.Y(), y / r);
  // Test v.Dot():
  EXPECT_EQ(v.Dot(v), x*x + y*y);
  // Test v.Cross():
  EXPECT_EQ(v.Cross(v), 0);
  auto u = V2{-y, x};
  EXPECT_EQ(v.Cross(u), x*x + y*y);
  EXPECT_EQ(v.Cross(u) + u.Cross(v), 0);
}

}  // namespace geometry
}  // namespace mini

int main(int argc, char* argv[]) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}