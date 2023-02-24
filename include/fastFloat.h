#ifndef FAST_FLOAT_CLASS_H
#define FAST_FLOAT_CLASS_H

class fastFloat {
public:
    static int fastFloor(float x) {
        if (x < 0 && int(x) != x)
            return int(x - 1);
        else
            return int(x);
    }

    static int fastFloor(double x) {
        if (x < 0 && int(x) != x)
            return int(x - 1);
        else
            return int(x);
    }

    static int fastCeil(float x) {
        if (x > 0 && int(x) != x)
            return int(x + 1);
        else
            return int(x);
    }

    static int fastCeil(double x) {
        if (x > 0 && int(x) != x)
            return int(x + 1);
        else
            return int(x);
    }
};

#endif