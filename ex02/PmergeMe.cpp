/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PmergeMe.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbui-quo <tbui-quo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/06 15:47:56 by tbui-quo          #+#    #+#             */
/*   Updated: 2026/01/06 15:47:57 by tbui-quo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "PmergeMe.hpp"

#include <cstdlib>
#include <limits>

// Default constructor
PmergeMe::PmergeMe()
{
}

PmergeMe::PmergeMe(char *argv[])
{
	parseInput(argv);
}

// Copy constructor
PmergeMe::PmergeMe(const PmergeMe &other)
	:_vec(other._vec), _deq(other._deq)
{
}

// Copy Assignment operator overload
PmergeMe &PmergeMe::operator=(const PmergeMe &other)
{
	if (this != &other)
	{
		_vec = other._vec;
		_deq = other._deq;
	}
	return (*this);
}

// Destructor
PmergeMe::~PmergeMe(void)
{
}

void PmergeMe::parseInput(char *argv[])
{
	for (int i = 0; argv[i]; ++i)
	{
		std::stringstream ss(argv[i]);
		std::string word;
		bool foundNumberInArg = false;

		while (ss >> word)
		{
			char *end;
			long value = std::strtol(word.c_str(), &end, 10);

			if (*end != '\0' || value <= 0 || value > std::numeric_limits<int>::max())
				throw std::runtime_error("Invalid input");

			_vec.push_back(static_cast<int>(value));
			_deq.push_back(static_cast<int>(value));
			foundNumberInArg = true;
		}

		if (!foundNumberInArg)
			throw std::runtime_error("Empty argument provided");
	}
}

void PmergeMe::handleSmallInput() const
{
	std::cout << "Before: ";
	printShortSequence(_vec);

	std::cout << "After:  ";
	printShortSequence(_vec);

	std::cout << "Time to process a range of " << _vec.size()
			  << " elements with std::vector : 0.00000 us" << std::endl;

	std::cout << "Time to process a range of " << _vec.size()
			  << " elements with std::deque  : 0.00000 us" << std::endl;
}

int PmergeMe::jacobsthal(int n)
{
	if (n == 0)
		return 0;
	if (n == 1)
		return 1;

	int a = 0; // same as J(n-2)
	int b = 1; // same as J(n-1)
	for (int i = 2; i <= n; ++i)
	{
		int c = b + 2 * a; //J(n) = J(n-1) + 2 * J(n-2)
		a = b; //shift J(n-2) forward
		b = c; //shift J(n-1) forward
	}
	return b;
}

void PmergeMe::binaryInsert(std::deque<int> &arr, int value)
{
	if (arr.empty())
	{
		arr.push_back(value);
		return;
	}
	int left = 0;
	int right = arr.size() - 1;
	while (left <= right)
	{
		int middle = (left + right) / 2;
		if (value <= arr[middle])
			right = middle - 1 ;
		else if (value > arr[middle])
			left = middle + 1;
	}
	arr.insert(arr.begin() + left , value);
}

std::deque<int> PmergeMe::fordJohnsonDeque(const std::deque<int> &input)
{
	if (input.size() <= 1)
		return input;

	// 1. Create ordered pairs
	std::deque<Pair> pairs;

	size_t i = 0;
	for (; i + 1 < input.size(); i += 2)
		pairs.push_back(Pair(input[i], input[i + 1]));

	// 1.5 handle odd element
	int odd = -1;
	if (i < input.size())
		odd = input[i];

	// 2. Recursively sort pairs by their large element
	std::deque<int> leaders;
	for (size_t k = 0; k < pairs.size(); ++k)
		leaders.push_back(pairs[k].large);

	leaders = fordJohnsonDeque(leaders);

	// 3. Find each leader's original pair and move it to sortedPairs
	std::deque<Pair> sortedPairs;
	for (size_t l = 0; l < leaders.size(); ++l)
	{
		for (size_t p = 0; p < pairs.size(); ++p)
		{
			if (pairs[p].large == leaders[l])
			{
				sortedPairs.push_back(pairs[p]);
				pairs.erase(pairs.begin() + p);
				break;
			}
		}
	}

	// 4. Build MAIN and PEND
	std::deque<int> main;
	std::deque<int> pend;

	main.push_back(sortedPairs[0].small);
	main.push_back(sortedPairs[0].large);

	for (size_t k = 1; k < sortedPairs.size(); ++k)
	{
		main.push_back(sortedPairs[k].large);
		pend.push_back(sortedPairs[k].small);
	}

	// 5. Jacobsthal insertion
	size_t prev = 0;
	for (int j = 1; ; ++j)
	{
		size_t bound = jacobsthal(j);
		if (bound >= pend.size())
			bound = pend.size();

		for (size_t x = bound; x-- > prev;)
			binaryInsert(main, pend[x]);

		if (bound == pend.size())
			break;

		prev = bound;
	}

	// 6. Insert odd element
	if (odd != -1)
		binaryInsert(main, odd);

	return main;
}

void PmergeMe::binaryInsert(std::vector<int> &arr, int value)
{
	if (arr.empty())
	{
		arr.push_back(value);
		return;
	}
	int left = 0;
	int right = arr.size() - 1;
	while (left <= right)
	{
		int middle = (left + right) / 2;
		if (value <= arr[middle])
			right = middle - 1 ;
		else if (value > arr[middle])
			left = middle + 1;
	}
	arr.insert(arr.begin() + left , value);
}

std::vector<int> PmergeMe::fordJohnsonVector(const std::vector<int> &input)
{
	if (input.size() <= 1)
		return input;

	// 1. Create ordered pairs
	std::vector<Pair> pairs;
	pairs.reserve(input.size() / 2);

	size_t i = 0;
	for (; i + 1 < input.size(); i += 2)
		pairs.push_back(Pair(input[i], input[i + 1]));

	// 1.5 handle odd element
	int odd = -1;
	if (i < input.size())
		odd = input[i];

	// 2. Recursively sort pairs by their large element
	std::vector<int> leaders;
	for (size_t k = 0; k < pairs.size(); ++k)
		leaders.push_back(pairs[k].large);

	leaders = fordJohnsonVector(leaders);

	// 3. Find each leader's original pair and move it to sortedPairs to preserve the small-large relationship
	std::vector<Pair> sortedPairs;
	for (size_t l = 0; l < leaders.size(); ++l)
	{
		for (size_t p = 0; p < pairs.size(); ++p)
		{
			if (pairs[p].large == leaders[l])
			{
				sortedPairs.push_back(pairs[p]);
				pairs.erase(pairs.begin() + p);
				break;
			}
		}
	}
	// 4. Build MAIN and PEND
	std::vector<int> main;
	std::vector<int> pend;

	main.push_back(sortedPairs[0].small);
	main.push_back(sortedPairs[0].large);
	for (size_t k = 1; k < sortedPairs.size(); ++k)
	{
		main.push_back(sortedPairs[k].large);
		pend.push_back(sortedPairs[k].small);
	}

	//5. Jacobsthal insertion
	size_t prev = 0;
	for (int j = 1; ; ++j)
	{
		size_t bound = jacobsthal(j);
		if (bound >= pend.size())
			bound = pend.size();

		for (size_t x = bound; x-- > prev;)
			binaryInsert(main, pend[x]);

		if (bound == pend.size())
			break;

		prev = bound;
	}

	//6. Insert odd element
	if (odd != -1)
		binaryInsert(main, odd);

	return main;
}

void PmergeMe::run()
{
	if (_vec.size() <= 1)
	{
		handleSmallInput();
		return;
	}

	std::cout << "Before: ";
	printShortSequence(_vec);

	// VECTOR SORT
	clock_t start_vec = clock();
	std::vector<int> sortedVec = fordJohnsonVector(_vec);
	clock_t end_vec = clock();

	double time_vec =
		static_cast<double>(end_vec - start_vec) * 1e6 / CLOCKS_PER_SEC;

	// AFTER
	std::cout << "After:  ";
	printShortSequence(sortedVec);

	// VECTOR TIME
	std::cout << "Time to process a range of " << _vec.size()
			  << " elements with std::vector : "
			  << std::fixed << std::setprecision(5)
			  << time_vec << " us" << std::endl;

	// DEQUE SORT
	clock_t start_deq = clock();
	std::deque<int> sortedDeq = fordJohnsonDeque(_deq);
	clock_t end_deq = clock();

	double time_deq =
		static_cast<double>(end_deq - start_deq) * 1e6 / CLOCKS_PER_SEC;

	// DEQUE TIME
	std::cout << "Time to process a range of " << _deq.size()
			  << " elements with std::deque  : "
			  << std::fixed << std::setprecision(5)
			  << time_deq << " us" << std::endl;
}
