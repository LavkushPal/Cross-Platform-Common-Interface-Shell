#include <iostream>
using namespace std;

void waveForm(int a[][4],int row,int col){

  for(int j=0;j<col;j++){
    
    if(j%2==0)
    {
      for(int i=0;i<row;i++)
        cout<<" "<<a[i][j];
    }
    else{
      for(int i=row-1;i>=0;i--){
        cout<<" "<<a[i][j];
      }
    }
    cout<<endl;

  }

}


int main(){

  int row=4;
  int col=4;
  int arr[4][4]={1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16};

  waveForm(arr,row,col);
    return 0;
}