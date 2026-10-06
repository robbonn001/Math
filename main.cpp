#include "Matrix.h"

int main() {
	//Matrix mat0(2, 2);
	//std::cin >> mat0;
	//std::cout << mat0 << '\n';
	double data[] = { 1.0, 2.0, 3.0,
		4.0, 5.0, 6.0,
		7.0, 8.0, 9.0 };
	Matrix mat1(3, 3, data);
	std::cout << mat1 << '\n';
	mat1 *= 2.0; // Умножение матрицы на скаляр
	std::cout << mat1 << '\n';
	mat1 += mat1;
	std::cout << mat1 << '\n';
	std::cout << mat1[0][0] << " " << mat1[1][1] << " " << mat1[2][2] << '\n';
	Matrix mat3(2, 2, { {1,2}, {3,4} });
	std::cout << mat3 << '\n';
	std::cout << mat3.T() << '\n';
	std::cout << mat3*mat3 << '\n';
	MathVector<MathVector<double>> temp(5);
	for (int i = 0;i < 5;++i) {
		temp[i] = MathVector<double>(5);
		for (int j = 0;j < 5;++j) {
			temp[i][j] = i + j;
		}
	}
	Matrix mat4(temp);
	std::cout << mat4 << '\n';
	Matrix mat5(mat4);
	std::cout << mat5 << '\n';
}