//Blackjack
//Plays a simple version of the casino game of blackjack; for 1 - 7 players

//Headers
#include <iostream> 
#include <string> 
#include <vector> 
#include <algorithm>
#include <ctime>

using namespace std; 

//Class for an individual playing card:
class Card
{
public:
	enum rank { ACE = 1, TWO, THREE, FOUR, FIVE, SIX, SEVEN, EIGHT, NINE, TEN, JACK, QUEEN, KING }; //enumeration for all 13 ranks
	enum suit { CLUBS, DIAMONDS, HEARTS, SPADES }; //enumeration for all 4 suits

	//overloading << operator so can send Card object to standard output 
	friend ostream& operator<<(ostream& os, const Card& aCard);

	Card(rank r = ACE, suit s = SPADES, bool ifu = true);

	//returns the value of a card, 1 - 11 
	int GetValue() const;

	//flips a card; if face up, becomes face down and vice versa 
	void Flip();

private:
	rank m_Rank; //rank of the card
	suit m_Suit; //suit of the card
	bool m_IsFaceUp; //indicates whether the card is face up
};

//definitoin for Card() constructor
Card::Card(rank r, suit s, bool ifu) : m_Rank(r), m_Suit(s), m_IsFaceUp(ifu) 
{}

//returns the value of the card
int Card::GetValue() const 
{
		//if a cards is face down, its value is 0 
		int value = 0;

		//if card is face up...
		if (m_IsFaceUp)
		{
			//value is number showing on card 
			value = m_Rank;
			//value is 10 for face cards
			if (value > 10)
			{
				value = 10;
			}
		}
		return value;
}

//flips a card; if face up, becomes face down and vice versa 
void Card::Flip() 
{
	m_IsFaceUp = !(m_IsFaceUp);
}

//Class for a collection of cards
class Hand 
{
public:
	Hand(); //set hand capacity

	virtual ~Hand(); //clear hand

	//adds a card to the hand 
	void Add(Card* pCard);

	//clears hand of all cards 
	void Clear();

	//gets hand total value, intelligently treats aces as 1 or 11
	int GetTotal() const;

protected:
	vector<Card*> m_Cards; //vector of card pointers
};

//set hand capacity to 7
Hand::Hand()
{
	m_Cards.reserve(7);
}

//clear hand
Hand::~Hand()
{
	Clear();
}

//add card to hand
void Hand::Add(Card* pCard) 
{
	m_Cards.push_back(pCard);
}

//clear vector of pointers in hand
void Hand::Clear() 
{
	//iterate through vector, freeing all memory on the heap 
	vector<Card*>::iterator iter = m_Cards.begin();
	for (iter = m_Cards.begin(); iter != m_Cards.end(); ++iter) 
	{
		delete *iter;
		*iter = 0;
	}
	//clear vector of pointers
	m_Cards.clear();
}

//gets total value of the hand
int Hand::GetTotal() const 
{
	//if no cards in hand, return 0 
	if (m_Cards.empty())
	{
		return 0;
	}

	//if a first card has value of 0, then card is face down; return 0 
	if (m_Cards[0]->GetValue() == 0)
	{
		return 0; 
	}

	//add up card values, treat each ace as 1
	int total = 0; //set total to 0
	vector<Card*>::const_iterator iter; //establish vector of cards and a constant iterator
	for (iter = m_Cards.begin(); iter != m_Cards.end(); ++iter) //iterate through cards in hand
	{
		total += (*iter)->GetValue(); //add values of each card up
	}

	//determine if hand contains an ace
	bool containsAce = false; //hand does not contain ace
	for (iter = m_Cards.begin(); iter != m_Cards.end(); ++iter) //iterate through cards
	{
		if ((*iter)->GetValue() == Card::ACE) //if card is an ace
		{
			containsAce = true; //hand does contain ace
		}
	}

	//if hand contains ace and total is low enough, treat ace as 11 
	if (containsAce && total <= 11)
	{
		//add only 10 since we’ve already added 1 for the ace
		total += 10; 
	}

	return total;
}

