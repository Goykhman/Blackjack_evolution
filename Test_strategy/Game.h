/**
 * "Game" inherits a single "Deck" of cards and plays the "BasicStrategy".
 * It is a one-player game. In the constructor for the "Game" we specify
 * the bankroll for the player and the dealer which they start with,
 * and the (fixed) bet size which the player will wager on each round.
 * We play the game by calling the "play()" method for the specified
 * number of rounds, where each round is played with the "one_round()" method.
 */

/**
 **The specific rules are:
 * Player can double down on split pairs except split aces.
 * Split aces can receive up to 3 cards.
 * Dealer stands on soft 17.
 * Split hands cannot be split again.
 * Pairs of 21 on split hands don't count as natural.
 * Natural pays 1.5 of the bet.
 */

using namespace std;

class Game : public Deck, public BasicStrategy{
public:
    // Constructor, give the starting bankroll for the player and the
    // dealer, and the (fixed) bet size.
    Game(int, int, int);
    
    // One round, played until either player or dealer wins/busts.
    // Will call the BasicStrategy methods for the player.
    void one_round();
    
    // One round when player has split given rank.
    void split_round(const char &);
    
    // Clear the variables for the next round.
    void clear_for_next_round();
    
    // Play the specified number of rounds. The dealer will "reset()" the
    // deck (set pointer to zero and shuffle) when the play counter is
    // randomly found between the two specified numbers.
    void play(const int &, const int &, const int &);
    
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
    vector<int> get_player_time_series(){
        return player_time_series;
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
    int get_draws(){
        return draws;
    }
    int get_rounds_played(){
        return rounds_played;
    }
    int get_times_player_doubled_down(){
        return times_player_doubled_down;
    }
    int get_times_player_doubled_down_and_won(){
        return times_player_doubled_down_and_won;
    }
    int get_times_player_doubled_down_and_lost(){
        return times_player_doubled_down_and_lost;
    }
    int get_times_player_split(){
        return times_player_split;
    }
    int get_times_player_split_and_won(){
        return times_player_split_and_won;
    }
    int get_times_player_split_and_lost(){
        return times_player_split_and_lost;
    }
    vector<char> get_player_hand(){
        return player_hand;
    }
    vector<char> get_dealer_hand(){
        return dealer_hand;
    }
    bool get_split(){
        return split;
    }
private:
    int player_bankroll;
    int dealer_bankroll;
    int bet_size;
    
    // Player's bankroll time series.
    vector<int> player_time_series;
    
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
    
    // How many times player won, how many times the result was a draw,
    // and how many rounds have been played.
    int player_won;
    int draws;
    int rounds_played;
    
    // How many times player has doubled down.
    int times_player_doubled_down;
    // How many times when player has doubled down and won.
    int times_player_doubled_down_and_won;
    // How many times when player has doubled down and lost.
    int times_player_doubled_down_and_lost;
    
    // How many times player has split.
    int times_player_split;
    // How many times when player has split and won.
    int times_player_split_and_won;
    // How many times when player has split and lost.
    int times_player_split_and_lost;
    
    // Hands of player and dealer.
    vector<char> player_hand;
    vector<char> dealer_hand;
    
    // If player splits.
    bool split;
};

Game::Game(int p, int d, int b){
    player_bankroll=p;
    dealer_bankroll=d;
    bet_size=b;
    
    player_time_series={};
    
    // Default state of each hand is hard, the player/dealer have soft
    // hand if exactly one card is ace without the total going over 21.
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
    
    player_won=0;
    draws=0;
    rounds_played=0;
    
    times_player_doubled_down=0;
    times_player_doubled_down_and_won=0;
    times_player_doubled_down_and_lost=0;
    
    times_player_split=0;
    times_player_split_and_won=0;
    times_player_split_and_lost=0;
    
    split=false;
    
    // Reset the deck at the beginning of the game.
    reset();
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
    //cout << "Player is dealt the card " << show_card(ind) << endl;
    player_count+=value(rank,player_soft);
    
    // Deal card to dealer.
    ind=deal_card();
    rank=show_rank(ind);
    dealer_hand.push_back(rank);
    if(rank=='A')
        dealer_soft=true;
    //cout << "Dealer is dealt the card " << show_card(ind) << endl;
    dealer_count+=value(rank,dealer_soft);
    
    // Deal card to player.
    ind=deal_card();
    rank=show_rank(ind);
    if(rank==player_hand[0])
        player_pair=true;
    player_hand.push_back(rank);
    if(rank=='A')
        player_soft=true;
    //cout  << "Player is dealt the card " << show_card(ind) << endl;
    player_count+=value(rank,player_soft);
    
    // Check for two aces.
    if(player_count==22){
        player_count=12;
        // the hand is still soft because one of the aces still counts as 11.
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
    //cout << "Dealer is dealt the card " << show_card(ind) << endl;
    dealer_count+=value(rank,dealer_soft);
    
    // Check for two aces.
    if(dealer_count==22){
        // the hand is still soft because one of the aces still counts as 11.
        dealer_count=12;
    }
    
    // Check for natural.
    if(dealer_count==21){
        dealer_natural=true;
    }
    
    // Player's decisions.
    
    // Split.
    if(player_pair){
        split=split_pair(player_hand[0],dealer_hand[0]);
    }
    if(split){
        //cout << "Player splits" << endl;
        times_player_split++;
        char player_rank=player_hand[0];
        // Play the first split hand.
        player_hand={player_rank};
        if(player_rank=='A')
            player_soft=true;
        else
            player_soft=false;
        player_count=value(player_rank,player_soft);
        player_doubled_down=false;
        //cout << "Playing the first split card" << endl;
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
        //cout << "Playing the second split card" << endl;
        split_round(player_rank);
        return;
    }
    
    // Check for natural win/draw.
    if(player_natural){
        if(dealer_natural){
            //cout << "Both player and dealer have a natural. Draw" << endl;
            draws+=1;
            player_time_series.push_back(player_bankroll);
            return;
        }
        //cout << "Player wins with a natural" << endl;
        player_bankroll+=1.5*bet_size;
        dealer_bankroll-=1.5*bet_size;
        player_won+=1;
        player_time_series.push_back(player_bankroll);
        return;
    }
    
    // Double down.
    if(player_soft&&soft_double_down(player_hand,dealer_hand[0])){
        //cout << "Player doubles down on their soft hand" << endl;
        player_doubled_down=true;
        times_player_doubled_down++;
    }
    else if(!player_soft&&hard_double_down(player_hand,dealer_hand[0])){
        //cout << "Player doubles down on their hard hand" << endl;
        player_doubled_down=true;
        times_player_doubled_down++;
    }
    
    // Continue to checking the originally dealt pair for hit/stand.
    
    while(true){
        // If player doubled down and its hand size is less than 3, we
        // have to deal one more card to the player, so we cannot break.
        
        if(!(player_doubled_down&&player_hand.size()<3)&&player_count==21)
            break;
        // If player's hand is soft check whether it should stand.
        if(!(player_doubled_down&&player_hand.size()<3)
           &&player_soft&&soft_stand(player_count,dealer_hand[0])){
            //cout << "Soft hand stand" << endl;
            break;
        }
        // If player's hand is hard check whether it should stand.
        if(!(player_doubled_down&&player_hand.size()<3)
           &&!player_soft&&hard_stand(player_hand.size(),player_count,
                        dealer_hand[0],player_hand[0],player_hand[1])){
            //cout << "Hard hand stand" << endl;
            break;
        }
        // If player has doubled down it can receive only one more card.
        if(player_doubled_down&&player_hand.size()==3){
            //cout << "Standing after after doubling down" << endl;
            break;
        }
        
        // Hit the player.
        int ind=deal_card();
        char rank=show_rank(ind);
        int val=value(rank,player_soft);
        //cout << "Dealing to player " << show_card(ind) << endl;
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
        
        if(player_count>21)
            player_busted=true;
        if(player_busted){
            //cout << "Player busted "<< " with the hand" << endl;
            //for(auto card : player_hand)
            //    cout << card << "\t";
            //cout << endl;
            //cout <<" at  the count " << player_count << endl;
            player_bankroll-=bet_size;
            dealer_bankroll+=bet_size;
            if(player_doubled_down){
                times_player_doubled_down_and_lost++;
                player_bankroll-=bet_size;
                dealer_bankroll+=bet_size;
            }
            player_time_series.push_back(player_bankroll);
            return;
        }
    }
    
    // Dealer's hand.
    
    while(true){
        // Dealer stops if it goes to 17 or higher.
        if(dealer_count>=17){
            //cout << "Dealer count >= 17" << endl;
            break;
        }
        // Otherwise hit the dealer.
        int ind=deal_card();
        char rank=show_rank(ind);
        int val=value(rank,dealer_soft);
        //cout << "Dealing to dealer " << show_card(ind) << endl;
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
        //cout << "Dealer busted "<< " with the hand" << endl;
        //for(auto card : dealer_hand)
        //    cout << card << '\t';
        //cout << endl;
        //cout <<" at  the count " << dealer_count << endl;
        player_won+=1;
        dealer_bankroll-=bet_size;
        player_bankroll+=bet_size;
        if(player_doubled_down){
            times_player_doubled_down_and_won++;
            dealer_bankroll-=bet_size;
            player_bankroll+=bet_size;
        }
        player_time_series.push_back(player_bankroll);
        return;
    }
    
    if(player_count==dealer_count){
        //cout << "Player hand " << endl;
        //for(auto card : player_hand)
        //    cout << card << '\t';
        //cout << endl;
        //cout <<" at  the count " << player_count << endl;
        //cout << " pushes dealer hand " << endl;
        //for(auto card : dealer_hand)
        //    cout << card << '\t';
        //cout << endl;
        //cout <<" at  the count " << dealer_count << endl;
        draws+=1;
        player_time_series.push_back(player_bankroll);
        return;
    }
    if(player_count>dealer_count){
        //cout << "Player hand " << endl;
        //for(auto card : player_hand)
        //    cout << card << '\t';
        //cout << endl;
        //cout <<" at  the count " << player_count << endl;
        //cout << "beats dealer hand " << endl;
        //for(auto card : dealer_hand)
        //    cout << card << '\t';
        //cout << endl;
        //cout <<" at  the count " << dealer_count << endl;
        player_won+=1;
        player_bankroll+=bet_size;
        dealer_bankroll-=bet_size;
        if(player_doubled_down){
            times_player_doubled_down_and_won++;
            dealer_bankroll-=bet_size;
            player_bankroll+=bet_size;
        }
        player_time_series.push_back(player_bankroll);
        return;
    }
    if(player_count<dealer_count){
        //cout << "Player hand " << endl;
        //for(auto card : player_hand)
        //    cout << card << '\t';
        //cout << endl;
        //cout <<" at  the count " << player_count << endl;
        //cout << "loses to dealer hand " << endl;
        //for(auto card : dealer_hand)
        //    cout << card << '\t';
        //cout << endl;
        //cout <<" at  the count " << dealer_count << endl;
        player_bankroll-=bet_size;
        dealer_bankroll+=bet_size;
        if(player_doubled_down){
            times_player_doubled_down_and_lost++;
            dealer_bankroll+=bet_size;
            player_bankroll-=bet_size;
        }
        player_time_series.push_back(player_bankroll);
        return;
    };
}

void Game::split_round(const char & player_rank){
    
    int ind;
    char rank;
    
    // Deal card to player.
    ind=deal_card();
    rank=show_rank(ind);
    //cout << "Player is dealt the card " << show_card(ind) << endl;
    player_hand.push_back(rank);
    if(rank=='A'&&!player_soft)
        player_soft=true;
    player_count+=value(rank,player_soft);
    
    // Check for two aces.
    if(player_count==22){
        player_count=12;
    }
    
    // Player's decisions.
    
    // Double down. Can't double down on split aces.
    if(player_count!=21&&player_hand[0]!='A'&&player_soft&&soft_double_down(player_hand,
                                                  dealer_hand[0])){
        //cout << "Player doubles down on their soft hand" << endl;
        player_doubled_down=true;
        times_player_doubled_down++;
    }
    else if(player_count!=21&&player_hand[0]!='A'&&!player_soft&&
            hard_double_down(player_hand,dealer_hand[0])){
        //cout << "Player doubles down on their hard hand" << endl;
        player_doubled_down=true;
        times_player_doubled_down++;
    }
    
    // Continue to checking the originally dealt pair for hit/stand.
    
    while(true){
        if(!(player_doubled_down&&player_hand.size()<3)&&player_count==21)
            break;
        // If player's hand is soft check whether it should stand.
        if(!(player_doubled_down&&player_hand.size()<3)
           &&player_soft&&soft_stand(player_count,dealer_hand[0])){
            //cout << "Soft stand" << endl;
            break;
        }
        // If player's hand is hard check whether it should stand.
        if(!(player_doubled_down&&player_hand.size()<3)
           &&!player_soft&&hard_stand(player_hand.size(),player_count,
                    dealer_hand[0],player_hand[0],player_hand[1])){
            //cout << "Hard stand" << endl;
            break;
        }
        // If player has split aces it can receive only one more card.
        if(player_rank=='A'&&player_hand.size()==3){
            //cout << "Split aces so stand" << endl;
            break;
        }
        // If player has doubled down it can receive only one more card.
        if(player_doubled_down&&player_hand.size()==3){
            //cout << "Standing after after doubling down" << endl;
            break;
        }
        
        // Hit the player
        int ind=deal_card();
        char rank=show_rank(ind);
        int val=value(rank,player_soft);
        //cout << "Dealing to player " << show_card(ind) << endl;
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

        if(player_count>21)
            player_busted=true;
        if(player_busted){
            //cout << "Player busted "<< " with the hand" << endl;
            //for(auto card : player_hand)
            //    cout << card << "\t";
            //cout << endl;
            //cout <<" at  the count " << player_count << endl;
            player_bankroll-=bet_size;
            dealer_bankroll+=bet_size;
            times_player_split_and_lost++;
            if(player_doubled_down){
                player_bankroll-=bet_size;
                dealer_bankroll+=bet_size;
                times_player_doubled_down_and_lost++;
            }
            player_time_series.push_back(player_bankroll);
            return;
        }
    }
    
    // Dealer's hand. Only fill it if we are processing first split hand.
    if(dealer_hand.size()==2){
        while(true){
            // Dealer stops if it goes to 17 or higher.
            if(dealer_count>=17){
                //cout << "Dealer count >= 17" << endl;
                break;
            }
            
            // Hit the dealer
            int ind=deal_card();
            char rank=show_rank(ind);
            int val=value(rank,dealer_soft);
            //cout << "Dealing to dealer " << show_card(ind) << endl;
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
        //cout << "Dealer busted "<< " with the hand" << endl;
        //for(auto card : dealer_hand)
        //    cout << card << "\t";
        //cout << endl;
        //cout <<" at  the count " << dealer_count << endl;
        player_won+=1;
        dealer_bankroll-=bet_size;
        player_bankroll+=bet_size;
        times_player_split_and_won++;
        if(player_doubled_down){
            player_bankroll+=bet_size;
            dealer_bankroll-=bet_size;
            times_player_doubled_down_and_won++;
        }
        player_time_series.push_back(player_bankroll);
        return;
    }
    
    if(player_count==dealer_count){
        //cout << "Draw" << endl;
        draws+=1;
        player_time_series.push_back(player_bankroll);
        return;
    }
    if(player_count>dealer_count){
        //cout << "Player count " << player_count << " higher than the dealer count " << dealer_count << endl;
        player_won+=1;
        player_bankroll+=bet_size;
        dealer_bankroll-=bet_size;
        times_player_split_and_won++;
        if(player_doubled_down){
            player_bankroll+=bet_size;
            dealer_bankroll-=bet_size;
            times_player_doubled_down_and_won++;
        }
        player_time_series.push_back(player_bankroll);
        return;
    }
    if(player_count<dealer_count){
        //cout << "Player count " << player_count <<
        //" lower than the dealer count " << dealer_count << endl;
        player_bankroll-=bet_size;
        dealer_bankroll+=bet_size;
        times_player_split_and_lost++;
        if(player_doubled_down){
            player_bankroll-=bet_size;
            dealer_bankroll+=bet_size;
            times_player_doubled_down_and_lost++;
        }
        player_time_series.push_back(player_bankroll);
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

// Reshuffling happens after we go through 1/3 of the deck.
void Game::play(const int & rounds, const int & A, const int & B){
    
    while(rounds_played<rounds){
        if(player_bankroll<=0||dealer_bankroll<=0)
            break;
        ++rounds_played;
        clear_for_next_round();
        
        //cout << "rounds played = " << rounds_played << endl;
        //cout << "get_pointer() = " << get_pointer() << endl;
        //cout << "get_order().size()/3 = " <<
        //get_order().size()/3 << endl;
        
        // Check whether is depleted to a third, and if yes reshuffle.
        if(get_pointer()>get_order().size()/3){
            //cout << "Attempting to reset" << endl;
            reset();
            //cout << "Reset pointer = " << get_pointer() << endl;
        }

        one_round();
        //cout << "Played a new round" << endl;
    }
}
