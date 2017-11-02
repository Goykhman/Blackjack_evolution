using namespace std;

/**
 
 Chromosome is a set of matrices encoding:
 
 * 100 split entries, row by row
 * 100 soft double down entries, row by row
 * 200 hard double down entries, row by row
 * 200 soft stand entries, row by row
 * 200 hard stand entries, row by row
 
 In the flattened form chromosome is a vector
 with 800 entries of 0 and 1, of the row serialization
 of the matrices described above.
 
 For instance, in the light-weight version the chromosome
 
 M={A,B}
 
 where
 
 A={{a,b},{c,d}} (matrix with the rows {a,b} and {c,d})
 B={{e,f},{g,h}} (matrix with the rows {a,b} and {c,d})
 
 gets serialized into
 
 M_serialized={a,b,c,d,e,f,g,h}
 
 Flattenning of the matrix-defined chromosome into a
 vector is important when we want to cross-over two
 chromosomes during the reproduction process, and when
 we want to initialize a new chromosome.
 
 */

class Chromosome{
public:
    // Default constructor, will initialize chromosome randomly.
    Chromosome();
    // Constructor with flattened chromosome.
    Chromosome(vector<int>);
    // Flattens chromosome into one vector.
    vector<int> flatten();
    // Generates randomly a number from 0 to the given number minus 1.
    int rand_index(int);
    // Generate index for card of given rank (0-9).
    int card_index(const char &);
    // Generate index for possible sums (0-19).
    int sum_index(const int &);
    // Interfaces to private variables.
    int get_split(const int & i, const int & j){
        return split[i][j];
    }
    int get_soft_double_down(const int & i, const int & j){
        return soft_double_down[i][j];
    }
    int get_hard_double_down(const int & i, const int & j){
        return hard_double_down[i][j];
    }
    int get_soft_stand(const int & i, const int & j){
        return soft_stand[i][j];
    }
    int get_hard_stand(const int & i, const int & j){
        return hard_stand[i][j];
    }
private:
    vector<vector<int>> split;
    vector<vector<int>> soft_double_down;
    vector<vector<int>> hard_double_down;
    vector<vector<int>> soft_stand;
    vector<vector<int>> hard_stand;
};

Chromosome::Chromosome(){
    // Begin by initializing the chromosome matrices to zero.
    for(int i=0;i<10;++i){
        vector<int> row(10,0);
        split.push_back(row);
    }
    for(int i=0;i<10;++i){
        vector<int> row(10,0);
        soft_double_down.push_back(row);
    }
    for(int i=0;i<20;++i){
        vector<int> row(10,0);
        hard_double_down.push_back(row);
    }
    for(int i=0;i<20;++i){
        vector<int> row(10,0);
        soft_stand.push_back(row);
    }
    for(int i=0;i<20;++i){
        vector<int> row(10,0);
        hard_stand.push_back(row);
    }
    // Fill in the chromosome matrices with random entries.
    for(int i=0;i<10;++i){
        for(int j=0;j<10;++j){
            int r=rand_index(2);
            split[i][j]=r;
        }
    }
    // We cannot double down on A-T, so the last row of soft
    // double down chromosome matrix will always be zero.
    // (which won't stop it from mutating away from zero.)
    for(int i=0;i<9;++i){
        for(int j=0;j<10;++j){
            int r=rand_index(2);
            soft_double_down[i][j]=r;
        }
    }
    // this might mutate.
    for(int j=0;j<10;++j){
        soft_double_down[9][j]=0;
    }
    // Hard double down cannot be on sum=21.
    for(int i=0;i<19;++i){
        for(int j=0;j<10;++j){
            int r=rand_index(2);
            hard_double_down[i][j]=r;
        }
    }
    // this also might mutate.
    for(int j=0;j<10;++j){
        hard_double_down[19][j]=0;
    }
    // Hard stand, always stand on 21.
    for(int i=0;i<19;++i){
        for(int j=0;j<10;++j){
            int r=rand_index(2);
            hard_stand[i][j]=r;
        }
    }
    // this also might mutate, but will have no phenotypic expression.
    for(int j=0;j<10;++j){
        hard_stand[19][j]=1;
    }
    // Soft stand, always stand on 21.
    for(int i=0;i<19;++i){
        for(int j=0;j<10;++j){
            int r=rand_index(2);
            soft_stand[i][j]=r;
        }
    }
    // this also might mutate, but will have no phenotypic expression.
    for(int j=0;j<10;++j){
        soft_stand[19][j]=1;
    }
}

