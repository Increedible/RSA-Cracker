#include <iostream>
#include <string>
#include <algorithm>
#include <chrono>
#include <string>

class BigInteger {
private:
    std::string value;

public:
    // Constructors
    BigInteger() : value("0") {}
    BigInteger(const int i) : value(std::to_string(i)) {}
    BigInteger(const std::string& str) : value(str) {}

    friend std::ostream& operator<<(std::ostream& os, const BigInteger& obj) {
        os << obj.value;
        return os;
    }

    // Comparison operators
    bool operator==(const BigInteger& other) const {
        return value == other.value;
    }

    bool operator<(const BigInteger& other) const {
        if (value.size() < other.value.size()) return true;
        if (value.size() > other.value.size()) return false;
        return value < other.value;
    }

    bool operator>(const BigInteger& other) const {
        if (value.size() > other.value.size()) return true;
        if (value.size() < other.value.size()) return false;
        return value > other.value;
    }

    bool operator<=(const BigInteger& other) const {
        if (value.size() < other.value.size()) return true;
        if (value.size() > other.value.size()) return false;
        return value <= other.value;
    }

    bool operator>=(const BigInteger& other) const {
        if (value.size() > other.value.size()) return true;
        if (value.size() < other.value.size()) return false;
        return value >= other.value;
    }

    // Arithmetic operators
    BigInteger operator+(const BigInteger& other) const {
        BigInteger result;
        int carry = 0;
        int i = value.size() - 1, j = other.value.size() - 1;
        while (i >= 0 || j >= 0 || carry) {
            int x = i >= 0 ? value[i--] - '0' : 0;
            int y = j >= 0 ? other.value[j--] - '0' : 0;
            int sum = x + y + carry;
            result.value += (sum % 10) + '0';
            carry = sum / 10;
        }
        reverse(result.value.begin(), result.value.end());
        return result.value.substr(0, result.value.size() - 1);
    }

    BigInteger operator-(const BigInteger& other) const {
        BigInteger result;
        int borrow = 0;
        int i = value.size() - 1, j = other.value.size() - 1;
        while (i >= 0 || j >= 0) {
            int x = i >= 0 ? value[i--] - '0' : 0;
            int y = j >= 0 ? other.value[j--] - '0' : 0;
            int diff = x - y - borrow;
            if (diff < 0) {
                diff += 10;
                borrow = 1;
            }
            else {
                borrow = 0;
            }
            result.value += diff + '0';
        }
        reverse(result.value.begin(), result.value.end());
        return result.value.substr(0, result.value.size() - 1);
    }

    BigInteger operator*(const BigInteger& other) const {
        BigInteger result;
        int n = value.size(), m = other.value.size();
        result.value.resize(n + m, '0');
        for (int i = n - 1; i >= 0; i--) {
            int carry = 0;
            for (int j = m - 1; j >= 0; j--) {
                int p = (value[i] - '0') * (other.value[j] - '0') + carry + (result.value[i + j + 1] - '0');
                result.value[i + j + 1] = p % 10 + '0';
                carry = p / 10;
            }
            result.value[i] += carry;
        }
        result.value.erase(0, std::min(result.value.find_first_not_of('0'), result.value.size() - 1));
        return result;
    }

    BigInteger operator/(const BigInteger& other) const {
        if (other == BigInteger("0")) {
            throw std::runtime_error("Division by zero");
        }
        BigInteger quotient;
        BigInteger remainder = *this;
        while (remainder >= other) {
            BigInteger temp = other;
            int shift = remainder.value.size() - other.value.size();
            if (shift > 0) {
                temp.value.insert(0, shift, '0');
            }
            else {
                shift = 0;
            }
            while (shift >= 0 && remainder >= temp) {
                remainder = remainder - temp;
                BigInteger powerOf2("1");
                for (int i = 0; i < shift; i++) {
                    powerOf2 = powerOf2 * BigInteger("10");
                }
                quotient = quotient + powerOf2;
                if (shift == 0) {
                    break;
                }
                temp.value.erase(0, 1);
                shift--;
            }
        }
        quotient.value.erase(0, std::min(quotient.value.find_first_not_of('0'), quotient.value.size() - 1));
        if (quotient.value.empty()) {
            return BigInteger("0");
        }
        return quotient;
    }

    BigInteger operator^(int power) const {
        if (power == 0) {
            return BigInteger("1");
        }
        if (power % 2 == 0) {
            BigInteger half = *this ^ (power / 2);
            return half * half;
        }
        else {
            return *this * (*this ^ (power - 1));
        }
    }

    BigInteger operator%(const BigInteger& other) const {
        if (other == BigInteger("0")) {
            throw std::runtime_error("Division by zero");
        }
        BigInteger remainder = *this;
        while (remainder >= other) {
            BigInteger temp = other;
            int shift = remainder.value.size() - other.value.size();
            if (shift > 0) {
                temp.value.insert(0, shift, '0');
            }
            else {
                shift = 0;
            }
            while (shift >= 0 && remainder >= temp) {
                remainder = remainder - temp;
                if (shift == 0) {
                    break;
                }
                temp.value.erase(0, 1);
                shift--;
            }
        }
        remainder.value.erase(0, std::min(remainder.value.find_first_not_of('0'), remainder.value.size() - 1));
        if (remainder.value.empty()) {
            return BigInteger("0");
        }
        return remainder;
    }

    // Conversion operator
    operator std::string() const {
        return value;
    }

    // toString() method
    std::string toString() const {
        return value;
    }
};

unsigned long long p = 5002219;
unsigned long long q = 5000551;

unsigned long long euclid(unsigned long long f, unsigned long long s) {
    while (s != 0) {
        unsigned long long r = f % s;
        f = s;
        s = r;
    }
    return f;
}

unsigned long long modular_pow(unsigned long long base, unsigned long long exponent, unsigned long long modulus) {
    unsigned long long result = 1;
    base = base % modulus;
    while (exponent > 0) {
        if (exponent % 2 == 1) {
            result = (result * base) % modulus;
        }
        exponent = exponent >> 1;
        base = (base * base) % modulus;
    }
    return result;
}

unsigned long long divisors(unsigned long long n, unsigned long long g) {
    unsigned long long i = 1;
    auto start_time = std::chrono::high_resolution_clock::now();
    while (true) {
        unsigned long long temp = modular_pow(g, i, n) - 1;
        if (std::chrono::high_resolution_clock::now() - start_time > std::chrono::milliseconds(g * 1000) || temp >= 18446744073709551615) {
            return -1;
        }
        unsigned long long gcd = euclid(temp, n);
        if (gcd > 1 && gcd < n) {
            return gcd;
        }
        i++;
    }
}

int main() {
    unsigned long long n = p * q;
    unsigned long long g = 2;
    unsigned long long div = -1;
    while (div == -1) {
        std::cout << "g = " << g << std::endl;
        div = divisors(n, g);
        g++;
    }
    std::cout << "p = " << div << " q = " << n / div << std::endl;
}