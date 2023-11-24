#include<iostream>
using namespace std;

unsigned long hashSdbm(string str){
        unsigned long hash = 0;
        int i=0;
        while (i<str.length())
            hash = (unsigned long)str[i++] + (hash << 6) + (hash << 16) - hash;
        return hash;
}
int main(){
    cout<<hashSdbm("FOO")%7;
}