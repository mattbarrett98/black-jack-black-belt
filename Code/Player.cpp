//
//  Player.cpp
//  Blackjack
//
//  Created by Matt barrett on 28/12/2021.
//

#include "Player.hpp"

// Pick a card from the deck, and remove the card from deckOfCards
std::vector<int> Player::getCard(std::vector<int> deckOfCards) {
    hand.push_back(deckOfCards[0]);
    deckOfCards.erase(deckOfCards.begin());
    return deckOfCards;
}

// Players get 2 cards to start in Blackjack
std::vector<int> Player::getHand(std::vector<int> deckOfCards) {
    deckOfCards = getCard(deckOfCards);
    deckOfCards = getCard(deckOfCards);
    return deckOfCards;
}

// Calculate value of hand - an ace is worth 11 unless it makes the player bust, then value is 1
int Player::calculateHandValue() {
    handValue = 0;
    usableAce = false;
    int numAces = 0;
    for (int i = 0; i < hand.size(); i++) {
        if (hand[i] == 11) {numAces++;}
        else {handValue += hand[i];}
    }
    if (numAces > 1) {
        handValue += numAces - 1;
        numAces = 1;
    }
    if (numAces > 0) {
        if (handValue <= 10) {
            handValue += 11;
            usableAce = true;
        }
        else {
            handValue++;
            usableAce = false;
        }
    }
    handValueStates.push_back(handValue);
    usableAceStates.push_back(usableAce);
    return handValue;
}

// If cards sum to 21 the player has Blackjack
bool Player::blackjackCheck() {
    return hand[0]+hand[1] == maxValue;
}
