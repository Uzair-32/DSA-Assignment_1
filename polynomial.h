#ifndef POLYNOMIAL_H
#define POLYNOMIAL_H

#include <string>

class Polynomial {
public:
    virtual ~Polynomial() {}

    virtual void insertTerm(int coefficient, int exponent) = 0;
    virtual std::string toString() const = 0;
    virtual Polynomial* add(const Polynomial& other) const = 0;
    virtual Polynomial* multiply(const Polynomial& other) const = 0;
    virtual Polynomial* derivative() const = 0;
};

#endif

