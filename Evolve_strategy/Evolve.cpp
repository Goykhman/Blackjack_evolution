/**
 
 The strategies are encoded in the Chromosome class, which prescribes
 a particular decision for the given game situation. The Chromosome
 class has the chromosome vector at its core, which is a vector
 of 0 and 1 having N entries, each entry corresponding to one
 of the possible game situations and answering a decision-making question.
 Having 0 at that entry corresponds to the answer 'no', and having
 1 corresponds to the answer 'yes'. Here we are taking advantage of the
 fact that a blackjack strategy can be formulated in a series of
 yes/no prescriptions: should we stand/double down/split for the
 given game configuration. Details, such as the distinction between
 specific card composition of the given hand count, are ignored in this
 parametrization.
 
 The strategies are played out in the Game class, which inherits the
 Chromosome class. The Game class plays R rounds of game of the
 single player following the given strategy, against the dealer.
 We are concerned with the fraction of the wealth which the player
 wins/loses, and use it as a performance score of the strategy.
 
 Performance score becomes the fit score when we evolve the strategy
 evolutionary.
 
 Selection acts on the population of Game objects, each
 being a game playing the strategy specified by its chromosome.
 
 Game object has two kinds of constructors. These in turn
 correspond to two different constructors of the Chromosome
 object, which is inherited by the Game. The first constructor
 intializes the chromosome randomly, while the second constructor
 takes the argument vector as the assigned value of the
 chromosome. The chromosome encodes the Game strategy for any
 possible game situation.
 
 In this file we will use the second constructor for the Game
 object, which initializes the chromosome by the given vector.
 The latter is read from the .csv file.
 
 Similarly we can initialize the random population of strategies and
 then evolve these strategies over a certain number of steps.

*/

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
#include <ctime>
#include <sstream>
#include <map>

#include "Chromosome.h"
#include "Deck.h"
#include "Game.h"
#include "Quicksort.h"

using namespace std;

class Evolve{
public:
    // Constructor takes as an argument initial bankrolls
    // for player and dealer, bet size, propagation rate,
    // selection rate, size of the population, and number
    // of rounds played, used to calculate fit scores.
    Evolve(int, int, int, double, double, int, int);
    // Constructor which also initializes all members of
    // population to basic strategy.
    Evolve(int, int, int, double, double, int, int,vector<int>);
    // Calculate fit scores for the current population.
    void update_fit_scores();
    // Select a parent from the set with given fit scores.
    int select_parent(const vector<double> &);
    // Produce an offspring for the given parents.
    Game offspring(const int &, const int &);
    // Produce a new generation. Returns mean score of the
    // 'select' most fit strategies.
    double new_generation();
    // Evolve over given number of steps.
    void evolve(const int &);
    // Print out mean strategies in the given population.
    void mean_strategy();
    // Read file with the strategy.
    vector<int> read_strategy(const string &);
    // Interfaces to private variables.
    double get_propagate(){
        return propagate;
    }
    double get_selection_rate(){
        return selection_rate;
    }
    Game get_population(int i){
        return population[i];
    }
    vector<double> get_fit_scores(){
        return fit_scores;
    }
    int get_population_size(){
        return M;
    }
    vector<double> get_score_time_series(){
        return score_time_series;
    }
private:
    // Initial player's bankroll.
    int p;
    // Initial dealer's bankroll.
    int d;
    // Bet size.
    int b;
    // Propagation rate, the probability to pass the gene identical for
    // both parents to the offspring without a mutation.
    double propagate;
    // Selection rate, the fraction of the most fit strategies selected
    // to be passed on to the next generation and to reproduce.
    double selection_rate;
    // Population of Games, each Game inherits Chromosome encoding the
    // strategy which it plays according to.
    vector<Game> population;
    // Fit scores for the members of the population.
    vector<double> fit_scores;
    // Number of strategies in the population.
    int M;
    // Number of rounds played used to calculate fit scores.
    int R;
    // Map between index and a card.
    map<int,char> card;
    // Time series of the mean population scores.
    vector<double> score_time_series;
};

