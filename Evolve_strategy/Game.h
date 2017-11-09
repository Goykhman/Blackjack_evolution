/**
 
 Game class inherits the Deck class and the Chromosome class.
 
 Game class plays the blackjack game for the player following
 the strategy encoded in the inherited Chromosome class. Default
 constructor for the Game will call default constructor for the
 Chromosome, which is random. We can call constructor for the Game
 with the vector chormosome encoding the desired strategy, which
 will be passed along to the corresponding Chromosome constructor.
 
 The game is played using a single deck of cards inherited from
 the Deck class.
 
 Dealer stands on soft 17. Cannot double down on split aces. Split
 aces receive up to 3 cards in total per hand. Natural pays 3:2,
 split two-card 21 is not natural.
 
 */

using namespace std;

class Game : public Deck, public Chromosome{
public:
    // Default constructor, give the starting bankroll
    // to the player and the dealer, and the fixed bet size.
    // Chromosome is then initialized randomly.
    Game(int, int, int);
    // Constructor for the given chromosome vector.
    Game(int, int, int, vector<int>);
    // One round, played until either player or dealer wins/busts.
    void one_round();
    // One round when player has split given rank.
    void split_round(const char &);
    // Clear the variables for the next round.
    void clear_for_next_round();
    // Play the specified number of rounds.
    void play(const int &);
    
    // Strategy phenotype for chromosomes; 1 for 'yes', 0 for 'no':
    
    // If split for the given rank in pair against given dealer upcard.
    int if_split(const char & , const char & );
    // If soft dd for the given non-ace rank against given dealer upcard.
    int if_soft_double_down(const char & , const char & );
    // If hard dd for the given hand against given dealer upcard.
    int if_hard_double_down(const vector<char> & , const char & );
    // If hard stand for the given count against given dealer upcard.
    int if_hard_stand(const int & , const char & );
    // If soft stand for the given count against given dealer upcard.
    int if_soft_stand(const int & , const char & );
    
    // Set player's bankroll.
    void set_player_bankroll(const int & P){
        player_bankroll=P;
        return;
    }
    // Set dealer's bankroll.
    void set_dealer_bankroll(const int & D){
        dealer_bankroll=D;
        return;
    }
    // Interfaces to the private variables
    int get_player_bankroll(){
        return player_bankroll;
    }
    int get_dealer_bankroll(){
        return dealer_bankroll;
    }
    int get_bet_size(){
        return bet_size;
    }
    bool get_player_soft(){
        return player_soft;
    }
    bool get_dealer_soft(){
        return dealer_soft;
    }
    int get_player_count(){
        return player_count;
    }
    int get_dealer_count(){
        return dealer_count;
    }
    bool get_player_doubled_down(){
        return player_doubled_down;
    }
    bool get_player_pair(){
        return player_pair;
    }
    bool get_player_natural(){
        return player_natural;
    }
    bool get_dealer_natural(){
        return dealer_natural;
    }
    bool get_player_busted(){
        return player_busted;
    }
    bool get_dealer_busted(){
        return dealer_busted;
    }
    int get_player_won(){
        return player_won;
    }
    int get_rounds_played(){
        return rounds_played;
    }
    vector<char> get_player_hand(){
        return player_hand;
    }
    vector<char> get_dealer_hand(){
        return dealer_hand;
    }
private:
    int player_bankroll;
    int dealer_bankroll;
    int bet_size;
    // Whether player or dealer has a soft hand.
    bool player_soft;
    bool dealer_soft;
    // Counts of the totals for player and dealer hands.
    // If player or dealer has a soft hand then it's a soft count.
    // It will be automatically re-calculated as a hard count if the
    // hand becomes hard.
    int player_count;
    int dealer_count;
    // If player has doubled down.
    bool player_doubled_down;
    // If player has a pair.
    bool player_pair;
    // If player or dealer has a natural.
    bool player_natural;
    bool dealer_natural;
    // If the player or dealer has busted.
    bool player_busted;
    bool dealer_busted;
    // How many times player won, how many times the result was
    // a draw, and how many rounds have been played.
    int player_won;
    int rounds_played;
    // Hands of player and dealer.
    vector<char> player_hand;
    vector<char> dealer_hand;
    // If player splits.
    bool split;
};

