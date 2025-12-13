
#include "BitcoinExchange.hpp"

// Default constructor
BitcoinExchange::BitcoinExchange()
{
	// std::cout << "Default constructor called" << std::endl;
}

// Copy constructor
BitcoinExchange::BitcoinExchange(const BitcoinExchange& other)
	:_rates(other._rates)
{
	// std::cout << "Copy constructor called" << std::endl;
}

// Copy Assignment operator overload
BitcoinExchange &BitcoinExchange::operator=(const BitcoinExchange& other)
{
	// std::cout << "Copy assignment operator called" << std::endl;
	if (this != &other)
		this->_rates = other._rates;
	return (*this);
}

// Destructor
BitcoinExchange::~BitcoinExchange(void)
{
	// std::cout << "Destructor called" << std::endl;
}

bool	isValidDate(const std::string& date_str)
{
	//YYYY-MM-DD
	//Check length (4 + 1 + 2 + 1 + 2 = 10)
	if (date_str.length() != 10)
		return (false);

	//Check hyphen placement
	if (date_str[4] != '-' || date_str[7] != '-')
		return (false);
	int year, month, day;
	char hyph1, hyph2;

	std::istringstream iss(date_str);
	if (!(iss >> year >> hyph1 >> month >> hyph2 >> day))
		return (false);

	std::string junk;
	if (iss >> junk)
		return false;

	int daysInMonth[] =
	{
		31, 28, 31, 30, 31, 30,
		31, 31, 30, 31, 30, 31
	};

	bool isLeap = (year % 4 == 0 && year % 100 != 0) ||
		(year % 400 == 0);

	if (isLeap && month == 2)
		daysInMonth[1] = 29;

	if (day < 1 || day > daysInMonth[month - 1])
		return false;
	return (true);
}

void	BitcoinExchange::parseDatabaseLine(const std::string& line)
{
	std::string::size_type commaPos = line.find(',');
	if (commaPos == std::string::npos)
		throw InvalidLineError();

	std::string date = line.substr(0, commaPos);
	std::string price = line.substr(commaPos + 1);

	if (!isValidDate(date))
		throw InvalidDateError();

	double price_rate;
	std::istringstream iss2(price);
	if (!(iss2 >> price_rate))
		throw InvalidPriceError();

	std::string junk;
	if (iss2 >> junk)
		throw InvalidPriceError();

	if (price_rate < 0)
		throw InvalidPriceError();
	_rates[date] = price_rate;
	// debug
	// std::cout << "\nStream Status Check:" << std::endl;
	// std::cout << "  Good: " << (iss.good() ? "True" : "False") << std::endl;
	// std::cout << "  Fail: " << (iss.fail() ? "True" : "False") << std::endl;
	// std::cout << "  EOF:  " << (iss.eof() ? "True" : "False") << std::endl;
}

void	BitcoinExchange::loadDatabase()
{
	std::ifstream file(DATA_BASE_FILE_NAME);
	if (!file.is_open())
		throw OpenFileError();

	std::string line;
	if (!std::getline(file, line)) //Check if file can be read at at all
		throw ColumnInvalidError();
	line.erase(line.find_last_not_of(" \t\r\n") + 1);
	if (line != "date,exchange_rate")
		throw ColumnInvalidError();

	while (std::getline(file, line))
	{
		parseDatabaseLine(line);
	}
}