Evolve::Evolve(int P, int D, int B, double prop, double sel_rate, int m, int r){
    p=P;
    d=D;
    b=B;
    propagate=prop;
    selection_rate=sel_rate;
    M=m;
    R=r;
    for(int i=0;i<M;++i){
        // Use default constructor for the Game, which will initiliaze
        // the corresponding Chromosome randomly.
        Game game(p,d,b);
        population.push_back(game);
    }
    // Calculate the fit scores for the initialized population.
    update_fit_scores();
    // Map between indexes and card ranks.
    card[0]='A';
    card[1]='2';
    card[2]='3';
    card[3]='4';
    card[4]='5';
    card[5]='6';
    card[6]='7';
    card[7]='8';
    card[8]='9';
    card[9]='T';
}

// Constructor initializing each member of population to given strategy.
Evolve::Evolve(int P, int D, int B, double prop, double sel_rate, int m, int r,
               vector<int> chrom){
    p=P;
    d=D;
    b=B;
    propagate=prop;
    selection_rate=sel_rate;
    M=m;
    R=r;
    for(int i=0;i<M;++i){
        Game game(p,d,b,chrom);
        population.push_back(game);
    }
    // Calculate the fit scores for the initialized population.
    update_fit_scores();
    // Map between indexes and card ranks.
    card[0]='A';
    card[1]='2';
    card[2]='3';
    card[3]='4';
    card[4]='5';
    card[5]='6';
    card[6]='7';
    card[7]='8';
    card[8]='9';
    card[9]='T';
}

// Calculating fit scores doesn't change the Game's attributes.
// we create copies of all Game objects and play R rounds of
// game on the copies.
void Evolve::update_fit_scores(){
    // Void the fit scores attribute of the Evolve class first.
    fit_scores={};
    // Fit score for each strategy 'i' is calculated as a
    // final bankroll after 'R' rounds of game.
    for(int i=0;i<M;++i){
        // Creates a copy of the Game object.
        Game game=population[i];
        game.play(R);
        // First of all check whether the player went bankrupt.
        if(game.get_player_bankroll()<=0){
            fit_scores.push_back(0);
            continue;
        }
        double final_bankroll=game.get_player_bankroll();
        double score=final_bankroll/p;
        fit_scores.push_back(score);
    }
}

int Evolve::select_parent(const vector<double> & fit_scores){
    // Normalize fit scores into the vector 'probs' of probabilities
    // to pick a strategy/parent based on its performance.
    vector<double> probs;
    double sum=0;
    for(double s : fit_scores)
        sum+=s;
    // 'prev' is the way to track where we are in the interval [0,1],
    // so that we pick a random number in [0,1] uniformly, and identify
    // the correspoding parent.
    double prev=0;
    double r=((double) rand() /RAND_MAX);
    for(int i=0;i<fit_scores.size();++i){
        double p=fit_scores[i]/sum+prev;
        if(r<p)
            return i;
        prev=p;
    }
    return fit_scores.size()-1;
}

Game Evolve::offspring(const int & i, const int & j){
    // Produce an offspring for parents 'i' and 'j'.
    vector<int> offspring_chrom;
    Game parent_i=population[i];
    Game parent_j=population[j];
    vector<int> parent_i_chrom=parent_i.flatten();
    vector<int> parent_j_chrom=parent_j.flatten();
    // If parents have opposite genes at the given location
    // then the child will receive a gene randomly from one of the
    // parents with probability proportional to their fitness.
    double fi=fit_scores[i];
    double fj=fit_scores[j];
    double prop_i=fi/(fi+fj);
    // Go over chromosome slots 'k'.
    for(int k=0;k<parent_i_chrom.size();++k){
        int gik=parent_i_chrom[k];
        int gjk=parent_j_chrom[k];
        double r=((double) rand() /RAND_MAX);
        if(gik==gjk){
            if(r<propagate)
                offspring_chrom.push_back(gik);
            else{
                int mutate=(gik+1)%2;
                offspring_chrom.push_back(mutate);
            }
        }
        else{
            if(r<prop_i)
                offspring_chrom.push_back(gik);
            else
                offspring_chrom.push_back(gjk);
        }
    }
    Game offspring(p,d,b,offspring_chrom);
    return offspring;
}

