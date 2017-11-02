#include <iostream>
#include <cstdlib>
#include <time.h>
#include <string>
#include <vector>
#include <algorithm>
#include <fstream>
#include <array>
#include <random>       
#include <chrono> 

#include "Deck.h"
#include "BasicStrategy.h"
#include "Game.h"

using namespace std;

void play_game(Game game1){
    // Run 10000 rounds of game. Here 30 and 51 are redundant argruments,
    // the Game actually doesn't have any role for those parameters any more.
    // The former meaning was that the dealer would spontaneously reshuffle the
    // deck once the deck pointer was between 30 and 51, that is, when between
    // 30 and 51 cards have been dealt.
    game1.play(10000,30,51);
    
    double total_number_of_wins=game1.get_player_won();
    double total_number_of_draws=game1.get_draws();
    double total_rounds_played=game1.get_rounds_played();
    double total_rounds_losses=total_rounds_played-total_number_of_wins-total_number_of_draws;
    double prob_win=total_number_of_wins/total_rounds_played;
    double prob_loss=total_rounds_losses/total_rounds_played;
    double prob_draw=total_number_of_draws/total_rounds_played;
    
    cout << "Probability of player win is " << prob_win << endl;
    cout << "Probability of player loss is " << prob_loss << endl;
    cout << "Probability of draw is " << prob_draw << endl;
    cout << "Player's edge is " << prob_win-prob_loss << endl;
    
    string filename="player_time_series.csv";
    vector<int> player_time_series_1=game1.get_player_time_series();
    ofstream myfile(filename);
    int vsize = player_time_series_1.size()-1;
    for(int n=0; n<vsize; n++){
        myfile << player_time_series_1[n];
        myfile << "," ;
    }
    myfile << player_time_series_1[vsize-1];
    
    cout << "Player's bankroll is " << game1.get_player_bankroll() << endl;
    cout << "Dealer's bankroll is " << game1.get_dealer_bankroll() << endl;
    cout << "Player has won " << game1.get_player_won() << " games" << endl;
    cout << "Total number of rounds played is "<< game1.get_rounds_played() << endl;
    cout << "Total number of draws played is " << game1.get_draws() << endl;
    cout << "Total number of draws and wins is " << game1.get_draws()+
                                                    game1.get_player_won() << endl;
    cout << "Total number of times player doubled down " << game1.get_times_player_doubled_down() << endl;
    cout << "Total number of times player doubled down and won " << game1.get_times_player_doubled_down_and_won() << endl;
    cout << "Total number of times player doubled down and lost " << game1.get_times_player_doubled_down_and_lost() << endl;
    cout << "Total number of times player split " << game1.get_times_player_split() << endl;
    cout << "Total number of times player split and won " << game1.get_times_player_split_and_won() << endl;
    cout << "Total number of times player split and lost " << game1.get_times_player_split_and_lost() << endl;
    cout << "Remember that the split hands are played twice, so the sum of number of drawn split hands, won split hands, and lost split hands is twice the number of split hands" << endl;
    cout << "Size of player time series is " << game1.get_player_time_series().size() << " which should be larger than the number of rounds played by the number of hands split." <<endl;
}

