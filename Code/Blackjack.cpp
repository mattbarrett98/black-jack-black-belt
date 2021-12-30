//
//  Blackjack.cpp
//  Blackjack
//
//  Created by Matt barrett on 29/12/2021.
//

#include "Blackjack.hpp"

/* Assign reward to gamblers actions (for the simple Monte Carlo approach)
 -the first index is for the value of the gambler's hand, which can take values
 in [4,21] so we minus 4 to map these values to [0,17]
 - second index indicates whether the gsmbler has a usable ace or not
 - third is the value of dealer's face up card, minus 2 sonce it is in [2,11]
 - fourth indicates the current card count state
 - fifth indicates whether or not the gambler can split their hand
 - sixth is the gambler's action (0 = stand, 1 = hit, 2 = double down, 3 = split)
 */
void Blackjack::monteCarloUpdate(int result, int splitResult=0) {
    // Assign rewards when the gambler has split their hand
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
    
    // Reward when the gambler has doubled down
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
    
    // Rewards for a normal hand (no splits or double down)
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
void Blackjack::qValueUpdate(int result, int splitResult=0) {
    
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
void Blackjack::applyRewards(std::string approach, int result, int splitResult=0) {
    if (approach == "Monte Carlo") {
        monteCarloUpdate(result, splitResult);
    }
    if (approach == "Q-learning") {
        qValueUpdate(result, splitResult);
    }
}


// Update the card count and clear hands
void Blackjack::updateAndClear() {
    
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
std::vector<int> Blackjack::splitHandExplore(std::vector<int> deckOfCards) {
    gambler.split = true;
    gambler.calculateHandValue();
    splitGambler.hand.push_back(gambler.hand[0]);
    gambler.hand.erase(gambler.hand.begin());
    deckOfCards = gambler.getCard(deckOfCards);
    deckOfCards = splitGambler.getCard(deckOfCards);
    gambler.updateCardCount(gambler.hand.back());
    gambler.updateCardCount(splitGambler.hand.back());
    gambler.cardCountStates.push_back(gambler.cardCount);
    
    // If the gambler has split aces they can't take any further action
    if (gambler.hand[0] == 11) {
        gambler.calculateHandValue();
        splitGambler.calculateHandValue();
    }
    // For any other split card the gambler can continue play as normal on both hands
    else {
        deckOfCards = gambler.policy(deckOfCards);
        splitGambler.cardCountStates.push_back(gambler.cardCount);
        splitGambler.cardCount = gambler.cardCount;
        deckOfCards = splitGambler.policy(deckOfCards);
        gambler.cardCount = splitGambler.cardCount;
        
    }
    return deckOfCards;
}

// Play large number of games with a stochastic policy for the gambler & record rewards to learn optimal actions
void Blackjack::exploration(int numberPlays, std::string approach) {
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
        
        // If first 2 cards are the same, split with probability 1/4
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
std::vector<int> Blackjack::splitHandTest(std::vector<int> deckOfCards) {
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


// Check who has the highest score (and adjust money, wins/draws/losses)
void Blackjack::checkWinner(int gamblersHandValue, int dealersHandValue, int bet) {
    if (gamblersHandValue > dealersHandValue) {
        gambler.wins++;
        gambler.money += bet;
    } else if (gamblersHandValue == dealersHandValue) {
        gambler.draws++;
    } else {
        gambler.losses++;
        gambler.money -= bet;
    }
}


// Play games with the optimal policy and record number of wins to evaluate the policy
void Blackjack::testPolicy(int numberPlays) {
    for (int i = 0; i < numberPlays; i++) {
        updateAndClear();
        walletTrack.push_back(gambler.money);
    
        if (!shoe.isDeckBigEnough()) {
            shoe.reset();
            gambler.cardCount = 0;
        }
        
        gambler.placeBet();
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
                continue;
            }
        } else {
            if (gambler.blackjackCheck()) {
                gambler.money += 1.5*gambler.bet;
                gambler.wins++;
                continue;
            }
        }
        
        shoe.deckOfCards = gambler.optimalPolicy(qValues, shoe.deckOfCards, dealer.hand[0]);
        
        if (gambler.split) {
            nSplitHands++;
            shoe.deckOfCards = splitHandTest(shoe.deckOfCards);
            
            int result1 = 0;
            int result2 = 0;
            
            if (gambler.handValueStates.back() > gambler.maxValue) {
                result1 = -1;
                gambler.losses++;
                gambler.money -= gambler.bet;
            }
            
            if (splitGambler.handValueStates.back() > splitGambler.maxValue) {
                result2 = -1;
                gambler.losses++;
                gambler.money -= splitGambler.bet;
            }
            
            if (result1==-1 && result2==-1) {
                continue;
            }
            
            shoe.deckOfCards = dealer.policy(shoe.deckOfCards);
            
            if (dealer.handValueStates.back() > dealer.maxValue) {
                if (result1 != -1) {
                    gambler.wins++;
                    gambler.money += gambler.bet;
                }
                if (result2 != -1) {
                    gambler.wins++;
                    gambler.money += splitGambler.bet;
                }
                continue;
            }
            if (result1 != -1) {
                checkWinner(gambler.handValueStates.back(), dealer.handValueStates.back(), gambler.bet);
            }
           
            if (result2 != -1) {
                checkWinner(splitGambler.handValueStates.back(), dealer.handValueStates.back(), splitGambler.bet);
            }
            
            
        } else {
            if (gambler.handValueStates.back() > gambler.maxValue) {
                gambler.money -= gambler.bet;
                gambler.losses++;
                continue;
            }
            shoe.deckOfCards = dealer.policy(shoe.deckOfCards);
    
            if (dealer.handValueStates.back() > dealer.maxValue) {
                gambler.money += gambler.bet;
                gambler.wins++;
                continue;
            }
            checkWinner(gambler.handValueStates.back(), dealer.handValueStates.back(), gambler.bet);
        }
    }
}
