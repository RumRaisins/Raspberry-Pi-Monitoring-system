#include <iostream>
using namespace std;


int main(){
    int *a = new int[125000000];

    for(int i = 0 ; i < 125000000 ; ++i){
        a[i] = 1;
    }
    cin >> a[123];
    return 0;
}
