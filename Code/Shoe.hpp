//
//  Shoe.hpp
//  Blackjack
//
//  Created by Matt barrett on 28/12/2021.
//

#ifndef Shoe_hpp
#define Shoe_hpp
#include <vector>
#include <random>

class Shoe {
public:
    
    int numberOfDecks = 1;
    int numberOfCardsInOneDeck = 52;
    int minDeckSize = numberOfCardsInOneDeck*numberOfDecks/4;
    std::vector<int> deckOfCards;
    
    // Use a constructor which calls reset to automatically initialise deck of cards
    Shoe();
    
    // Check the Deck has enough cards to play BlackJack
    bool isDeckBigEnough();
    
    // Reset the deck when the number of cards in the deck gets too low
    void reset();
    
};

#endif /* Shoe_hpp */
