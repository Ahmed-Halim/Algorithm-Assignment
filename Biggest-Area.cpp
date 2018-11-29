#include <bits/stdc++.h>
using namespace std;

struct Building {
    int height;
    int width;
};

class SegmentTree {
private:
    int * ST , n , TreeSize , TreeHeight , CurrentBiggestArea = 0;
    pair <int , int> Range;
    Building * data;
    
    void Build(int v, int start, int end) {
        if (start == end)
            ST[v] = start;
        else {
            int mid = (start + end) / 2;
            Build(v*2 , start , mid);
            Build((v*2)+1, mid+1, end );
            ST[v] = MinIndex(ST[v*2] , ST[v*2+1]);
        }
    }
    
    int Query (int v , int start, int end, int l, int r) {
        if (l > r)
            return -1;
        if (l == start && r == end)
            return ST[v];
        int mid = (start + end) / 2;
        return MinIndex(Query (v*2, start, mid, l, min(r,mid)) , Query (v*2+1, mid+1, end, max(l,mid+1), r));
    }
    
    int MinIndex(int i, int j) {
        if (i == -1) return j;
        if (j == -1) return i;
        return (data[i].height < data[j].height) ? i : j;
    }
    
    int GetWidth(int l , int r) {
        if (l == 0)
            return data[r].width;
        else
            return data[r].width - data[l-1].width;
    }
    
    int Partation(int l, int r) {
        if (l > r) return -1;
        
        int IndexMinHeight = Query(1 , 0 , n - 1 , l , r);
        
        int left = Partation(l, IndexMinHeight-1);
        int right = Partation(IndexMinHeight+1, r);
        int AreaWithCurrentMinHeight = data[IndexMinHeight].height * GetWidth(l , r);
        
        int BestArea = max(max(left , right) , AreaWithCurrentMinHeight);

        if (BestArea > CurrentBiggestArea) {
            CurrentBiggestArea = BestArea;
            if (left == BestArea) {
                Range = {l , IndexMinHeight-1};
            } else if (right == BestArea) {
                Range = {IndexMinHeight+1 , r};
            } else {
                Range = {l , r};
            }
        }
        
        return BestArea;
    }
    
    
public:
    SegmentTree(Building _data[] , int size) {
        data = _data;
        n = size;
        TreeHeight = (int)(ceil(log2(n)));
        TreeSize = 2*(int)pow(2, TreeHeight) - 1;
        ST = new int[TreeSize];
        memset(ST , sizeof(ST) , 0);
        Build(1, 0, n - 1);
    }
    
    int getBiggestArea() {
        return Partation(0 , n-1);
    }
    
    void getBuildings () {
        cout << "Buildings Number : ";
        for (int i = Range.first; i <= Range.second; i++) {
            cout << i+1 << " ";
        }
    }
    
};


int main() {
    int n;
    cout << "Enter number of buildings : ";
    cin >> n;
    Building * arr = new Building[n];
    for (int i = 0; i < n; i++) {
        cout << "Enter width of building " << i+1 << " : ";
        cin >> arr[i].width;
        cout << "Enter height of building " << i+1 << " : ";
        cin >> arr[i].height;
        
        if (i > 0) arr[i].width += arr[i-1].width;
    }
    
    SegmentTree ST(arr , n);
    
    cout << "Biggest Area : " << ST.getBiggestArea() << endl;
    ST.getBuildings();
    
    return 0;
}
