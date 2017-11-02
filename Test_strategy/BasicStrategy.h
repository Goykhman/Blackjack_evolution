/**
 * BasicStrategy is a repository of methods for playing the basic strategy.
 */

using namespace std;

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

class BasicStrategy{
public:
    BasicStrategy() {
        for(int i=0;i<10;++i){
            vector<int> row(10,0);
            table_split.push_back(row);
        }
        for(int i=0;i<10;++i){
            vector<int> row(10,0);
            table_soft_double_down.push_back(row);
        }
        for(int i=0;i<20;++i){
            vector<int> row(10,0);
            table_hard_double_down.push_back(row);
        }
        for(int i=0;i<20;++i){
            vector<int> row(10,0);
            table_soft_stand.push_back(row);
        }
        for(int i=0;i<20;++i){
            vector<int> row(10,0);
            table_hard_stand.push_back(row);
        }
        //**
        string chrom_file="strategy_chromosome.csv";
        vector<int> chrom=read_strategy(chrom_file);
        //**
        vector<int> split_flattened;
        vector<int> soft_double_down_flattened;
        vector<int> hard_double_down_flattened;
        vector<int> soft_stand_flattened;
        vector<int> hard_stand_flattened;
        // Parse the 'chrom' into these 5 vectors.
        copy(chrom.begin(),chrom.begin()+100,back_inserter(split_flattened));
        copy(chrom.begin()+100,chrom.begin()+200,back_inserter(soft_double_down_flattened));
        copy(chrom.begin()+200,chrom.begin()+400,back_inserter(hard_double_down_flattened));
        copy(chrom.begin()+400,chrom.begin()+600,back_inserter(soft_stand_flattened));
        copy(chrom.begin()+600,chrom.begin()+800,back_inserter(hard_stand_flattened));
        // De-serialize the flat vectors into chromosome matrices.
        for(int i=0;i<10;i++)
            for(int j=0;j<10;j++)
                table_split[i][j]=split_flattened[10*i+j];
        for(int i=0;i<10;i++)
            for(int j=0;j<10;j++)
                table_soft_double_down[i][j]=soft_double_down_flattened[10*i+j];
        for(int i=0;i<20;i++)
            for(int j=0;j<10;j++)
                table_hard_double_down[i][j]=hard_double_down_flattened[10*i+j];
        for(int i=0;i<20;i++)
            for(int j=0;j<10;j++)
                table_soft_stand[i][j]=soft_stand_flattened[10*i+j];
        for(int i=0;i<20;i++)
            for(int j=0;j<10;j++)
                table_hard_stand[i][j]=hard_stand_flattened[10*i+j];
    };
    // Value of the rank, for soft or hard hand.
    int value(const char &, const bool &);
    // If stand for hard hand of given size, value, against dealer upcard.
    // We also need the first two player's cards to make sure it stands
    // with 7-7 against dealer's 10; the latter and the hand size is only
    // used for Thorp's basic strategy.
    bool hard_stand(const int &,const int &, const char &, const char &,
                    const char &);
    // If stand for soft hand of given value, against dealer upcard.
    bool soft_stand(const int &, const char &);
    // If double down for given hard hand, against dealer upcard.
    bool hard_double_down(const vector<char> & , const char &);
    // If doouble down for given soft hand, against dealer upcard.
    bool soft_double_down(const vector<char> & , const char &);
    // If split a pair.
    bool split_pair(const char &, const char &);
    // Card index.
    int card_index(const char &);
    // Sum index.
    int sum_index(const int &);
    // Interfaces to private variables.
    int get_split(const int & i, const int & j){
        return table_split[i][j];
    }
    int get_soft_double_down(const int & i, const int & j){
        return table_soft_double_down[i][j];
    }
    int get_hard_double_down(const int & i, const int & j){
        return table_hard_double_down[i][j];
    }
    int get_soft_stand(const int & i, const int & j){
        return table_soft_stand[i][j];
    }
    int get_hard_stand(const int & i, const int & j){
        return table_hard_stand[i][j];
    }
private:
    vector<vector<int>> table_split;
    vector<vector<int>> table_soft_double_down;
    vector<vector<int>> table_hard_double_down;
    vector<vector<int>> table_soft_stand;
    vector<vector<int>> table_hard_stand;
};

int BasicStrategy::value(const char & rank, const bool & soft){
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

int BasicStrategy::card_index(const char & card){
    switch(card){
        case 'A':
            return 0;
            break;
        case '2':
            return 1;
            break;
        case '3':
            return 2;
            break;
        case '4':
            return 3;
            break;
        case '5':
            return 4;
            break;
        case '6':
            return 5;
            break;
        case '7':
            return 6;
            break;
        case '8':
            return 7;
            break;
        case '9':
            return 8;
            break;
        case 'T':
            return 9;
            break;
    }
    return 0;
}

int BasicStrategy::sum_index(const int & player_count){
    switch(player_count){
        case 2:
            return 0;
            break;
        case 3:
            return 1;
            break;
        case 4:
            return 2;
            break;
        case 5:
            return 3;
            break;
        case 6:
            return 4;
            break;
        case 7:
            return 5;
            break;
        case 8:
            return 6;
            break;
        case 9:
            return 7;
            break;
        case 10:
            return 8;
            break;
        case 11:
            return 9;
            break;
        case 12:
            return 10;
            break;
        case 13:
            return 11;
            break;
        case 14:
            return 12;
            break;
        case 15:
            return 13;
            break;
        case 16:
            return 14;
            break;
        case 17:
            return 15;
            break;
        case 18:
            return 16;
            break;
        case 19:
            return 17;
            break;
        case 20:
            return 18;
            break;
        case 21:
            return 19;
            break;
    }
    return 0;
}

bool BasicStrategy::split_pair(const char & player_card, const char & dealer_card){
    int i=card_index(player_card);
    int j=card_index(dealer_card);
    return get_split(i,j)==1;
}

bool BasicStrategy::soft_double_down(const vector<char> & player_hand, const char & dealer_card){
    char player_card=player_hand[0];
    if(player_card=='A')
        player_card=player_hand[1];
    // cannot double down on the natural.
    if(player_card=='T')
        return false;
    int i=card_index(player_card);
    int j=card_index(dealer_card);
    return get_soft_double_down(i,j)==1;
}

bool BasicStrategy::hard_double_down(const vector<char> & player_hand, const char & dealer_card){
    int val=0;
    for(const char & c : player_hand)
        val+=value(c,false);
    int i=sum_index(val);
    int j=card_index(dealer_card);
    return get_hard_double_down(i,j)==1;
}

bool BasicStrategy::hard_stand(const int & player_hand_size,
                              const int & hand_value,
                              const char & dealer_upcard,
                              const char & player_1,
                              const char & player_2){
    int i=sum_index(hand_value);
    int j=card_index(dealer_upcard);
    return get_hard_stand(i,j)==1;
}

bool BasicStrategy::soft_stand(const int & player_count, const char & dealer_card){
    int i=sum_index(player_count);
    int j=card_index(dealer_card);
    return get_soft_stand(i,j)==1;
}
