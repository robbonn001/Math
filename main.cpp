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

	MathVector<int> vec3 = vec1 + vec2;
	MathVector<int> vec4 = vec3;
	std::cout << (vec3 == vec4) << '\n';

	const MathVector<int> vec5({ 1,2,3 });
	std::cout << vec5 << '\n';

	MathVector<int> vec6({ 1,2,3 });

	vec6.popFront();
	vec6.pushBack(4);

	std::cout << vec6 << '\n';
}