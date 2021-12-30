//
//  Player.hpp
//  Blackjack
//
//  Created by Matt barrett on 28/12/2021.
//

#ifndef Player_hpp
#define Player_hpp
#include <vector>

// Base class from which Gambler and Dealer will inherit (this is because the dealer and gambler have some shared functionality)
class Player {
public:
    
    int maxValue = 21;
    std::vector<int> hand;
    int handValue;
    bool usableAce;
    std::vector<int> handValueStates;
    std::vector<int> usableAceStates;
    
    // Pick a card from the deck, and remove the card from deckOfCards
    std::vector<int> getCard(std::vector<int> deckOfCards);
    
    // Players get 2 cards to start in Blackjack
    std::vector<int> getHand(std::vector<int> deckOfCards);
    
    // Calculate value of hand - an ace is worth 11 unless it makes the player bust, then value is 1
    int calculateHandValue();
    
    // If cards sum to 21 the player has Blackjack
    bool blackjackCheck();
    
};

#endif /* Player_hpp */