double Evolve::new_generation(){
    // Compose the map with the key being the index
    // of the strategy in the 'population' array,
    // and the value being the fit score of that strategy.
    vector<vector<double>> fit_map;
    for(double i=0;i<M;++i){
        vector<double> game_fit={i,fit_scores[i]};
        fit_map.push_back(game_fit);
    }
    // Sort this map based on the fit.
    // This sorting is in the increasing order.
    Quicksort qs;
    qs.sort(fit_map,0,M-1);
    // We will select 'select' most fit and will need to
    // produce 'fill' new strategies.
    int select=selection_rate*M;
    int fill=M-select;
    // selected indexes and fit scores will be saved here.
    vector<int> select_indexes;
    vector<double> select_fit_scores;
    // scores of the 'select' most fit.
    double scores_of_fit=0;
    // the new population will be saved here.
    vector<Game> new_population;
    for(int i=0;i<select;++i){
        // Remember it's inverse order in the sorted scores array.
        int ind=fit_map[M-1-i][0];
        double score=fit_map[M-1-i][1];
        // cout << score << endl;
        // Add the scores.
        scores_of_fit+=score;
        select_indexes.push_back(ind);
        select_fit_scores.push_back(score);
        // Remember to reset bankroll's of the stratgies to their
        // original values, so that everyone in the next generation
        // starts equally.
        population[ind].set_player_bankroll(p);
        population[ind].set_dealer_bankroll(d);
        new_population.push_back(population[ind]);
    }
    scores_of_fit/=select;
    // produce 'fill' new strategies as offsprings of the
    // 'select' parents. The probability to choose each
    // parent is proportional to its fit score.
    int ct=0;
    while(ct<fill){
        int i0=select_parent(select_fit_scores);
        int j0=select_parent(select_fit_scores);
        // make sure different parents are selected.
        while(j0==i0)
            j0=select_parent(select_fit_scores);
        // indexes of selected parents in the original population.
        int i=select_indexes[i0];
        int j=select_indexes[j0];
        // produce child for these parents.
        Game child=offspring(i,j);
        new_population.push_back(child);
        ct++;
    }
    // update the population.
    population=new_population;
    return scores_of_fit;
}

void Evolve::evolve(const int & generations){
    for(int i=0;i<generations;++i){
        //cout << currentDateTime() << endl;
        update_fit_scores();
        double mean_fit=new_generation();
        cout << "fit for generation " << i << " is " << mean_fit << endl;
        score_time_series.push_back(mean_fit);
    }
}

