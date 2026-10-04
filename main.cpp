#include "MathVector.h"

int main() {
	MathVector<int> vec1({1,2,3});
	MathVector<int> vec2({6,7,8});
	int a[] = { 4,5 };
	int b[] = { -1,-2 };
	vec1.insertMany(a, 2, 0);
	vec2.insertMany(b, 2, 3);

	std::cout << vec1 * vec2 << '\n';
	std::cout << vec1 << '\n';
	std::cout << vec2 << '\n';
}