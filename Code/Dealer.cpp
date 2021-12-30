//
//  Dealer.cpp
//  Blackjack
//
//  Created by Matt barrett on 29/12/2021.
//

#include "Dealer.hpp"

// Dealer has to hit as long as his hand is worth less than or equal to 16
std::vector<int> Dealer::policy(std::vector<int> deckOfCards) {
    // Dealer will also hit on 17 if they have a usable ace (in some games)
    if (hitSoft17) {
        while ( (calculateHandValue()<=hitThreshold) || (handValue==hitThreshold+1 && usableAce) ) {
            deckOfCards = getCard(deckOfCards);
        }
    } else {
        while (calculateHandValue() <= hitThreshold) {
            deckOfCards = getCard(deckOfCards);
        }
    }
    return deckOfCards;
}

// At the end of a game we clear the dealer's hand
void Dealer::clearHand() {
    hand.clear();
    usableAceStates.clear();
    handValueStates.clear();
}
