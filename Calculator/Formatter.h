#pragma once

#include <string>
#include <vector>
#include "CalculatorEnums.h"
using namespace std;

static class Formatter{
public:
	vector<string> generatePostfix(string& str);	//Convert the given string to postfix notation

private:
	int operatorPrecedence(Operator op);	//Returns the precedence of the given operator
};

