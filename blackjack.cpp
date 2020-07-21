#include <algorithm> // std::shuffle
#include <array>
#include <ctime> // std::time
#include <iostream>
#include <random> // std::mt19937

//using enums for CardSuits and CardRanks
enum class CardSuit
{
  SUIT_CLUB,
  SUIT_DIAMOND,
  SUIT_HEART,
  SUIT_SPADE,
 
  MAX_SUITS
};
 
enum class CardRank
{
  RANK_2,
  RANK_3,
  RANK_4,
  RANK_5,
  RANK_6,
  RANK_7,
  RANK_8,
  RANK_9,
  RANK_10,
  RANK_JACK,
  RANK_QUEEN,
  RANK_KING,
  RANK_ACE,
 
  MAX_RANKS
};

//using stuct for Card
struct Card
{
	CardRank rank;
	CardSuit suit;	
};

struct Player
{
	int score;
};

using deck_type = std::array<Card, 52>;
using index_type = deck_type::size_type;
 
// Maximum score before losing.
constexpr int maximumScore{ 21 };
 
// Minimum score that the dealer has to have.
constexpr int minimumDealerScore{ 17 };

void printCard(const Card& card)
{
  switch (card.rank)
  {
  case CardRank::RANK_2:
    std::cout << '2';
    break;
  case CardRank::RANK_3:
    std::cout << '3';
    break;
  case CardRank::RANK_4:
    std::cout << '4';
    break;
  case CardRank::RANK_5:
    std::cout << '5';
    break;
  case CardRank::RANK_6:
    std::cout << '6';
    break;
  case CardRank::RANK_7:
    std::cout << '7';
    break;
  case CardRank::RANK_8:
    std::cout << '8';
    break;
  case CardRank::RANK_9:
    std::cout << '9';
    break;
  case CardRank::RANK_10:
    std::cout << 'T';
    break;
  case CardRank::RANK_JACK:
    std::cout << 'J';
    break;
  case CardRank::RANK_QUEEN:
    std::cout << 'Q';
    break;
  case CardRank::RANK_KING:
    std::cout << 'K';
    break;
  case CardRank::RANK_ACE:
    std::cout << 'A';
    break;
  default:
    std::cout << '?';
    break;
  }
 
  switch (card.suit)
  {
  case CardSuit::SUIT_CLUB:
    std::cout << 'C';
    break;
  case CardSuit::SUIT_DIAMOND:
    std::cout << 'D';
    break;
  case CardSuit::SUIT_HEART:
    std::cout << 'H';
    break;
  case CardSuit::SUIT_SPADE:
    std::cout << 'S';
    break;
  default:
    std::cout << '?';
    break;
  }
}
 
int getCardValue(const Card& card)
{
  // Handle rank 2 - 10. We could do this in the switch, but it would be long.
  if (card.rank <= CardRank::RANK_10)
  {
    return (static_cast<int>(card.rank) + 2);
  }
 
  switch (card.rank)
  {
  case CardRank::RANK_JACK:
  case CardRank::RANK_QUEEN:
  case CardRank::RANK_KING:
    return 10;
  case CardRank::RANK_ACE:
    return 11;
  default:
    // Should never happen. If it does, we have a bug in our code.
    return 0;
  }
}
 
void printDeck(const deck_type& deck)
{
  for (const auto& card : deck)
  {
    printCard(card);
    std::cout << ' ';
  }
 
  std::cout << '\n';
}
 
deck_type createDeck()
{
  deck_type deck{};
 
  index_type card{ 0 };
 
  auto suits{ static_cast<int>(CardSuit::MAX_SUITS) };
  auto ranks{ static_cast<int>(CardRank::MAX_RANKS) };
 
  for (int suit{ 0 }; suit < suits; ++suit)
  {
    for (int rank{ 0 }; rank < ranks; ++rank)
    {
      deck[card].suit = static_cast<CardSuit>(suit);
      deck[card].rank = static_cast<CardRank>(rank);
      ++card;
    }
  }
 
  return deck;
}
 
void shuffleDeck(deck_type& deck)
{
  static std::mt19937 mt{ static_cast<std::mt19937::result_type>(std::time(nullptr)) };
 
  std::shuffle(deck.begin(), deck.end(), mt);
}


//boolean for player wanting to stand or hit
bool playerWantsHit(){
	while (true){
		std::cout << "(h) to hit, or (s) to stand: ";
		char ch;
		std::cin >> ch;
		//switch stament
		switch (ch){
			case 'h':
			return true;
			case 's':
			return false;
		}
	}
}

//returns is players busts or to keep playing
bool playerTurn(const deck_type& deck, index_type& nextCardIndex, Player& player){
	/*pass in a var fo type deck
	the index type refers to the next card
	and a player reference
	*/
	while (true){
		std::cout << "You have " << player.score << '\n';
		if (player.score > maximumScore){
			return true;
		}
		else{
			if (playerWantsHit()){
				player.score += getCardValue(deck[nextCardIndex]);
			}
			else{
				return false;
			}
		}
	}
}

//now if delear goes bust
bool dealerTurn(const deck_type& deck, index_type& nextCardIndex, Player& dealer){
	/* all the dealer can do is just draw cards when it is his turn */
	while (dealer.score < minimumDealerScore){
		dealer.score += getCardValue(deck[nextCardIndex]);
		std::cout << "The dealer's score is now " << dealer.score << '\n';
	}
	return (dealer.score > maximumScore); //if dealer busts!
}

//now play the game
bool playBlackJack(const deck_type& deck){
	//index of the card to be drawn next. This cannot overrun the array, because a plyaer will lose before all cards are used up

	index_type nextCardIndex = 0;

	//Create the dealer
	Player dealer = { getCardValue(deck[nextCardIndex++]) };

	//dealer's card is face up, the playe can see it
	std::cout << "The dealer is showing " << dealer.score << '\n';

	//create plyaer and give them two cards
	Player player = {getCardValue(deck[nextCardIndex]) + getCardValue(deck[nextCardIndex + 1])};
	nextCardIndex += 2;

	if (playerTurn(deck,nextCardIndex,player))
		return false;
	if (dealerTurn(deck, nextCardIndex,dealer))
		return true;
	return (player.score > dealer.score);
}

int main(){
	auto deck = createDeck();
	shuffleDeck(deck);
	if (playBlackJack(deck))
		std::cout << "You win!" << '\n';
	else
		std::cout << "You lose! " <<'\n';
	return 0;
}

