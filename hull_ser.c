#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

typedef struct {
    int x;
    int y;
} point;

typedef struct {
    point a;
    point b;
    int side;
} division;

double absolute(double a){
    if(a>0) return a;
    return -1*a;
}

double get_dist(point a, point b, point c){
    return absolute((c.y - a.y)*(b.x - a.x) - (b.y - a.y)*(c.x - a.x)); 
}

double get_side(point a, point b, point c){
    int val = (c.y - a.y)*(b.x - a.x) - (b.y - a.y)*(c.x - a.x);
    if(val>0) return 1;
    else if(val<0) return -1;
    return 0;
}

int compare_val(point a, point b){
    if(a.x==b.x && a.y==b.y) return 1;
    return 0;
}

int main(){
    division div_arr[1000];
    point ans[1000];
    int curr = 0;
    int ans_curr = 0;

    point arr[1000];
    int arr_size = 0;

    int h, w;

    char str[999];
    FILE * file;
    file = fopen( "test.txt" , "r");
    fscanf(file, "%s", str);
    w = atoi(str);
    fscanf(file, "%s", str);
    h = atoi(str);
    
    int image[h][w];
    for(int i=0;i<h;i++){
        for(int j=0;j<w;j++){
            fscanf(file, "%s", str);
            image[h-i-1][j] = atoi(str);    
        }
    }

    int max_x =-1;
    int min_x =w;
    int max_x_y;
    int min_x_y;

    for(int i=0;i<h;i++){
        for(int j=0;j<w;j++){
            if(image[h-i-1][j]==1){
                point new = {j, h-i-1};
                arr[arr_size] = new;
                arr_size++;
                if(j>max_x){
                    max_x = j;
                    max_x_y = h-i-1;
                }
                if(j<min_x){
                    min_x = j;
                    min_x_y = h-i-1;
                }
            }
        }
    }

    point left = {min_x, min_x_y};
    point right = {max_x, max_x_y};

    division up = {left, right, 1};
    division down = {left, right, -1};

    div_arr[curr] = up;
    curr++;
    div_arr[curr] = down;
    curr++;

    while(curr!=0){
        division temp = div_arr[curr-1];
        curr--;
        int max_ind = -1;
        double max_dist = 0;
        for(int i=0;i<arr_size;i++){
            int dist = get_dist(temp.a, temp.b, arr[i]);
            if((get_side(temp.a, temp.b, arr[i]) == temp.side) && (dist>max_dist)){
                max_dist = dist;
                max_ind = i;
            }
        }
        if(max_ind==-1){
            ans[ans_curr] = temp.a;
            ans_curr++;
            ans[ans_curr] = temp.b;
            ans_curr++;
        }
        else{
            division temp1 = {arr[max_ind], temp.a, -1*get_side(arr[max_ind], temp.a, temp.b)};
            division temp2 = {arr[max_ind], temp.b, -1*get_side(arr[max_ind], temp.b, temp.a)};
            div_arr[curr] = temp1;
            curr++;
            div_arr[curr] = temp2;
            curr++;
        }
    }

    point redundant = {-1, -1};

    for(int i=0;i<ans_curr;i++){
        for(int j=i+1;j<ans_curr;j++){
            if(compare_val(ans[i], ans[j])){
                ans[i] = redundant;
                break;
            }
        }
    }

    for(int i=0;i<ans_curr;i++){
        if(ans[i].x!=-1){
            printf("%d %d\n", ans[i].x, ans[i].y);
        }
    }

}