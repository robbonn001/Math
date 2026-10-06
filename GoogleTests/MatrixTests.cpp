#include "pch.h"
#include "Matrix.h"
#include <stdexcept>
#include <cmath>

// ============================================================
// Тесты Matrix
// ============================================================

class MatrixTest : public ::testing::Test {
protected:
    void SetUp() override {}
    void TearDown() override {}

    // Вспомогательная функция для сравнения матриц с допуском
    void assertMatrixEqual(const Matrix& m1, const Matrix& m2, double epsilon = 1e-9) {
        EXPECT_EQ(m1.rows(), m2.rows());
        EXPECT_EQ(m1.cols(), m2.cols());
        for (size_t i = 0; i < m1.rows(); ++i) {
            for (size_t j = 0; j < m1.cols(); ++j) {
                EXPECT_NEAR(m1[i][j], m2[i][j], epsilon)
                    << "Mismatch at [" << i << "][" << j << "]";
            }
        }
    }
};

// Конструкторы

TEST_F(MatrixTest, DefaultConstructor) {
    Matrix m;
    EXPECT_EQ(m.rows(), 0u);
    EXPECT_EQ(m.cols(), 0u);
}

TEST_F(MatrixTest, ConstructorWithSize) {
    Matrix m(3, 4);
    EXPECT_EQ(m.rows(), 3u);
    EXPECT_EQ(m.cols(), 4u);
    // Все элементы должны быть 0
    for (size_t i = 0; i < 3; ++i) {
        for (size_t j = 0; j < 4; ++j) {
            EXPECT_EQ(m[i][j], 0.0);
        }
    }
}

TEST_F(MatrixTest, ConstructorWithInitializerList) {
    Matrix m(2, 3, { {1.0, 2.0, 3.0},
                      {4.0, 5.0, 6.0} });
    EXPECT_EQ(m.rows(), 2u);
    EXPECT_EQ(m.cols(), 3u);
    EXPECT_EQ(m[0][0], 1.0);
    EXPECT_EQ(m[0][2], 3.0);
    EXPECT_EQ(m[1][0], 4.0);
    EXPECT_EQ(m[1][2], 6.0);
}

TEST_F(MatrixTest, CopyConstructor) {
    Matrix m1(2, 2, { {1.0, 2.0}, {3.0, 4.0} });
    Matrix m2(m1);
    EXPECT_EQ(m2.rows(), 2u);
    EXPECT_EQ(m2.cols(), 2u);
    EXPECT_EQ(m2[0][0], 1.0);
    EXPECT_EQ(m2[1][1], 4.0);
}

TEST_F(MatrixTest, ConstructorFromArray) {
    double data[] = { 1.0, 2.0, 3.0, 4.0, 5.0, 6.0 };
    Matrix m(2, 3, data);
    EXPECT_EQ(m.rows(), 2u);
    EXPECT_EQ(m.cols(), 3u);
    EXPECT_EQ(m[0][0], 1.0);
    EXPECT_EQ(m[0][2], 3.0);
    EXPECT_EQ(m[1][0], 4.0);
    EXPECT_EQ(m[1][2], 6.0);
}

// Доступ к элементам

TEST_F(MatrixTest, ElementAccess) {
    Matrix m(2, 2, { {1.0, 2.0}, {3.0, 4.0} });
    EXPECT_EQ(m[0][0], 1.0);
    EXPECT_EQ(m[0][1], 2.0);
    EXPECT_EQ(m[1][0], 3.0);
    EXPECT_EQ(m[1][1], 4.0);
}

TEST_F(MatrixTest, ElementModification) {
    Matrix m(2, 2);
    m[0][0] = 1.0;
    m[0][1] = 2.0;
    m[1][0] = 3.0;
    m[1][1] = 4.0;
    EXPECT_EQ(m[0][0], 1.0);
    EXPECT_EQ(m[1][1], 4.0);
}

// Размеры

TEST_F(MatrixTest, RowsAndCols) {
    Matrix m(5, 7);
    EXPECT_EQ(m.rows(), 5u);
    EXPECT_EQ(m.cols(), 7u);
}

TEST_F(MatrixTest, IsSquare) {
    Matrix m1(3, 3);
    EXPECT_TRUE(m1.isSquare());

    Matrix m2(3, 4);
    EXPECT_FALSE(m2.isSquare());
}

// Арифметические операции

