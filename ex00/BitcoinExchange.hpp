#pragma once

# include <iostream>
# include <fstream>
# include <sstream>
# include <map>
# include <exception>
# include <string>

#define DATA_BASE_FILE_NAME "data.csv"
class BitcoinExchange
{
	private:
		std::map<std::string, double> _rates;
	public:
		BitcoinExchange();
		BitcoinExchange(const BitcoinExchange &other);
		BitcoinExchange& operator=(const BitcoinExchange &other);
		~BitcoinExchange();

		void	loadDatabase();
		void	processInputFile(const std::string& filename);

	private:
		void	parseDatabaseLine(const std::string& line);
		void	processLine(const std::string& line);

		// bool	isValidDate(const std::string &date_str);
		// bool	isLeapYear(int year) const;
		// bool	isValidValue(const std::string& value) const;
		// bool	isNumber(const std::string& value) const;

		std::string findClosestLowerData(const std::string& date) const;
		double	getExchangeRate(const std::string& date) const;

		void	printError(const std::string& message) const;

	class OpenFileError : public std::exception
	{
	public:
		virtual const char* what() const throw()
		{
			return ("File could not be opened");
		}
	};

	class ColumnInvalidError : public std::exception
	{
	public:
		virtual const char* what() const throw()
		{
			return ("Invalid column error. Should be 'date,exchange_rate'.");
		}
	};

	class InvalidLineError : public std::exception
	{
	public:
		virtual const char* what() const throw()
		{
			return ("Invalid line error. A token is missing");
		}
	};
	class InvalidDateError : public std::exception
	{
	public:
		virtual const char* what() const throw()
		{
			return ("Invalid Date error. Something is wrong with the date");
		}
	};
	class InvalidPriceError : public std::exception
	{
	public:
		virtual const char* what() const throw()
		{
			return ("Invalid line error. Something is wrong with the price");
		}
	};
};
