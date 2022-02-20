#include <iostream>
#include <algorithm>
#include <vector>
#include <random>
#include <unordered_set>
#include <sstream>

// let array have at least 1 element
constexpr auto MinArraySize = 1;
constexpr auto MaxArraySize = 256;

constexpr auto MinNumArrays = 0;
constexpr auto MaxNumArrays = 100;

// number of arrays is a natural number
static_assert(MinNumArrays >= 0);
// max array size can't be less then num of arrays, because array sizes are unique
static_assert(MaxNumArrays <= MaxArraySize);


void print_array_of_arrays(const std::vector<std::vector<int>> &arr_of_random_arrs) {
    for (const auto &curr_vec: arr_of_random_arrs) {
        for (const auto n: curr_vec) {
            std::cout << n << " ";
        }
        std::cout << "|" << std::endl;
    }
    std::cout << std::endl;
}

int read_natural_number(std::istream &input_stream,
                        const int min_num_arrays, const int max_num_arrays) {
    const auto is_integer = [](const double value) {
        double int_part = 0.0;
        return std::modf(value, &int_part) == 0.0;
    };

    std::string raw_value;

    // read, ignoring whitespaces at beg
    while (std::getline(input_stream >> std::ws, raw_value)) {
        std::istringstream stream{raw_value};

        double value{};
        stream >> value;

        // if error or extra symb at the end
        if (!stream || !(stream >> std::ws).eof()) {
            std::cout << "Invalid input. Input value must be a natural number. Please, try again:" << std::endl;
            continue;
        }

        // 4.0 is ok, 4.672 is not ok
        if (!is_integer(value)) {
            std::cout << "Input value must be a natural number. Please, try again:" << std::endl;
            continue;
        }

        if (value < min_num_arrays || value > max_num_arrays) {
            std::cout << "Input value must be in the range from " <<
                      min_num_arrays << " to " << max_num_arrays << ", try again:" << std::endl;
            continue;
        }

        return static_cast<int>(value);
    }

    throw std::runtime_error("Failed to read value");
}

template<typename RandomGenerator>
std::vector<int> generate_n_random_unique_nums(const int n, const int min, const int max,
                                               RandomGenerator &&generator) {
    using Distribution = std::uniform_int_distribution<>;
    using Param = Distribution::param_type;

    if (max - min + 1 < n)
        throw std::runtime_error("Can't generate such unique numbers");

    // in [min, max] range
    Distribution distribution(min, max);
    std::unordered_set<int> used;
    used.reserve(n);
    while (used.size() != n)
        used.insert(distribution(generator));

    std::vector<int> result(n);
    std::copy(used.begin(), used.end(), result.begin());
    return result;
}

std::vector<std::vector<int>> generate_and_sort_arrays(int n) {
    // 1. generate sizes of arrays
    std::random_device rd;
    std::mt19937 mt{rd()};
    std::vector<int> n_random_unique_nums = generate_n_random_unique_nums(n, MinArraySize, MaxArraySize, mt);

    // 2. create n arrays
    std::vector<std::vector<int>> array_of_arrays;
    array_of_arrays.reserve(n);

    std::srand(rd());
    // 3. fill each array with random numbers
    for (int i = 0; i < n; i++) {
        std::vector<int> curr_arr(n_random_unique_nums[i], 0);
        std::generate(curr_arr.begin(), curr_arr.end(), std::rand);
        array_of_arrays.emplace_back(std::move(curr_arr));
    }

    std::cout << "Generated arrays:" << std::endl;
    print_array_of_arrays(array_of_arrays);

    // 4. sort arrays:
    // - with even ordinal number in ascending order,
    // - with odd ordinal number in descending order
    // ordinal number starts from 1
    for (int i = 0; i < n; i++) {
        if ((i + 1) % 2 == 0) { // even ordinal number, ascending order
            std::sort(array_of_arrays[i].begin(), array_of_arrays[i].end());
        } else { // odd ordinal number, descending order
            std::sort(array_of_arrays[i].rbegin(), array_of_arrays[i].rend());
        }
    }

    // 5. return array with sorted arrays
    return array_of_arrays;
}


int main() {
    try {
        std::cout << "Please, enter a number of arrays:" << std::endl;
        const int input_int = read_natural_number(std::cin, MinNumArrays, MaxNumArrays);

        std::vector<std::vector<int>> res = generate_and_sort_arrays(input_int);

        std::cout << "Sorted arrays:" << std::endl;
        print_array_of_arrays(res);
    } catch (const std::exception &e) {
        std::cerr << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

