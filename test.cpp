#include <iostream>
#include <string>
#include <sstream>

// Forward declaration for helper function
bool isLeap(int year);

bool validate_date_format_and_values(const std::string& date_str) {
    // 1. FORMAT CHECK (YYYY-MM-DD)
    // Check length (4 + 1 + 2 + 1 + 2 = 10)
    if (date_str.length() != 10) {
        std::cerr << "Format Error: Date string length is not 10." << std::endl;
        return false;
    }
    // Check hyphen placement
    if (date_str[4] != '-' || date_str[7] != '-') {
        std::cerr << "Format Error: Hyphens missing or misplaced." << std::endl;
        return false;
    }

    // 2. EXTRACTION AND TYPE CHECK
    int year, month, day;
    char sep1, sep2; // To read and discard the hyphens

    // Create a string stream with the date string
    std::istringstream iss(date_str);

    // Attempt to extract: Year, Hyphen, Month, Hyphen, Day
    // Note: The stream extraction handles leading/trailing whitespace,
    // but the format check above handles the literal hyphen positions.
    if (!(iss >> year >> sep1 >> month >> sep2 >> day)) {
        // This fails if year/month/day are not composed of digits,
        // or if the hyphens (sep1/sep2) are missing.
        std::cerr << "Extraction Error: Failed to extract integers." << std::endl;
        std::cerr << day;
        return false;
    }
    std::cerr << day << std::endl;
    // Crucial: Check if there is extra junk data after the DD
    std::string junk;
    if (iss >> junk) {
        std::cerr << "Format Error: Extra characters found after the day." << std::endl;
        return false;
    }


    // 3. RANGE CHECK (General)
    if (year < 1) {
        std::cerr << "Range Error: Year must be positive." << std::endl;
        return false;
    }
    if (month < 1 || month > 12) {
        std::cerr << "Range Error: Month is out of range (1-12)." << std::endl;
        return false;
    }
    if (day < 1 || day > 31) {
        std::cerr << "Range Error: Day is out of range (1-31)." << std::endl;
        return false;
    }

    // 4. MONTH LENGTH CHECK (Specific)
    // Array to hold the maximum number of days in each month (index 0 unused)
    //             J  F  M  A  M  J  J  A  S  O  N  D
    int daysInMonth[] = {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

    // Adjust for February in a leap year
    if (month == 2 && isLeap(year)) {
        daysInMonth[2] = 29;
    }

    // Final check against the specific month's length
    if (day > daysInMonth[month]) {
        std::cerr << "Calendar Error: Day " << day << " is too high for month "
                  << month << " in year " << year << "." << std::endl;
        return false;
    }

    // All checks passed
    return true;
}

// 5. LEAP YEAR HELPER FUNCTION (C++98 compatible)
bool isLeap(int year) {
    // A year is a leap year if it is divisible by 4,
    // UNLESS it is divisible by 100 but NOT by 400.
    return (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
}

// --- MAIN FUNCTION FOR TESTING ---
int main() {
    std::string dates_to_test[] = {
        // "2009-01-02",  // Valid
        // "2009/01/02",  // Format Error
        // "2009-13-02",  // Range Error (Month)
        // "22-12-2023",  // Format Error (Layout)
        // "2023-02-29",  // Calendar Error (Feb 29 on non-leap year)
        // "2024-02-29",  // Valid (2024 is a leap year)
        // "2000-02-30",  // Calendar Error (Day too high)
        "2000-02-2a"  // Extra Data Error
    };

    int num_tests = sizeof(dates_to_test) / sizeof(dates_to_test[0]);

    std::cout << "--- Date Validation Tests ---" << std::endl;
    for (int i = 0; i < num_tests; ++i) {
        std::string date = dates_to_test[i];
        std::cout << "\nTesting: [" << date << "] -> ";
        if (validate_date_format_and_values(date)) {
            std::cout << "VALID" << std::endl;
        } else {
            std::cout << "INVALID" << std::endl;
        }
    }

    return 0;
}