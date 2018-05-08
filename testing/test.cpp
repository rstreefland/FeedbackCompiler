#include <iostream>
#include <cmath>
using namespace std;

int main(void) { 

	float var1 = 2;
	float pi = 3.14;

	while (var1 <= 5) {
	cout << "In while loop" << endl;
	var1 = var1 + 1;
	cout << var1 << endl;
	}

	do {
	cout << "In do-while loop" << endl;
	var1 = var1 + 1;
	cout << var1 << endl;
	} while (var1 < 8);
	float var2 = 8.27684;

	if (var2 > var1) {
	cout << "In if statement" << endl;
	cout << var2 << endl;
	}

	if (pi != 3.14) {
	cout << "NOT EQUAL" << endl;
	} else {
	cout << "EQUAL" << endl;
	}

	cout << "Printing an different NaN:" << endl;
	float var3 = 0;
	var3 = NAN;
	cout << var3 << endl;
	cout << "Printing positive infinity:" << endl;
	float var4 = 9;
	var4 = INFINITY;
	cout << var4 << endl;
	cout << "Printing negative infinity:" << endl;
	float var5 = -4;
	var5 = -INFINITY;
	cout << var5 << endl;
	cout << "Printing negative zero:" << endl;
	float var6 = 0;
	cout << var6 << endl;
	var6 = 8;
	var6 = -17.3333;
	cout << var5 << endl;
}