Chromosome::Chromosome(vector<int> chrom){
    // Begin by initializing the chromosome matrices to zero.
    for(int i=0;i<10;++i){
        vector<int> row(10,0);
        split.push_back(row);
    }
    for(int i=0;i<10;++i){
        vector<int> row(10,0);
        soft_double_down.push_back(row);
    }
    for(int i=0;i<20;++i){
        vector<int> row(10,0);
        hard_double_down.push_back(row);
    }
    for(int i=0;i<20;++i){
        vector<int> row(10,0);
        soft_stand.push_back(row);
    }
    for(int i=0;i<20;++i){
        vector<int> row(10,0);
        hard_stand.push_back(row);
    }
    // Chromosome vector 'chrom' is parsed into 5 vectors, which
    // are to be flattened chromosome matrices.
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
            split[i][j]=split_flattened[10*i+j];
    for(int i=0;i<10;i++)
        for(int j=0;j<10;j++)
            soft_double_down[i][j]=soft_double_down_flattened[10*i+j];
    for(int i=0;i<20;i++)
        for(int j=0;j<10;j++)
            hard_double_down[i][j]=hard_double_down_flattened[10*i+j];
    for(int i=0;i<20;i++)
        for(int j=0;j<10;j++)
            soft_stand[i][j]=soft_stand_flattened[10*i+j];
    for(int i=0;i<20;i++)
        for(int j=0;j<10;j++)
            hard_stand[i][j]=hard_stand_flattened[10*i+j];
}

vector<int> Chromosome::flatten(){
    vector<int> chrom;
    vector<int> split_flatten;
    for(int i=0;i<10;++i)
        for(int j=0;j<10;++j)
            split_flatten.push_back(split[i][j]);
    copy(split_flatten.begin(),split_flatten.end(),back_inserter(chrom));
    vector<int> soft_double_down_flatten;
    for(int i=0;i<10;++i)
        for(int j=0;j<10;++j)
            soft_double_down_flatten.push_back(soft_double_down[i][j]);
    copy(soft_double_down_flatten.begin(),soft_double_down_flatten.end(),back_inserter(chrom));
    vector<int> hard_double_down_flatten;
    for(int i=0;i<20;++i)
        for(int j=0;j<10;++j)
            hard_double_down_flatten.push_back(hard_double_down[i][j]);
    copy(hard_double_down_flatten.begin(),hard_double_down_flatten.end(),back_inserter(chrom));
    vector<int> soft_stand_flatten;
    for(int i=0;i<20;++i)
        for(int j=0;j<10;++j)
            soft_stand_flatten.push_back(soft_stand[i][j]);
    copy(soft_stand_flatten.begin(),soft_stand_flatten.end(),back_inserter(chrom));
    vector<int> hard_stand_flatten;
    for(int i=0;i<20;++i)
        for(int j=0;j<10;++j)
            hard_stand_flatten.push_back(hard_stand[i][j]);
    copy(hard_stand_flatten.begin(),hard_stand_flatten.end(),back_inserter(chrom));
    return chrom;
}

int Chromosome::rand_index(int c){
    int bin=INT_MAX/c;
    int r=rand();
    r/=bin;
    return r;
}

int Chromosome::card_index(const char & card){
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

int Chromosome::sum_index(const int & player_count){
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
