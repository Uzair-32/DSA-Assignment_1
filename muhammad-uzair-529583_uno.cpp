#include "uno.h"
#include <iostream>
#include <string>
#include <vector>
#include <random>
#include <algorithm>
#include <sstream>

using namespace std;

// Enum for card colors
enum class Color { RED, GREEN, BLUE, YELLOW };

// Enum for card types
enum class Type { NUMBER, SKIP, REVERSE, DRAW_TWO };

// Structure representing a UNO card
struct Card {
    Color color; // Card color
    Type type;   // Card type (number/action)
    int value;   // Number value (for number cards), -1 for action cards

    // Convert card to human-readable string
    string toString() const {
        string c;
        switch(color) {
            case Color::RED: c = "Red"; break;
            case Color::GREEN: c = "Green"; break;
            case Color::BLUE: c = "Blue"; break;
            case Color::YELLOW: c = "Yellow"; break;
        }

        string t;
        if (type == Type::NUMBER) {
            t = to_string(value);
        } else if (type == Type::SKIP) {
            t = "Skip"; 
        } else if (type == Type::REVERSE) {
            t = "Reverse";
        } else if (type == Type::DRAW_TWO) {
            t = "Draw Two";
        }

        return c + " " + t;
    }
};

// Implementation of the UNOGame interface
class UNOImpl : public UNOGame {
private:
    int numPlayers;                  // Number of players
    vector<vector<Card>> players;    // Player hands
    vector<Card> deck;               // Draw deck
    vector<Card> discard;            // Discard pile
    int currentPlayer;               // Index of current player
    int direction;                   // 1 for clockwise, -1 for counter-clockwise
    mt19937 rng;                     // Random number generator

public:
    UNOImpl(int n) : numPlayers(n), players(n), currentPlayer(0), direction(1), rng(1234) {}

    // Initialize the game: build deck, deal cards, set up discard pile
    void initialize() override {
        players.assign(numPlayers, {});
        discard.clear();
        currentPlayer = 0;
        direction = 1;
        buildDeck();
        dealCard();
    }

    // Play one turn for the current player
    void playTurn() override {
        if (isGameOver()) return;

        Card top = discard.back();
        auto &hand = players[currentPlayer];
        int playIndex = -1;

        // Priority: color match
        for (int i = 0; i < (int)hand.size(); ++i) {
            if (hand[i].color == top.color) { playIndex = i; break; }
        }
        // Number match
        if (playIndex == -1 && top.type == Type::NUMBER) {
            for (int i = 0; i < (int)hand.size(); ++i) {
                if (hand[i].type == Type::NUMBER && hand[i].value == top.value) {
                    playIndex = i; break;
                }
            }
        }
        // Action priority: Skip → Reverse → DrawTwo
        if (playIndex == -1) {
            for (int i = 0; i < (int)hand.size(); ++i) {
                if (hand[i].type == Type::SKIP) { playIndex = i; break; }
            }
        }
        if (playIndex == -1) {
            for (int i = 0; i < (int)hand.size(); ++i) {
                if (hand[i].type == Type::REVERSE) { playIndex = i; break; }
            }
        }
        if (playIndex == -1) {
            for (int i = 0; i < (int)hand.size(); ++i) {
                if (hand[i].type == Type::DRAW_TWO) { playIndex = i; break; }
            }
        }

        if (playIndex != -1) {
            // Play the selected card
            Card played = hand[playIndex];
            hand.erase(hand.begin() + playIndex);
            discard.push_back(played);

            if (hand.size() == 1) {
                cout << "UNO! Player " << currentPlayer << " has one card left!\n";
            }

            applyAction(played);
        } else {
            // Draw a card if no playable card
            if (!deck.empty()) {
                Card drawn = drawCard();
                if (canPlay(drawn, top)) {
                    discard.push_back(drawn);
                    applyAction(drawn);
                } else {
                    hand.push_back(drawn);
                }
            }
        }

        // Move to next player
        currentPlayer = nextPlayerIndex();
    }

    // Check if any player has won
    bool isGameOver() const override {
        for (int i = 0; i < numPlayers; ++i) {
            if(players[i].empty()) return true;
        }
        return false;
    }

    // Return the index of the winner, or -1 if no winner
    int getWinner() const override {
        for (int i = 0; i < numPlayers; ++i) {
            if(players[i].empty()) return i;
        }
        return -1; // No winner yet
    }

    // Return a string describing the current game state
    std::string getState() const override {
        ostringstream oss;
        oss << "Player " << currentPlayer << "'s turn, Direction: "
            << (direction == 1 ? "Clockwise" : "Counter-clockwise")
            << ", Top: " << discard.back().toString() << ", Players cards: ";
        for (int i = 0; i < numPlayers; ++i) {
            oss << "P" << i << ":" << players[i].size();
            if (i != numPlayers - 1) oss << ", ";
        }
        return oss.str();
    }

private:
    // Build a standard UNO deck
    void buildDeck() {
        deck.clear();

        for(int c = 0; c < 4; ++c) {
            Color color = static_cast<Color>(c);

            // One 0 card per color
            deck.push_back({color, Type::NUMBER, 0});

            // Two of each 1-9, Skip, Reverse, DrawTwo per color
            for(int v = 1; v <= 9; ++v) {
                deck.push_back({color, Type::NUMBER, v});
                deck.push_back({color, Type::NUMBER, v});
            }

            // Two Skip, Reverse, DrawTwo per color
            deck.push_back({color, Type::SKIP, -1});
            deck.push_back({color, Type::SKIP, -1});            
            deck.push_back({color, Type::REVERSE, -1});
            deck.push_back({color, Type::REVERSE, -1});
            deck.push_back({color, Type::DRAW_TWO, -1});
            deck.push_back({color, Type::DRAW_TWO, -1});
        }
    }

    // Deal 7 cards to each player and set up the discard pile
    void dealCard() {
        shuffle(deck.begin(), deck.end(), rng);
        for(int i = 0; i < 7; ++i) {
            for(int p = 0; p < numPlayers; ++p) {
                players[p].push_back(deck.back());
                deck.pop_back();
            }
        }

        discard.push_back(deck.back());
        deck.pop_back();
    }

    // Draw a card from the deck
    Card drawCard() {
        Card c = deck.back();
        deck.pop_back();
        return c;
    }

    // Check if a card can be played on top of another card
    bool canPlay(const Card &c, const Card &top) const {
        if(c.color == top.color) return true;
        if(c.type == Type::NUMBER && top.type == Type::NUMBER && c.value == top.value) return true;
        if(c.type != Type::NUMBER && c.type == top.type) return true;   
        return false;
    }

    // Apply the action of a played card (Skip, Reverse, Draw Two)
    void applyAction(const Card &c) {
        if(c.type == Type::SKIP) {
            currentPlayer = nextPlayerIndex();
        } else if(c.type == Type::REVERSE) {
            direction *= -1;
        } else if(c.type == Type::DRAW_TWO) {
            int target = nextPlayerIndex();
            for (int i = 0; i < 2; ++i) {
                if(!deck.empty()) {
                    players[target].push_back(drawCard());
                }
            }
            currentPlayer = target;
        }
    }

    // Calculate the index of the next player based on direction
    int nextPlayerIndex() const {
        return (currentPlayer + direction + numPlayers) % numPlayers;
    }
};
