//
//  Dealer.hpp
//  Blackjack
//
//  Created by Matt barrett on 29/12/2021.
//

#ifndef Dealer_hpp
#define Dealer_hpp
#include "Player.hpp"

// Dealer inherits basic functionality, and also has things only they do such as their fixed policy
class Dealer: public Player {
public:
    int hitThreshold = 16;
    bool hitSoft17 = false;
    
    // Dealer has to twist as long as his hand is worth less than or equal to 16
    std::vector<int> policy(std::vector<int> deckOfCards);
    
    // At the end of a game we clear the dealer's hand
    void clearHand();

};

#endif /* Dealer_hpp */
