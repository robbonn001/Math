#include "pch.h"
#include "MathVector.h"
#include <stdexcept>
#include <string>

// ============================================================
// Тесты MathVector
// ============================================================

TEST(MathVectorTest, DefaultConstructor) {
    MathVector<double> v;
    EXPECT_EQ(v.size(), 0u);
}

TEST(MathVectorTest, ConstructorWithSize) {
    MathVector<double> v(5);
    EXPECT_EQ(v.size(), 5u);
    for (size_t i = 0; i < 5; ++i) {
        EXPECT_EQ(v[i], 0.0);
    }
}

TEST(MathVectorTest, InitializerListConstructor) {
    MathVector<double> v{ 1.0, 2.0, 3.0 };
    EXPECT_EQ(v.size(), 3u);
    EXPECT_EQ(v[0], 1.0);
    EXPECT_EQ(v[2], 3.0);
}

TEST(MathVectorTest, CopyConstructor) {
    MathVector<double> v1{ 1.0, 2.0, 3.0 };
    MathVector<double> v2(v1);
    EXPECT_EQ(v2.size(), 3u);
    EXPECT_EQ(v2[0], 1.0);
}

TEST(MathVectorTest, ScalarMultiplication) {
    MathVector<double> v{ 1.0, 2.0, 3.0 };
    MathVector<double> result = v * 2.0;
    EXPECT_EQ(result[0], 2.0);
    EXPECT_EQ(result[1], 4.0);
    EXPECT_EQ(result[2], 6.0);
}

TEST(MathVectorTest, ScalarMultiplicationAssign) {
    MathVector<double> v{ 1.0, 2.0, 3.0 };
    v *= 3.0;
    EXPECT_EQ(v[0], 3.0);
    EXPECT_EQ(v[1], 6.0);
    EXPECT_EQ(v[2], 9.0);
}

TEST(MathVectorTest, Addition) {
    MathVector<double> v1{ 1.0, 2.0, 3.0 };
    MathVector<double> v2{ 4.0, 5.0, 6.0 };
    MathVector<double> result = v1 + v2;
    EXPECT_EQ(result[0], 5.0);
    EXPECT_EQ(result[1], 7.0);
    EXPECT_EQ(result[2], 9.0);
}

TEST(MathVectorTest, Subtraction) {
    MathVector<double> v1{ 4.0, 5.0, 6.0 };
    MathVector<double> v2{ 1.0, 2.0, 3.0 };
    MathVector<double> result = v1 - v2;
    EXPECT_EQ(result[0], 3.0);
    EXPECT_EQ(result[1], 3.0);
    EXPECT_EQ(result[2], 3.0);
}

TEST(MathVectorTest, DotProduct) {
    MathVector<double> v1{ 1.0, 2.0, 3.0 };
    MathVector<double> v2{ 4.0, 5.0, 6.0 };
    double result = v1 * v2;
    EXPECT_DOUBLE_EQ(result, 32.0); // 1*4 + 2*5 + 3*6 = 32
}

TEST(MathVectorTest, AdditionAssign) {
    MathVector<double> v1{ 1.0, 2.0, 3.0 };
    MathVector<double> v2{ 4.0, 5.0, 6.0 };
    v1 += v2;
    EXPECT_EQ(v1[0], 5.0);
    EXPECT_EQ(v1[1], 7.0);
    EXPECT_EQ(v1[2], 9.0);
}

TEST(MathVectorTest, SubtractionAssign) {
    MathVector<double> v1{ 4.0, 5.0, 6.0 };
    MathVector<double> v2{ 1.0, 2.0, 3.0 };
    v1 -= v2;
    EXPECT_EQ(v1[0], 3.0);
    EXPECT_EQ(v1[1], 3.0);
    EXPECT_EQ(v1[2], 3.0);
}

TEST(MathVectorTest, DifferentSizes) {
    MathVector<double> v1{ 1.0, 2.0, 3.0 };
    MathVector<double> v2{ 4.0, 5.0 };

    EXPECT_THROW(v1 + v2, std::logic_error);
}

TEST(MathVectorTest, Equality) {
    MathVector<double> v1{ 1.0, 2.0, 3.0 };
    MathVector<double> v2{ 1.0, 2.0, 3.0 };
    EXPECT_TRUE(v1 == v2);
}

TEST(MathVectorTest, Inequality) {
    MathVector<double> v1{ 1.0, 2.0, 3.0 };
    MathVector<double> v2{ 1.0, 2.0, 4.0 };
    EXPECT_FALSE(v1 == v2);
}

TEST(MathVectorTest, CopyAssignment) {
    MathVector<double> v1{ 1.0, 2.0, 3.0 };
    MathVector<double> v2;
    v2 = v1;
    EXPECT_EQ(v2.size(), 3u);
    EXPECT_EQ(v2[0], 1.0);
}

TEST(MathVectorTest, MoveAssignment) {
    MathVector<double> v1{ 1.0, 2.0, 3.0 };
    MathVector<double> v2;
    v2 = std::move(v1);
    EXPECT_EQ(v2.size(), 3u);
    EXPECT_EQ(v1.size(), 0u);
}