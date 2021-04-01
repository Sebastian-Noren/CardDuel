#include <iostream>
#include <ctime>
#include <string>
#include <limits>
#include <vector>
#include <algorithm>

using std::cout;
using std::cin;
using std::endl;
using std::string;
using std::vector;


// enumeration of the different ranks and suits and a corresponding String array to map them together.
enum Rank {
    Two, Three, Four, Five, Six, Seven, Eight, Nine, Ten, Jack, Queen, King, Ace
};
static const string RANK_STR[] = {"Two", "Three", "Four", "Five", "Six", "Seven", "Eight", "Nine", "Ten", "Jack",
                                  "Queen", "King", "Ace"};

// clubs = klover, Diamonds = ruter, Hjarter = hearts and Spades = spader.
enum Suit {
    Clubs, Diamonds, Hearts, Spades
};
static const string SUIT_STR[] = {"Clubs", "Diamonds", "Hearts", "Spades"};


struct Card {
    Rank rank;
    Suit suit;
};

// A custom type of Deck that holds vector of type cards.
struct Deck {
    vector<Card> cards;
};

// player will be able to hold a card and 3 score variables to keep track of wins/losses.
//Same is done with AI struct but without wins and losses.
struct Player {
    Card player_card;
    short player_score{0};
    short player_wins{0};
    short player_losses{0};

};

struct AI {
    Card AI_card;
    short AI_score{0};
};

// instance variables
bool gameLoop{true};
Player player;
AI ai_player;
const short MAX_DECK_SIZE{52};

// preloading the functions
void initDeck(Deck &deck);

void printDeck(const Deck &deck);

string cardToString(const Suit &suit, const Rank &rank);

void shuffleDeck(Deck &deck);

Card dealCard(Deck &deck);

bool checkWinner(int &count);

void startApplication();

void gameStart();

void clearInput();

int main() {

    startApplication();

    return 0;
}

/* Starts the main infinite gaming loop where player can chose if he want to play or quit the application.
 * If player chose to quit he have to accept that he wants to quit by pressing y/Y.
 * Using a bool for the gameLoop that will be set to false which cancel the infinite loop if player press y.
 * Also using a input security loop for protect against bad inputs. Which calls a clearInput function.
 * If the player chose playing the next function "gameStart()" will be called. To start the game. */
void startApplication() {

    // init current time as seed every time the application is restarted.
    srand(time(nullptr));
    short choice;

    /* infinite gameloop and prints out a menu for the player
    Shows the total current wins/losses*/
    while (gameLoop) {
        cout << "\n********** MAIN MENU ********\n";
        cout << "* Wins: " << player.player_wins << "\t Losses: " << player.player_losses << endl;
        cout << "*****************************\n";
        cout << "1. Play a game\n";
        cout << "2. Quit application" << endl;
        cout << ">>";
        cin >> choice;
        //Input security loop. Switch case for handling menu inputs
        while (cin.fail()) {
            clearInput();
            cin >> choice;
        }
        switch (choice) {
            case 1:
                //Calling the gamestart function
                gameStart();
                break;
            case 2:
                // player inputs if he wants to quit or not, if yes the gameStart are set to false and breaks the application loop
                char confirm;
                cout << "OK, Do you want to close the application? (y/n)" << endl;
                cin >> confirm;
                if (confirm == 'y' || confirm == 'Y') {
                    cout << "Welcome back and play again!" << endl;
                    gameLoop = false;
                }
                break;
            default:
                cout << "Please enter a number from the menu." << endl;
                break;
        }
    }
    // Make so the console don´t directly closes.
    cout << "Press enter to close application!" << endl;
    cin.get();
    cin.get();

}

void gameStart() {
    Deck card_deck;
    // Calls the initdeck() function with the deck as a parameter, Then resets the two players score.
    initDeck(card_deck);
    player.player_score = 0;
    ai_player.AI_score = 0;
    int count = 0;
    //  cout << "Player score: " << player.player_score << " AI player: " << ai_player.AI_score << endl;
    while (count < 3) {

        /* This sections first display the score before the player and AI_player.
         * The the player draws a card from the deck and next the AI player draws his card from the deck
         * This is the displayed with the help of the cardToString() function. */
        player.player_card = dealCard(card_deck);
        ai_player.AI_card = dealCard(card_deck);
        cout << "Player score: " << player.player_score << " AI player: " << ai_player.AI_score << endl;
        cout << "Turn: " << (count + 1) << endl;
        cout << "Player draws: " << cardToString(player.player_card.suit, player.player_card.rank) << endl;
        cout << "Ai draw: " << cardToString(ai_player.AI_card.suit, ai_player.AI_card.rank) << endl;
        /* This code section starting to compare the two players card.
         * First a check is done if the rank is equal. Because if it is the a new check to compare the highest color (suit)
         * If the players suit is higher then he wins and score increase with one else second player wins and his score increase.
         * Same will happen if the ranks are not equal in the first check. then the rank compares against each other as intended.
         * By doing the same score increments and checks.*/
        if (player.player_card.rank == ai_player.AI_card.rank) {
            cout << "Equal Rank! " << endl;
            if (player.player_card.suit > ai_player.AI_card.suit) {
                cout << "Player wins!\n" << endl;
                player.player_score++;
            } else {
                cout << "AI player wins!\n" << endl;
                ai_player.AI_score++;
            }
        } else {
            if (player.player_card.rank > ai_player.AI_card.rank) {
                cout << "Player wins!\n" << endl;
                player.player_score++;
            } else {
                cout << "AI player wins!\n" << endl;
                ai_player.AI_score++;
            }
        }
        count++;
        if (checkWinner(count)){
            break;
        }
    }
}

