//
//  main.cpp
//  Blackjack
//
//  Created by Matt barrett on 28/12/2021.
//

#include "Blackjack.hpp"
#include "Tests.hpp"
#include <iostream>
#include <chrono>
#include <thread>

int main() {
    
    // Run unit tests
    runAllTests();
    
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
    
    // Perform exploration and find q values
    Blackjack game;
    int nExplore = 1e7;
    game.exploration(nExplore, "Q-learning");

    // Perform 5 independent simulations of 109,200 games
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
    int nTest = 109200;
    game.testPolicy(nTest);
    game2.testPolicy(nTest);
    game3.testPolicy(nTest);
    game4.testPolicy(nTest);
    game5.testPolicy(nTest);
    
    // Print results of simulations
    for (int i = 0; i < game.walletTrack.size()-1; i+=1000) {
        std::cout << game.walletTrack[i] << " " << game2.walletTrack[i] << " " << game3.walletTrack[i] <<  " " << game4.walletTrack[i] << " " << game5.walletTrack[i] << " " << i << "\n";
    }
    std::cout << game.walletTrack.back() << " " << game2.walletTrack.back() << " " << game3.walletTrack.back() << " " << game4.walletTrack.back() << " " << game5.walletTrack.back() << " " << nTest << "\n\n";
    
    // Print win/draw/loss rate
    std::cout << 1.0*game.gambler.wins/(nTest+game.nSplitHands) << "\n";
    std::cout << 1.0*game.gambler.draws/(nTest+game.nSplitHands) << "\n";
    std::cout << 1.0*game.gambler.losses/(nTest+game.nSplitHands) << "\n\n";
    
    // Find optimal policy for states where splitting isn't possible
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
    
    // Find optimal policy for states where splitting is possible
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
    
    // Print optimal policy (for when the card count is 0)
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
    
    return 0;
}