// Default constructor.
Game::Game(int p, int d, int b) {
    player_bankroll=p;
    dealer_bankroll=d;
    bet_size=b;
    // Default state of each hand is hard, the player/dealer have soft
    // hand if exactly one card is ace without the total going over 21.
    player_soft=false;
    dealer_soft=false;
    //**
    player_count=0;
    dealer_count=0;
    player_doubled_down=false;
    player_pair=false;
    player_natural=false;
    dealer_natural=false;
    player_busted=false;
    dealer_busted=false;
    player_won=0;
    rounds_played=0;
    split=false;
    // Reset the deck at the beginning of the game.
    reset();
}

// Constructor with the given chromosome.
Game::Game(int p, int d, int b, vector<int> chrom) : Chromosome(chrom){
    player_bankroll=p;
    dealer_bankroll=d;
    bet_size=b;
    // Default state of each hand is hard, the player/dealer have soft
    // hand if exactly one card is ace without the total going over 21.
    player_soft=false;
    dealer_soft=false;
    //**
    player_count=0;
    dealer_count=0;
    player_doubled_down=false;
    player_pair=false;
    player_natural=false;
    dealer_natural=false;
    player_busted=false;
    dealer_busted=false;
    player_won=0;
    rounds_played=0;
    split=false;
    // Reset the deck at the beginning of the game.
    reset();
}

int Game::if_split(const char & player_card, const char & dealer_card){
    int i=card_index(player_card);
    int j=card_index(dealer_card);
    return get_split(i,j);
}

int Game::if_soft_double_down(const char & player_card, const char & dealer_card){
    if(player_card=='T')
        return 0;
    int i=card_index(player_card);
    int j=card_index(dealer_card);
    return get_soft_double_down(i,j);
}

int Game::if_hard_double_down(const vector<char> & player_hand, const char & dealer_card){
    int val=0;
    for(const char & c : player_hand)
        val+=value(c,false);
    int i=sum_index(val);
    int j=card_index(dealer_card);
    return get_hard_double_down(i,j);
}

int Game::if_hard_stand(const int & player_count, const char & dealer_card){
    int i=sum_index(player_count);
    int j=card_index(dealer_card);
    return get_hard_stand(i,j);
}

int Game::if_soft_stand(const int & player_count, const char & dealer_card){
    int i=sum_index(player_count);
    int j=card_index(dealer_card);
    return get_soft_stand(i,j);
}

