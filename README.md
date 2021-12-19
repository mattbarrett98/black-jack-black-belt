# Intro

A project coding the popular casino game of Blackjack in C++. We code the game using object oriented programming (OOP), taking advantage of OOP concepts such as constructors, inheritance and composition. We then use reinforcement learning to try and learn the optimal strategy for Blackjack - in particular we will use the Monte Carlo control algorithm and the Q learning algorithm. This project is unique from a lot of material online since here we allow the optimal strategy to vary with the card count, whereas generally policies online are invariant to the card count.  

# Set up 

In our implementation we play using only 1 deck of cards, although in casinos nowadays they will use around 6-8 decks shuffled together to negate the effect of card counting. We do this in an attempt to investigate how card counters would have been able to make a profit in the days where only a single deck was used. We are also only considering the case where there is 1 gambler playing against the dealer, although in real life multiple other gamblers may also be playing. 

The aim of the game is to get the sum of your cards to be as close as possible to 21, but not above, as this would result in you being 'bust' and losing. Cards 2-10 are worth the number on them, and Jacks, Queens and Kings are worth 10. An ace is worth 11 as long as valuing it as such doesn't cause the player to be bust, in this case the value changes to 1. 

The game starts with both the gambler and dealer receiving a hand of 2 cards each. A player has 'Blackjack' if they have an ace and a card worth 10, meaning their hand has the highest value of 21. If there are any Blackjacks present the game will end immediately in one of three ways:
* If the gambler has Blackjack and the dealer does not, the gambler wins automatically.
* Alternatively if the dealer has the only Blackjack, they win automatically.
* If they both have Blackjack, it is a draw.

If there are no Blackjacks, then the gambler must choose to 'stand' (not get anymore cards) or 'hit' (add 1 more card to their hand). If the gambler goes bust at any point they lose the game. If the gambler is not bust after making his decisions, then the dealer must hit or stand. Unlike the gambler, the dealer has no choice in his actions, they must stand if the value of their hand is ≥ 17, and hit otherwise. Finally, if the dealer busts he loses, or if not:
* The player with the higher hand value wins,
* Or it is a draw if they both have the same score.

At the start of each game, the dealer checks if the deck has at least 13 cards (1/4 of the deck) in it. If it does they deal as normal, if not they first shuffle all cards back together to reset the deck and then deal. 

## Split

In addition to hitting and standing, the gambler has the choice to split after receiving their inital two cards if these cards have the same value. In this case they will separate their cards into two hands and are dealt another card for each hand, with the gambler's initial bet wagered on each hand. In the case of split aces, the gambler will receive the extra card for each hand and then has to stand on both of them. For any other split value, the gambler is allowed to take any further actions they like, playing each of their hands one at a time.       

## Double down

The last action that we allow the gambler in this implementation is double down. This is where after receiving their initial two cards, the gambler can double his bet under the condition that they will receive one and only one more card. Here we will allow the gambler to double down after splitting their hand (except of course for split aces).

## Card counting

As mentioned we are going to simulate the games under the assumption that the gambler knows how to count cards. We will use the most common card counting strategy, the Hi-Lo strategy. It works by keeping a running count for the deck, which starts at 0 when the deck has just been shuffled and no cards have yet been dealt from the deck. The gambler adjusts the running count each time they observe a new card in the following way:
* +1 for cards 2, 3, 4, 5, 6
* +0 for cards 7, 8, 9
* -1 for cards 10, J, Q, K, A 
 

# Reinforcement learning
The state at any time is defined by 4 values: the current sum of the gamblers hand ∈ {4,5,...,21}, whether or not the gambler has an ace still worth 11 (1 if they do, 0 if not), the value of the dealer's face up card ∈ {2,3,...,11} and the current card count ∈ {-20,-19,...,20}. The possible actions of the gambler are just stick or twist. 

The first approach we use is a simple Monte Carlo approach. We simulate a game and apply the received reward to every state-action pair observed in the game. We define the received reward to be +1 if the gambler wins, 0 for a draw and -1 for a loss. After simulating a large number of games we calculate the mean reward for each state-action pair, and for each state we pick the optimal action to be the one with the highest mean reward. In RL terminology, we have 

