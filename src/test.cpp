#include "globaldef.hpp"
#include "parser.hpp"
#include <iostream>
using namespace std;
int main(int argc, char* argv[]) {
    Parser p(argv[1]);
    p.getline();
    cout << endl;
    p.getline();
}