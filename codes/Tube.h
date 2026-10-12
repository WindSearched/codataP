//
// Created by ws on 2026/10/12.
//

#ifndef CODATAP_TUBE_H
#define CODATAP_TUBE_H
#include <vector>

template<typename T>
class Tube {
private:
    std::vector<T> tube;
    int count = 0;
    int cast(int i) {
        return i % tube.size();
    }
public:
    int pointer = -1;//always last putted variable index

    Tube(){}
    Tube(int length) : tube(length) {
    }

    bool IsFull() {
        return count >= tube.size();
    }
    int Head() {
        if (IsFull()) {
            return cast(pointer + 1);
        }
        else {
            return 0;
        }
    }
    T Get(int index) {
        int p = cast(Head() + index);
        return tube[p];
    }
    T First() {
        return Get(0);
    }
    T Last() {
        return Get(tube.size() - 1);
    }
    void Put(T value) {
        if (!IsFull())
            count++;
        pointer = cast(pointer + 1);
        tube[pointer] = value;
    }

    void New(int size) {
        tube.resize(size);
        count = 0;
        pointer = -1;
    }
};


#endif //CODATAP_TUBE_H
