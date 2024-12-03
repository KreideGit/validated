## Validated

validated is a C++ 20 header-only library designed to enhance type safety and robustness by encapsulating data validation directly into the type system. It allows developers to declare types whose assignments are automatically validated against a specified criterion, ensuring that values always conform to expected formats or rules before they are used.

### Features

* Type Safety: Integrates validation into the type system, preventing invalid states and reducing validation bugs.
* Validators: Supports lambda expressions and custom validator classes.
* Compile Time: Fully `constexpr` enabled.
* Ease of Use: Simplifies code by reducing boilerplate validation checks scattered throughout the codebase.
* Header-Only: Easy to integrate with no need for compiled binaries, making it perfect for inclusion in both small and large projects.
Getting Started

### Usage
To use validated, define a type by specifying the underlying type T and a validator V. For example, to create a validated email type:

```c++
#include <iostream>
#include <regex>

#include <ck/validated.hpp>

using is_email_t = decltype([](const std::string& v) {
    static const std::regex pattern(
        R"((\w+)(\.|_)?(\w*)@(\w+)(\.(\w+))+)"
    );
    return std::regex_match(v, pattern);
});

int main() {
    ck::validated<std::string, is_email_t> email("user@example.com");

    std::cout << "Email: " << *email << std::endl;

    try {
        email = "invalid-email";
    } catch (const ck::validation_error&) {
        std::cout << "Invalid email address!" << std::endl;
    }

    return 0;
}
```
### How It Works

validated<T, V> works by wrapping a type T with a custom validator V. Every assignment to a validated type is checked against the validator, which should implement an operator() accepting a value of type T and returning a bool indicating whether the value is valid.

### Contributing

Contributions are welcome! Feel free to fork the repository and submit pull requests, or open issues for bugs, feature requests, and discussions.

### License

Distributed under the MIT License. See LICENSE for more information.