//Class for a gnereic Blackjack player:
class GenericPlayer : public Hand 
{
	//overload function, allows Genericplayer objects to be displayed on screen
	friend ostream& operator<<(ostream& os, const GenericPlayer& aGenericPlayer);

public:
	//generic player constructor prototype
	GenericPlayer(const string& name = "");

	virtual ~GenericPlayer(); //GenericPlayer destructor prototype

	//indicates whether or not generic player wants to keep hitting 
	virtual bool IsHitting() const = 0;

	//returns whether generic player has busted - has a total greater than 21 
	bool IsBusted() const;

	//announces that the generic player busts 
	void Bust() const;

protected:
	string m_Name; //name of player
};

//generic player constructor definition
GenericPlayer::GenericPlayer(const string& name) : m_Name(name)
{}

//generic player destructor
GenericPlayer::~GenericPlayer()
{}

//function that decides player is busted if total is over 21
bool GenericPlayer::IsBusted() const 
{
	return (GetTotal() > 21);
}

//function that displays the player who busts
void GenericPlayer::Bust() const 
{
	cout << m_Name << " busts.\n";
}

//Class for a human player, derived from generic player
class Player : public GenericPlayer 
{
public:
	//constructor for player name
	Player(const string& name = ""); 
	
	//destructor for player
	virtual ~Player();

	//returns whether or not the player wants another hit 
	virtual bool IsHitting() const;

	//announces that the player wins 
	void Win() const;

	//announces that the player loses 
	void Lose() const;

	//announces that the player pushes
	void Push() const;
};

//definition for player constructor
Player::Player(const string& name) : GenericPlayer(name)
{}

//definition for player destructor
Player::~Player()
{}

//asks player if they want to hit, returns T/F
bool Player::IsHitting() const 
{
	cout << m_Name << ", do you want a hit? (Y/N): "; 
	char response;
	cin >> response;
	return (response == 'y' || response == 'Y');
}

//announces winner
void Player::Win() const 
{
	cout << m_Name << " wins.\n";
}

//announces loser
void Player::Lose() const 
{
	cout << m_Name << " loses.\n";
}

//announces when a player pushes
void Player::Push() const 
{
	cout << m_Name << " pushes.\n";
}

//class representing house, derived from generic player
class House : public GenericPlayer 
{
public:
	//constructor for house
	House(const string& name = "House"); 
	
	//descructor for house
	virtual ~House();

	//indicates whether house is hitting - will always hit on 16 or less 
	virtual bool IsHitting() const;

	//flips over first card
	void FlipFirstCard();
};

//definition for house constructor
House::House(const string& name) : GenericPlayer(name)
{}

//definition for house destructor
House::~House()
{}

//definition for IsHitting, house hits if total is 16 or less
bool House::IsHitting() const 
{
	return (GetTotal() <= 16);
}

//flips first card if cards are not empty, otherwise announces there is no card to flip
void House::FlipFirstCard() 
{
	if (!(m_Cards.empty())) 
	{
		m_Cards[0]->Flip();
	}
	else 
	{
		cout << "No card to flip!\n";
	}
}

//Class representing a deck of cards, derived from Hand
class Deck : public Hand 
{
public:
	//deck constructor
	Deck();

	//deck destructor
	virtual ~Deck();

	//create a standard deck of 52 cards 
	void Populate();

	//shuffle cards 
	void Shuffle();
	
	//deal one card to a hand 
	void Deal(Hand& aHand);

	//give additional cards to a generic player
	void AdditionalCards(GenericPlayer& aGenericPlayer);
};

//reserves a max of 52 cards for the deck, runs Populate()
Deck::Deck()
{
	m_Cards.reserve(52);
	Populate();
}

