#include <string.h>

class A{
public:
    char c;
    char d;
};

class B : public A{
public:
    char str[5];
};

int main(int argc, char** argv){
    B b;
    b.c= 'A';
    b.d= 'B';
    strcpy(b.str, "1234");
    // We need to set a break point at this line to see the memory layout.
    return 0;
}
