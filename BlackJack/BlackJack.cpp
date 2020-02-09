// BlackJack.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <cstring>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <algorithm>
#include "Utils.h"

using namespace std;

const char* INPUT_ERROR_STRING = "Input Error! Please try again. ";

/* Enums */

enum
{
	PLAYER_NAME_SIZE = 25,
	CARD_NAME_SIZE = 25,
	DECK_SIZE = 52,
	NUM_OF_SUITS = 4,
	NUM_OF_CARDS_PER_SUIT = 13,
};

enum PlayerType 
{
	PT_HUMAN = 0,
	PT_DEALER
};

enum CardSuit 
{
	CS_HEART = 0,
	CS_DIAMOND,
	CS_CLUB,
	CS_SPADE
};

enum ActionType 
{
	AT_HIT = 0,
	AT_STAY
};

enum CardType
{
	CT_ACE = 0,
	CT_TWO,
	CT_THREE,
	CT_FOUR,
	CT_FIVE,
	CT_SIX,
	CT_SEVEN,
	CT_EIGHT,
	CT_NINE,
	CT_TEN,
	CT_JACK,
	CT_QUEEN,
	CT_KING
};

enum CardValue
{
	CV_TWO = 2,
	CV_THREE = 3,
	CV_FOUR = 4,
	CV_FIVE = 5,
	CV_SIX = 6,
	CV_SEVEN = 7,
	CV_EIGHT = 8,
	CV_NINE = 9,
	CV_TEN = 10,
	CV_JACK = 10,
	CV_QUEEN = 10,
	CV_KING = 10,
	CV_ACE = 11
};

/* Structs */

struct Card
{
	CardSuit cardSuit;
	CardType cardType;
	int value;
};

struct Player
{
	PlayerType playerType;
	char playerName[PLAYER_NAME_SIZE];
	Card playerHand[5];
};

struct Deck
{
	Card cards[DECK_SIZE];
};

/* Game Functions */

void PlayGame(Player& player, Player& dealer, Deck& deck);
bool WantToPlayAgain();
void DisplayWinner(int& playerSum, int& dealerSum, bool playerFailure, bool dealerFailure);

/* Deal Cards and View */

void AddCardToHand(Player& player, Deck& deck, int& currentCard, int& playerCard, int& playerHand);
void ViewPlayerHand(Player& player, int length);
void ViewDealerHand(Player& dealer, int length, bool turn);
const char* checkCardSuit(Card& card);
const char* checkCardType(Card& card);

/* Initialize Players, Cards, and Deck */

void InitializePlayer(Player& player, const char* playerName);
void InitializeCard(Card& card, CardType cardType, CardSuit cardSuit, int value);
void AddCardsToDeck(Deck& deck, CardSuit cardSuit, int start);
int CalculateSum(Player& player, int length);

/* Player/Dealer Actions */

ActionType AskForAction();
ActionType FindAction(char action);

/* Changing card values */

bool promptToChangeValueOfCard(Player& player, int playerHandLength, Card& card);
void changeValueOfCard(Card& card, int newValue);




int main()
{

	srand(time(0));

	Player player1;
	Player dealer;

	Deck blackjackDeck;
	AddCardsToDeck(blackjackDeck, CS_HEART, 0);
	AddCardsToDeck(blackjackDeck, CS_DIAMOND, 13);
	AddCardsToDeck(blackjackDeck, CS_CLUB, 26);
	AddCardsToDeck(blackjackDeck, CS_SPADE, 39);

	InitializePlayer(player1, "Player");
	InitializePlayer(dealer, "Dealer");

	do
	{

		PlayGame(player1, dealer, blackjackDeck);

	} while (WantToPlayAgain());
	
	return 0;
}

