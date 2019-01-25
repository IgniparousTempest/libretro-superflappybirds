#ifndef LR_SUPERFLAPPYBIRDS_AUXILLARY_H
#define LR_SUPERFLAPPYBIRDS_AUXILLARY_H

#include <stack>

class Auxillary {
public:
    static std::stack<int> digits(int number) {
        std::stack<int> sd;
        while (number > 0) {
            int digit = number % 10;
            number /= 10;
            sd.push(digit);
        }
        if (sd.empty())
            sd.push(0);

        return sd;
    }
};

#endif //LR_SUPERFLAPPYBIRDS_AUXILLARY_H
