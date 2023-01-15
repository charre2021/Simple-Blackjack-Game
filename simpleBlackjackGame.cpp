#include <iostream>
#include <array>
#include <algorithm>
#include <ctime>
#include <random>

constexpr int g_blackjackLimit{21};
constexpr int g_aceValue{11};
constexpr int g_dealerLimt{17};

enum class Rank {
    Two,
    Three,
    Four,
    Five,
    Six,
    Seven,
    Eight,
    Nine,
    Ten,
    Jack,
    Queen,
    King,
    Ace
};

enum class Suit {
    Clubs,
    Diamonds,
    Hearts,
    Spades
};

struct Card {
    Rank rank{};
    Suit suit{};
};

struct Scores 
{
    int dealer_sum{};
    int player_sum{};
};

using Deck = std::array<Card, 52>;

void printCard(const Card& card) {
    char r {};
    char s {};
    switch (card.rank) {
        case Rank::Two : r = '2'; break;
        case Rank::Three : r = '3'; break;
        case Rank::Four : r = '4'; break;
        case Rank::Five : r = '5'; break;
        case Rank::Six : r = '6'; break;
        case Rank::Seven : r = '7'; break;
        case Rank::Eight : r = '8'; break;
        case Rank::Nine : r = '9'; break;
        case Rank::Ten : r = 'T'; break;
        case Rank::Jack : r = 'J'; break;
        case Rank::Queen : r = 'Q'; break;
        case Rank::King : r = 'K'; break;
        case Rank::Ace : r = 'A'; break;
        default : r = '?'; break;
    }
    switch (card.suit) {
        case Suit::Clubs : s = 'C'; break;
        case Suit::Diamonds : s = 'D'; break;
        case Suit::Hearts : s = 'H'; break;
        case Suit::Spades : s = 'S'; break;
        default : r = '?'; break;
    }

    std::cout << r << s << " ";
}

Deck createDeck() {
    Deck Deck{};
    int cardPlace{0}; 
    for (int j {static_cast<int>(Suit::Clubs)}; j <= static_cast<int>(Suit::Spades); j++)
    {
        for (int i {static_cast<int>(Rank::Two)}; i <= static_cast<int>(Rank::Ace); i++)
        {
            Deck[cardPlace].rank = static_cast<Rank>(i);
            Deck[cardPlace].suit = static_cast<Suit>(j);
            cardPlace++;
        }
    }
    return Deck;
}

void printDeck(const Deck& dk) {
    for (const auto& card : dk)
    {
        printCard(card);
    }
    std::cout << '\n';
}

void shuffleDeck(Deck& unshf_dk)
{
    std::mt19937 mt{ static_cast<std::mt19937::result_type>(std::time(nullptr)) };
    std::shuffle(unshf_dk.begin(), unshf_dk.end(), mt);
}

int getCardValue(const Card& crd)
{
    switch (crd.rank) {
        case Rank::Two : return 2; break;
        case Rank::Three : return 3; break;
        case Rank::Four : return 4; break;
        case Rank::Five : return 5; break;
        case Rank::Six : return 6; break;
        case Rank::Seven : return 7; break;
        case Rank::Eight : return 8; break;
        case Rank::Nine : return 9; break;
        case Rank::Ten : return 10; break;
        case Rank::Jack : return 10; break;
        case Rank::Queen : return 10; break;
        case Rank::King : return 10; break;
        case Rank::Ace : return 11; break;
        default : return 0; break;
    }
}

Scores& addCardValueToScore(Scores& scoreToChange, bool playerFlag, int cardValueInQuestion) 
{
    if (playerFlag)
    {
        scoreToChange.player_sum += cardValueInQuestion;
        if (scoreToChange.player_sum > g_blackjackLimit && cardValueInQuestion == 11)
        {
            scoreToChange.player_sum -= 10;
        }
    } else {
        scoreToChange.dealer_sum += cardValueInQuestion;
        if (scoreToChange.dealer_sum > g_blackjackLimit && cardValueInQuestion == 11)
        {
            scoreToChange.dealer_sum -= 10;
        }
    } 
    return scoreToChange;
}

Scores setup(Deck& decck) {

    Scores current_score{};

    bool isItPlayer{};
    for (int i{0}; i < 4; i++)
    {
        if (i % 2 == 0)
        {
            isItPlayer = true;
        } else {
            isItPlayer = false;
        }
        addCardValueToScore(current_score, isItPlayer, getCardValue(decck[i]));
    }
    
    return current_score;
}

void playerPlays(Card* pos_ptr, Deck& dcek, Scores& scr, bool& bust_flag)
{
    for (pos_ptr; pos_ptr != (&dcek[0] + dcek.size()); pos_ptr++)
    {
        std::cout << "Your current score is: " << scr.player_sum << "\n";
        std::cout << "Do you want to hit or stand?" << "\n";
        std::string choice{};
        std::cin >> choice;
        if (choice == "hit") 
        {
            addCardValueToScore(scr, true, getCardValue(*pos_ptr));
            if (scr.player_sum > g_blackjackLimit)
            {
                bust_flag = true;
                return;
            }             
        } else {
            return;
        }
    }
}

void dealerPlays(Card* pos_ptr, Deck& dck, Scores& scr, bool& bust_flag)
{
    for (pos_ptr; pos_ptr != (&dck[0] + dck.size()); pos_ptr++)
    {
        std::cout << "Dealer's current score is: " << scr.dealer_sum << "\n";
        if (scr.dealer_sum >= g_dealerLimt)
        {
            return;
        } else {
            addCardValueToScore(scr, false, getCardValue(*pos_ptr));
            if (scr.dealer_sum > g_blackjackLimit)
            {
                bust_flag = true;
                return;
            }    
        }
    }
}

bool& chooseToContinue(bool& choiceToPlay)
{
    std::cout << "Do you want to play again? (Y/N)" << "\n";
    std::string continueChoice{};
    std::cin >> continueChoice;
    if (continueChoice == "Y" || continueChoice == "Yes")
    {
        return choiceToPlay;
    } else {
        choiceToPlay = false;
        return choiceToPlay;
    }
}

bool& playBlackjack(bool& playChoice) {
    Deck deck{createDeck()};
    Deck& deckRef{deck};
    shuffleDeck(deckRef);
    std::cout << "Let's play Blackjack!" << "\n";
    bool bust{false};
    Scores score{setup(deck)};
    Card* pos{(&deck[0]) + 4};
    playerPlays(pos, deck, score, bust);
    if (bust) {
        std::cout << "Bust! You lose.\n";
        return chooseToContinue(playChoice);
    }  
    dealerPlays(pos, deck, score, bust);
    if (bust) {
        std::cout << "Dealer busted! You win!\n";
        return chooseToContinue(playChoice);
    } else {
        std::cout << "Final scores: \n";
        std::cout << "Player score : " << score.player_sum << "\n";
        std::cout << "Dealer score : " << score.dealer_sum << "\n";
        if (score.player_sum > score.dealer_sum)
        {
            std::cout << "You had the highest score! You win!\n";
        } else if (score.player_sum == score.dealer_sum) {
            std::cout << "Tie! Neither wins.\n";
        } else {
            std::cout << "Dealer had the highest score! You lose...womp womp\n";
        }
    }
    return chooseToContinue(playChoice);
}

int main() {
    bool play{true};
    while (play) {
        playBlackjack(play);    
    }
    return 0;
}

