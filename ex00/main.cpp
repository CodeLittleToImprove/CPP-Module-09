/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbui-quo <tbui-quo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 16:11:14 by tbui-quo          #+#    #+#             */
/*   Updated: 2026/01/06 15:47:26 by tbui-quo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "BitcoinExchange.hpp"

int main(int argc, char *argv[])
{
	if (argc != 2)
	{
		std::cout << "Error: could not open file." << std::endl;
		std::cerr << "Usage: ./btc <filename>" << std::endl;
		return (1);
	}

	std::string inputFile = argv[1];
	try
	{
		BitcoinExchange exchange;
		exchange.loadDatabase();
		exchange.processInputFile(inputFile);
	}
	catch (const std::exception& e)
	{
		std::cerr << e.what() << std::endl;
	}
	return (0);
}
