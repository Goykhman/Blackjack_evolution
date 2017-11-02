using namespace std;

class Quicksort{
public:
    int pivot (vector<vector<double>> & s, int a, int b){
        int i=a-1;
        for (int j=a; j<=b-1; j++){
            if(s[j][1]<=s[b][1]){
                i++;
                vector<double> c=s[i];
                s[i]=s[j];
                s[j]=c;
            }
        }
        vector<double> c=s[i+1];
        s[i+1]=s[b];
        s[b]=c;
        return (i+1);
    }
    
    void sort(vector<vector<double>> & s, int i, int j){
        if(i<j){
            int p = pivot(s, i, j);
            sort(s, i, p-1);
            sort(s, p+1, j);
        }
    }
};