/* This function initialize the whole deck before a game by filling it with cards.
 * To fill it with each suit and ranks. A nested for loop needs to be used.
 * The short integers 4 and 13 is to represent the 4 suits and 13 ranks. And to fill the whole deck with 52 cards.
 * 4*13 =52. A card variable is created and then initialized with a suit and rank for every iteration.
 * This is then added to the decks vector of cards. This will be done to all 52 cards are added to the deck.
 * As a last step the deck will be shuffled to randomize the order of the cards.*/
void initDeck(Deck &init_deck) {
    short suit_size = 4;
    short rank_size = 13;
    Card card{};
    for (int suit = 0; suit < suit_size; suit++) {
        for (int rank = 0; rank < rank_size; rank++) {
            card.suit = Suit(suit); // typecast a enum with the number
            card.rank = Rank(rank);
            init_deck.cards.push_back(card);
        }
    }
    shuffleDeck(init_deck);
}

/*Takes input of suit and rank and match them with corresponding string array
 * Later return a string name of them. */
string cardToString(const Suit &suit, const Rank &rank) {
    string str_dec = RANK_STR[rank] + " of " + SUIT_STR[suit];
    return str_dec;
}

/*This function will take the deck and random pick a card from the deck and return the selected card.
 * First a check is done to see that the deck is not empty to get out of bounds.
 * To get a random card from the deck a random index is get by using the deck size as bound.
 * Used because the deck will shrink when a card is taken out of the deck. The selected card will be saved in a variable Card
 * that will be returned. Also the last step is to erase the selected card from the deck before the functions stops.
 * If the deck is empty a message will display nor cards left.  (should not be able to happen in this program)*/
Card dealCard(Deck &deck) {
    if (!deck.cards.empty()) {
        Card card{};
        short index = rand() % deck.cards.size();
        card = deck.cards.at(index);
        deck.cards.erase(deck.cards.begin() + index);
        return card;
    } else {
        cout << "No Cards!" << endl;
        return {};
    }
}

/* This is a Shuffle function to shuffle elements in a vector. Because needed to shuffle the vector of cards in the deck.
 * We take the deck as a parameter. A temporally vector of cards is created to hold shuffled cards.
 * Then it needs to loop through the whole deck of cards until it is empty.
 * same as in the init function a random card is taken from the original deck by an index and added to the temp vector.
 * It is then removed from the original deck. This will repeat until the whole deck is empty and temp is filled up.
 * Last step is to assign the temp vector back to the original deck with the shuffled cards. This is a standard swap algorithm.*/
void shuffleDeck(Deck &deck) {
    vector<Card> temp;
    while (!deck.cards.empty()) {
        short index = rand() % deck.cards.size();
        temp.push_back(deck.cards.at(index));
        deck.cards.erase(deck.cards.begin() + index);
    }
    deck.cards = temp;
    cout << "Card shuffle done!\n" << endl;
}

//Testing purpose, prints the whole deck in a for loop
void printDeck(const Deck &deck) {
    for (int i = 0; i < MAX_DECK_SIZE; ++i) {
        cout << cardToString(deck.cards.at(i).suit, deck.cards.at(i).rank) << endl;
    }

}

/* If either the player or AI_player reach an score of 3 this will execute.
*  it prints out the current score then checks with an if statement if it is the player that has the score 3.
*  And displays him as the winner and adding one win to his win score. Else it displays the AI is the winner
*  and add one to the players losses. Then break the loop to return to main*/
bool checkWinner(int &count) {
    if ((player.player_score > ai_player.AI_score && (count == 2)) ||
    ((player.player_score > ai_player.AI_score) && (count == 3))) {
        cout << "Player score: " << player.player_score << " AI player: " << ai_player.AI_score << endl;
        cout << "Player is the winner of the game!\n" << endl;
        player.player_wins++;
        return true;
    } else if ((ai_player.AI_score > player.player_score && (count == 2)) ||
    (ai_player.AI_score > player.player_score && (count == 3)) ) {
        cout << "Player score: " << player.player_score << " AI player: " << ai_player.AI_score << endl;
        cout << "AI player is the winner of the game!, you Lose!\n" << endl;
        player.player_losses++;
        return true;
    } else{
        return false;
    }
}

// Clears a wrong input from the user, With cin.clear the clears fail flag. Other one discard rest of the input
void clearInput() {
    cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::cout << "Invalid input, please enter your choice again.\n";
}




