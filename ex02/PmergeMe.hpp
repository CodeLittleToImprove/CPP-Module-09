/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PmergeMe.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbui-quo <tbui-quo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/06 15:48:03 by tbui-quo          #+#    #+#             */
/*   Updated: 2026/01/06 15:48:04 by tbui-quo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

# include <iostream>
# include <vector>
# include <deque>
# include <ctime>
# include <iomanip> // for setprecision
# include <sstream>

struct Pair
{
	int small;
	int large;

	Pair(int a, int b)
	{
		if (a < b)
		{
			small = a;
			large = b;
		}
		else
		{
			small = b;
			large = a;
		}
	}
};

class PmergeMe
{
	private:
		std::vector<int> _vec;
		std::deque<int> _deq;
		void parseInput(char *argv[]);
		void handleSmallInput() const;
		void binaryInsert(std::vector<int>& arr, int value);
		void binaryInsert(std::deque<int>& arr, int value);
		int jacobsthal(int n);
		std::vector<int> fordJohnsonVector(const std::vector<int> &input);
		std::deque<int> fordJohnsonDeque(const std::deque<int> &input);
		template <typename T>
		void printSequence(const T& seq) const
		{
			for (size_t i = 0; i < seq.size(); i++)
				std::cout << seq[i] << " ";
			std::cout << std::endl;
		}
		template <typename T>
		void printShortSequence(const T& seq) const
		{
			size_t size = seq.size();
			if (size < 5)
			{
				printSequence(seq);
				return;
			}
			for (size_t i = 0; i < 4; i++)
				std::cout << seq[i] << " ";
			std::cout << "[...] " << seq[size -1] << std::endl;
		}
		// template <typename T> helper function to print single steps
		// void printStep(const std::string &label, const T &c)
		// {
		// 	std::cout << label << ": ";
		// 	for (typename T::const_iterator it = c.begin(); it != c.end(); ++it)
		// 		std::cout << *it << " ";
		// 	std::cout << std::endl;
		// }
	public:
		PmergeMe();
		PmergeMe(char *argv[]);
		PmergeMe(const PmergeMe &other);
		PmergeMe& operator=(const PmergeMe &other);
		~PmergeMe();

		void run();
};

