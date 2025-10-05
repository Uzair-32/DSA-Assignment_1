#include "texteditor.h"
#include <iostream>
#include <string>
#include <algorithm>
#include <stack>

using namespace std;

// Implementation of the TextEditor interface using two stacks
class TextEditorImpl : public TextEditor { 
private:
    stack<char> left;  // Characters to the left of the cursor
    stack<char> right; // Characters to the right of the cursor

public:
    TextEditorImpl() {}

    // Insert character at the cursor position (push to left stack)
    void insertChar(char c) override {
        left.push(c);
    }

    // Delete character before the cursor (pop from left stack)
    void deleteChar() override {
        if(!left.empty()) {
            left.pop();
        }
    }

    // Move cursor one position left (move from left stack to right stack)
    void moveLeft() override {
        if(!left.empty()) {
            right.push(left.top());
            left.pop();
        }
    }

    // Move cursor one position right (move from right stack to left stack)
    void moveRight() override {
        if(!right.empty()) {
            left.push(right.top());
            right.pop();
        }
    }

    // Return the current text with cursor position indicated by '|'
    string getTextWithCursor() const override {
        stack<char> leftCopy = left;
        stack<char> rightCopy = right;

        string leftText;
        while(!leftCopy.empty()) {
            leftText.push_back(leftCopy.top());
            leftCopy.pop();
        }

        string rightText;
        while(!rightCopy.empty()) {
            rightText.push_back(rightCopy.top());
            rightCopy.pop();
        }

        // Reverse to get correct order
        reverse(leftText.begin(), leftText.end());
        reverse(rightText.begin(), rightText.end());

        // '|' represents the cursor position
        return leftText + "|" + rightText;
    }
};

