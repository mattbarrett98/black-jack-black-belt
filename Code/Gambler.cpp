//
//  Gambler.cpp
//  Blackjack
//
//  Created by Matt barrett on 29/12/2021.
//

#include "Gambler.hpp"

    
// Initially the gambler has a purely random policy, which we use in order to explore the rewards for all state action pairs
std::vector<int> Gambler::policy(std::vector<int> deckOfCards) {
    while (calculateHandValue() < maxValue) {
        
        // Double down with probability 1/4
        if (hand.size() == 2) {
            int random = rand()%4;
            doubleDown = random==0;
            if (doubleDown) {
                deckOfCards = getCard(deckOfCards);
                updateCardCount(hand.back());
                actions.push_back(2);
                calculateHandValue();
                break;
            }
        }
        
        // Hit or stand with probabilities 1/2
        int hit = rand()%2;
        actions.push_back(hit);
        if (hit) {
            deckOfCards = getCard(deckOfCards);
            updateCardCount(hand.back());
            cardCountStates.push_back(cardCount);
                
        } else {
            break;
        }
    }
    return deckOfCards;
}
    
// After exploration the gambler will attempt to estimate the optimal policy, and use this instead
std::vector<int> Gambler::optimalPolicy(double qValues[18][2][10][3][2][4], std::vector<int> deckOfCards, int dealersCard) {
    while (calculateHandValue() < maxValue) {
        // Split if first 2 cards are the same and if it is the optimal action
        if (hand.size()==2 && hand[0]==hand[1] && qValues[handValue - 4][usableAce][dealersCard - 2][cardCount + 20][1][3]>qValues[handValue - 4][usableAce][dealersCard - 2][whichCountState(cardCount)][1][0] && qValues[handValue - 4][usableAce][dealersCard - 2][whichCountState(cardCount)][1][3]>qValues[handValue - 4][usableAce][dealersCard - 2][whichCountState(cardCount)][1][1] && qValues[handValue - 4][usableAce][dealersCard - 2][whichCountState(cardCount)][1][3]>qValues[handValue - 4][usableAce][dealersCard - 2][whichCountState(cardCount)][1][2] && !split) {
            
            split = true;
            break;
            
        }
        
        // Double down if it is optimal and gambler has 2 cards
        if (hand.size()==2 && qValues[handValue - 4][usableAce][dealersCard - 2][whichCountState(cardCount)][hand[0] == hand[1]][2]>qValues[handValue - 4][usableAce][dealersCard - 2][whichCountState(cardCount)][hand[0] == hand[1]][1] && qValues[handValue - 4][usableAce][dealersCard - 2][whichCountState(cardCount)][hand[0] == hand[1]][2]>qValues[handValue - 4][usableAce][dealersCard - 2][whichCountState(cardCount)][hand[0] == hand[1]][0]) {
                bet *= 2;
                deckOfCards = getCard(deckOfCards);
                updateCardCount(hand.back());
                calculateHandValue();
                break;
        }
        
        // Hit if it is optimal, otherwise stand
        if (qValues[handValue - 4][usableAce][dealersCard - 2][whichCountState(cardCount)][hand[0] == hand[1]][0] < qValues[handValue - 4][usableAce][dealersCard - 2][whichCountState(cardCount)][hand[0] == hand[1]][1]) {
                deckOfCards = getCard(deckOfCards);
                updateCardCount(hand.back());
        } else {
            break;
        }
    }
    return deckOfCards;
}
    
// Counting cards (HiLo strategy), an advanced technique which gives the gambler more information about the state of the deck
void Gambler::updateCardCount(int card) {
    if (2 <= card && card <= 6) {
        cardCount++;
    } else if (7 <= card && card <= 9) {
        ;
    } else {
        cardCount--;
    }
}
    
// Bet money based off of the current card count
void Gambler::placeBet() {
    if (cardCount <= 0) {
        bet = 50;
    } else if (1 <= cardCount && cardCount <= 10) {
        bet = 50*cardCount;
    } else {
        bet = 500;
    }
}

// Map the card count to one of 3 possible states
int Gambler::whichCountState(int count) {
    if (count <= -1) {
        return 0;
    } else if (count == 0) {
        return 1;
    } else {
        return 2;
    }
}
    
// At the end of a game we clear the gambler's hand
void Gambler::clearHand() {
    hand.clear();
    usableAceStates.clear();
    handValueStates.clear();
    actions.clear();
    cardCountStates.clear();
    split = false;
}
