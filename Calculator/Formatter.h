#pragma once

#include <string>
#include <vector>
#include "CalculatorEnums.h"
using namespace std;

static class Formatter{
public:
	static vector<string> generatePostfix(const string& str, Order ord);	//Convert the given string to postfix notation

private:
	static int operatorPrecedence(char c, Order ord);	//Returns the precedence of the given operator
};