TEST_F(MatrixTest, ScalarMultiplication) {
    Matrix m(2, 2, { {1.0, 2.0}, {3.0, 4.0} });
    Matrix result = m * 2.0;
    EXPECT_EQ(result[0][0], 2.0);
    EXPECT_EQ(result[0][1], 4.0);
    EXPECT_EQ(result[1][0], 6.0);
    EXPECT_EQ(result[1][1], 8.0);
}

TEST_F(MatrixTest, ScalarMultiplicationAssign) {
    Matrix m(2, 2, { {1.0, 2.0}, {3.0, 4.0} });
    m *= 3.0;
    EXPECT_EQ(m[0][0], 3.0);
    EXPECT_EQ(m[0][1], 6.0);
    EXPECT_EQ(m[1][0], 9.0);
    EXPECT_EQ(m[1][1], 12.0);
}

TEST_F(MatrixTest, Addition) {
    Matrix m1(2, 2, { {1.0, 2.0}, {3.0, 4.0} });
    Matrix m2(2, 2, { {5.0, 6.0}, {7.0, 8.0} });
    Matrix result = m1 + m2;
    EXPECT_EQ(result[0][0], 6.0);
    EXPECT_EQ(result[0][1], 8.0);
    EXPECT_EQ(result[1][0], 10.0);
    EXPECT_EQ(result[1][1], 12.0);
}

TEST_F(MatrixTest, AdditionAssign) {
    Matrix m1(2, 2, { {1.0, 2.0}, {3.0, 4.0} });
    Matrix m2(2, 2, { {5.0, 6.0}, {7.0, 8.0} });
    m1 += m2;
    EXPECT_EQ(m1[0][0], 6.0);
    EXPECT_EQ(m1[1][1], 12.0);
}

TEST_F(MatrixTest, Subtraction) {
    Matrix m1(2, 2, { {5.0, 6.0}, {7.0, 8.0} });
    Matrix m2(2, 2, { {1.0, 2.0}, {3.0, 4.0} });
    Matrix result = m1 - m2;
    EXPECT_EQ(result[0][0], 4.0);
    EXPECT_EQ(result[0][1], 4.0);
    EXPECT_EQ(result[1][0], 4.0);
    EXPECT_EQ(result[1][1], 4.0);
}

TEST_F(MatrixTest, SubtractionAssign) {
    Matrix m1(2, 2, { {5.0, 6.0}, {7.0, 8.0} });
    Matrix m2(2, 2, { {1.0, 2.0}, {3.0, 4.0} });
    m1 -= m2;
    EXPECT_EQ(m1[0][0], 4.0);
    EXPECT_EQ(m1[1][1], 4.0);
}

TEST_F(MatrixTest, MatrixMultiplication) {
    Matrix m1(2, 2, { {1.0, 2.0}, {3.0, 4.0} });
    Matrix m2(2, 2, { {5.0, 6.0}, {7.0, 8.0} });
    Matrix result = m1 * m2;
    // [1*5+2*7, 1*6+2*8] = [19, 22]
    // [3*5+4*7, 3*6+4*8] = [43, 50]
    EXPECT_EQ(result[0][0], 19.0);
    EXPECT_EQ(result[0][1], 22.0);
    EXPECT_EQ(result[1][0], 43.0);
    EXPECT_EQ(result[1][1], 50.0);
}

TEST_F(MatrixTest, MatrixMultiplicationNonSquare) {
    Matrix m1(2, 3);
    m1[0][0] = 1; m1[0][1] = 2; m1[0][2] = 3;
    m1[1][0] = 4; m1[1][1] = 5; m1[1][2] = 6;

    Matrix m2(3, 2);
    m2[0][0] = 7; m2[0][1] = 8;
    m2[1][0] = 9; m2[1][1] = 10;
    m2[2][0] = 11; m2[2][1] = 12;

    Matrix result = m1 * m2;
    EXPECT_EQ(result.rows(), 2u);
    EXPECT_EQ(result.cols(), 2u);
    // [1*7+2*9+3*11, 1*8+2*10+3*12] = [58, 64]
    // [4*7+5*9+6*11, 4*8+5*10+6*12] = [139, 154]
    EXPECT_EQ(result[0][0], 58.0);
    EXPECT_EQ(result[0][1], 64.0);
    EXPECT_EQ(result[1][0], 139.0);
    EXPECT_EQ(result[1][1], 154.0);
}

