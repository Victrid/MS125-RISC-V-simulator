#include "core.hpp"
#include <iostream>

using namespace std;

int main(int argc, char** argv) {
    switch (argc) {
    case 1: {
        core_session C(cin, cin);
        cout << C.run();
        break;
    }
    case 2: {
        fstream f;
        f.open(argv[1]);
        if (!f)
            throw exception();
        core_session C(f, cin);
        cout << C.run();
    } break;
    case 3: {
        fstream f1, f2;
        f1.open(argv[1]);
        f2.open(argv[2], ios::out);
        if ((!f1) || (!f2))
            throw exception();
        core_session C(f1, f2);
        cout << C.run();
    } break;
    default:
        throw invalid_argument("Wrong argument!");
    }
    return 0;
}