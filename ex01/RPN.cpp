
#include "RPN.hpp"
#include <sstream>
#include <stdexcept>
#include <cctype>
#include <iostream>

// Default constructor
RPN::RPN()
{}

// Copy constructor
RPN::RPN(const RPN &other)
	:_stack(other._stack)
{}

// Copy Assignment operator overload
RPN &RPN::operator=(const RPN &other)
{
	if (this != &other)
		this->_stack = other._stack;
	return (*this);
}

// Destructor
RPN::~RPN(void)
{}

int RPN::evaluate(const std::string& expression)
{
	std::istringstream iss(expression);
	std::string token;

	while (iss >> token)
	{
		if (token.length() == 1 && std::isdigit(token[0]))
			_stack.push(token[0] - '0');

		else if ((token.length() == 1) && (token[0] == '+' || token[0] == '-' ||
				token[0] == '*' || token[0] == '/'))
			performOperation(token[0]);
		else
			throw std::runtime_error("Not allowed expression");
	}

	if (_stack.size() != 1)
	{
		throw std::runtime_error("Multiple Error types possible");
	}
	return _stack.top();
}

void RPN::performOperation(char op)
{
	if (_stack.size() < 2)
		throw std::runtime_error("Not enough numbers in stack");

	int b = _stack.top();
	_stack.pop();
	int a = _stack.top();
	_stack.pop();

	int result;

	if (op == '+')
		result = a + b;
	else if (op == '-')
		result = a - b;
	else if (op == '*')
		result = a * b;
	else if (op == '/')
	{
		if (b == 0)
			throw std::runtime_error("Divide by zero not allowed");
		result = a / b;
	}
	else
		throw std::runtime_error("Not reachable Error :)");

	_stack.push(result);
}
