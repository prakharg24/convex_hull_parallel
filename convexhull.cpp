#include "iostream"
#include "vector"
#include "ctime"
#include "omp.h"

using namespace std;

struct division{
    pair <int, int> a;
    pair <int, int> b;
    int side;
};

int absolute(double a){
    if(a>0) return a;
    return -1*a;
}

int get_side(pair <int, int> a, pair <int, int> b, pair <int, int> c){
    int val = (c.second - a.second)*(b.first - a.first) - (b.second - a.second)*(c.first - a.first);
    if(val>0) return 1;
    else if(val<0) return -1;
    return 0;
}

int get_dist(pair <int, int> a, pair <int, int> b, pair <int, int> c){
    int val = (c.second - a.second)*(b.first - a.first) - (b.second - a.second)*(c.first - a.first);
    return val;
}

int compare_val(pair <int, int> a, pair <int, int> b){
    if(a.first==b.first && a.second==b.second) return 1;
    return 0;
}

vector< pair<int, int> > calcConvexHull(vector< vector<int> > image, int num_threads) {
    omp_set_num_threads(num_threads);

    vector< division > div_arr;
    vector< pair<int, int> > ans;

    vector< pair<int, int> > arr;

    int h = image.size();
    int w = image[0].size();

    int max_x =-1;
    int min_x = h;
    int max_x_y;
    int min_x_y;

    // #pragma omp parallel for
    for(int i=0;i<h;i++){
        for(int j=0;j<w;j++){
            if(image[i][j]==1){
                // #pragma omp critical
                // {
                arr.push_back(make_pair(i, j));
                if(i>max_x){
                    max_x = i;
                    max_x_y = j;
                }
                if(i<min_x){
                    min_x = i;
                    min_x_y = j;
                }
                // }
            }
        }
    }
    

    pair <int, int> left = make_pair(min_x, min_x_y);
    pair <int, int> right = make_pair(max_x, max_x_y);

    division up = {left, right, 1};
    division down = {left, right, -1};

    div_arr.push_back(up);
    div_arr.push_back(down);

    ans.push_back(left);
    ans.push_back(right);

    int len = arr.size();

    vector <division> new_wor;
    while(div_arr.size()!=0){
        #pragma omp parallel for
        for(int i=0;i<div_arr.size();i++){
            division temp = div_arr[i];
            int max_ind = -1;
            double max_dist = 0;
            for(int i=0;i<len;i++){
                int side;
                int dist = get_dist(temp.a, temp.b, arr[i]);
                if(dist>0) side = 1;
                else if(dist<0) side = -1;
                else side = 0;
                if(side == temp.side && absolute(dist)>max_dist){
                    max_dist = absolute(dist);
                    max_ind = i;
                }
            }
            if(max_ind!=-1){
                division temp1 = {arr[max_ind], temp.a, -1*get_side(arr[max_ind], temp.a, temp.b)};
                division temp2 = {arr[max_ind], temp.b, -1*get_side(arr[max_ind], temp.b, temp.a)};
                #pragma omp critical
                {
                new_wor.push_back(temp1);
                new_wor.push_back(temp2);
                ans.push_back(arr[max_ind]);
                }
            }
        }
        div_arr = new_wor;
        new_wor.clear();
    }
    return ans;
}