void PlayGame(Player& player, Player& dealer, Deck& deck)
{
	ClearScreen();

	player.playerType = PT_HUMAN;
	dealer.playerType = PT_DEALER;

	int n = sizeof(deck.cards) / sizeof(deck.cards[0]);

	random_shuffle(deck.cards, deck.cards + n);

	int currentDeckCard = 0;
	int playerCard = 0;
	int dealerCard = 0;
	int sumPlayerHand = 0;
	int sumDealerHand = 0;
	int playerHandLength = 0;
	int dealerHandLength = 0;

	bool playerTurn = false;
	bool dealerTurn = false;
	bool didPlayerFail = false;
	bool didDealerFail = false;

	ActionType action;

	AddCardToHand(player, deck, currentDeckCard, playerCard, playerHandLength);
	AddCardToHand(player, deck, currentDeckCard, playerCard, playerHandLength);
	AddCardToHand(dealer, deck, currentDeckCard, dealerCard, dealerHandLength);
	AddCardToHand(dealer, deck, currentDeckCard, dealerCard, dealerHandLength);

	ViewPlayerHand(player, playerHandLength);

	sumPlayerHand = CalculateSum(player, playerHandLength);
	sumDealerHand = CalculateSum(dealer, dealerHandLength);

	bool gameOver = false;

	do 
	{
		if (sumPlayerHand == 21)
		{
			cout << "Wow, you already have 21!" << endl;
			gameOver = true;
		}
		else
		{
			for (int i = 0; i < playerHandLength; i++)
			{
				if (player.playerHand[i].cardType == CT_ACE)
				{
					bool input;
					input = promptToChangeValueOfCard(player, playerHandLength, player.playerHand[i]);
					if (input == true)
					{
						changeValueOfCard(player.playerHand[i], 1);
						sumPlayerHand = 0;
						sumPlayerHand = CalculateSum(player, playerHandLength);
					}
				}
			}
			do
			{
				cout << "------------------------------------------------" << endl;
				cout << "Your current sum is: " << sumPlayerHand << endl;
				playerTurn = true;
				action = AskForAction();

				if (action == AT_HIT)
				{
					cout << "Adding card to hand..." << endl;
					AddCardToHand(player, deck, currentDeckCard, playerCard, playerHandLength);
					ViewPlayerHand(player, playerHandLength);
					for (int i = 0; i < playerHandLength; i++)
					{
						if (player.playerHand[i].cardType == CT_ACE)
						{
							bool input;
							input = promptToChangeValueOfCard(player, playerHandLength, player.playerHand[i]);
							if (input == true)
							{
								changeValueOfCard(player.playerHand[i], 1);
								sumPlayerHand = 0;
								sumPlayerHand = CalculateSum(player, playerHandLength);
							}
						}
					}
					sumPlayerHand = CalculateSum(player, playerHandLength);
					if (sumPlayerHand == 21)
					{
						cout << "You have 21!" << endl;
						playerTurn = false;
						gameOver = true;
					}
					else if (sumPlayerHand > 21)
					{
						cout << "Oh no! You've exceeded 21!" << endl;
						playerTurn = false;
						gameOver = true;
						didPlayerFail = true;
					}
					else
					{
						cout << "You're sum is now: " << sumPlayerHand << endl;
					}
				}
				else
				{
					cout << "Choosing to stay with sum of: " << sumPlayerHand << endl;
					playerTurn = false;
				}
			} while (playerTurn);
		}
		if (gameOver == false)
		{
			do
			{
				cout << "------------------------------------------------" << endl;
				dealerTurn = true;
				cout << "Dealer is taking their turn now..." << endl;
				ViewDealerHand(dealer, dealerHandLength, dealerTurn);
				sumDealerHand = CalculateSum(dealer, dealerHandLength);
				cout << "Dealer's sum is: " << sumDealerHand << endl;

				if (sumDealerHand == 21)
				{
					cout << "Dealer has 21, they are staying obviously" << endl;
					dealerTurn = false;
					gameOver = true;
				}
				else if (sumDealerHand > 21)
				{
					cout << "The Dealer failed and exceeded 21!" << endl;
					dealerTurn = false;
					didDealerFail = true;
					gameOver = true;
				}
				else if (sumDealerHand < 17 && sumDealerHand < sumPlayerHand)
				{
					cout << "Dealer is hitting to stay in the game..." << endl;
					AddCardToHand(dealer, deck, currentDeckCard, dealerCard, dealerHandLength);
				}
				else if (sumDealerHand == sumPlayerHand)
				{
					cout << "Dealer is tied with you, so they are going to hit..." << endl;
					AddCardToHand(dealer, deck, currentDeckCard, dealerCard, dealerHandLength);
				}
				else
				{
					cout << "Dealer is choosing to stay" << endl;
					dealerTurn = false;
					gameOver = true;
				}
			} while (dealerTurn);
		}
	} while (!gameOver);
	
	DisplayWinner(sumPlayerHand, sumDealerHand, didPlayerFail, didDealerFail);

}

bool promptToChangeValueOfCard(Player& player, int playerHandLength, Card& card)
{
	char input;
	
	const char validInput[] = { 'y', 'n' };

	input = GetCharacter("Would you like to change the value of this Ace?", INPUT_ERROR_STRING, validInput, 2, CC_LOWER_CASE);

	return input == 'y';
}

void changeValueOfCard(Card& card, int newValue)
{
	card.value = newValue;
}

int CalculateSum(Player& player, int length)
{
	int sum = 0;

	for (int i = 0; i < length; i++)
	{
		sum = sum + player.playerHand[i].value;
	}

	return sum;
}

void DisplayWinner(int& playerSum, int& dealerSum, bool playerFailure, bool dealerFailure)
{
	if (playerFailure == true)
	{
		cout << "You're run is over, you should be more careful in the future." << endl;
	}
	else if (dealerFailure == true)
	{
		cout << "Congrats on your victory since the dealer went over 21!" << endl;
	}
	else if (playerSum > dealerSum)
	{
		cout << "You won with the dealer only managing " << dealerSum << "! congrats on winning with a sum of: " << playerSum << endl;
	}
	else if (playerSum == dealerSum)
	{
		cout << "You tied the dealer, who had " << dealerSum << ", and you had " << playerSum << endl;
	}
	else
	{
		cout << "Dealer wins with a sum of " << dealerSum << "! Better luck next time player." << endl;
	}
}

