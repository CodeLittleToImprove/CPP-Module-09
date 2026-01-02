
#include "PmergeMe.hpp"

#include <cstdlib>

// Default constructor
PmergeMe::PmergeMe()
{
	// std::cout << "Default constructor called" << std::endl;
}

PmergeMe::PmergeMe(char *argv[])
{
	parseInput(argv);
}

// Copy constructor
PmergeMe::PmergeMe(const PmergeMe &other)
	:_vec(other._vec), _deq(other._deq)
{
	// std::cout << "Copy constructor called" << std::endl;
}

// Copy Assignment operator overload
PmergeMe &PmergeMe::operator=(const PmergeMe &other)
{
	// std::cout << "Copy assignment operator called" << std::endl;
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
	// std::cout << "Destructor called" << std::endl;
}

void PmergeMe::parseInput(char *argv[])
{
	for (int i = 0; argv[i]; ++i)
	{
		char *end;
		long value = std::strtol(argv[i], &end, 10);

		if (*end != '\0' || value <= 0)
			throw std::runtime_error("Invalid input");

		_vec.push_back(static_cast<int>(value));
		_deq.push_back(static_cast<int>(value));
	}
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
		int c = b + 2 * a; //J(n) = J(n-1) + J(n-2)
		a = b; //shift J(n-2) forward
		b = c; //shift J(n-1) forward
	}
	return b;
}

std::vector<int> PmergeMe::fordJohnsonVector(const std::vector<int> &input)
{
	if (input.size() <= 1)
		return input;

	// 1. Create pairs
	// 2. Sort inside each pair
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

	// 4.5 Extract the losers
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



















	// 6. Insert pend elements into main
	// 7. Insert the odd element (if any)
}

void PmergeMe::run()
{
	std::cout << "Before: ";
	// printShortSequence(_vec);
	printSequence(_deq);

	std::vector<int> sortedVec = fordJohnsonVector(_vec);


	std::cout << "After:  ";
	// printShortSequence(_vec);
	printSequence(_deq);
}