![formula](https://render.githubusercontent.com/render/math?math=\Large\pi(s)=\argmax_{a%20\in%20A}Q(s,a)) 

where π(s) is our estimate of the optimal policy, A is the action space and Q(s,a) is the mean reward attained taking action a in state s. The policy of the gambler in these simulations is purely random, sticking or twisting with probability 0.5 no matter the state. We use multithreading to allow us to parallelise the simulations and take advantage of each of the 8 cores on our cpu. 

We play 125 million games in each of our 8 threads, for a total of 1 billion games (taking 18 minutes). Using the optimal policy found from this we play a further 1 million games in order to evaluate the policy. We find that under this policy, the gambler:
* wins 42.69%
* draws 7.18%
* loses 50.13%.

It seems as though we could improve on this by using a more appropriate algorithm to find the optimal policy. For the Monte Carlo approach, the reward is applied to all state action pairs observed. This means that if the gambler wins the game then all the actions that he took in that game will be rewarded. This is problematic since it is likely that not all of the actions were actually the best choice, since their policy in this exploration phase is completely random. We propose instead the Q learning algorithm. We change the meaning of Q(s,a) to be the Q value associated with taking action a in state s. All Q values start at 0 and after each game we update our Q values for each state-action pair observed in the game:

![formula](https://render.githubusercontent.com/render/math?math=\Large\Q(s,a)=Q(s,a)%2B\alpha\[r%2B\gamma\max_{a%20\in%20A}Q(s^{%27},a)%2DQ(s,a)\].)

Here α is the learning rate, γ is the discount value and r is the reward for taking action a in state s. For the terminal state-action pair the reward is +1, 0, or -1 for a win, draw or loss, and r = 0 for all non-terminal state-action pairs. Also, s' is the state obtained after taking action a in state s. The max term is an estimate of the optimal future reward, which is 0 for the terminal state-action pair since there are no more actions to be taken. Setting γ = 1, we have 

![formula](https://render.githubusercontent.com/render/math?math=\Large\Q(s,a)=Q(s,a)%2B\alpha\[\max_{a%20\in%20A}Q(s^{%27},a)%2DQ(s,a)\])

for any non-terminal state-action pairs, and 

![formula](https://render.githubusercontent.com/render/math?math=\Large\Q(s,a)=Q(s,a)%2B\alpha\[r%2DQ(s,a)\])

for the terminal state-action pair.

We play 10 million games (with random policy) and perform these Q value updates with α = 0.001. Then defining the optimal action given any state to be the one with the highest Q value, and playing another 1 million games adopting these optimal actions, the gambler:
* wins 43.29%
* draws 8.30%
* loses 48.42%.

Looking deeper into the results of this Q learning algorithm, I noticed that the current card count has little to no effect on the optimal action for each state i.e. the optimal policy is roughly the same regardless of what the card count is. The use of card counting becomes apparent when we look at the win rate for different card counts:

Card count before game | -4 | -3 | -2 | -1 | 0 | 1 | 2 | 3 | 4 |
--- | --- | --- | --- | --- | --- | --- | --- | --- | --- |
% games won | 42.33 | 42.24 | 42.88 | 42.98 | 43.24 | 43.71 | 43.66 | 43.83 | 44.62 |

As the card count increases, the win rate increases as well. In plain terms, as the proportion of 10s and aces in the deck increases, the gambler becomes more likely to win. This is also of benefit to the gambler since more 10s and aces means more chance of getting Blackjack, which win more money than non-Blackjack wins. 

# Betting

Now we will explore how we can use the card count to the benefit of the gambler. So far we have looked at the win rates, but of course the objective of Blackjack is to try and make money. Now we know the gambler's chances of winning go up with the card count, we propose a realistic betting strategy that takes advantage of this. 

For a card count ≤ 1, we bet the minimum. Ideally for negative card counts we would not bet since we know we are less likely to win, however this would likely attract suspicion. For a card count > 1, we bet the card count * minimum bet up to a maximum of 6 * minimum bet. Setting a maximum bet reduces the spread of your bets which again reduces chances of arousing suspicion and being removed from the casino. Let's assume a minimum bet of $1 and a card count c, then the bet is

![formula](https://render.githubusercontent.com/render/math?math=\Large\text{%241%20%20if%20%20c%20}%3C%3D\1)

![formula](https://render.githubusercontent.com/render/math?math=\Large\text{%24c%20%20if%20%202}%3C%3D\text{c}%3C%3D\5)

![formula](https://render.githubusercontent.com/render/math?math=\Large\text{%246%20%20if%20%20c%20}%3E%3D\6.)


