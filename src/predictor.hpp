#ifndef __PREDICTOR_HPP__
#define __PREDICTOR_HPP__
#include "globaldef.hpp"
#include <map>
#include <queue>
#include <unordered_map>

using namespace std;

class sature {
private:
    bool b0;
    bool b1;

public:
    bool query() { return b0; };
    void increase() {
        if (b0) {
            if (!b1) {
                b1 = true;
            }
        } else {
            if (b1) {
                b0 = true;
                b1 = false;
            } else {
                b1 = true;
            }
        }
    }
    void decrease() {
        if (b0) {
            if (b1) {
                b1 = false;
            } else {
                b0 = false;
                b1 = true;
            }
        } else if (b1)
            b1 = false;
    }
    sature() : b0(true), b1(false){};
};
class Predictor {
    unordered_map<taddr, sature> strmap;
    queue<bool> predictbool;

public:
//DEBUG
    int tot = 0;
    int cor = 0;
    bool query(taddr address) {
        //True means jump
        if (strmap.find(address) == strmap.end())
            strmap.insert(pair<taddr, sature>(address, sature()));
        bool b = strmap[address].query();
        predictbool.push(b);
        tot++;
        return b;
    };
    bool validate(taddr address, bool jumped) {
        if (predictbool.front() ^ jumped) {
            //fail to predict.
            strmap[address].decrease();
            while (!predictbool.empty())
                predictbool.pop();
            return false;
        } else {
            strmap[address].increase();
            predictbool.pop();
            cor++;
            return true;
        }
    }
};
#endif