//deck destructor
Deck::~Deck()
{}

//clears the cards, then creates a new standard deck
void Deck::Populate() 
{
	Clear();
	
	//create standard deck
	for (int s = Card::CLUBS; s <= Card::SPADES; ++s) //for loop through suits
	{
		for (int r = Card::ACE; r <= Card::KING; ++r) //for loop through values
		{
			Add(new Card(static_cast<Card::rank>(r), static_cast<Card::suit>(s))); //casts the int loop variables to the proper enumerated types
		}
	}
}

//shuffles the cards in the deck by randomly arraging the pointers in th m_Cards with random_suffle() from the Standard TGemplate Library 
void Deck::Shuffle()
{
	random_shuffle(m_Cards.begin(), m_Cards.end());
}

//deals one card from the deck to the hand
void Deck::Deal(Hand& aHand) 
{
	//if deck is not empty..
	if (!m_Cards.empty()) 
	{
		aHand.Add(m_Cards.back()); //add a card to Hand, referencing the last element in deck
		m_Cards.pop_back(); //remove card from deck
	}
	else 
	{
		cout << "Out of cards. Unable to deal."; //display that the deck is out of cards
	}
}

void Deck::AdditionalCards(GenericPlayer& aGenericPlayer) 
{
	cout << endl;
	//continue to deal a card as long as generic player isn’t busted and 
	//wants another hit
	while (!(aGenericPlayer.IsBusted()) && aGenericPlayer.IsHitting()) 
	{
		Deal(aGenericPlayer);
		cout << aGenericPlayer << endl;

		if (aGenericPlayer.IsBusted())
		{
			aGenericPlayer.Bust();
		}
	}
}

//Class representing a game of Blackjack
class Game 
{
public:
	Game(const vector<string>& names); //class constructor, accepts a reference to a vector of string objects(player names)
	
	~Game(); //game destructor

	//plays the game of blackjack 
	void Play(); //play function

private:
	Deck m_Deck; //deck
	House m_House; //cpu player
	vector<Player> m_Players; //human players
};

Game::Game(const vector<string>& names) 
{
	//create a vector of players from a vector of names 
	vector<string>::const_iterator pName;

	// iterate through player names
	for (pName = names.begin(); pName != names.end(); ++pName) 
	{
		m_Players.push_back(Player(*pName)); //add player pointer to vector of players
	}

	//seed the random number generator 
	srand(static_cast<unsigned int>(time(0)));
	m_Deck.Populate(); //populate deck
	m_Deck.Shuffle(); //shuffle deck
}

//definition for game destructor
Game::~Game()
{}

//plays a round of blackjack
void Game::Play() 
{
	//deal initial 2 cards to everyone 
	vector<Player>::iterator pPlayer; 
	for (int i = 0; i < 2; ++i) //iteration for 2 cards each
	{
		for (pPlayer = m_Players.begin(); pPlayer != m_Players.end(); ++pPlayer) //iterate through players
		{
			m_Deck.Deal(*pPlayer); //deals cards to players
		}
		m_Deck.Deal(m_House); //deals cards to house
	}

	//hide house’s first card 
	m_House.FlipFirstCard();

	//display everyone’s hand
	for (pPlayer = m_Players.begin(); pPlayer != m_Players.end(); ++pPlayer)  //iterate through players
	{
		cout << *pPlayer << endl; //display player's cards
	}
	cout << m_House << endl; //display house's card

	//deal additional cards to players
	for (pPlayer = m_Players.begin(); pPlayer != m_Players.end(); ++pPlayer)  //iterate through players
	{
		m_Deck.AdditionalCards(*pPlayer); //deal additional cards
	}

	//reveal house’s first card 
	m_House.FlipFirstCard(); 
	cout << endl << m_House;

	//deal additional cards to house 
	m_Deck.AdditionalCards(m_House);

	//if house busts, everyone else wins
	if (m_House.IsBusted()) 
	{
		//everyone still playing wins
		for (pPlayer = m_Players.begin(); pPlayer != m_Players.end(); ++pPlayer)  //iterate through players
		{
			if (!(pPlayer->IsBusted())) //if player has not also busted
			{
				pPlayer->Win(); //player wins
			}
		}
	}
	else
	{
		//compare each player still playing to house
		for (pPlayer = m_Players.begin(); pPlayer != m_Players.end(); ++pPlayer)  //iterate through players
		{
			if (!(pPlayer->IsBusted())) //if player has not busted
			{
				if (pPlayer->GetTotal() > m_House.GetTotal()) //if player's total is greater than house
				{
					pPlayer->Win(); //player wins
				}
				else if (pPlayer->GetTotal() < m_House.GetTotal()) //if player's total is less than house
				{
					pPlayer->Lose(); //player loses
				}
				else 
				{
					pPlayer->Push(); //player pushes
				}
			}
		}
	}

	//remove everyone’s cards
	for (pPlayer = m_Players.begin(); pPlayer != m_Players.end(); ++pPlayer) //iterate through players
	{
		pPlayer->Clear(); //clear players' cards
	}
	m_House.Clear(); //clear house's cards
}


