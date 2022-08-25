#include "pch.h"
#include "Calculator.h"
#include <stdexcept>
#include <stack>
#include <math.h>
#include <cstdlib>

Calculator::Calculator(int maxDec) : maxDecimals{maxDec}{}	//Constructor

/*
	Converts string input to a postfix vector, and passes
	that vector to the private calculation method.
*/
string Calculator::calculate(const string& calc) {
	double result = calculate(Formatter::generatePostfix(calc, order));
	if (disregardCalc) {
		disregardCalc = false;
		return calc;
	}
	else
		return formatOutput(result);
}

/*
	Sets the value stored in memory
*/
void Calculator::setMemory(const string& str) {
	if (isNumber(str)) {
		memory = stod(str);
	}
	else {
		isError = true;
		errorType = ErrorType::MEMORY;
	}
}

/*
	Returns a reference to the value stored in memory
*/
string& Calculator::getMemory() {
	return to_string(memory);
}

/*
	Adds the given value to the value stored in memory
*/
void Calculator::memoryAdd(const string& val) {
	if (isNumber(val)) {
		memory = stod(val) + memory;
	}
	else {
		isError = true;
		errorType = ErrorType::MEMORY;
	}
}

/*
	Subtracts the given value from the value stored in memory
*/
void Calculator::memorySub(const string& val) {
	if (isNumber(val)) {
		memory = memory - stod(val);
	}
	else {
		isError = true;
		errorType = ErrorType::MEMORY;
	}
}

/*
	Returns true if memory is not zero
*/
bool Calculator::hasMemory() {
	if (memory != 0)
		return true;
	else
		return false;
}

/*
	Returns true if there is an error
*/
bool Calculator::hasError() {
	return isError;
}

/*
	Clears the error flag
*/
void Calculator::clearError() {
	isError = false;
	errorType = ErrorType::NONE;
}

/* 
	Returns the type of the current error
*/
ErrorType Calculator::getErrorType() {
	return errorType;
}

/*
	Sets the maximum number of decimal places
*/
void Calculator::setMaxDecimals(int max) {
	maxDecimals = max;
}

/*
	Sets the order of operations
*/
void Calculator::setOrder(Order ord) {
	order = ord;
}

/*
	Returns the order of operations
*/
Order Calculator::getOrder() {
	return order;
}

/*
	Performs evaluation of a postfix expression
*/
double Calculator::calculate(const vector<string>& postfix) {
	double finalResult;
	double operationResult{ 0 };
	double op1, op2;
	stack<double> stack;
	size_t size = postfix.size();

	//Iterate through each item in the postfix list
	for (int i = 0; i < size; ++i) {

		string item{ postfix.at(i) };

		//Push numbers onto the stack
		if (isNumber(item))
			stack.push(stod(item));

		/*
			Square roots and trigonometric functions are unary
			operators, so they are handled separately here.
		*/
		else if (item.at(0) == SQRT || item.at(0) == SIN || item.at(0) == COS || item.at(0) == TAN) {

			/*
				A running calculation style of order of operations doesn't work with
				the same methods of square roots, as it is a unary operator, so the
				running calculations try to calculate before there is sufficient input.
				This will always cause an input error before a square root sign, which
				is addressed here.
			*/
			if (order == Order::RUNNING && errorType == ErrorType::INPUT) {
				clearError();

				/*
					If the unary operator is the last item on the list and is not preceded by
					a number in the postfix list, set the flag to disregard the calculation for
					now, and return an arbitrary value.
				*/
				char currentOp{ postfix.at(postfix.size() - 1).at(0) };
				if ((currentOp == SQRT || currentOp == SIN || currentOp == COS || currentOp == TAN) 
						&& !isNumber(postfix.at(postfix.size() - 2))) {
					disregardCalc = true;
					return 0;
				}

				//Split the operation into two, with the unary operator marking the divide
				string lastOperator;

				vector<string>::const_iterator firstA = postfix.begin();
				vector<string>::const_iterator lastA = postfix.begin() + i - 1;
				vector<string> subVectorA(firstA, lastA);

				vector<string>::const_iterator firstB = postfix.begin() + i - 1;
				vector<string>::const_iterator lastB = postfix.begin() + i + 1;
				vector<string> subVectorB(firstB, lastB);

				//If the first half is greater than three items, there will be one operator
				//that needs to be moved to the end of the expression
				if (!isNumber(postfix.at(i - 2))) {
					lastOperator = subVectorA.back();
					subVectorA.pop_back();
				}

				//Calculate the result of the unary operation
				operationResult = calculate(subVectorB);

				//Add the result to the other half
				subVectorA.push_back(to_string(operationResult));

				/*
					If the list goes beyond the square root sign, the remaining end is made into
                    a third list, to be added back once the square root in the middle has been
                    evaluated.
				*/
				size_t size{ postfix.size() };
				if (size > i + 1)
					for (int j = i + 1; j < size; ++j)
						subVectorA.push_back(postfix.at(j));
				/*
					Push the result onto the stack. This set of operations bypasses the regular
					logic, as the evaluation took place in recursive calls to this function, so
					we break out of the loop.
				*/
				stack.push(calculate(subVectorA));
				break;
			}
			
			if (stack.empty()) {
				isError = true;
				errorType = ErrorType::INPUT;
			}
			else {
				op1 = stack.top();
				stack.pop();

				switch (item.at(0)) {
				case SQRT:
					//Prevent square root of negative number
					if (op1 < 0) {
						isError = true;
						errorType = ErrorType::NEQSQRT;
					}
					else {
						//Calculate square root
						operationResult = sqrt(op1);
					}
					break;
				case SIN:
					//Calculate sine
					operationResult = sin(op1);
					break;
				case COS:
					//Calculate cosine
					operationResult = cos(op1);
					break;
				case TAN:
					operationResult = tan(op1);
					break;
				}
				stack.push(operationResult);
			}		
		}
		/*
			If there is an operator, pop two operands off the stack and
			perform the calculation.
		*/
		else {
			char c = item.at(0);
			if (stack.empty()) {
				isError = true;
				errorType = ErrorType::INPUT;
			}
			else {
				op2 = stack.top();
				stack.pop();
				op1 = stack.top();
				stack.pop();

				//Determine calculation type, and perform it
				switch (c) {
				case EXP:
					operationResult = pow(op1, op2);
					break;
				case MULT:
					operationResult = op1 * op2;
					break;
				case DIV:
					if (op2 == 0) {
						isError = true;
						errorType = ErrorType::DIVZERO;
					}
					else
						operationResult = op1 / op2;
					break;
				case ADD:
					operationResult = op1 + op2;
					break;
				case SUB:
					operationResult = op1 - op2;
					break;
				}
				stack.push(operationResult);
			}
		}
	}
	//Check if an error occured during calculation
	if (isError)
		return 0;
	else {
		if (stack.empty()) {
			isError = true;
			errorType = ErrorType::INPUT;
			finalResult = 0;
		}
		else {
			finalResult = stack.top();
			stack.pop();
		}
		return finalResult;
	}
}

/*
	Returns true if the given string represents a correctly
	formatted number.
*/
bool Calculator::isNumber(const string& str) {
	char* end = nullptr;
	double val = strtod(str.c_str(), &end);
	return end != str.c_str() && *end == '\0' && val != HUGE_VAL;
}

/*
	Formats the output result as a string for display
*/
string& Calculator::formatOutput(double num) {
	return to_string(num);
}