void Evolve::mean_strategy(){
    // Compose the map with the key being the index
    // of the strategy in the 'population' array,
    // and the value being the fit score of that strategy.
    vector<vector<double>> fit_map;
    for(double i=0;i<M;++i){
        vector<double> game_fit={i,fit_scores[i]};
        fit_map.push_back(game_fit);
    }
    // Sort this map based on the fit
    // this sorthing is in the increasing order.
    Quicksort qs;
    qs.sort(fit_map,0,M-1);
    // We will select 'select' most fit.
    int select=selection_rate*M;
    double tot_fit=0;
    for(int i=0;i<select;++i)
        tot_fit+=fit_map[M-1-i][1];
    double mean_fit=tot_fit/select;
    // fit_weights will save the fit weights of the fit
    // startegies over the total among the fit.
    vector<double> fit_weights;
    for(int i=0;i<select;++i){
        double ft=fit_map[M-1-i][1];
        ft/=tot_fit;
        fit_weights.push_back(ft);
    }
    //**
    cout << "*****************************************************************" << endl;
    cout << "Mean fit of the select most fit = " << mean_fit << endl;
    cout << "*****************************************************************" << endl;
    //**
    vector<vector<double>> mean_split(10,vector<double>(10,0));
    vector<vector<double>> mean_soft_double_down(10,vector<double>(10,0));
    vector<vector<double>> mean_hard_double_down(20,vector<double>(10,0));
    vector<vector<double>> mean_soft_stand(20,vector<double>(10,0));
    vector<vector<double>> mean_hard_stand(20,vector<double>(10,0));
    //**
    vector<double> mean_split_flatten(100);
    vector<double> mean_soft_double_down_flatten(100);
    vector<double> mean_hard_double_down_flatten(200);
    vector<double> mean_soft_stand_flatten(200);
    vector<double> mean_hard_stand_flatten(200);
    //**
    for(int k=0;k<select;++k){
        Game g=population[fit_map[M-1-k][0]];
        for(int i=0;i<10;i++)
            for(int j=0;j<10;j++){
                mean_split[i][j]+=(g.get_split(i,j)*fit_weights[k]);
                mean_split_flatten[10*i+j]+=(g.get_split(i,j)*fit_weights[k]);
            }
        for(int i=0;i<10;i++)
            for(int j=0;j<10;j++){
                mean_soft_double_down[i][j]+=(g.get_soft_double_down(i,j)*fit_weights[k]);
                mean_soft_double_down_flatten[10*i+j]+=(g.get_soft_double_down(i,j)*fit_weights[k]);
            }
        for(int i=0;i<20;i++)
            for(int j=0;j<10;j++){
                mean_hard_double_down[i][j]+=(g.get_hard_double_down(i,j)*fit_weights[k]);
                mean_hard_double_down_flatten[10*i+j]+=(g.get_hard_double_down(i,j)*fit_weights[k]);
            }
        for(int i=0;i<20;i++)
            for(int j=0;j<10;j++){
                mean_soft_stand[i][j]+=(g.get_soft_stand(i,j)*fit_weights[k]);
                mean_soft_stand_flatten[10*i+j]+=(g.get_soft_stand(i,j)*fit_weights[k]);
            }
        for(int i=0;i<20;i++)
            for(int j=0;j<10;j++){
                mean_hard_stand[i][j]+=(g.get_hard_stand(i,j)*fit_weights[k]);
                mean_hard_stand_flatten[10*i+j]+=(g.get_hard_stand(i,j)*fit_weights[k]);
            }
    }
    // Save to file
    vector<double> chromosome;
    copy(mean_split_flatten.begin(),mean_split_flatten.end(),back_inserter(chromosome));
    copy(mean_soft_double_down_flatten.begin(),mean_soft_double_down_flatten.end(),back_inserter(chromosome));
    copy(mean_hard_double_down_flatten.begin(),mean_hard_double_down_flatten.end(),back_inserter(chromosome));
    copy(mean_soft_stand_flatten.begin(),mean_soft_stand_flatten.end(),back_inserter(chromosome));
    copy(mean_hard_stand_flatten.begin(),mean_hard_stand_flatten.end(),back_inserter(chromosome));
    //**
    ofstream file_chromosome;
    file_chromosome.open("chrom.csv");
    int vsize = chromosome.size()-1;
    for(int n=0; n<vsize; n++){
        file_chromosome << chromosome[n];
        file_chromosome << "," ;
    }
    file_chromosome << chromosome[vsize-1];
    file_chromosome.close();
    // Print to console
    cout << "*****************************************************************" << endl;
    cout << "                            Mean split                           "<< endl;
    cout << "*****************************************************************" << endl;
    cout << "*****" << " ";
    cout << "  A  " << " ";
    cout << "  2  " << " ";
    cout << "  3  " << " ";
    cout << "  4  " << " ";
    cout << "  5  " << " ";
    cout << "  6  " << " ";
    cout << "  7  " << " ";
    cout << "  8  " << " ";
    cout << "  9  " << " ";
    cout << "  T  " << " ";
    cout << endl;
    for(int i=0;i<10;i++){
        cout << "  " << card[i] << "  " << " ";
        for(int j=0;j<10;j++){
            double s=mean_split[i][j];
            int s_i=int(100*s);
            s=s_i/100.0;
            ostringstream strs;
            strs << s;
            string str = strs.str();
            while(str.size()<5)
                str+=" ";
            cout << str << " ";
        }
        cout << endl;
    }
    cout << "*****************************************************************" << endl;
    cout << "                     Mean soft double down                       " << endl;
    cout << "*****************************************************************" << endl;
    cout << "*****" << " ";
    cout << "  A  " << " ";
    cout << "  2  " << " ";
    cout << "  3  " << " ";
    cout << "  4  " << " ";
    cout << "  5  " << " ";
    cout << "  6  " << " ";
    cout << "  7  " << " ";
    cout << "  8  " << " ";
    cout << "  9  " << " ";
    cout << "  T  " << " ";
    cout << endl;
    for(int i=0;i<9;i++){
        cout << "  " << card[i] << "  " << " ";
        for(int j=0;j<10;j++){
            double s=mean_soft_double_down[i][j];
            int s_i=int(100*s);
            s=s_i/100.0;
            ostringstream strs;
            strs << s;
            string str = strs.str();
            while(str.size()<5)
            str+=" ";
            cout << str << " ";
        }
        cout << endl;
    }
    cout << "*****************************************************************" << endl;
    cout << "                       Mean hard double down                     " << endl;
    cout << "*****************************************************************" << endl;
    cout << "*****" << " ";
    cout << "  A  " << " ";
    cout << "  2  " << " ";
    cout << "  3  " << " ";
    cout << "  4  " << " ";
    cout << "  5  " << " ";
    cout << "  6  " << " ";
    cout << "  7  " << " ";
    cout << "  8  " << " ";
    cout << "  9  " << " ";
    cout << "  T  " << " ";
    cout << endl;
    for(int i=0;i<19;i++){
        cout << "  " << i+2 << "  ";
        if(i<8)
            cout << " ";
        for(int j=0;j<10;j++){
            double s=mean_hard_double_down[i][j];
            int s_i=int(100*s);
            s=s_i/100.0;
            ostringstream strs;
            strs << s;
            string str = strs.str();
            while(str.size()<5)
            str+=" ";
            cout << str << " ";
        }
        cout << endl;
    }
    cout << "*****************************************************************" << endl;
    cout << "                          Mean soft stand                        " << endl;
    cout << "*****************************************************************" << endl;
    cout << "*****" << " ";
    cout << "  A  " << " ";
    cout << "  2  " << " ";
    cout << "  3  " << " ";
    cout << "  4  " << " ";
    cout << "  5  " << " ";
    cout << "  6  " << " ";
    cout << "  7  " << " ";
    cout << "  8  " << " ";
    cout << "  9  " << " ";
    cout << "  T  " << " ";
    cout << endl;
    for(int i=0;i<19;i++){
        cout << "  " << i+2 << "  ";
        if(i<8)
            cout << " ";
        for(int j=0;j<10;j++){
            double s=mean_soft_stand[i][j];
            int s_i=int(100*s);
            s=s_i/100.0;
            ostringstream strs;
            strs << s;
            string str = strs.str();
            while(str.size()<5)
            str+=" ";
            cout << str << " ";
        }
        cout << endl;
    }
    cout << "*****************************************************************" << endl;
    cout << "                         Mean hard stand                         " << endl;
    cout << "*****************************************************************" << endl;
    cout << "*****" << " ";
    cout << "  A  " << " ";
    cout << "  2  " << " ";
    cout << "  3  " << " ";
    cout << "  4  " << " ";
    cout << "  5  " << " ";
    cout << "  6  " << " ";
    cout << "  7  " << " ";
    cout << "  8  " << " ";
    cout << "  9  " << " ";
    cout << "  T  " << " ";
    cout << endl;
    for(int i=0;i<19;i++){
        cout << "  " << i+2 << "  ";
        if(i<8)
            cout << " ";
        for(int j=0;j<10;j++){
            double s=mean_hard_stand[i][j];
            int s_i=int(100*s);
            s=s_i/100.0;
            ostringstream strs;
            strs << s;
            string str = strs.str();
            while(str.size()<5)
            str+=" ";
            cout << str << " ";
        }
        cout << endl;
    }
    cout << endl;
    cout << endl;
}

