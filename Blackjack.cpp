/* main.cpp to learn
c++ basics */

//  Created by Matt barrett on 14/10/2021.

#include <iostream>
#include <fstream>
#include <string>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <numeric>
#include <random>
#include <chrono>
#include <thread>
#include <algorithm>

class Shoe {
public:
    
    int numberOfDecks = 1;
    int numberOfCardsInOneDeck = 52;
    int minDeckSize = numberOfCardsInOneDeck*numberOfDecks/4;
    std::vector<int> deckOfCards;
    
    // Use a constructor which calls reset to automatically initialise deck of cards
    Shoe() {
        reset();
    }
    
    // Check the Deck has enough cards to play BlackJack
    bool isDeckBigEnough() {
        return deckOfCards.size() >= minDeckSize;
    }
    
    // Reset the deck when the number of cards in the deck gets too low
    void reset() {
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
    
};



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
    std::vector<int> getCard(std::vector<int> deckOfCards) {
        hand.push_back(deckOfCards[0]);
        deckOfCards.erase(deckOfCards.begin());
        return deckOfCards;
    }
    
    // Players get 2 cards to start in Blackjack
    std::vector<int> getHand(std::vector<int> deckOfCards) {
        deckOfCards = getCard(deckOfCards);
        deckOfCards = getCard(deckOfCards);
        return deckOfCards;
    }
    