void calculate_edge_and_bankroll(const int & rounds){
    vector<double> edges={};
    vector<double> tot_wins={};
    vector<double> tot_losses={};
    vector<int> player_bankrolls={};
    vector<double> prob_double_down={};
    vector<double> prob_double_down_won={};
    vector<double> prob_double_down_loss={};
    vector<double> prob_split={};
    vector<double> prob_split_won={};
    vector<double> prob_split_loss={};
    for(int round=0;round<rounds;++round){
        Game game1(1000,2000,2);
        game1.play(10000,30,51);

        double total_number_of_wins=game1.get_player_won();
        double total_rounds_played=game1.get_rounds_played();
        double total_number_of_draws=game1.get_draws();
        double total_rounds_losses=total_rounds_played-total_number_of_wins-total_number_of_draws;
        double prob_win=total_number_of_wins/total_rounds_played;
        double prob_loss=total_rounds_losses/total_rounds_played;
        double edge=prob_win-prob_loss;
        edges.push_back(edge);

        player_bankrolls.push_back(game1.get_player_bankroll());

        double tot_double_downs=game1.get_times_player_doubled_down();
        double double_downs_won=game1.get_times_player_doubled_down_and_won();
        double double_downs_lost=game1.get_times_player_doubled_down_and_lost();
        prob_double_down.push_back(tot_double_downs/total_rounds_played);
        prob_double_down_won.push_back(double_downs_won/tot_double_downs);
        prob_double_down_loss.push_back(double_downs_lost/tot_double_downs);

        double tot_splits=game1.get_times_player_split();
        double split_won=game1.get_times_player_split_and_won();
        double split_lost=game1.get_times_player_split_and_lost();
        prob_split.push_back(tot_splits/total_rounds_played);
        prob_split_won.push_back(split_won/(2*tot_splits));
        prob_split_loss.push_back(split_lost/(2*tot_splits));
        
        tot_wins.push_back(total_number_of_wins/total_rounds_played);
        tot_losses.push_back(total_rounds_losses/total_rounds_played);
    }

    string filename="edges.csv";
    ofstream myfile1(filename);
    int vsize = edges.size()-1;
    for(int n=0; n<vsize; n++){
        myfile1 << edges[n];
        myfile1 << "," ;
    }
    myfile1 << edges[vsize-1];
    
    filename="tot_wins.csv";
    ofstream myfilew(filename);
    vsize = tot_wins.size()-1;
    for(int n=0; n<vsize; n++){
        myfilew << tot_wins[n];
        myfilew << "," ;
    }
    myfilew << tot_wins[vsize-1];
    
    filename="tot_losses.csv";
    ofstream myfilel(filename);
    vsize = tot_losses.size()-1;
    for(int n=0; n<vsize; n++){
        myfilel << tot_losses[n];
        myfilel << "," ;
    }
    myfilel << tot_losses[vsize-1];

    filename="player_bankrolls.csv";
    ofstream myfile2(filename);
    vsize = player_bankrolls.size()-1;
    for(int n=0; n<vsize; n++){
        myfile2 << player_bankrolls[n];
        myfile2 << "," ;
    }
    myfile2 << player_bankrolls[vsize-1];

    filename="prob_double_down.csv";
    ofstream myfile3(filename);
    vsize = prob_double_down.size()-1;
    for(int n=0; n<vsize; n++){
        myfile3 << prob_double_down[n];
        myfile3 << "," ;
    }
    myfile3 << prob_double_down[vsize-1];

    filename="prob_double_down_won.csv";
    ofstream myfile4(filename);
    vsize = prob_double_down_won.size()-1;
    for(int n=0; n<vsize; n++){
        myfile4 << prob_double_down_won[n];
        myfile4 << "," ;
    }
    myfile4 << prob_double_down_won[vsize-1];

    filename="prob_double_down_loss.csv";
    ofstream myfile5(filename);
    vsize = prob_double_down_loss.size()-1;
    for(int n=0; n<vsize; n++){
        myfile5 << prob_double_down_loss[n];
        myfile5 << "," ;
    }
    myfile5 << prob_double_down_loss[vsize-1];

    filename="prob_split.csv";
    ofstream myfile6(filename);
    vsize = prob_split.size()-1;
    for(int n=0; n<vsize; n++){
        myfile6 << prob_split[n];
        myfile6 << "," ;
    }
    myfile6 << prob_split[vsize-1];

    filename="prob_split_won.csv";
    ofstream myfile7(filename);
    vsize = prob_split_won.size()-1;
    for(int n=0; n<vsize; n++){
        myfile7 << prob_split_won[n];
        myfile7 << "," ;
    }
    myfile7 << prob_split_won[vsize-1];

    filename="prob_split_loss.csv";
    ofstream myfile8(filename);
    vsize = prob_split_loss.size()-1;
    for(int n=0; n<vsize; n++){
        myfile8 << prob_split_loss[n];
        myfile8 << "," ;
    }
    myfile8 << prob_split_loss[vsize-1];
}

int main(){
    srand(100000000*time(NULL));
    Game game1(1000,2000,2);
    
    // Sample game
    play_game(game1);
    
    // Play 1000 games/rounds. Each game will be 10000 rounds.
    int rounds1=1000;
    calculate_edge_and_bankroll(rounds1);
    
    return 0;
}
