#include<iostream>

using namespace std;

int main(int argc, char **argv) {
  int *array = new int[100];
  delete [] array;
  array[argc]==1;  //can't detected
  cout<< "passed 1st"<<endl;

  array[argc] = array[argc];
  cout<< "passed 2nd"<<endl;

  array[argc]=100;  // BOOM
  cout<<"passed 3rd"<<endl;
}
