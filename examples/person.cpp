#include <ck/validated.hpp>

#include <string>
#include <iostream>

using not_empty_t = decltype([](const std::string& v) {
    return !v.empty();
});

using name_t = ck::validated<std::string, not_empty_t>;

template<size_t Max>
using less_than_t = decltype([](size_t v) {
    return v < Max;
});

using age_t = ck::validated<size_t, less_than_t<150>>;

struct person {
    name_t firstName;
    name_t lastName;
    age_t age;
};

int main(int argc, char** argv) {
    person p1 { "John", "Doe", 31ul };

    std::cout << *p1.firstName << ' ' << *p1.lastName << " is " << *p1.age << " years old!\n";

    try {
        p1.firstName = "";
    } catch (ck::validation_error&) {
        std::cout << "First name cannot be empty!\n";
    }
}