    // Calculate value of hand - an ace is worth 11 unless it makes the player bust, then value is 1
    int calculateHandValue() {
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
    bool blackjackCheck() {
        return hand[0]+hand[1] == maxValue;
    }
};



// The gambler inherits the basic functionality that all people in the game have (the dealer will also inherit this functionality). However the gambler can do some things that the dealer can't, such as counting cards or placing bets
class Gambler: public Player {
public:
    std::vector<int> actions;
    std::vector<int> cardCountStates;
    bool doubleDown;
    bool split = false;
    int cardCount;
    float money = 10000;
    int bet;
    int wins = 0;
    int draws = 0;
    int losses = 0;

    
    // Initially the gambler has a purely random policy, which we use in order to explore the rewards for all state action pairs
    std::vector<int> policy(std::vector<int> deckOfCards) {
        while (calculateHandValue() < maxValue) {
            
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
    std::vector<int> optimalPolicy(double qValues[18][2][10][3][2][4], std::vector<int> deckOfCards, int dealersCard) {
        while (calculateHandValue() < maxValue) {
            if (hand.size()==2 && hand[0]==hand[1] && qValues[handValue - 4][usableAce][dealersCard - 2][cardCount + 20][1][3]>qValues[handValue - 4][usableAce][dealersCard - 2][whichCountState(cardCount)][1][0] && qValues[handValue - 4][usableAce][dealersCard - 2][whichCountState(cardCount)][1][3]>qValues[handValue - 4][usableAce][dealersCard - 2][whichCountState(cardCount)][1][1] && qValues[handValue - 4][usableAce][dealersCard - 2][whichCountState(cardCount)][1][3]>qValues[handValue - 4][usableAce][dealersCard - 2][whichCountState(cardCount)][1][2] && !split) {
                split = true;
                break;
            }
            
            if (hand.size()==2 && qValues[handValue - 4][usableAce][dealersCard - 2][whichCountState(cardCount)][hand[0] == hand[1]][2]>qValues[handValue - 4][usableAce][dealersCard - 2][whichCountState(cardCount)][hand[0] == hand[1]][1] && qValues[handValue - 4][usableAce][dealersCard - 2][whichCountState(cardCount)][hand[0] == hand[1]][2]>qValues[handValue - 4][usableAce][dealersCard - 2][whichCountState(cardCount)][hand[0] == hand[1]][0]) {
                bet *= 2;
                deckOfCards = getCard(deckOfCards);
                updateCardCount(hand.back());
                calculateHandValue();
                break;
            }
            
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
    void updateCardCount(int card) {
        if (2 <= card && card <= 6) {
            cardCount++;
        } else if (7 <= card && card <= 9) {
            ;
        } else {
            cardCount--;
        }
        
    }
    
    // Bet money based off of the current card count
    void placeBet() {
        if (cardCount <= 0) {
            bet = 50;
        } else if (1 <= cardCount && cardCount <= 10) {
            bet = 50*cardCount;
        } else {
            bet = 500;
        }
        
    }
    
    int whichCountState(int count) {
        if (count <= -1) {
            return 0;
        } else if (count == 0) {
            return 1;
        } else {
            return 2;
        }
    }
    
    // At the end of a game we clear the gambler's hand
    void clearHand() {
        hand.clear();
        usableAceStates.clear();
        handValueStates.clear();
        actions.clear();
        cardCountStates.clear();
        split = false;
    }
    
};


// Dealer inherits basic functionality, and also has things only they do such as their fixed policy
class Dealer: public Player {
public:
    int hitThreshold = 16;
    bool hitSoft17 = false;
    
    // Dealer has to twist as long as his hand is worth less than or equal to 16
    std::vector<int> policy(std::vector<int> deckOfCards) {
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
    void clearHand() {
        hand.clear();
        usableAceStates.clear();
        handValueStates.clear();
    }

};



// We use composition to bring a deck, dealer and gambler together, so we can play Blackjack
class Blackjack {
public:
    double stateActionTotalReward[18][2][10][3][2][4] = {};
    int stateActionCount[18][2][10][3][2][4] = {};
    float alpha = 0.001;
    float gamma = 1;
    double qValues[18][2][10][3][2][4] = {};
    int optimalPolicy[18][2][10][41][2];
    int nSplitHands = 0;
    
    float moneyByCount[41] = {};
    int countCount[41] = {};
    int winsByCount[41] = {};
    int splitsByCount[41] = {};
    std::vector<float> walletTrack;
    
    // Create all objects required for a game of Blackjack
    Shoe shoe;
    Gambler gambler;
    Gambler splitGambler;
    Dealer dealer;
    
    
    // Assign reward to gamblers actions (for the simple Monte Carlo approach)
    void monteCarloUpdate(int result, int splitResult=0) {
        if (gambler.split) {
            int index1 = gambler.handValueStates[0] - 4;
            int index2 = gambler.usableAceStates[0];
            int index3 = dealer.hand[0] - 2;
            int index4 = gambler.whichCountState(gambler.cardCountStates[0]);
            int index5 = 1;
            int index6 = 3;
            stateActionCount[index1][index2][index3][index4][index5][index6]++;
            stateActionTotalReward[index1][index2][index3][index4][index5][index6] += result+splitResult;
            return;
        }
        
        if (gambler.doubleDown) {
            int index1 = gambler.handValueStates[0] - 4;
            int index2 = gambler.usableAceStates[0];
            int index3 = dealer.hand[0] - 2;
            int index4 = gambler.whichCountState(gambler.cardCountStates[0]);
            int index5 = gambler.hand[0]==gambler.hand[1];
            int index6 = gambler.actions[0];
            stateActionCount[index1][index2][index3][index4][index5][index6]++;
            stateActionTotalReward[index1][index2][index3][index4][index5][index6] += result;
            return;
        }
        
        for (int i = 0; i < gambler.actions.size(); i++) {
            int index1 = gambler.handValueStates[i] - 4;
            int index2 = gambler.usableAceStates[i];
            int index3 = dealer.hand[0] - 2;
            int index4 = gambler.whichCountState(gambler.cardCountStates[i]);
            int index5 = gambler.hand[0]==gambler.hand[1] && i==0;
            int index6 = gambler.actions[i];
            stateActionCount[index1][index2][index3][index4][index5][index6]++;
            stateActionTotalReward[index1][index2][index3][index4][index5][index6] += result;
        }
    }
    
    
    
    // Update the q values (for the Q-learning algorithm) using 3 states for the card count
    void qValueUpdate(int result, int splitResult=0) {
        
        if (gambler.split) {
            double maxFutureReward1;
            double maxFutureReward2;
            double prevQ = qValues[gambler.handValueStates[0] - 4][gambler.usableAceStates[0]][dealer.hand[0] - 2][gambler.whichCountState(gambler.cardCountStates[0])][1][3];
            if (gambler.hand.size() == 2) {
                maxFutureReward1 = 0;
            }
            else {
                maxFutureReward1 = std::max(std::max(qValues[gambler.handValueStates[1] - 4][gambler.usableAceStates[1]][dealer.hand[0] - 2][gambler.whichCountState(gambler.cardCountStates[1])][0][0], qValues[gambler.handValueStates[1] - 4][gambler.usableAceStates[1]][dealer.hand[0] - 2][gambler.whichCountState(gambler.cardCountStates[1])][0][1]), qValues[gambler.handValueStates[1] - 4][gambler.usableAceStates[1]][dealer.hand[0] - 2][gambler.whichCountState(gambler.cardCountStates[1])][0][2]);
                result = 0;
            }
            if (splitGambler.hand.size() == 2) {
                maxFutureReward2 = 0;
            } else {
                maxFutureReward2 = std::max(std::max(qValues[splitGambler.handValueStates[0] - 4][splitGambler.usableAceStates[0]][dealer.hand[0] - 2][gambler.whichCountState(splitGambler.cardCountStates[0])][0][0], qValues[splitGambler.handValueStates[0] - 4][splitGambler.usableAceStates[0]][dealer.hand[0] - 2][gambler.whichCountState(splitGambler.cardCountStates[0])][0][1]), qValues[splitGambler.handValueStates[0] - 4][splitGambler.usableAceStates[0]][dealer.hand[0] - 2][gambler.whichCountState(splitGambler.cardCountStates[0])][0][2]);
                splitResult = 0;
            }

            double increment = alpha * (result + splitResult + gamma*(maxFutureReward1 + maxFutureReward2) - prevQ);
            qValues[gambler.handValueStates[0] - 4][gambler.usableAceStates[0]][dealer.hand[0] - 2][gambler.whichCountState(gambler.cardCountStates[0])][1][3] += increment;
            return;
        }
        
        
        if (gambler.doubleDown) {
            double prevQ = qValues[gambler.handValueStates[0] - 4][gambler.usableAceStates[0]][dealer.hand[0] - 2][gambler.whichCountState(gambler.cardCountStates[0])][gambler.hand[0] == gambler.hand[1]][gambler.actions[0]];
            
            double increment = alpha * (result*2 - prevQ);
            
            qValues[gambler.handValueStates[0] - 4][gambler.usableAceStates[0]][dealer.hand[0] - 2][gambler.whichCountState(gambler.cardCountStates[0])][gambler.hand[0] == gambler.hand[1]][gambler.actions[0]] += increment;
            
            return;
        }
        
        
        for (int i = 0; i < gambler.actions.size(); i++) {
            if (i != gambler.actions.size() - 1) {
                
                double prevQ = qValues[gambler.handValueStates[i] - 4][gambler.usableAceStates[i]][dealer.hand[0] - 2][gambler.whichCountState(gambler.cardCountStates[i])][gambler.hand[0]==gambler.hand[1] && i==0][gambler.actions[i]];
                
                double maxFutureReward = std::max(qValues[gambler.handValueStates[i+1] - 4][gambler.usableAceStates[i+1]][dealer.hand[0] - 2][gambler.whichCountState(gambler.cardCountStates[i+1])][gambler.hand[0]==gambler.hand[1] && i==0][0], qValues[gambler.handValueStates[i+1] - 4][gambler.usableAceStates[i+1]][dealer.hand[0] - 2][gambler.whichCountState(gambler.cardCountStates[i+1])][gambler.hand[0]==gambler.hand[1] && i==0][1]);
                
                double increment = alpha * (gamma*maxFutureReward - prevQ);
                
                qValues[gambler.handValueStates[i] - 4][gambler.usableAceStates[i]][dealer.hand[0] - 2][gambler.whichCountState(gambler.cardCountStates[i])][gambler.hand[0]==gambler.hand[1] && i==0][gambler.actions[i]] += increment;
                
            } else {
                double prevQ = qValues[gambler.handValueStates[i] - 4][gambler.usableAceStates[i]][dealer.hand[0] - 2][gambler.whichCountState(gambler.cardCountStates[i])][gambler.hand[0]==gambler.hand[1] && i==0][gambler.actions[i]];
                
                double increment = alpha * (result - prevQ);
                
                qValues[gambler.handValueStates[i] - 4][gambler.usableAceStates[i]][dealer.hand[0] - 2][gambler.whichCountState(gambler.cardCountStates[i])][gambler.hand[0]==gambler.hand[1] && i==0][gambler.actions[i]] += increment;
            }
        }
    }
    
    // Update rewards either via our monte carlo approach or q learning approach
    void applyRewards(std::string approach, int result, int splitResult=0) {
        if (approach == "Monte Carlo") {
            monteCarloUpdate(result, splitResult);
        }
        if (approach == "Q-learning") {
            qValueUpdate(result, splitResult);
        }
    }
    
    // Update the card count and clear hands
    void updateAndClear() {
        
        for (int i = 1; i < dealer.hand.size(); i++) {
            gambler.updateCardCount(dealer.hand[i]);
        }
        
        if (gambler.split) {
            splitGambler.clearHand();
        }
        gambler.clearHand();
        dealer.clearHand();
    }
    
    // Split hand into 2 different hands (the implementation for the exploration part)
    std::vector<int> splitHandExplore(std::vector<int> deckOfCards) {
        gambler.split = true;
        gambler.calculateHandValue();
        splitGambler.hand.push_back(gambler.hand[0]);
        gambler.hand.erase(gambler.hand.begin());
        deckOfCards = gambler.getCard(deckOfCards);
        deckOfCards = splitGambler.getCard(deckOfCards);
        gambler.updateCardCount(gambler.hand.back());
        gambler.updateCardCount(splitGambler.hand.back());
        gambler.cardCountStates.push_back(gambler.cardCount);
        
        if (gambler.hand[0] == 11) {
            gambler.calculateHandValue();
            splitGambler.calculateHandValue();
        } else {
            deckOfCards = gambler.policy(deckOfCards);
            splitGambler.cardCountStates.push_back(gambler.cardCount);
            splitGambler.cardCount = gambler.cardCount;
            deckOfCards = splitGambler.policy(deckOfCards);
            gambler.cardCount = splitGambler.cardCount;
            
        }
        return deckOfCards;
    }
    
    // Play large number of games with a stochastic policy for the gambler & record rewards to learn optimal actions
    void exploration(int numberPlays, std::string approach) {
        for (int i = 0; i < numberPlays; i++){
            if (i != 0) {
                updateAndClear();
            }
    
            if (!shoe.isDeckBigEnough()) {
                shoe.reset();
                gambler.cardCount = 0;
            }
            
            shoe.deckOfCards = gambler.getHand(shoe.deckOfCards);
            shoe.deckOfCards = dealer.getHand(shoe.deckOfCards);
        
            gambler.updateCardCount(gambler.hand[0]);
            gambler.updateCardCount(gambler.hand[1]);
            gambler.updateCardCount(dealer.hand[0]);
            if (dealer.blackjackCheck()) {
                if (gambler.blackjackCheck()) {
                    continue;
                } else {
                    continue;
                }
            } else {
                if (gambler.blackjackCheck()) {
                    continue;
                }
            }
            
            gambler.cardCountStates.push_back(gambler.cardCount);
            
            int random = rand()%4;
            bool split = random==0;
            
            if (gambler.hand[0]==gambler.hand[1] && split) {
                shoe.deckOfCards = splitHandExplore(shoe.deckOfCards);
                
                int result1 = 0;
                int result2 = 0;
                
                if (gambler.handValueStates.back() > gambler.maxValue) {
                    result1 = -1;
                }
                
                if (splitGambler.handValueStates.back() > splitGambler.maxValue) {
                    result2 = -1;
                }
                
                if (result1==-1 && result2==-1) {
                    applyRewards(approach, result1, result2);
                    continue;
                }
                
                shoe.deckOfCards = dealer.policy(shoe.deckOfCards);
                
                if (dealer.handValueStates.back() > dealer.maxValue) {
                    if (result1 != -1) {result1 = 1;}
                    if (result2 != -1) {result2 = 1;}
                    applyRewards(approach, result1, result2);
                    continue;
                }
                if (result1 != -1) {
                    if (gambler.handValueStates.back() > dealer.handValueStates.back()) {
                        result1 = 1;
                    } else if (gambler.handValueStates.back() == dealer.handValueStates.back()) {
                        result1 = 0;
                    } else {
                        result1 = -1;
                    }
                }
                 
                if (result2 != -1) {
                    if (splitGambler.handValueStates.back() > dealer.handValueStates.back()) {
                        result2 = 1;
                    } else if (splitGambler.handValueStates.back() == dealer.handValueStates.back()) {
                        result2 = 0;
                    } else {
                        result2 = -1;
                    }
                }
                applyRewards(approach, result1, result2);
            }
            
            else {
                shoe.deckOfCards = gambler.policy(shoe.deckOfCards);
                
                if (gambler.handValueStates.back() > gambler.maxValue) {
                    applyRewards(approach, -1);
                    continue;
                }
                
                shoe.deckOfCards = dealer.policy(shoe.deckOfCards);
                
                if (dealer.handValueStates.back() > dealer.maxValue) {
                    applyRewards(approach, 1);
                    continue;
                }
                if (gambler.handValueStates.back() > dealer.handValueStates.back()) {
                    applyRewards(approach, 1);
                } else if (gambler.handValueStates.back() == dealer.handValueStates.back()) {
                    applyRewards(approach, 0);
                } else {
                    applyRewards(approach, -1);
                }
            }
        }
    }
    
    // Split hand into 2 different hands (the implementation for the test part)
    std::vector<int> splitHandTest(std::vector<int> deckOfCards) {
        gambler.split = true;
        splitGambler.split = true;
        splitGambler.bet = gambler.bet;
        splitGambler.hand.push_back(gambler.hand[0]);
        gambler.hand.erase(gambler.hand.begin());
        deckOfCards = gambler.getCard(deckOfCards);
        deckOfCards = splitGambler.getCard(deckOfCards);
        gambler.updateCardCount(gambler.hand.back());
        gambler.updateCardCount(splitGambler.hand.back());
        
        if (gambler.hand[0] == 11) {
            gambler.calculateHandValue();
            splitGambler.calculateHandValue();
        } else {
            deckOfCards = gambler.optimalPolicy(qValues, deckOfCards, dealer.hand[0]);
            splitGambler.cardCount = gambler.cardCount;
            deckOfCards = splitGambler.optimalPolicy(qValues, deckOfCards, dealer.hand[0]);
            gambler.cardCount = splitGambler.cardCount;
            
        }
        return deckOfCards;
    }
    
    // Play games with the optimal policy and record number of wins to evaluate the policy
    void testPolicy(int numberPlays) {
        for (int i = 0; i < numberPlays; i++) {
            updateAndClear();
            walletTrack.push_back(gambler.money);
        
            if (!shoe.isDeckBigEnough()) {
                shoe.reset();
                gambler.cardCount = 0;
            }
            //gambler.bet = 1;
            gambler.placeBet();
            int initialCount = gambler.cardCount;
            countCount[initialCount + 20]++;
            
            shoe.deckOfCards = gambler.getHand(shoe.deckOfCards);
            shoe.deckOfCards = dealer.getHand(shoe.deckOfCards);
        
            gambler.updateCardCount(gambler.hand[0]);
            gambler.updateCardCount(gambler.hand[1]);
            gambler.updateCardCount(dealer.hand[0]);
        
            if (dealer.blackjackCheck()) {
                if (gambler.blackjackCheck()) {
                    gambler.draws++;
                    continue;
                } else {
                    gambler.money -= gambler.bet;
                    gambler.losses++;
                    moneyByCount[initialCount + 20] -= gambler.bet;
                    continue;
                }
            } else {
                if (gambler.blackjackCheck()) {
                    gambler.money += 1.5*gambler.bet;
                    gambler.wins++;
                    winsByCount[initialCount + 20]++;
                    moneyByCount[initialCount + 20] += 1.5*gambler.bet;
                    continue;
                }
            }
            
            shoe.deckOfCards = gambler.optimalPolicy(qValues, shoe.deckOfCards, dealer.hand[0]);
            
            if (gambler.split) {
                nSplitHands++;
                splitsByCount[initialCount + 20]++;
                shoe.deckOfCards = splitHandTest(shoe.deckOfCards);
                
                int result1 = 0;
                int result2 = 0;
                
                if (gambler.handValueStates.back() > gambler.maxValue) {
                    result1 = -1;
                    gambler.losses++;
                    gambler.money -= gambler.bet;
                    moneyByCount[initialCount + 20] -= gambler.bet;
                }
                
                if (splitGambler.handValueStates.back() > splitGambler.maxValue) {
                    result2 = -1;
                    gambler.losses++;
                    gambler.money -= splitGambler.bet;
                    moneyByCount[initialCount + 20] -= gambler.bet;
                }
                
                if (result1==-1 && result2==-1) {
                    continue;
                }
                
                shoe.deckOfCards = dealer.policy(shoe.deckOfCards);
                
                if (dealer.handValueStates.back() > dealer.maxValue) {
                    if (result1 != -1) {
                        gambler.wins++;
                        winsByCount[initialCount + 20]++;
                        gambler.money += gambler.bet;
                        moneyByCount[initialCount + 20] += gambler.bet;
                    }
                    if (result2 != -1) {
                        gambler.wins++;
                        winsByCount[initialCount + 20]++;
                        gambler.money += splitGambler.bet;
                        moneyByCount[initialCount + 20] += gambler.bet;
                    }
                    continue;
                }
                if (result1 != -1) {
                    if (gambler.handValueStates.back() > dealer.handValueStates.back()) {
                        gambler.wins++;
                        winsByCount[initialCount + 20]++;
                        gambler.money += gambler.bet;
                        moneyByCount[initialCount + 20] += gambler.bet;
                    } else if (gambler.handValueStates.back() == dealer.handValueStates.back()) {
                        gambler.draws++;
                    } else {
                        gambler.losses++;
                        gambler.money -= gambler.bet;
                        moneyByCount[initialCount + 20] -= gambler.bet;
                    }
                }
               
                if (result2 != -1) {
                    if (splitGambler.handValueStates.back() > dealer.handValueStates.back()) {
                        gambler.wins++;
                        winsByCount[initialCount + 20]++;
                        gambler.money += splitGambler.bet;
                        moneyByCount[initialCount + 20] += gambler.bet;
                    } else if (splitGambler.handValueStates.back() == dealer.handValueStates.back()) {
                        gambler.draws++;
                    } else {
                        gambler.losses++;
                        gambler.money -= splitGambler.bet;
                        moneyByCount[initialCount + 20] -= gambler.bet;
                    }
                }
                
                
            } else {
                if (gambler.handValueStates.back() > gambler.maxValue) {
                    gambler.money -= gambler.bet;
                    gambler.losses++;
                    moneyByCount[initialCount + 20] -= gambler.bet;
                    continue;
                }
                shoe.deckOfCards = dealer.policy(shoe.deckOfCards);
        
                if (dealer.handValueStates.back() > dealer.maxValue) {
                    gambler.money += gambler.bet;
                    gambler.wins++;
                    winsByCount[initialCount + 20]++;
                    moneyByCount[initialCount + 20] += gambler.bet;
                    continue;
                }
                if (gambler.handValueStates.back() > dealer.handValueStates.back()) {
                    gambler.money += gambler.bet;
                    gambler.wins++;
                    winsByCount[initialCount + 20]++;
                    moneyByCount[initialCount + 20] += gambler.bet;
                } else if (gambler.handValueStates.back() == dealer.handValueStates.back()) {
                    gambler.draws++;
                } else {
                    gambler.money -= gambler.bet;
                    gambler.losses++;
                    moneyByCount[initialCount + 20] -= gambler.bet;
                }
            }
        
        }
    }
    
};


int main() {
    
    srand((unsigned)time(0));
    
    /* CODE FOR THE MULTITHREADED MONTE CARLO APPROACH
    
    // Time our MC simulations
    std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
    // Number of simulations run in each thread
    int nExplore = 1.25e8;
    // We have an 8 core CPU, so we create 8 game objects so we can run explorationPhase for each of them in parallel
    Blackjack game;
    Blackjack game2;
    Blackjack game3;
    Blackjack game4;
    Blackjack game5;
    Blackjack game6;
    Blackjack game7;
    Blackjack game8;
    
    // Run the simulations in parallel using multithreading
    std::thread thread1([&] (Blackjack * game) { game->exploration(nExplore, "Monte Carlo"); }, &game);
    std::thread thread2([&] (Blackjack * game) { game->exploration(nExplore, "Monte Carlo"); }, &game2);
    std::thread thread3([&] (Blackjack * game) { game->exploration(nExplore, "Monte Carlo"); }, &game3);
    std::thread thread4([&] (Blackjack * game) { game->exploration(nExplore, "Monte Carlo"); }, &game4);
    std::thread thread5([&] (Blackjack * game) { game->exploration(nExplore, "Monte Carlo"); }, &game5);
    std::thread thread6([&] (Blackjack * game) { game->exploration(nExplore, "Monte Carlo"); }, &game6);
    std::thread thread7([&] (Blackjack * game) { game->exploration(nExplore, "Monte Carlo"); }, &game7);
    std::thread thread8([&] (Blackjack * game) { game->exploration(nExplore, "Monte Carlo"); }, &game8);
    
    // Wait for all 8 simulations to finish before the rest of the script is executed
    thread1.join();
    thread2.join();
    thread3.join();
    thread4.join();
    thread5.join();
    thread6.join();
    thread7.join();
    thread8.join();
    
    // Combine results from all simulations to estimate optimal policy
    for (int i = 0; i < 18; i++) {
        for (int j = 0; j < 2; j++) {
            for (int k = 0; k < 10; k++) {
                for (int L = 0; L < 3; L++) {
                    for (int m = 0; m < 2; m++) {
                        for (int n = 0; n < 4; n++) {
                            double totalReward = game.stateActionTotalReward[i][j][k][L][m][n] + game2.stateActionTotalReward[i][j][k][L][m][n] + game3.stateActionTotalReward[i][j][k][L][m][n] + game4.stateActionTotalReward[i][j][k][L][m][n] + game5.stateActionTotalReward[i][j][k][L][m][n] + game6.stateActionTotalReward[i][j][k][L][m][n] + game7.stateActionTotalReward[i][j][k][L][m][n] + game8.stateActionTotalReward[i][j][k][L][m][n];
                            int totalCount = game.stateActionCount[i][j][k][L][m][n] + game2.stateActionCount[i][j][k][L][m][n] + game3.stateActionCount[i][j][k][L][m][n] + game4.stateActionCount[i][j][k][L][m][n] + game5.stateActionCount[i][j][k][L][m][n] + game6.stateActionCount[i][j][k][L][m][n] + game7.stateActionCount[i][j][k][L][m][n] + game8.stateActionCount[i][j][k][L][m][n];
                            game.qValues[i][j][k][L][m][n] = totalReward/totalCount;
                        }
                    }
                }
            }
        }
    }
    // 1 million games to evaluate the policy we have found
    int nTests = 1e6;
    game.testPolicy(nTests);
    std::cout << 1.0*game.gambler.wins/(nTests + game.nSplitHands) << std::endl;
    std::cout << 1.0*game.gambler.draws/(nTests + game.nSplitHands) << std::endl;
    std::cout << 1.0*game.gambler.losses/(nTests + game.nSplitHands) << std::endl;
    
    std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
    std::cout << "Time difference = " << std::chrono::duration_cast<std::chrono::seconds>(end - begin).count() << "[s]" << std::endl;
     */
     
    
    
    
    // CODE FOR Q-LEARNING + TABLEAU
    std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
    
    Blackjack game;
    int nExplore = 1e8;
    int nTest = 109200;
    game.exploration(nExplore, "Q-learning");
    game.testPolicy(nTest);
    
    Blackjack game2; Blackjack game3; Blackjack game4; Blackjack game5;
    for (int i = 0; i < 18; i++) {
        for (int j = 0; j < 2; j++) {
            for (int k = 0; k < 10; k++) {
                for (int l = 0; l < 3; l++) {
                    for (int m = 0; m < 2; m++) {
                        for (int n = 0; n < 4; n++) {
                            game2.qValues[i][j][k][l][m][n] = game.qValues[i][j][k][l][m][n];
                            game3.qValues[i][j][k][l][m][n] = game.qValues[i][j][k][l][m][n];
                            game4.qValues[i][j][k][l][m][n] = game.qValues[i][j][k][l][m][n];
                            game5.qValues[i][j][k][l][m][n] = game.qValues[i][j][k][l][m][n];
                        }
                    }
                }
            }
        }
    }
    game2.testPolicy(nTest);
    game3.testPolicy(nTest);
    game4.testPolicy(nTest);
    game5.testPolicy(nTest);
    
    for (int i = 0; i < game.walletTrack.size()-1; i+=1000) {
        std::cout << game.walletTrack[i] << " " << game2.walletTrack[i] << " " << game3.walletTrack[i] <<  " " << game4.walletTrack[i] << " " << game5.walletTrack[i] << " " << i << "\n";
    }
    std::cout << game.walletTrack.back() << " " << game2.walletTrack.back() << " " << game3.walletTrack.back() << " " << game4.walletTrack.back() << " " << game5.walletTrack.back() << " " << 109200 << "\n";
    
    std::cout << 1.0*game.gambler.wins/(nTest+game.nSplitHands) << std::endl;
    std::cout << 1.0*game.gambler.draws/(nTest+game.nSplitHands) << std::endl;
    std::cout << 1.0*game.gambler.losses/(nTest+game.nSplitHands) << std::endl;
     
    std::cout << game.gambler.money << std::endl << std::endl;
    std::cout << game2.gambler.money << std::endl << std::endl;
    std::cout << game3.gambler.money << std::endl << std::endl;
    std::cout << game4.gambler.money << std::endl << std::endl;
    std::cout << game5.gambler.money << std::endl << std::endl;
    
    std::string optimalPolicy[18][2][10][3];
    long action;
    for (int i = 0; i < 18; i++) {
        for (int j = 0; j < 2; j++) {
            for (int k = 0; k < 10; k++) {
                for (int l = 0; l < 3; l++) {
                    std::vector<double> arr;
                    for (int m = 0; m < 3; m++) {
                        arr.push_back(game.qValues[i][j][k][l][0][m]);
                    }
                    action = std::distance(arr.begin(),std::max_element(arr.begin(), arr.end()));
                    if (action == 0) {optimalPolicy[i][j][k][l] = "Stand";}
                    if (action == 1) {optimalPolicy[i][j][k][l] = "Hit";}
                    if (action == 2) {
                        if (arr[0] > arr[1]) {optimalPolicy[i][j][k][l] = "Double/Stand";}
                        else {optimalPolicy[i][j][k][l] = "Double/Hit";}
                    }
                }
            }
        }
    }
    
    std::string splitOptimalPolicy[18][2][10][3];
    for (int i = 0; i < 18; i++) {
        for (int j = 0; j < 2; j++) {
            for (int k = 0; k < 10; k++) {
                for (int l = 0; l < 3; l++) {
                    std::vector<double> arr;
                    for (int m = 0; m < 4; m++) {
                        arr.push_back(game.qValues[i][j][k][l][1][m]);
                    }
                    action = std::distance(arr.begin(),std::max_element(arr.begin(), arr.end()));
                    if (action == 0) {splitOptimalPolicy[i][j][k][l] = "Stand";}
                    if (action == 1) {splitOptimalPolicy[i][j][k][l] = "Hit";}
                    if (action == 2) {
                        if (arr[0] > arr[1]) {splitOptimalPolicy[i][j][k][l] = "Double/Stand";}
                        else {splitOptimalPolicy[i][j][k][l] = "Double/Hit";}
                    }
                    if (action == 3) {splitOptimalPolicy[i][j][k][l] = "Split";}
                }
            }
        }
    }
    
    for (int i = 1; i < 14; i++) {
        for (int j=0; j<9; j++) {
            std::cout << i + 4 << " " << j + 2 << " " << optimalPolicy[i][0][j][1] << "\n";
        }
        std::cout << i + 4 << " A " << optimalPolicy[i][0][9][1] << "\n";
    }
    
    for (int i = 9; i < 18; i++) {
        for (int j=0; j<9; j++) {
            std::cout << i + 4 << " " << j + 2 << " " << optimalPolicy[i][1][j][1] << "\n";
        }
        std::cout << i + 4 << " A " << optimalPolicy[i][1][9][1] << "\n";
    }
    
    for (int i=0; i<9; i++) {
        for (int j = 0; j < 9; j++) {
            std::cout << i + 2 << " " << j + 2 << " " << splitOptimalPolicy[2*i][0][j][1] << "\n";
        }
        std::cout << i + 2 << " A " << splitOptimalPolicy[2*i][0][9][1] << "\n";
    }
    
    for (int j = 0; j < 9; j++) {
        std::cout << "A " << j + 2 << " " << splitOptimalPolicy[12 - 4][1][j][1] << "\n";
    }
    std::cout << "A A " << splitOptimalPolicy[12 - 4][1][9][1] << "\n";

    
    std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
    std::cout << "Time difference = " << std::chrono::duration_cast<std::chrono::seconds>(end - begin).count() << "[s]" << std::endl;
    
    return 0;
}
