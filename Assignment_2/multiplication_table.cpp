#include <iostream>

using namespace std;

int multiply(int a, int b) {
    int product = 0;
    for (int i = 0; i < 3; ++i) 
        if ((b >> i) & 1) 
            product ^= (a << i);
    if (product & 16) 
        product ^= (11 << 1);
        
    if (product & 8) {
        product ^= 11;  
    }
    return product;
}

int main() {
    cout<<"  x | 0  1  2  3  4  5  6  7\n";
    cout<<"----+------------------------\n";
    for (int i = 0; i < 8; ++i) {
        cout<<"  "<<i<<" | ";
        for (int j = 0; j < 8; ++j) {
            cout<<" "<<multiply(i, j)<<" ";
        }
        cout<<endl;
    }
    return 0;
}