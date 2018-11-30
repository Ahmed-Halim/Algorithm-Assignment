// Name : Ahmed Mohamed Abdelhalim
// ID : 162097

#include <iostream>
#include <utility>
#include <cmath>
#include <algorithm>
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
    
    // construct segment tree that hold indexes of minimum height
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
    
    // function return the index of the mimimum value in range l : r
    int Query (int v , int start, int end, int l, int r) {
        if (l > r)
            return -1;
        if (l == start && r == end)
            return ST[v];
        int mid = (start + end) / 2;
        return MinIndex(Query (v*2, start, mid, l, min(r,mid)) , Query (v*2+1, mid+1, end, max(l,mid+1), r)); //do recursion in log(n) in the segment tree to find the index
    }
    
    // function return index of minmum height of two values
    int MinIndex(int i, int j) {
        if (i == -1) return j;
        if (j == -1) return i;
        return (data[i].height < data[j].height) ? i : j;
    }
    
    // function get sum of width in range [l : r] from prefix sum array
    int GetWidth(int l , int r) {
        if (l == 0)
            return data[r].width;
        else
            return data[r].width - data[l-1].width;
    }
    
    // utility function return the maximum value between 3 values
    int GetMax(int x , int y ,  int z) {
        return max(max(x , y) , z);
    }
    
    // function divide array int sub-array and calculate the area fit condition of min height and return the max area in the end. in addition, store range of the building of biggest area.
    int Partition(int l, int r) {
        if (l > r) return -1;
        
        int IndexMinHeight = Query(1 , 0 , n - 1 , l , r); // find index of min height using segment tree query
        
        int left = Partition(l, IndexMinHeight-1); // recursively  go to left sub-array to get the biggest area of it
        int right = Partition(IndexMinHeight+1, r); // recursively  go to right sub-array to get the biggest area of it
        int AreaWithCurrentMinHeight = data[IndexMinHeight].height * GetWidth(l , r); // calculate area by multiply min height in this range by the sum width of this range
        
        int BestArea = GetMax(left , right , AreaWithCurrentMinHeight); // store max area of 3 parts

        if (BestArea > CurrentBiggestArea) { // if area obtained from this sub call is greater than last greater one then store the range of this sub call as it will be the start and end point of building with our answer
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
    SegmentTree(Building _data[] , int size) { // Constructor
        data = _data;
        n = size;
        TreeHeight = (int)(ceil(log2(n)));
        TreeSize = 2*(int)pow(2, TreeHeight) - 1;
        ST = new int[TreeSize];
        memset(ST , sizeof(ST) , 0);
        Build(1, 0, n - 1);
    }
    
    int getBiggestArea() { // function return biggest area by calling partation function
        return Partition(0 , n-1);
    }
    
    void getBuildings () { // function print buildings with biggest area
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
        
        if (i > 0) arr[i].width += arr[i-1].width; // build prefix-sum array
    }
    
    SegmentTree ST(arr , n); // create segment tree hold indexes of min height for every range
    
    cout << "Biggest Area : " << ST.getBiggestArea() << endl;
    ST.getBuildings();
    return 0;
}