TEST_F(MatrixTest, MatrixMultiplicationIncompatibleSizes) {
    Matrix m1(2, 3);
    Matrix m2(2, 3);
    EXPECT_THROW(m1 * m2, std::invalid_argument);
}

// Транспонирование

TEST_F(MatrixTest, Transpose) {
    Matrix m(2, 3, { {1.0, 2.0, 3.0}, {4.0, 5.0, 6.0} });
    Matrix result = m.T();
    EXPECT_EQ(result.rows(), 3u);
    EXPECT_EQ(result.cols(), 2u);
    EXPECT_EQ(result[0][0], 1.0);
    EXPECT_EQ(result[0][1], 4.0);
    EXPECT_EQ(result[1][0], 2.0);
    EXPECT_EQ(result[1][1], 5.0);
    EXPECT_EQ(result[2][0], 3.0);
    EXPECT_EQ(result[2][1], 6.0);
}

TEST_F(MatrixTest, TransposeSquare) {
    Matrix m(2, 2, { {1.0, 2.0}, {3.0, 4.0} });
    Matrix result = m.T();
    EXPECT_EQ(result[0][0], 1.0);
    EXPECT_EQ(result[0][1], 3.0);
    EXPECT_EQ(result[1][0], 2.0);
    EXPECT_EQ(result[1][1], 4.0);
}

// Определитель

TEST_F(MatrixTest, Determinant2x2) {
    Matrix m(2, 2, { {1.0, 2.0}, {3.0, 4.0} });
    double det = m.determinant();
    EXPECT_NEAR(det, -2.0, 1e-9); // 1*4 - 2*3 = -2
}

TEST_F(MatrixTest, Determinant3x3) {
    Matrix m(3, 3, { {1.0, 2.0, 3.0},
             {4.0, 5.0, 6.0},
             {7.0, 8.0, 10.0} });
    double det = m.determinant();
    EXPECT_NEAR(det, -3.0, 1e-9);
}

TEST_F(MatrixTest, DeterminantNonSquare) {
    Matrix m(2, 3);
    EXPECT_THROW(m.determinant(), std::logic_error);
}

TEST_F(MatrixTest, DeterminantIdentity) {
    Matrix m(2, 2, { {1.0, 0.0}, {0.0, 1.0} });
    EXPECT_NEAR(m.determinant(), 1.0, 1e-9);
}

// Обратная матрица

//TEST_F(MatrixTest, Inverse2x2) {
//    Matrix m(2, 2, { {1.0, 2.0}, {3.0, 4.0} });
//    Matrix inv = m.inverse();
//
//    // Проверяем, что m * inv = I
//    Matrix identity = m * inv;
//    EXPECT_NEAR(identity[0][0], 1.0, 1e-9);
//    EXPECT_NEAR(identity[0][1], 0.0, 1e-9);
//    EXPECT_NEAR(identity[1][0], 0.0, 1e-9);
//    EXPECT_NEAR(identity[1][1], 1.0, 1e-9);
//}
//
//TEST_F(MatrixTest, Inverse3x3) {
//    Matrix m{ {1.0, 2.0, 3.0},
//             {0.0, 1.0, 4.0},
//             {5.0, 6.0, 0.0} };
//    Matrix inv = m.inverse();
//
//    Matrix identity = m * inv;
//    for (size_t i = 0; i < 3; ++i) {
//        for (size_t j = 0; j < 3; ++j) {
//            double expected = (i == j) ? 1.0 : 0.0;
//            EXPECT_NEAR(identity[i][j], expected, 1e-9);
//        }
//    }
//}
//
//TEST_F(MatrixTest, InverseSingularMatrix) {
//    Matrix m{ {1.0, 2.0}, {2.0, 4.0} }; // det = 0
//    EXPECT_THROW(m.inverse(), std::logic_error);
//}
//
//TEST_F(MatrixTest, InverseNonSquare) {
//    Matrix m(2, 3);
//    EXPECT_THROW(m.inverse(), std::logic_error);
//}
//
//// Специальные матрицы
//
//TEST_F(MatrixTest, IdentityMatrix) {
//    Matrix m = Matrix::identity(3);
//    EXPECT_EQ(m.rows(), 3u);
//    EXPECT_EQ(m.cols(), 3u);
//    for (size_t i = 0; i < 3; ++i) {
//        for (size_t j = 0; j < 3; ++j) {
//            double expected = (i == j) ? 1.0 : 0.0;
//            EXPECT_EQ(m[i][j], expected);
//        }
//    }
//}
//
//TEST_F(MatrixTest, ZeroMatrix) {
//    Matrix m = Matrix::zero(2, 3);
//    EXPECT_EQ(m.rows(), 2u);
//    EXPECT_EQ(m.cols(), 3u);
//    for (size_t i = 0; i < 2; ++i) {
//        for (size_t j = 0; j < 3; ++j) {
//            EXPECT_EQ(m[i][j], 0.0);
//        }
//    }
//}