ActionType AskForAction()
{
	char action;
	const char validActions[] = { 'H', 'S' };

	action = GetCharacter("Please choose whether to Hit (H) or Stay (S): ", INPUT_ERROR_STRING, validActions, 2, CC_UPPER_CASE);

	return FindAction(action);
}

ActionType FindAction(char action)
{
	ActionType newAction;

	if (action == 'H')
	{
		newAction = AT_HIT;
	}
	else
	{
		newAction = AT_STAY;
	}
	return newAction;
}

bool WantToPlayAgain()
{
	char input;

	const char validInput[2] = { 'y', 'n' };

	input = GetCharacter("Would you like to play again? (y/n): ", INPUT_ERROR_STRING, validInput, 2, CC_LOWER_CASE);

	return input == 'y';
}

void AddCardToHand(Player& player, Deck& deck, int& currentCard, int& playerCard, int& playerHand)
{
	player.playerHand[playerCard] = deck.cards[currentCard];
	currentCard++;
	playerCard++;
	playerHand++;
}

void ViewPlayerHand(Player& player, int length)
{
	for (int h = 0; h < length; h++)
	{
			cout << "You have: an " << checkCardType(player.playerHand[h]) << " of " << checkCardSuit(player.playerHand[h]) << endl;
	}
}

void ViewDealerHand(Player& dealer, int length, bool turn)
{	
	if (turn == false)
	{
		cout << "Dealer has: an " << checkCardType(dealer.playerHand[0]) << " of " << checkCardSuit(dealer.playerHand[0]) << endl;
	}
	else
	{
		for (int h = 0; h < length; h++)
		{
			cout << "The Dealer has: an " << checkCardType(dealer.playerHand[h]) << " of " << checkCardSuit(dealer.playerHand[h]) << endl;
		}
	}
}

const char* checkCardSuit(Card& card)
{
	if (card.cardSuit == CS_HEART)
	{
		return "Hearts";
	}
	else if(card.cardSuit == CS_DIAMOND)
	{
		return "Diamonds";
	}
	else if (card.cardSuit == CS_CLUB)
	{
		return "Clubs";
	}
	else if(card.cardSuit == CS_SPADE)
	{
		return "Spades";
	}
	
	return "NONE";
}

const char* checkCardType(Card& card)
{
	switch (card.cardType)
	{
	case 0:
		return "Ace";
		break;
	case 1:
		return "Two";
		break;
	case 2:
		return "Three";
		break;
	case 3:
		return "Four";
		break;
	case 4:
		return "Five";
		break;
	case 5:
		return "Six";
		break;
	case 6:
		return "Seven";
		break;
	case 7:
		return "Eight";
		break;
	case 8:
		return "Nine";
		break;
	case 9:
		return "Ten";
		break;
	case 10:
		return "Jack";
		break;
	case 11:
		return "Queen";
		break;
	case 12:
		return "King";
		break;
	default:
		return "NONE";
	}
}


void AddCardsToDeck(Deck& deck, CardSuit cardSuit, int start)
{
	InitializeCard(deck.cards[start], CT_ACE, cardSuit, CV_ACE);
	InitializeCard(deck.cards[start+1], CT_TWO, cardSuit, CV_TWO);
	InitializeCard(deck.cards[start+2], CT_THREE, cardSuit, CV_THREE);
	InitializeCard(deck.cards[start+3], CT_FOUR, cardSuit, CV_FOUR);
	InitializeCard(deck.cards[start+4], CT_FIVE, cardSuit, CV_FIVE);
	InitializeCard(deck.cards[start+5], CT_SIX, cardSuit, CV_SIX);
	InitializeCard(deck.cards[start+6], CT_SEVEN, cardSuit, CV_SEVEN);
	InitializeCard(deck.cards[start+7], CT_EIGHT, cardSuit, CV_EIGHT);
	InitializeCard(deck.cards[start+8], CT_NINE, cardSuit, CV_NINE);
	InitializeCard(deck.cards[start+9], CT_TEN, cardSuit, CV_TEN);
	InitializeCard(deck.cards[start+10], CT_JACK, cardSuit, CV_JACK);
	InitializeCard(deck.cards[start+11], CT_QUEEN, cardSuit, CV_QUEEN);
	InitializeCard(deck.cards[start+12], CT_KING, cardSuit, CV_KING);
}

void InitializePlayer(Player& player, const char* playerName)
{
	if (playerName != nullptr && strlen(playerName) > 0)
	{
		strcpy_s(player.playerName, playerName);
	}
}

void InitializeCard(Card& card, CardType cardType, CardSuit cardSuit, int value)
{
	card.cardSuit = cardSuit;
	card.cardType = cardType;
	card.value = value;
}