//function prototypes
ostream& operator<<(ostream& os, const Card& aCard); //overloaded operator for Card object to be sent to cout
ostream& operator<<(ostream& os, const GenericPlayer& aGenericPlayer); //overloaded operator for GenericPlayer object to be sent to cout

//main function
int main() 
{
	cout << "\t\tWelcome to Blackjack!\n\n"; //welcome text

	int numPlayers = 0; //initial number of players
	while (numPlayers < 1 || numPlayers > 7) 
	{
		cout << "How many players? (1 - 7): "; //asks how many players
		cin >> numPlayers; //returns number of players
	}

	vector<string> names; //initializes a vector for player names
	string name;
	//iterate through players and put names in vector
	for (int i = 0; i < numPlayers; ++i)
	{
		cout << "Enter player name: "; 
		cin >> name; 
		names.push_back(name);
	}
	cout << endl;

	//the game loop
	Game aGame(names);
	char again = 'y';
	//asks player if they want to play again, plays until answer is N
	while (again != 'n' && again != 'N') 
	{
		aGame.Play();
		cout << "\nDo you want to play again? (Y/N): "; 
		cin >> again;
	}

	return 0;
}

//overloads << operator so Card object can be sent to cout 
ostream& operator<<(ostream& os, const Card& aCard)
{
	const string RANKS[] = { "0", "A", "2", "3", "4", "5", "6", "7", "8", "9", "10", "J", "Q", "K" }; //string of ranks
	const string SUITS[] = { "c", "d", "h", "s" }; //string of suits

	if (aCard.m_IsFaceUp) //display for when card is face up
	{
		os << RANKS[aCard.m_Rank] << SUITS[aCard.m_Suit];
	}
	else 
	{
		os << "XX"; //display for when card is face down
	}

	return os; //return the display
}

//overloads << operator so a GenericPlayer object can be sent to cout 
ostream& operator<<(ostream& os, const GenericPlayer& aGenericPlayer) 
{
os << aGenericPlayer.m_Name << ":\t"; //display for player name

vector<Card*>::const_iterator pCard; //vector of cards
if (!aGenericPlayer.m_Cards.empty()) //as long as player's cards are not empty
{
	for (pCard = aGenericPlayer.m_Cards.begin(); pCard != aGenericPlayer.m_Cards.end(); ++pCard) //iterate through cards
	{
		os << *(*pCard) << "\t"; //display card
	}
	if (aGenericPlayer.GetTotal() != 0) //if total is not zero
	{
		cout << "(" << aGenericPlayer.GetTotal() << ")"; //display this total
	}
}
else 
{
	os << "<empty>"; //else it is empty
}
return os; //return display
}