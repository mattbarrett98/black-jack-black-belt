//
//  Gambler.hpp
//  Blackjack
//
//  Created by Matt barrett on 29/12/2021.
//

#ifndef Gambler_hpp
#define Gambler_hpp
#include "Player.hpp"
#include <random>

// The gambler inherits the basic functionality that all people in the game have (the dealer will also inherit this functionality). However the gambler can do some things that the dealer can't, such as counting cards or placing bets
class Gambler: public Player {
public:
    std::vector<int> actions;
    std::vector<int> cardCountStates;
    bool doubleDown;
    bool split = false;
    int cardCount = 0;
    float money = 10000;
    int bet;
    int wins = 0;
    int draws = 0;
    int losses = 0;

    
    // Initially the gambler has a purely random policy, which we use in order to explore the rewards for all state action pairs
    std::vector<int> policy(std::vector<int> deckOfCards);
    
    // After exploration the gambler will attempt to estimate the optimal policy, and use this instead
    std::vector<int> optimalPolicy(double qValues[18][2][10][3][2][4], std::vector<int> deckOfCards, int dealersCard);
    
    // Counting cards (HiLo strategy), an advanced technique which gives the gambler more information about the state of the deck
    void updateCardCount(int card);
    
    // Bet money based off of the current card count
    void placeBet();
    
    // We define 3 unique states for the card count: when it is positive, negative or 0
    int whichCountState(int count);
    
    // At the end of a game we clear the gambler's hand
    void clearHand();
    
};

#endif /* Gambler_hpp */
