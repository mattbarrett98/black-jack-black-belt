# Intro

A project coding the popular casino game of Blackjack in C++. We code the game using object oriented programming (OOP), taking advantage of OOP concepts such as constructors, inheritance and composition. We then use reinforcement learning to try and learn the optimal strategy for Blackjack - in particular we will use the Monte Carlo control algorithm and the Q learning algorithm. 

# Set up 

In our implementation we play using only 1 deck of cards, although in casinos nowadays they will use around 6-8 decks shuffled together to negate the effect of card counting. We do this in an attempt to investigate how card counters would have been able to make a profit in the days where only a single deck was used. We are also only considering the case where there is 1 gambler playing against the dealer, although in real life multiple other gamblers may also be playing. 

The aim of the game is to get the sum of your cards to be as close as possible to 21, but not above, as this would result in you being 'bust' and losing. Cards 2-10 are worth the number on them, and Jacks, Queens and Kings are worth 10. An ace is worth 11 as long as valuing it as such doesn't cause the player to be bust, in this case the value changes to 1. 

The game starts with both the gambler and dealer receiving a hand of 2 cards each. A player has 'Blackjack' if they have an ace and a card worth 10, meaning their hand has the highest value of 21. If there are any Blackjacks present the game will end immediately in one of three ways:
* If the gambler has Blackjack and the dealer does not, the gambler wins automatically.
* Alternatively if the dealer has the only Blackjack, they win automatically.
* If they both have Blackjack, it is a draw.

If there are no Blackjacks, then the gambler must choose to 'stick' (not get anymore cards) or 'twist' (add 1 more card to their hand). If the gambler goes bust at any point they lose the game. If the gambler is not bust after making his decisions, then the dealer must stick or twist. Unlike the gambler, the dealer has no choice in his actions, they must stick if the value of their hand is ≥ 17, and twist otherwise. Finally, if the dealer busts he loses, or if not:
* The player with the higher hand value wins,
* Or it is a draw if they both have the same score.

At the start of each game, the dealer checks if the deck has at least 15 cards in it. If it does they deal as normal, if not they first shuffle all cards back together to reset the deck and then deal. 

As mentioned we are going to simulate the games under the assumption that the gambler knows how to count cards. We will use the most common card counting strategy, the Hi-Lo strategy. It works by keeping a running count for the deck, which starts at 0 when the deck has just been shuffled and no cards have yet been dealt from the deck. The gambler adjusts the running count each time they observe a new card in the following way:
* +1 for cards 2, 3, 4, 5, 6
* +0 for cards 7, 8, 9
* -1 for cards 10, J, Q, K, A 
 

# Reinforcement learning
The state at any time is defined by 4 values: the current sum of the gamblers hand ∈ {4,5,...,21}, whether or not the gambler has an ace still worth 11 (1 if they do, 0 if not), the value of the dealer's face up card ∈ {2,3,...,11} and the current card count ∈ {-20,-19,...,20}. The possible actions of the gambler are just stick or twist. 

The first approach we use is a simple Monte Carlo approach. We simulate a game and apply the received reward to every state-action pair observed in the game. We define the received reward to be +1 if the gambler wins, 0 for a draw and -1 for a loss. After simulating a large number of games we calculate the mean reward for each state-action pair, and for each state we pick the optimal action to be the one with the highest mean reward. In RL terminology, we have 

![formula](https://render.githubusercontent.com/render/math?math=\pi(s)=\Large\argmax_{a%20\in%20A}Q(s,a))

The policy of the gambler in these simulations is purely random, sticking or twisting with probability 0.5 no matter the state. We use multithreading to allow us to parallelise the simulations and take advantage of each of the 8 cores on our cpu. 

We play 125 million games in each of our 8 threads, for a total of 1 billion games (taking 18 minutes). Using the optimal policy found from this we play a further 1 million games in order to evaluate the policy. We find that under this policy, the gambler wins 42.78% of the games. 
