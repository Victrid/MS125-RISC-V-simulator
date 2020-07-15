#ifndef __PREDICTOR_HPP__
#define __PREDICTOR_HPP__
#include "globaldef.hpp"
#include <map>
#include <queue>
#include <unordered_map>

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

class Adapttrain {
public:
    unsigned char HR = 0;
    sature PT[0b11111111];
    bool query() {
        return PT[HR].query();
    }
    void validate(bool ans) {
        if (ans)
            PT[HR].increase();
        else
            PT[HR].decrease();
        HR = (HR << 1) | (ans & 1);
    }
};

class Predictor {
    std::unordered_map<taddr, Adapttrain> strmap;
    std::queue<bool> predictbool;

public:
    int hit = 0;
    int tot = 0;
    bool query(taddr address) {
        //True means jump
        if (strmap.find(address) == strmap.end())
            strmap.insert(std::pair<taddr, Adapttrain>(address, Adapttrain()));
        bool b = strmap[address].query();
        predictbool.push(b);
        return b;
    };
    bool validate(taddr address, bool jumped) {
        tot++;
        strmap[address].validate(jumped);
        if (predictbool.front() ^ jumped) {
            //fail to predict.
            while (!predictbool.empty())
                predictbool.pop();
            return false;
        } else {
            hit++;
            predictbool.pop();
            return true;
        }
    }
    void clear() {
        while (!predictbool.empty())
            predictbool.pop();
    }
};
#endif