// Input is to be a string with filename and .csv appended to it.
// This is probabilistic version: it reads a .csv file with the
// mean strategy and generates a sample strategy probabilistically.
// Non-probabilistic read_strategy() is defined outside Evolve class.
vector<int> Evolve::read_strategy(const string & file){
    string from(file);
    ifstream is(from);
    string str;
    getline(is,str);
    vector<double> chrom;
    int i=0;
    while(i<str.size()){
        int j=i;
        string entry;
        while(j<str.size()&&str[j]!=',')
            ++j;
        entry=str.substr(i,j-i);
        double e=stod(entry);
        chrom.push_back(e);
        i=j+1;
    }
    vector<int> ret;
    for(int i=0;i<chrom.size();++i){
        double r=((double) rand() /RAND_MAX);
        if(r<chrom[i])
            ret.push_back(1);
        else
            ret.push_back(0);
    }
    return ret;
}

// Input is to be a string with filename and .csv appended to it.
vector<int> read_strategy(const string & file){
    string from(file);
    ifstream is(from);
    string str;
    getline(is,str);
    vector<int> chrom;
    int i=0;
    while(i<str.size()){
        int j=i;
        string entry;
        while(j<str.size()&&str[j]!=',')
            ++j;
        entry=str.substr(i,j-i);
        int e=stod(entry);
        chrom.push_back(e);
        i=j+1;
    }
    return chrom;
}

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

