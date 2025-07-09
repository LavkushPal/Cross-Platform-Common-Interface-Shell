#include <iostream>
using namespace std;

void reverse_row(int a[][4],int r,int c){

    //first approach
    for(int i=0;i<r;i++){
        for(int j=0;j<c/2;j++)
            swap(a[i][j],a[i][c-j-1]);
    }

    int start,end;

    //second approach
    for(int i=0;i<r;i++){
        start=0,end=c-1;
        while(start<end)
        {
            swap(a[i][start],a[i][end]);
            start++;
            end--;
        }
            
    }
}

void sum_of_diagonal(int a[][4],int r,int c){

    int sum=0;
    for(int i=0;i<r;i++)
        {
            sum+=a[i][i];
            sum+=a[i][c-i-1];
        }
        cout<<"sum of diagonal"<<sum;
}

void row_max_sum(int a[][4],int r,int c){
    int max1=INT_MIN,index=0,sum=0;

    for(int row=0;row<r;row++){
        sum=0;
        for(int col=0;col<c;col++){
            sum+=a[row][col];
        }
        max1=max(max1,sum);
        index=row;
    }

    cout<<"max sum is "<<max1<<" and row is "<<index;

}

void pass_2d(int a[][4]){
    //you have to mention column in function see above
    // this need for calculating the actual address 
   // cout<<a[2][3];
    
    
    }


int main(){

    int r,c;
    int a1[3][4]={1,2,3,4,5,6,7,8,9,10,11,12};
    int a2[3][4]={1,2,3,4,5,6,7,8,9,10,11,12};
  
    int a3[3][4];
    int a4[4][4];

    //atctually 2d stored in a 1d array manner so if you want to find actual index number of a[2][3]
    // then [ r_index*c+c_index ]  2*4+3
    //otherwise if given actual index then you wanna find visual 2d row col 
    //then row_index=index/c      and col_index=index%c
    //and if you wanna find actual address of like a[2][3] then follow the ds note by bala krishna sir approach

    pass_2d(a1);

    //2d addition

    for(int i=0;i<r;i++){
        for(int j=0;j<c;j++){
            a3[i][j]=a1[i][j]+a2[i][j];
        }
    }

    //call function to print max in a row
    row_max_sum(a1,3,4);

    //print sum of diagonal in a matrix
    sum_of_diagonal(a4,4,4);

    //reverse each row of matrix
    reverse_row(a4,4,4);

    return 0;
}