void Game::one_round(){
    // Dealing the pairs to player and dealer.
    // The fist card dealt to dealer is face up.
    int ind;
    char rank;
    // Deal card to player.
    ind=deal_card();
    rank=show_rank(ind);
    player_hand.push_back(rank);
    if(rank=='A')
        player_soft=true;
    player_count+=value(rank,player_soft);
    // Deal card to dealer.
    ind=deal_card();
    rank=show_rank(ind);
    dealer_hand.push_back(rank);
    if(rank=='A')
        dealer_soft=true;
    dealer_count+=value(rank,dealer_soft);
    // Deal card to player.
    ind=deal_card();
    rank=show_rank(ind);
    if(rank==player_hand[0])
        player_pair=true;
    player_hand.push_back(rank);
    if(rank=='A')
        player_soft=true;
    player_count+=value(rank,player_soft);
    // Check for two aces.
    if(player_count==22){
        player_count=12;
        player_pair=true;
    }
    // Check for natural.
    if(player_count==21){
        player_natural=true;
    }
    // Deal card to dealer.
    ind=deal_card();
    rank=show_rank(ind);
    dealer_hand.push_back(rank);
    if(rank=='A')
        dealer_soft=true;
    dealer_count+=value(rank,dealer_soft);
    // Check for two aces.
    if(dealer_count==22){
        dealer_count=12;
    }
    // Check for natural.
    if(dealer_count==21){
        dealer_natural=true;
    }
    // Player's decisions.
    // Split.
    if(player_pair){
        split=(if_split(player_hand[0],dealer_hand[0])==1);
    }
    if(split){
        char player_rank=player_hand[0];
        // Play the first split hand.
        player_hand={player_rank};
        if(player_rank=='A')
            player_soft=true;
        else
            player_soft=false;
        player_count=value(player_rank,player_soft);
        player_doubled_down=false;
        split_round(player_rank);
        // Play the second split hand.
        player_hand={player_rank};
        if(player_rank=='A')
            player_soft=true;
        else
            player_soft=false;
        player_count=value(player_rank,player_soft);
        player_doubled_down=false;
        player_busted=false;
        split_round(player_rank);
        return;
    }
    // Check for natural win/draw.
    if(player_natural){
        if(dealer_natural){
            return;
        }
        player_bankroll+=1.5*bet_size;
        dealer_bankroll-=1.5*bet_size;
        player_won+=1;
        return;
    }
    // Double down.
    if(player_soft){
        char c=player_hand[0];
        if(player_hand[0]=='A')
            c=player_hand[1];
        if(if_soft_double_down(c,dealer_hand[0])==1)
            player_doubled_down=true;
    }
    else if(!player_soft&&if_hard_double_down(player_hand,dealer_hand[0])==1){
        player_doubled_down=true;
    }
    // Continue to checking the originally dealt pair for hit/stand.
    while(true){
        if(!(player_doubled_down&&player_hand.size()<3)&&player_count==21)
            break;
        // If player's hand is soft check whether it should stand.
        if(!(player_doubled_down&&player_hand.size()<3)
           &&player_soft&&(if_soft_stand(player_count,dealer_hand[0])==1)){
            break;
        }
        // If player's hand is hard check whether it should stand.
        if(!(player_doubled_down&&player_hand.size()<3)
           &&!player_soft&&if_hard_stand(player_count,dealer_hand[0])==1){
            break;
        }
        // If player has doubled down it can receive only one more card.
        if(player_doubled_down&&player_hand.size()==3){
            break;
        }
        // Hit the player.
        int ind=deal_card();
        char rank=show_rank(ind);
        int val=value(rank,player_soft);
        player_hand.push_back(rank);
        // Check whether hard or soft hand status should be changed.
        if(rank=='A'){
            // Check whether the current ace can make previously hard hand soft.
            if(!player_soft&&player_count+11<=21){
                player_soft=true;
                val+=10;
            }
            // If the hand is soft then val has been calculated as 11, and the
            // prior player_count can't be more than 20. Which means the current
            // ace should be calculated as 1 and the hand is still soft, because
            // we still have one ace as 11.
            else if(player_soft)
                val-=10;
        }
        else{
            // Check whether the current val will exceed the player count over 21.
            // If the hand was soft (which can be if there was exactly one ace
            // counting as 11) we can salvage that by recalculating the player
            // count and making the hand hard.
            if(player_count+val>21&&player_soft){
                player_count-=10;
                player_soft=false;
            }
        }
        player_count+=val;
        //**
        if(player_count>21)
            player_busted=true;
        if(player_busted){
            player_bankroll-=bet_size;
            dealer_bankroll+=bet_size;
            if(player_doubled_down){
                player_bankroll-=bet_size;
                dealer_bankroll+=bet_size;
            }
            return;
        }
    }
    // Dealer's hand.
    while(true){
        // Dealer stops if it goes to 17 or higher.
        if(dealer_count>=17){
            break;
        }
        // Otherwise hit the dealer.
        int ind=deal_card();
        char rank=show_rank(ind);
        int val=value(rank,dealer_soft);
        dealer_hand.push_back(rank);
        // Check whether hard or soft hand status should be changed.
        if(rank=='A'){
            // Check whether the current ace can make previously hard hand soft.
            if(!dealer_soft&&dealer_count+11<=21){
                dealer_soft=true;
                val+=10;
            }
            // If the hand is soft then val has been calculated as 11, and the
            // prior dealer_count can't be more than 20. Which means the current
            // ace should be calculated as 1 and the hand is still soft, because
            // we still have one ace as 11.
            else if(dealer_soft)
                val-=10;
        }
        else{
            // Check whether the current val will exceed the dealer count over 21.
            // If the hand was soft (which can be if there was exactly one ace
            // counting as 11) we can salvage that by recalculating the dealer
            // count and making the hand hard.
            if(dealer_count+val>21&&dealer_soft){
                dealer_count-=10;
                dealer_soft=false;
            }
        }
        dealer_count+=val;
        if(dealer_count>21){
            dealer_busted=true;
            break;
        }
    }
    if(dealer_busted){
        player_won+=1;
        dealer_bankroll-=bet_size;
        player_bankroll+=bet_size;
        if(player_doubled_down){
            dealer_bankroll-=bet_size;
            player_bankroll+=bet_size;
        }
        return;
    }
    if(player_count==dealer_count){
        return;
    }
    if(player_count>dealer_count){
        player_won+=1;
        player_bankroll+=bet_size;
        dealer_bankroll-=bet_size;
        if(player_doubled_down){
            dealer_bankroll-=bet_size;
            player_bankroll+=bet_size;
        }
        return;
    }
    if(player_count<dealer_count){
        player_bankroll-=bet_size;
        dealer_bankroll+=bet_size;
        if(player_doubled_down){
            dealer_bankroll+=bet_size;
            player_bankroll-=bet_size;
        }
        return;
    }
}

