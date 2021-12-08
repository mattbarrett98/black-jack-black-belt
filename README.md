# Using RL to crack Blackjack

A project coding the popular casino game of Blackjack in C++. We code the game using object oriented programming (OOP), taking advantage of OOP concepts such as constructors, inheritance and composition. We then use reinforcement learning to try and learn the optimal strategy for Blackjack - in particular we will use the Monte Carlo control algorithm and the Q learning algorithm. 

# Set up 

In our implementation we play using only 1 deck of cards, although in casinos nowadays they will use around 6-8 decks shuffled together to negate the effect of card counting. We do this in an attempt to investigate how card counters would have been able to make a profit in the days where only a single deck was used. We are also only considering the case where there is 1 gambler playing against the dealer, although in real life multiple other gamblers may also be playing. 

The aim of the game is to get the sum of your cards to be as close as possible to 21, but not above, as this would result in you being 'bust' and losing. Cards 2-10 are worth the number on them, and Jacks, Queens and Kings are worth 10. An ace is worth 11 as long as valuing it as such doesn't cause the player to be bust, in this case the value changes to 1. 

The game starts with both the gambler and dealer receiving a hand of 2 cards each. A player has 'Blackjack' if they have an ace and a card worth 10, meaning their hand has the highest value of 21. If there are any Blackjacks present the game will end immediately in one of three ways:
• if the gambler has Blackjack and the dealer does not, the gambler wins automatically,
• alternatively if the dealer has the only Blackjack, they win automatically,
• and if they both have Blackjack, it is a tie.

If there are no Blackjacks, then the gambler must choose to 'stick' (not get anymore cards) or 'twist' (add 1 more card to their hand). 
