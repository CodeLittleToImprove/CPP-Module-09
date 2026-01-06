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

	// 1. Create pairs && 2. Sort inside each pair
	int odd_element = -1;
	if (input.size() % 2 != 0)
		odd_element = input.back();

	std::deque<Pair> temp;
	size_t start = 0;

	while (start < input.size() && start + 1 < input.size())
	{
		temp.push_back(Pair(input[start], input[start + 1]));
		start += 2;
	}

	// 3. Extract the leaders (the aᵢ elements)
	std::deque<int> A;
	for (std::deque<Pair>::iterator it = temp.begin(); it != temp.end(); ++it)
	{
		A.push_back(it->large);
	}

	// 4. Recursively sort the leaders (aᵢ)
	A = fordJohnsonDeque(A);

	// 4.5 Extract the losers (bᵢ)
	std::deque<int> B;
	for (std::deque<Pair>::iterator it = temp.begin(); it != temp.end(); ++it)
	{
		B.push_back(it->small);
	}

	// 5. Build main
	std::deque<int> main;
	main.push_back(B[0]);
	main.push_back(A[0]);
	if (A.size() > 1)
	{
		for (std::deque<int>::iterator it = A.begin() + 1; it != A.end(); ++it)
		{
			main.push_back(*it);
		}
	}
	// 5.5 Build pend
	std::deque<int> pend;
	if (B.size() > 1)
	{
		for (std::deque<int>::iterator it = B.begin() + 1; it != B.end(); ++it)
		{
			pend.push_back(*it);
		}
	}
	if (odd_element != -1)
		pend.push_back(odd_element);

	// 6. Insert pend elements into main using Jacobsthal boundaries
	std::deque<int> boundaries;
	int previous = 0;
	int current = 0;
	for (int i = 1; ; ++i)
	{
		size_t J = jacobsthal(i);
		current = J;
		if (J >= pend.size())
		{
			boundaries.push_back(pend.size()); // end boundary
			break;
		}
		if (previous == current)
			continue;
		boundaries.push_back(J);
		previous = J;
	}
	// 6.2 Convert boundaries to ranges and binary-insert
	previous = 0;
	for (std::deque<int>::iterator b_it = boundaries.begin(); b_it != boundaries.end(); ++b_it) // declares how many pends are allowed to inserted in one loop
	{
		int boundary = *b_it;
		for (int i = boundary; i > previous; --i) // reverse order
		{
			int value = pend[i - 1];
			binaryInsert(main, value);
		}
		previous = boundary;
	}

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

	// 1. Create pairs && 2. Sort inside each pair
	int odd_element = -1;
	if (input.size() % 2 != 0)
		odd_element = input.back();

	std::vector<Pair> temp;
	size_t start = 0;

	while (start < input.size() && start + 1 < input.size())
	{
		temp.push_back(Pair(input[start], input[start + 1]));
		start += 2;
	}

	// 3. Extract the leaders (the aᵢ elements)
	std::vector<int> A;
	for (std::vector<Pair>::iterator it = temp.begin(); it != temp.end(); ++it)
	{
		A.push_back(it->large);
	}

	// 4. Recursively sort the leaders (aᵢ)
	A = fordJohnsonVector(A);

	// 4.5 Extract the losers (bᵢ)
	std::vector<int> B;
	for (std::vector<Pair>::iterator it = temp.begin(); it != temp.end(); ++it)
	{
		B.push_back(it->small);
	}

	// 5. Build main
	std::vector<int> main;
	main.reserve(input.size());
	main.push_back(B[0]);
	main.push_back(A[0]);
	if (A.size() > 1)
	{
		for (std::vector<int>::iterator it = A.begin() + 1; it != A.end(); ++it)
		{
			main.push_back(*it);
		}
	}
	// 5.5 Build pend
	std::vector<int> pend;
	pend.reserve(input.size());
	if (B.size() > 1)
	{
		for (std::vector<int>::iterator it = B.begin() + 1; it != B.end(); ++it)
		{
			pend.push_back(*it);
		}
	}
	if (odd_element != -1)
		pend.push_back(odd_element);

	// 6. Insert pend elements into main using Jacobsthal boundaries
	std::vector<int> boundaries;
	int previous = 0;
	int current = 0;
	for (int i = 1; ; ++i)
	{
		size_t J = jacobsthal(i);
		current = J;
		if (J >= pend.size())
		{
			boundaries.push_back(pend.size()); // end boundary
			break;
		}
		if (previous == current)
			continue;
		boundaries.push_back(J);
		previous = J;
	}
	// 6.2 Convert boundaries to ranges and binary-insert
	previous = 0;
	for (std::vector<int>::iterator b_it = boundaries.begin(); b_it != boundaries.end(); ++b_it) // declares how many pends are allowed to inserted in one loop
	{
		int boundary = *b_it;
		for (int i = boundary; i > previous; --i) // reverse order
		{
			int value = pend[i - 1];
			binaryInsert(main, value);
		}
		previous = boundary;
	}

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
