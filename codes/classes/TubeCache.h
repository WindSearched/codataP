//
// Created by ws on 2026/10/23.
//

#ifndef CODATAP_TUBECACHE_H
#define CODATAP_TUBECACHE_H

#include "../Tube.h"

template <typename T>
class TubeCache {
public:
    Tube<T> tube;
    T front;
    int pointer = 0;

    T MoveFront();
    T MoveBack();
    T MoveTo(int index);
    void Revert();

    void New(int length);
    void Put(T item);
    void Put(std::vector<T> items);
    void PutFront(T item);
};

// ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
// ─── 成员函数具体实现（原 cpp 内容，必须在头文件中对所有包含该头文件的源文件可见） ───
// ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

template<typename T>
T TubeCache<T>::MoveTo(int index) {
    if (index == 0) {
        pointer = 0;
        return front;
    }
    else {
        pointer = index;
        return tube.Get(index -1);
    }
}

template<typename T>
T TubeCache<T>::MoveFront() {
    if (pointer == 0) {
        return front;
    }
    else {
        return MoveTo(--pointer);
    }
}

template<typename T>
T TubeCache<T>::MoveBack() {
    if (pointer == tube.size() - 1) {
        return tube.Last();
    }
    else {
        auto v = tube.Get(pointer);// pointer -1 +1
        if (v != T{}) {
            pointer++;
            return v;
        }
        else {
            return T{};
        }
    }
}

template<typename T>
void TubeCache<T>::Revert() {
    pointer = 0;
    front = T{};
}

template<typename T>
void TubeCache<T>::New(int length) {
    tube.New(length);
}

template<typename T>
void TubeCache<T>::Put(T item) {
    tube.Put(item);
}

template<typename T>
void TubeCache<T>::Put(std::vector<T> items) {
    for (auto item : items) {
        if (item == T{}) continue;
        tube.Put(item);
    }
}

template<typename T>
void TubeCache<T>::PutFront(T item) {
    front = item;
}

#endif //CODATAP_TUBECACHE_H
