
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

bool	BitcoinExchange::isValidDate(const std::string& date_str)
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
		return (false);

	int daysInMonth[] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

	bool isLeap = (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);

	if (isLeap && month == 2)
		daysInMonth[1] = 29;

	if (month < 1 || month > 12)
		return (false);

	if (day < 1 || day > daysInMonth[month - 1])
		return (false);

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
	std::istringstream iss(price);
	if (!(iss >> price_rate))
		throw InvalidPriceError();

	std::string junk;
	if (iss >> junk)
		throw InvalidPriceError();

	if (price_rate < 0)
		throw InvalidPriceError();
	_rates[date] = price_rate;
}

void	BitcoinExchange::loadDatabase()
{
	std::ifstream file(DATA_BASE_FILE_NAME);
	if (!file.is_open())
		throw OpenFileError();

	std::string line;
	if (!std::getline(file, line)) //Check if file can be read at all
		throw ColumnInvalidError();
	line.erase(line.find_last_not_of(" \t\r\n") + 1);
	if (line != "date,exchange_rate")
		throw ColumnInvalidError();

	while (std::getline(file, line))
	{
		if (line.empty())
			continue;
		parseDatabaseLine(line);
	}
	file.close();
}

std::string BitcoinExchange::trim(const std::string& str)
{
	const std::string whitespace = " \n\r\t";

	std::string::size_type start = str.find_first_not_of(whitespace);;
	if (start == std::string::npos)
		return "";

	std::string::size_type end = str.find_last_not_of(whitespace);
	return str.substr(start,end - start + 1);
}

double BitcoinExchange::getExchangeRate(const std::string& date) const
{
	// upper_bound returns the first element STRICTLY GREATER than date
	std::map<std::string, double>::const_iterator it = _rates.upper_bound(date);

	// If it's at the beginning, even the first date in DB is newer than input
	if (it == _rates.begin())
		throw DateTooEarlyError();

	// Move back one step to get the date <= input
	--it;
	return it->second;
}

void	BitcoinExchange::processUserLine(const std::string& line)
{
	std::string::size_type sep= line.find(" | "); // similar to size_t but more specific for strings
	if (sep == std::string::npos)
	{
		std::cout << "Error: missing seperator | => " << line << std::endl;
		return;
	}

	std::string date = trim(line.substr(0, sep));
	std::string valueStr = trim(line.substr(sep + 3));

	if (date.empty()) //Check if date becomes empty after trimming
	{
		std::cout << "Error: date.empty | => " << line << std::endl;
		return;
	}

	if (!isValidDate(date))
	{
		std::cout << "Error: bad date input => " << date << std::endl;
		return;
	}

	double value;
	std::istringstream iss(valueStr);
	if (!(iss >> value))
	{
		std::cout << "Error: bad value input => " << valueStr <<std::endl;
		return;
	}

	std::string junk;
	if (iss >> junk)
	{
		std::cout << "Error: extras input after value: " << value << " => "<< junk << std::endl;
		return;
	}

	if (value < 0)
	{
		std::cout << "Error: not a positive number => " << value << std::endl;
		return;
	}

	if (value == 0)
		value = 0;

	if (value > 1000)
	{
		std::cout << "Error: too large number => " << value << std::endl;
		return;
	}

	double rate;
	try
	{
		rate = getExchangeRate(date);
	}
	catch (const std::exception& e)
	{
		std::cout << "Error: " << e.what() << std::endl;
		return;
	}
	std::cout << date << " => " << value << " = " << (value * rate) << std::endl;
}

void	BitcoinExchange::processInputFile(const std::string& filename)
{
	std::ifstream inputFile(filename.c_str());
	if (!inputFile.is_open())
	{
		std::cout << "Error: could not open file." << std::endl;
		return;
	}

	std::string line;
	if (!std::getline(inputFile, line))
	{
		std::cout << "Error: could not parse input file because file is empty." << std::endl;
		return;
	}

	if (line != "date | value")
	{
		std::cout << "Error: bad input => " << line << std::endl;
		return;
	}

	while (std::getline(inputFile, line))
	{
		if (line.empty())
			continue;
		processUserLine(line);
	}
	inputFile.close();
}