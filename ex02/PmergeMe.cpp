
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

	// create pairs
	std::vector<Pair> temp;
	size_t start = 0;

	while (start < input.size() && start + 1 < input.size())
	{
		temp.push_back(Pair(input[start], input[start + 1]));
		start += 2;
	}
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