// Сравнение

TEST_F(MatrixTest, Equality) {
    Matrix m1(2, 2, { {1.0, 2.0}, {3.0, 4.0} });
    Matrix m2(2, 2, { {1.0, 2.0}, {3.0, 4.0} });
    EXPECT_TRUE(m1 == m2);
}

TEST_F(MatrixTest, Inequality) {
    Matrix m1(2, 2, { {1.0, 2.0}, {3.0, 4.0} });
    Matrix m2(2, 2, { {1.0, 2.0}, {3.0, 5.0} });
    EXPECT_FALSE(m1 == m2);
}

TEST_F(MatrixTest, InequalityDifferentSizes) {
    Matrix m1(2, 2);
    Matrix m2(2, 3);
    EXPECT_FALSE(m1 == m2);
}

// Операции со строками и столбцами

TEST_F(MatrixTest, GetRow) {
    Matrix m(2, 3, { {1.0, 2.0, 3.0}, {4.0, 5.0, 6.0} });
    MathVector<double> row = m.getRow(0);
    EXPECT_EQ(row.size(), 3u);
    EXPECT_EQ(row[0], 1.0);
    EXPECT_EQ(row[1], 2.0);
    EXPECT_EQ(row[2], 3.0);
}

TEST_F(MatrixTest, GetColumn) {
    Matrix m(2, 3, { {1.0, 2.0, 3.0}, {4.0, 5.0, 6.0} });
    MathVector<double> col = m.getColumn(1);
    EXPECT_EQ(col.size(), 2u);
    EXPECT_EQ(col[0], 2.0);
    EXPECT_EQ(col[1], 5.0);
}

TEST_F(MatrixTest, SetRow) {
    Matrix m(2, 3);
    MathVector<double> row{ 7.0, 8.0, 9.0 };
    m.setRow(0, row);
    EXPECT_EQ(m[0][0], 7.0);
    EXPECT_EQ(m[0][1], 8.0);
    EXPECT_EQ(m[0][2], 9.0);
}

TEST_F(MatrixTest, SetColumn) {
    Matrix m(2, 3);
    MathVector<double> col{ 10.0, 20.0 };
    m.setColumn(1, col);
    EXPECT_EQ(m[0][1], 10.0);
    EXPECT_EQ(m[1][1], 20.0);
}

// Вывод

TEST_F(MatrixTest, OutputOperator) {
    Matrix m(2, 2, { {1.0, 2.0}, {3.0, 4.0} });
    std::stringstream ss;
    ss << m;
    std::string output = ss.str();
    // Проверяем, что вывод содержит числа
    EXPECT_TRUE(output.find("1") != std::string::npos);
    EXPECT_TRUE(output.find("2") != std::string::npos);
    EXPECT_TRUE(output.find("3") != std::string::npos);
    EXPECT_TRUE(output.find("4") != std::string::npos);
}

// Edge cases

TEST_F(MatrixTest, EmptyMatrixOperations) {
    Matrix m1;
    Matrix m2;
    EXPECT_NO_THROW(m1 + m2);
    EXPECT_NO_THROW(m1 - m2);
    EXPECT_NO_THROW(m1 * 2.0);
}

TEST_F(MatrixTest, SingleElementMatrix) {
    Matrix m(1, 1, { {42.0} });
    EXPECT_EQ(m.rows(), 1u);
    EXPECT_EQ(m.cols(), 1u);
    EXPECT_EQ(m[0][0], 42.0);
    EXPECT_EQ(m.determinant(), 42.0);
}

TEST_F(MatrixTest, LargeMatrix) {
    Matrix m(100, 100);
    for (size_t i = 0; i < 100; ++i) {
        for (size_t j = 0; j < 100; ++j) {
            m[i][j] = static_cast<double>(i * 100 + j);
        }
    }
    EXPECT_EQ(m[50][50], 5050.0);
    EXPECT_EQ(m[99][99], 9999.0);
}

// ============================================================
// Main
// ============================================================

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}