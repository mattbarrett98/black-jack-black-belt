//
//  Tests.cpp
//  Blackjack
//
//  Created by Matt barrett on 29/12/2021.
//

#include <iostream>
#include "Tests.hpp"

void testCalculateHandValue() {
    // Store bools of whether the expected outcomes are met
    std::vector<bool> expectedOutcomeCorrect;
    // Cases with aces
    // Set up (case 0)
    Player player; player.hand.push_back(11); player.hand.push_back(11);
    // Expected outcome
    expectedOutcomeCorrect.push_back(player.calculateHandValue() == 12);
    
    // Case 1
    player.hand.clear(); player.hand.push_back(11); player.hand.push_back(11); player.hand.push_back(11);
    expectedOutcomeCorrect.push_back(player.calculateHandValue() == 13);
    
    // Case 2
    player.hand.clear(); player.hand.push_back(5); player.hand.push_back(11); player.hand.push_back(10);
    expectedOutcomeCorrect.push_back(player.calculateHandValue() == 16);
    
    
    // Trivial cases (case 3)
    player.hand.clear(); player.hand.push_back(5); player.hand.push_back(10); player.hand.push_back(10);
    expectedOutcomeCorrect.push_back(player.calculateHandValue() == 25);
    
    // Case 4
    player.hand.clear(); player.hand.push_back(4); player.hand.push_back(3); player.hand.push_back(8);
    expectedOutcomeCorrect.push_back(player.calculateHandValue() == 15);
     
    for (int i = 0; i < expectedOutcomeCorrect.size(); i++) {
        if (expectedOutcomeCorrect[i] == 0) {
            std::cout << "test case " << i << " failed (CalculateHandValue) \n";
        }
    }
    
}

void testCheckWinner() {
    std::vector<bool> expectedOutcomeCorrect;
    // Set up
    Blackjack game; game.checkWinner(21, 21, 1);
    // Check function side effects are as expected (case 0)
    expectedOutcomeCorrect.push_back(game.gambler.draws == 1);
    
    // Case 1
    game.checkWinner(5, 17, 1);
    expectedOutcomeCorrect.push_back(game.gambler.losses == 1);
    
    // Case 2
    game.checkWinner(19, 20, 1);
    expectedOutcomeCorrect.push_back(game.gambler.losses == 2);
    
    // Case 3
    game.checkWinner(18, 17, 1);
    expectedOutcomeCorrect.push_back(game.gambler.wins == 1);
    
    for (int i = 0; i < expectedOutcomeCorrect.size(); i++) {
        if (expectedOutcomeCorrect[i] == 0) {
            std::cout << "test case " << i << " failed (CheckWinner) \n";
        }
    }
    
}

void testGetHand() {
    std::vector<bool> expectedOutcomeCorrect;
    // Set up
    Player player; std::vector<int> deckOfCards; deckOfCards.push_back(11); deckOfCards.push_back(5);
    deckOfCards.push_back(10); deckOfCards.push_back(9);
    deckOfCards = player.getHand(deckOfCards);
    // Check function side effects are as expected
    expectedOutcomeCorrect.push_back(player.hand[0] == 11);
    expectedOutcomeCorrect.push_back(player.hand[1] == 5);
    expectedOutcomeCorrect.push_back(deckOfCards[0] == 10);
    expectedOutcomeCorrect.push_back(deckOfCards[1] == 9);
    
    for (int i = 0; i < expectedOutcomeCorrect.size(); i++) {
        if (expectedOutcomeCorrect[i] == 0) {
            std::cout << "test case " << i << " failed (GetHand) \n";
        }
    }
    
}

void testDealerPolicy() {
    std::vector<bool> expectedOutcomeCorrect;
    // Test cases where hit soft 17 is false first
    // Set up
    Dealer dealer; dealer.hand.push_back(3); dealer.hand.push_back(10);
    std::vector<int> deckOfCards; deckOfCards.push_back(3); deckOfCards.push_back(5);
    dealer.policy(deckOfCards);
    // Check outcome is as expected (case 0)
    expectedOutcomeCorrect.push_back(dealer.hand.size() == 4);
    
    //case 1
    dealer.hand.clear(); dealer.hand.push_back(9); dealer.hand.push_back(8);
    dealer.policy(deckOfCards);
    expectedOutcomeCorrect.push_back(dealer.hand.size() == 2);
    
    // Cases where hit soft 17 is true
    dealer.hitSoft17 = true;
    // Case 2
    dealer.hand.clear(); dealer.hand.push_back(11); dealer.hand.push_back(6);
    deckOfCards.push_back(2);
    dealer.policy(deckOfCards);
    expectedOutcomeCorrect.push_back(dealer.hand.size() == 3);
    
    // Case 3
    dealer.hand.clear(); dealer.hand.push_back(10); dealer.hand.push_back(6);
    deckOfCards.push_back(11); deckOfCards.push_back(5);
    dealer.policy(deckOfCards);
    expectedOutcomeCorrect.push_back(dealer.hand.size() == 3);
    
    for (int i = 0; i < expectedOutcomeCorrect.size(); i++) {
        if (expectedOutcomeCorrect[i] == 0) {
            std::cout << "test case " << i << " failed (DealerPolicy) \n";
        }
    }
    
}

void testUpdateCardCount() {
    std::vector<bool> expectedOutcomeCorrect;
    // Case 0
    Gambler gambler; gambler.updateCardCount(11);
    expectedOutcomeCorrect.push_back(gambler.cardCount == -1);
    
    // Case 1
    gambler.updateCardCount(10);
    expectedOutcomeCorrect.push_back(gambler.cardCount == -2);
    
    // Case 2
    gambler.updateCardCount(8);
    expectedOutcomeCorrect.push_back(gambler.cardCount == -2);
    
    // Case 3
    gambler.updateCardCount(2);
    expectedOutcomeCorrect.push_back(gambler.cardCount == -1);
    
    for (int i = 0; i < expectedOutcomeCorrect.size(); i++) {
        if (expectedOutcomeCorrect[i] == 0) {
            std::cout << "test case " << i << " failed (UpdateCardCount) \n";
        }
    }
    
}

void runAllTests() {
    testCalculateHandValue();
    testCheckWinner();
    testGetHand();
    testDealerPolicy();
    testUpdateCardCount();
}
