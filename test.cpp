#include <iostream>
#include <string>
#include <sstream>
#include <vector>
// --- MAIN FUNCTION FOR TESTING ---

int jacobsthal(int n)
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

int main()
{
    std::vector<int> jacob_numbers;
    int previous = 0;
    int current;
    for (int i = 1; ; ++i)
    {
        int J = jacobsthal(i);
        current = J;
        if (J >= 7)
        {
            jacob_numbers.push_back(11); // end boundary
            break;
        }
        if (previous == current)
            continue;
        std::cout << "pushed:" << J << std::endl;
        jacob_numbers.push_back(J);
        previous = J;
    }

    std::cout << "jacobsthal: " << std::endl;
    for (int i = 1; i <= 10; ++i)
    {

        std::cout << jacobsthal(i) << std::endl;
    }
    return 0;
}