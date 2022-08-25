#pragma once

//Constants
const char ADD = '+';
const char SUB = '-';
const char MULT = '*';
const char DIV = '/';
const char EXP = '^';
const char SQRT = '\u221A';
const char SIN = 's';
const char COS = 'c';
const char TAN = 't';
const char SCINOT = 'E';

//List of operators
enum class Operator { ADD = ADD, SUB = SUB, MULT = MULT, DIV = DIV, EXP = EXP, SQRT = SQRT, SIN = SIN, COS = COS, TAN = TAN };

//Different options for order of operations
enum class Order { REGULAR, RUNNING };

//Error types
enum class ErrorType { NONE, INPUT, DIVZERO, NEQSQRT, OTHER, MEMORY };