void Game::split_round(const char & player_rank){
    int ind;
    char rank;
    // Deal card to player.
    ind=deal_card();
    rank=show_rank(ind);
    player_hand.push_back(rank);
    if(rank=='A'&&!player_soft)
        player_soft=true;
    player_count+=value(rank,player_soft);
    // Check for two aces.
    if(player_count==22){
        player_count=12;
    }
    // Player's decisions.
    // Double down. Can't double down on split aces. Therefore the soft hand
    // is considered for double down only if we split non-aces and receive an ace.
    if(player_count!=21&&player_hand[0]!='A'&&(player_soft&&if_soft_double_down(player_hand[0],
                                                  dealer_hand[0])==1)){
        player_doubled_down=true;
    }
    else if(player_count!=21&&player_hand[0]!='A'&&!player_soft&&
            (if_hard_double_down(player_hand,dealer_hand[0])==1)){
        player_doubled_down=true;
    }
    // Continue to checking the originally dealt pair for hit/stand.
    while(true){
        if(!(player_doubled_down&&player_hand.size()<3)&&player_count==21)
            break;
        // If player's hand is soft check whether it should stand.
        if(!(player_doubled_down&&player_hand.size()<3)
           &&player_soft&&(if_soft_stand(player_count,dealer_hand[0])==1)){
            break;
        }
        // If player's hand is hard check whether it should stand.
        if(!(player_doubled_down&&player_hand.size()<3)
           &&!player_soft&&(if_hard_stand(player_count,dealer_hand[0])==1)){
            break;
        }
        // If player has split aces it can receive only one more card.
        if(player_rank=='A'&&player_hand.size()==3){
            break;
        }
        // If player has doubled down it can receive only one more card.
        if(player_doubled_down&&player_hand.size()==3){
            break;
        }
        // Hit the player
        int ind=deal_card();
        char rank=show_rank(ind);
        int val=value(rank,player_soft);
        player_hand.push_back(rank);
        // Check whether hard or soft hand status should be changed.
        if(rank=='A'){
            // Check whether the current ace can make previously hard hand soft.
            if(!player_soft&&player_count+11<=21){
                player_soft=true;
                val+=10;
            }
            // If the hand is soft then val has been calculated as 11, and the
            // prior player_count can't be more than 20. Which means the current
            // ace should be calculated as 1 and the hand is still soft, because
            // we still have one ace as 11.
            else if(player_soft)
                val-=10;
        }
        else{
            // Check whether the current val will exceed the player count over 21.
            // If the hand was soft (which can be if there was exactly one ace
            // counting as 11) we can salvage that by recalculating the player
            // count and making the hand hard.
            if(player_count+val>21&&player_soft){
                player_count-=10;
                player_soft=false;
            }
        }
        player_count+=val;
        //**
        if(player_count>21)
            player_busted=true;
        if(player_busted){
            player_bankroll-=bet_size;
            dealer_bankroll+=bet_size;
            if(player_doubled_down){
                player_bankroll-=bet_size;
                dealer_bankroll+=bet_size;
            }
            return;
        }
    }
    // Dealer's hand. Only fill it if we are processing first split hand.
    if(dealer_hand.size()==2){
        while(true){
            // Dealer stops if it goes to 17 or higher.
            if(dealer_count>=17){
                break;
            }
            // Hit the dealer
            int ind=deal_card();
            char rank=show_rank(ind);
            int val=value(rank,dealer_soft);
            dealer_hand.push_back(rank);
            // Check whether hard or soft hand status should be changed.
            if(rank=='A'){
                // Check whether the current ace can make previously hard hand soft.
                if(!dealer_soft&&dealer_count+11<=21){
                    dealer_soft=true;
                    val+=10;
                }
                // If the hand is soft then val has been calculated as 11, and the
                // prior dealer_count can't be more than 20. Which means the current
                // ace should be calculated as 1 and the hand is still soft, because
                // we still have one ace as 11.
                else if(dealer_soft)
                    val-=10;
            }
            else{
                // Check whether the current val will exceed the dealer count over 21.
                // If the hand was soft (which can be if there was exactly one ace
                // counting as 11) we can salvage that by recalculating the dealer
                // count and making the hand hard.
                if(dealer_count+val>21&&dealer_soft){
                    dealer_count-=10;
                    dealer_soft=false;
                }
            }
            dealer_count+=val;
            if(dealer_count>21){
                dealer_busted=true;
                break;
            }
        }
    }
    if(dealer_busted){
        player_won+=1;
        dealer_bankroll-=bet_size;
        player_bankroll+=bet_size;
        if(player_doubled_down){
            player_bankroll+=bet_size;
            dealer_bankroll-=bet_size;
        }
        return;
    }
    if(player_count==dealer_count){
        return;
    }
    if(player_count>dealer_count){
        player_won+=1;
        player_bankroll+=bet_size;
        dealer_bankroll-=bet_size;
        if(player_doubled_down){
            player_bankroll+=bet_size;
            dealer_bankroll-=bet_size;
        }
        return;
    }
    if(player_count<dealer_count){
        player_bankroll-=bet_size;
        dealer_bankroll+=bet_size;
        if(player_doubled_down){
            player_bankroll-=bet_size;
            dealer_bankroll+=bet_size;
        }
        return;
    };
}


void Game::clear_for_next_round(){
    player_soft=false;
    dealer_soft=false;
    player_count=0;
    dealer_count=0;
    player_doubled_down=false;
    player_pair=false;
    player_natural=false;
    dealer_natural=false;
    player_busted=false;
    dealer_busted=false;
    player_hand={};
    dealer_hand={};
    split=false;
}

// Reshuffling happens either after we go through 1/3 of the deck.
void Game::play(const int & rounds){
    while(rounds_played<rounds){
        if(player_bankroll<=0||dealer_bankroll<=0){
            // this is sepcific for our genetic selection goals,
            // to kill strategies which make the player go bankrupt.
            player_won=0;
            break;
        }
        if(get_pointer()>get_order().size()/3){
            reset();
        }
        ++rounds_played;
        clear_for_next_round();
        one_round();
    }
}
