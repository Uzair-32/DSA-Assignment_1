#include "polynomial.h"
#include <iostream>
#include <string>
#include <sstream>

using namespace std;

// Node structure for each term in the polynomial linked list
struct TermNode {
    int coeff;      // Coefficient of the term
    int exp;        // Exponent of the term
    TermNode* next; // Pointer to the next term

    TermNode(int c, int e) : coeff(c), exp(e), next(nullptr) {}
};

// Implementation of the Polynomial interface using a singly linked list
class PolynomialImp : public Polynomial {
    TermNode* head; // Head pointer for the linked list of terms

public:    
    PolynomialImp() : head(nullptr) {}

    // Destructor to free all nodes in the linked list
    virtual ~PolynomialImp() {
        while(head) {
            TermNode* temp = head;  
            head = head->next;
            delete temp;
        }
    } 

    // Insert a term into the polynomial, combining like terms and maintaining order
    void insertTerm(int coefficient, int exponent)  override {
        if (coefficient == 0) return;

        TermNode* newNode = new TermNode(coefficient, exponent);

        // Insert at head if list is empty or exponent is largest
        if(!head || head->exp < exponent) {
            newNode->next = head;
            head = newNode;
            return;
        }

        TermNode* curr = head;
        TermNode* prev = nullptr;

        // Find correct position to insert (descending order of exponent)
        while(curr && curr->exp > exponent) {
            prev = curr;
            curr = curr->next;
        }

        // If term with same exponent exists, add coefficients
        if(curr && curr->exp == exponent) {
            curr->coeff += coefficient;
            delete newNode;
            // Remove term if coefficient becomes zero
            if(curr->coeff == 0) {
                if(prev) prev->next = curr->next;
                else head = curr->next;
                delete curr;
            }
        } else {
            // Insert new term at correct position
            newNode->next = curr;
            if(prev) prev->next = newNode;
            else head = newNode;
        }
    }

    // Convert polynomial to a human-readable string
    string toString() const override {
        if(!head) return "0";

        TermNode* curr = head;
        stringstream ss;
        bool firstTerm = true;

        while(curr) {
            int c = curr->coeff;
            int e = curr->exp;

            // Handle sign and formatting
            if(!firstTerm) {
                if(c > 0) ss << " + ";
                else {
                    ss << " - ";
                    c = -c;
                }
            } else {
                if(c < 0) {
                    ss << "-";
                    c = -c;
                }
                firstTerm = false;
            }

            // Format term based on exponent
            if (e == 0) {
                ss << c;
            } else if (e == 1) {
                if (c == 1) ss << "x";
                else ss << c << "x";
            } else {
                if (c == 1) ss << "x^" << e;
                else ss << c << "x^" << e;
            }
            curr = curr->next;
        }

        return ss.str();
    }

    // Add two polynomials and return a new polynomial object
    Polynomial* add(const Polynomial& other) const override {
        const PolynomialImp* o = dynamic_cast<const PolynomialImp*>(&other);
        PolynomialImp* result = new PolynomialImp();

        // Copy terms from this polynomial
        TermNode* curr = this->head;
        while(curr) {
            result->insertTerm(curr->coeff, curr->exp);
            curr = curr->next;
        }

        // Copy terms from other polynomial
        if(o) {
            TermNode* curr2 = o->head;
            while(curr2) {
                result->insertTerm(curr2->coeff, curr2->exp);
                curr2 = curr2->next;
            }
        }

        return result;
    }

    // Multiply two polynomials and return a new polynomial object
    Polynomial* multiply(const Polynomial& other) const override {
        const PolynomialImp* o = dynamic_cast<const PolynomialImp*>(&other);
        PolynomialImp* result = new PolynomialImp();

        // Multiply each term of this polynomial with each term of other
        if(o) {
            for(TermNode* curr1 = this->head; curr1 != nullptr; curr1 = curr1->next) {
                for(TermNode* curr2 = o->head; curr2 != nullptr; curr2 = curr2->next) {
                    int newCoeff = curr1->coeff * curr2->coeff;
                    int newExp = curr1->exp + curr2->exp;
                    result->insertTerm(newCoeff, newExp);
                }
            }
        }
        return result;
    }

    // Compute the derivative of the polynomial and return a new polynomial object
    Polynomial* derivative() const override {
        PolynomialImp* result = new PolynomialImp();

        // Apply power rule to each term
        for(TermNode* curr = this->head; curr != nullptr; curr = curr->next) {
            if(curr->exp != 0) {
                int newCoeff = curr->coeff * curr->exp;
                int newExp = curr->exp - 1;
                result->insertTerm(newCoeff, newExp);
            }
        }
        return result;
    }
};