int main(){
    srand(1000*chrono::system_clock::now().time_since_epoch().count());
    //**
    // Player bankroll choice for evolution is 10000 (while for tests it
    // is 1000), because most of the players will lose a lot if it starts
    // playing at random. We need a higher initial bankroll to have at
    // least some of the strategies to have positive money finishes
    // on the initial stages of evolution.
    int player_bankroll=10000;
    int dealer_bankroll=10000;
    int bet=2;
    // Decreasing the propagation_rate speeds up the evolution and saturates
    // it very quickly. Mutation should be slow enough to prevent
    // destruction of the newly assembled efficient strategies. The
    // saturation effectively arises when the creation of good strategies
    // from the bad ones is offset by the destuction of the good
    // strategies. It is usually better to set propagation_rate as closer to
    // one as possible and allow the evolution to go on for longer.
    // Beware of the possibility to get stuck in local optima though.
    double propagation_rate=0.9999;
    // Clearly decreasing selection rate makes it appear that the very high
    // scores are representative of the evolutionary dynamics, which
    // at some point might start suffer from overfitting (for instance,
    // if we fail to accumulate a sufficient statistics of the most fit
    // strategies). However, taking the selection rate too high will
    // keep the undesirable strategies breeding, which will create a lot
    // of noise.
    double selection_rate=0.05;
    // Size of population should be scaled accordingly to propgatation rate
    // and selection rate.
    int size_of_population=5000;
    // Number of rounds played to calculate fit scores (final bankrolls).
    int play_rounds=10000;
    // Evolve at least until the fit score saturates.
    int evolve_generations=5;
    //**
    // Input chromosome, if we want to evolve starting from the population
    // where each member is initialized to that chromosome.
    string chrom_file="strategy_chromosome.csv";
    // Here we use the read_strategy() defined outside of the Evolve class.
    // The Evolve class has its own read_strategy() function, which takes
    // as argument an average chromosome and makes a chromosome out of it
    // probabilistically.
    vector<int> prob=read_strategy(chrom_file);
    //**
    Evolve ev1(player_bankroll,dealer_bankroll,bet,propagation_rate,
               selection_rate,size_of_population,play_rounds,prob);
    //**
    ev1.evolve(evolve_generations);
    ev1.update_fit_scores();
    ev1.mean_strategy();
    //**
    vector<double> scores=ev1.get_score_time_series();
    string filename_scores="scores.csv";
    ofstream scores_stream(filename_scores);
    for(int i=0;i<scores.size()-1;++i){
        scores_stream << scores[i];
        scores_stream << "," ;
    }
    scores_stream << scores[scores.size()-1];
    return 0;
}
