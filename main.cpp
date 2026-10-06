#include "Matrix.h"

int main() {
	double data[] = { 1.0, 2.0, 3.0,
		4.0, 5.0, 6.0,
		7.0, 8.0, 9.0 };
	Matrix mat1(3, 3, data);
	std::cout << mat1 << '\n';
	mat1 *= 2.0; // Умножение матрицы на скаляр
	std::cout << mat1 << '\n';
	mat1 += mat1;
	std::cout << mat1 << '\n';
	std::cout << mat1[0][0] << " " << mat1[1][1] << " " << mat1[2][2];
	Matrix mat3(2, 2, { {1,2}, {3,4} });
	std::cout << mat3 << '\n';
	std::cout << mat3.T() << '\n';
	std::cout << mat3*mat3 << '\n';
	// Пример использования MathVector
}