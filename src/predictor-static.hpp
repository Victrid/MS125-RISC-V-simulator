#ifndef __PREDICTOR_HPP__
#define __PREDICTOR_HPP__
#include "globaldef.hpp"
#include <map>
#include <queue>
#include <unordered_map>

using namespace std;

class Predictor {

public:
int tot=0;
int hit=0;
    bool query(taddr address) {
        return true;
    };
    bool validate(taddr address, bool jumped) {
        tot++;
        if(jumped)hit++;
        return jumped;
    }
    void clear() {
        return;
    }
};
#endif