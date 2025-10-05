# Assignment README

## Context

This assignment was given as part of a Data Structures and Algorithms course. The goal was to implement three different problems in C++ using appropriate data structures and object-oriented principles. Each problem required the use of an interface (provided as a `.h` file) and a concrete implementation (in a `.cpp` file). The problems included polynomial arithmetic, a simple text editor, and a simulation of the UNO card game. The assignment emphasized understanding and applying inheritance, abstract classes, and efficient data structure selection.

---

## Overview

This repository contains three main C++ source files:

- **muhammad-uzair-529583_polynomial.cpp**: Implements a polynomial class using a linked list for efficient term management and arithmetic operations.
- **muhammad-uzair-529583_texteditor.cpp**: Implements a simple text editor using two stacks to efficiently handle cursor movement and text editing.
- **muhammad-uzair-529583_uno.cpp**: Simulates a basic UNO card game, managing players, cards, and game logic.

> **Note:** The `.h` files are interfaces (abstract base classes) and are not meant to be compiled directly.  
> The `.cpp` files contain the actual implementations.  
> **These `.cpp` files may not contain a `main()` function by default. You need to add your own `main()` function to test and run the code.**

---

## Approach & Data Structures

### Polynomial (`muhammad-uzair-529583_polynomial.cpp`)
- **Data Structure Used:** **Linked List**
- **Why Linked List?**
  - Polynomials often require dynamic insertion, deletion, and merging of terms.
  - Linked lists allow efficient insertion and deletion of terms without shifting elements, which would be costly in arrays.
- **Why Not Array?**
  - Arrays require shifting elements for insertion/deletion and are not memory efficient for sparse polynomials.

### Text Editor (`muhammad-uzair-529583_texteditor.cpp`)
- **Data Structure Used:** **Two Stacks**
- **Why Stacks?**
  - Cursor movement and text editing can be efficiently managed by splitting the text into two stacks: left of the cursor and right of the cursor.
  - This allows O(1) insertions, deletions, and cursor moves.
- **Why Not Linked List?**
  - While a linked list could work, stacks provide a simpler and more efficient solution for this specific cursor-based editing scenario.

### UNO Game (`muhammad-uzair-529583_uno.cpp`)
- **Data Structure Used:** **Vectors (Dynamic Arrays)**
- **Why Vectors?**
  - Vectors allow dynamic resizing and easy access to player hands and the deck.
  - They are suitable for managing collections of cards and players in a game setting.
- **Why Not Linked List or Stack?**
  - Linked lists are not needed since card removal/addition is mostly at the end or by index.
  - Stacks do not provide the flexibility needed for random access to cards.

---

## Challenges Faced

- **UNO Game:**  
  I had never played UNO before, so I had to spend significant time learning the rules and game flow. Understanding the card types, actions, and turn logic was challenging.
- **Assignment Manual:**  
  The assignment manual provided very brief instructions, especially regarding class inheritance and interface implementation. Most of the time (about 70%) was spent figuring out the correct inheritance structure and how to connect the `.h` interface files with the `.cpp` implementations. The actual coding was relatively straightforward once the structure was clear.

---

## How to Run

1. **Open a terminal in the project directory.**
2. **Add a `main()` function to any `.cpp` file you want to run.**  
   The provided `.cpp` files may not contain a `main()` function by default.  
   Example for UNO:
   ```cpp
   int main() {
       int numPlayers = 4;
       UNOImpl game(numPlayers);
       game.initialize();
       while (!game.isGameOver()) {
           std::cout << game.getState() << std::endl;
           game.playTurn();
       }
       std::cout << "Winner: Player " << game.getWinner() << std::endl;
       return 0;
   }
   ```
3. **Compile the `.cpp` file.**  
   For example:
   ```
   g++ muhammad-uzair-529583_uno.cpp -o uno
   ./uno
   ```
4. **Make sure the corresponding `.h` files are in the same directory.**

---

## GitHub Repository

> [GitHub Link Here](https://github.com/Uzair-32/DSA-Assignment_1)

---

## Summary

- The main challenge was understanding the assignment requirements and UNO rules.
- The chosen data structures (linked list, stacks, vectors) were selected for their efficiency and suitability for each problem.
- Most time was spent on interface/class inheritance setup, not on the actual logic.
- **Remember to add your own `main()` function to test and run each implementation.**

---


