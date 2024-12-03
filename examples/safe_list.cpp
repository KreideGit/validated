#include <ck/validated.hpp>
#include <iostream>

class safe_list {
public:
    void push_back(int v) {
        data_.push_back(v);
    }

    int get(size_t i) {
        auto boundsCheck = [size = data_.size()](size_t i) {
            return i < size;
        };

        auto validated_index = ck::validated<size_t, decltype(boundsCheck)>{i, boundsCheck};
        return data_.at(*validated_index);
    }

private:
    std::vector<int> data_{};
};

int main(int argc, char** argv) {
    safe_list s;
    s.push_back(1);
    s.push_back(2);

    std::cout << s.get(0) << '\n';
    std::cout << s.get(1) << '\n';

    try {
        std::cout << s.get(2) << '\n';
    } catch (ck::validation_error&) {
        std::cout << "Index is out of bounds!\n";
    }
}