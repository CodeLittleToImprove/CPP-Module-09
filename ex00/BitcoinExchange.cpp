
#include "BitcoinExchange.hpp"

// Default constructor
BitcoinExchange::BitcoinExchange()
{
	// std::cout << "Default constructor called" << std::endl;
}

// Copy constructor
BitcoinExchange::BitcoinExchange(const BitcoinExchange &other)
	:_atribute(other._atribute)
{
	// std::cout << "Copy constructor called" << std::endl;
}

// Copy Assignment operator overload
BitcoinExchange &BitcoinExchange::operator=(const BitcoinExchange &other)
{
	// std::cout << "Copy assignment operator called" << std::endl;
	if (this != &other)
		this->member = other.member();
	return (*this);
}

// Destructor
BitcoinExchange::~BitcoinExchange(void)
{
	// std::cout << "Destructor called" << std::endl;
}


