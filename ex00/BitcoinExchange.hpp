/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   BitcoinExchange.hpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbui-quo <tbui-quo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/06 15:47:15 by tbui-quo          #+#    #+#             */
/*   Updated: 2026/01/06 15:47:16 by tbui-quo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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

		static std::string trim(const std::string& str);
		static bool isValidDate(const std::string& date_str);

		void	parseDatabaseLine(const std::string& line);
		void	processUserLine(const std::string& line);
		double	getExchangeRate(const std::string& date) const;
	public:
		BitcoinExchange();
		BitcoinExchange(const BitcoinExchange &other);
		BitcoinExchange& operator=(const BitcoinExchange &other);
		~BitcoinExchange();

		void	loadDatabase();
		void	processInputFile(const std::string& filename);
	class OpenFileError : public std::exception {
		virtual const char* what() const throw() { return "Error: could not open file."; }
	};
	class ColumnInvalidError : public std::exception {
		virtual const char* what() const throw() { return "Error: invalid database header."; }
	};
	class InvalidLineError : public std::exception {
		virtual const char* what() const throw() { return "Error: database line format invalid."; }
	};
	class InvalidDateError : public std::exception {
		virtual const char* what() const throw() { return "Error: invalid date format."; }
	};
	class InvalidPriceError : public std::exception {
		virtual const char* what() const throw() { return "Error: invalid price in database."; }
	};
	class DateTooEarlyError : public std::exception {
		virtual const char* what() const throw() { return "Date too early"; }
	};
};
