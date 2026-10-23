#include <bits/stdc++.h>
using namespace std;

int main() {
	char op;
	double a, b, res;

	cout << "enter the op (+ - x /)";
	cin >> op;

	cout << "enter ur two numbers: ";
	cin >> a >> b;

	if (op ==  '+')
		res = a + b;
	else if (op == '-')
		res = a - b;
	else if (op == 'x')
		res = a * b;
	else if (op == '/')
		res = a / b;
	else {
		cout << "op isnt valid. list of ops is +, -, x, /.";
		res = -DBL_MAX;
	}
	if (res != -DBL_MAX)
		cout << "result: " << res;
	return 0;
}
