//
//  Shoe.cpp
//  Blackjack
//
//  Created by Matt barrett on 28/12/2021.
//

#include "Shoe.hpp"

// Use a constructor which calls reset to automatically initialise deck of cards
Shoe::Shoe() {
    reset();
}
    
    // Check the Deck has enough cards to play BlackJack
bool Shoe::isDeckBigEnough() {
    return deckOfCards.size() >= minDeckSize;
}
    
// Reset the deck when the number of cards in the deck gets too low
void Shoe::reset() {
    // These are the values of all cards in Blackjack (aces worth 11)
    int cardValues[52] = { 2,3,4,5,6,7,8,9,10,10,10,10,11,
        2,3,4,5,6,7,8,9,10,10,10,10,11,
        2,3,4,5,6,7,8,9,10,10,10,10,11,
        2,3,4,5,6,7,8,9,10,10,10,10,11 };
        
    for (int i = 0; i < numberOfDecks; i++) {
        for (int j = 0; j < numberOfCardsInOneDeck; j++) {
            deckOfCards.push_back(cardValues[j]);
        }
    }
            
    // Fisher-Yates algorithm to shuffle the deck
    for (int i = numberOfDecks*numberOfCardsInOneDeck - 1; i > 0; --i) {
        int randomPositionInDeck = rand()%(i + 1);
        std::swap(deckOfCards[i], deckOfCards[randomPositionInDeck]);
        }
}

