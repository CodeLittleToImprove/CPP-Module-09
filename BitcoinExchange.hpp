#pragma once

# include <iostream>
# include <fstream>
# include <sstream>
# include <map>
# include <exception>
# include <string>

class BitcoinExchange
{
	private:
		std::map<std::string, double> _rates;
	public:
		BitcoinExchange();
		BitcoinExchange(const BitcoinExchange &other);
		BitcoinExchange& operator=(const BitcoinExchange &other);
		~BitcoinExchange();

		void	loadDatabase(const std::string& filename);
		void	processInputFile(const std::string& filename);

	private:
		void	parseDatabaseLine(const std::string& line);
		void	processLine(const std::string& line);

		bool	isValidDate(const std::string &data) const;
		bool	isLeapYear(int year) const;
		bool	isValidValue(const std::string& value) const;
		bool	isNumber(const std::string& value) const;

		std::string findClosestLowerData(const std::string& date) const;
		double	getExchangeRate(const std::string& date) const;

		void	printError(const std::string& message) const;
};
