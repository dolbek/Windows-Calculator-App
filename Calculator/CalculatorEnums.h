#pragma once

//List of operators
enum class Operator { ADD = '+', SUB = '-', MULT = '*', DIV = '/', EXP = '^', SQRT = '\u221A', SIN = 'sin', COS = 'cos', TAN = 'tan' };

//Different options for order of operations
enum class Order { REGULAR, RUNNING };

//Error types
enum class ErrorType { NONE, INPUT, DIVZERO, NEQSQRT, OTHER, MEMORY };