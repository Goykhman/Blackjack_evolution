/**
 A single deck of cards. The cards are always in the fixed order saved
 in the "cards" array in the "rank suit" format. The cards are drawn from
 the deck in the order of the "order" array, where we move the "pointer"
 pointing at given card in "order". The "order" array can be shuffled.
 Returning discarded cards to the deck can be done by setting "pointer"
 back to zero and calling "shuffle()" to the deck, which is wrapped into
 "reset()" method.
 */

using namespace std;

class Deck{
public:
    // Constructor.
    Deck();
    
    // Shuffle the "order" array.
    void Shuffle();
    
    // Set the pointer to zero and shuffle.
    void reset();
    
    // Pick a number from the "order" array to which the "pointer"
    // points and increment the pointer by one.
    int deal_card();
    
    // The "rank suit" from the "cards" array with the given index.
    string show_card(const int & );
    
    // The "rank" from the "cards" array with the given index.
    // Returns 'A' or '2'-'9' or 'T', assuming 'J'='K'='Q'='T'.
    char show_rank(const int & );
    
    // Interface to the private variables:
    vector<string> get_cards(){
        return cards;
    }
    array<int,52> get_order(){
        return order;
    }
    int get_pointer(){
        return pointer;
    }
    int value(const char &, const bool &);
private:
    vector<string> cards;
    array<int,52> order; // this container type facilitates shuffling
    int pointer;
};

Deck::Deck(){
    cards={};
    order={};
    for(int i=0;i<52;++i)
        order[i]=i;
    // hearts, diamonds, spades, clubs.
    vector<char> suits={'H','D','S','C'};
    vector<char> ranks={'2','3','4','5','6','7',
        '8','9','T','J','Q','K','A'};
    for(auto rank : ranks){
        for(auto suit : suits){
            string card="";
            card+=rank;
            card+=" ";
            card+=suit;
            cards.push_back(card);
        }
    }
    pointer=0;
}

void Deck::Shuffle(){
    unsigned seed =chrono::system_clock::now().time_since_epoch().count();
    shuffle(order.begin(), order.end(), default_random_engine(seed));
}

void Deck::reset(){
    pointer=0;
    Shuffle();
}

int Deck::deal_card(){
    int a=order[pointer++];
    return a;
}

string Deck::show_card(const int & index){
    string card=cards[index];
    return card;
}

char Deck::show_rank(const int & index){
    char rank=cards[index][0];
    if(rank=='J'||rank=='Q'||rank=='K')
        rank='T';
    return rank;
}

int Deck::value(const char & rank, const bool & soft){
    if(rank=='A'&&soft)
        return 11;
    if(rank=='A'&&!soft)
        return 1;
    if(rank>='2'&&rank<='9'){
        int v=rank-'0';
        return v;
    }
    return 10;
}
