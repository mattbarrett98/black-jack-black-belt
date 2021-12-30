//
//  Blackjack.hpp
//  Blackjack
//
//  Created by Matt barrett on 29/12/2021.
//

#ifndef Blackjack_hpp
#define Blackjack_hpp
#include "Shoe.hpp"
#include "Gambler.hpp"
#include "Dealer.hpp"

// We use composition to bring a deck, dealer and gambler together, so we can play Blackjack
class Blackjack {
public:
    double stateActionTotalReward[18][2][10][3][2][4] = {};
    int stateActionCount[18][2][10][3][2][4] = {};
    float alpha = 0.001;
    float gamma = 1;
    double qValues[18][2][10][3][2][4] = {};
    int nSplitHands = 0;
    std::vector<float> walletTrack;
    
    // Create all objects required for a game of Blackjack
    Shoe shoe;
    Gambler gambler;
    Gambler splitGambler;
    Dealer dealer;
    
    // Assign reward to gamblers actions (for the simple Monte Carlo approach)
    void monteCarloUpdate(int result, int splitResult);
    
    // Update the q values (for the Q-learning algorithm) using 3 states for the card count
    void qValueUpdate(int result, int splitResult);
    
    // Update rewards either via our monte carlo approach or q learning approach
    void applyRewards(std::string approach, int result, int splitResult);
    
    // Update the card count and clear hands
    void updateAndClear();
    
    // Split hand into 2 different hands (the implementation for the exploration part)
    std::vector<int> splitHandExplore(std::vector<int> deckOfCards);
    
    // Play large number of games with a stochastic policy for the gambler & record rewards to learn optimal actions
    void exploration(int numberPlays, std::string approach);
    
    // Split hand into 2 different hands (the implementation for the test part)
    std::vector<int> splitHandTest(std::vector<int> deckOfCards);
    
    // Check who has the highest score
    void checkWinner(int gamblersHandValue, int dealersHandValue, int bet);
    
    // Play games with the optimal policy and record number of wins to evaluate the policy
    void testPolicy(int numberPlays);
    
};

#endif /* Blackjack_hpp */
