// See the 'BasicStrategy.h' file for the description of
// card-to-index and total-to-index maps, and for the
// strategy matrices description.

// This file creates the chromosome .csv file encoding the
// Thorp's basic strategy. Change it accordingly to create
// any desired basic strategy.

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <sstream>
#include <fstream>

using namespace std;

vector<vector<int>> basic_split(10,vector<int>(10,0));
vector<vector<int>> basic_soft_double_down(10,vector<int>(10,0));
vector<vector<int>> basic_hard_double_down(20,vector<int>(10,0));
vector<vector<int>> basic_soft_stand(20,vector<int>(10,0));
vector<vector<int>> basic_hard_stand(20,vector<int>(10,0));

int main(){
    // Map between indexes and cards
    map<int,char> card;
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
    //** Splits.
    // Split A
    for(int j=0;j<10;++j){
        basic_split[0][j]=1;
    }
    // Split 2
    for(int j=1;j<7;++j){
        basic_split[1][j]=1;
    }
    // Split 3
    for(int j=1;j<7;++j){
        basic_split[2][j]=1;
    }
    // Split 4
    basic_split[3][4]=1;
    // Split 6
    for(int j=1;j<7;++j){
        basic_split[5][j]=1;
    }
    // Split 7
    for(int j=1;j<8;++j){
        basic_split[6][j]=1;
    }
    // Split 8
    for(int j=0;j<10;++j){
        basic_split[7][j]=1;
    }
    // Split 9
    for(int j=1;j<6;++j){
        basic_split[8][j]=1;
    }
    basic_split[8][7]=1;
    basic_split[8][8]=1;
    
    //** Soft double down.
    //Row is the second player's card (the first is A)
    // Second A
    basic_soft_double_down[0][4]=1;
    basic_soft_double_down[0][5]=1;
    // Second 2
    basic_soft_double_down[1][3]=1;
    basic_soft_double_down[1][4]=1;
    basic_soft_double_down[1][5]=1;
    // Second 3
    basic_soft_double_down[2][3]=1;
    basic_soft_double_down[2][4]=1;
    basic_soft_double_down[2][5]=1;
    // Second 4
    basic_soft_double_down[3][3]=1;
    basic_soft_double_down[3][4]=1;
    basic_soft_double_down[3][5]=1;
    // Second 5
    basic_soft_double_down[4][3]=1;
    basic_soft_double_down[4][4]=1;
    basic_soft_double_down[4][5]=1;
    // Second 6
    basic_soft_double_down[5][1]=1;
    basic_soft_double_down[5][2]=1;
    basic_soft_double_down[5][3]=1;
    basic_soft_double_down[5][4]=1;
    basic_soft_double_down[5][5]=1;
    // Second 7
    basic_soft_double_down[6][2]=1;
    basic_soft_double_down[6][3]=1;
    basic_soft_double_down[6][4]=1;
    basic_soft_double_down[6][5]=1;
    
    //** Hard double down.
    // Sum 8
    basic_hard_double_down[6][4]=1;
    basic_hard_double_down[6][5]=1;
    // Sum 9
    for(int j=1;j<6;++j){
        basic_hard_double_down[7][j]=1;
    }
    // Sum 10
    for(int j=1;j<9;++j){
        basic_hard_double_down[8][j]=1;
    }
    // Sum 11
    for(int j=0;j<10;++j){
        basic_hard_double_down[9][j]=1;
    }
    
    //** Soft stand.
    
    for(int i=16;i<20;++i){
        for(int j=1;j<8;++j){
            basic_soft_stand[i][j]=1;
        }
    }
    
    basic_soft_stand[17][8]=1;
    basic_soft_stand[18][8]=1;
    basic_soft_stand[19][8]=1;
    
    basic_soft_stand[17][9]=1;
    basic_soft_stand[18][9]=1;
    basic_soft_stand[19][9]=1;
    
    basic_soft_stand[16][0]=1;
    basic_soft_stand[17][0]=1;
    basic_soft_stand[18][0]=1;
    basic_soft_stand[19][0]=1;
    
    //** Hard stand.
    for(int i=15;i<20;++i){
        basic_hard_stand[i][0]=1;
    }
    for(int i=11;i<20;++i){
        basic_hard_stand[i][1]=1;
    }
    for(int i=11;i<20;++i){
        basic_hard_stand[i][2]=1;
    }
    for(int i=10;i<20;++i){
        basic_hard_stand[i][3]=1;
    }
    for(int i=10;i<20;++i){
        basic_hard_stand[i][4]=1;
    }
    for(int i=10;i<20;++i){
        basic_hard_stand[i][5]=1;
    }
    for(int i=15;i<20;++i){
        basic_hard_stand[i][6]=1;
    }
    for(int i=15;i<20;++i){
        basic_hard_stand[i][7]=1;
    }
    for(int i=15;i<20;++i){
        basic_hard_stand[i][8]=1;
    }
    for(int i=15;i<20;++i){
        basic_hard_stand[i][9]=1;
    }
    // Flatten.
    vector<int> basic_split_flatten(100,0);
    vector<int> basic_soft_double_down_flatten(100,0);
    vector<int> basic_hard_double_down_flatten(200,0);
    vector<int> basic_soft_stand_flatten(200,0);
    vector<int> basic_hard_stand_flatten(200,0);
    //**
    for(int i=0;i<10;++i){
        for(int j=0;j<10;++j){
            basic_split_flatten[10*i+j]=basic_split[i][j];
        }
    }
    for(int i=0;i<10;++i){
        for(int j=0;j<10;++j){
            basic_soft_double_down_flatten[10*i+j]=basic_soft_double_down[i][j];
        }
    }
    for(int i=0;i<20;++i){
        for(int j=0;j<10;++j){
            basic_hard_double_down_flatten[10*i+j]=basic_hard_double_down[i][j];
        }
    }
    for(int i=0;i<20;++i){
        for(int j=0;j<10;++j){
            basic_soft_stand_flatten[10*i+j]=basic_soft_stand[i][j];
        }
    }
    for(int i=0;i<20;++i){
        for(int j=0;j<10;++j){
            basic_hard_stand_flatten[10*i+j]=basic_hard_stand[i][j];
        }
    }
    vector<int> chromosome;
    copy(basic_split_flatten.begin(),basic_split_flatten.end(),
         back_inserter(chromosome));
    copy(basic_soft_double_down_flatten.begin(),basic_soft_double_down_flatten.end(),
         back_inserter(chromosome));
    copy(basic_hard_double_down_flatten.begin(),basic_hard_double_down_flatten.end(),
         back_inserter(chromosome));
    copy(basic_soft_stand_flatten.begin(),basic_soft_stand_flatten.end(),
         back_inserter(chromosome));
    copy(basic_hard_stand_flatten.begin(),basic_hard_stand_flatten.end(),
         back_inserter(chromosome));
    //**
    ofstream file_chromosome;
    file_chromosome.open("strategy_chromosome.csv");
    int vsize = chromosome.size()-1;
    for(int n=0; n<vsize; n++){
        file_chromosome << chromosome[n];
        file_chromosome << "," ;
    }
    file_chromosome << chromosome[vsize-1];
    file_chromosome.close();
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
            double s=basic_split[i][j];
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
    for(int i=0;i<10;i++){
        cout << "  " << card[i] << "  " << " ";
        for(int j=0;j<10;j++){
            double s=basic_soft_double_down[i][j];
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
    for(int i=0;i<20;i++){
        cout << "  " << i+2 << "  ";
        if(i<8)
            cout << " ";
        for(int j=0;j<10;j++){
            double s=basic_hard_double_down[i][j];
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
    for(int i=0;i<20;i++){
        cout << "  " << i+2 << "  ";
        if(i<8)
            cout << " ";
        for(int j=0;j<10;j++){
            double s=basic_soft_stand[i][j];
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
    for(int i=0;i<20;i++){
        cout << "  " << i+2 << "  ";
        if(i<8)
            cout << " ";
        for(int j=0;j<10;j++){
            double s=basic_hard_stand[i][j];
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
    return 0;
}
