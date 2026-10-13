//
// Created by ws on 2026/10/13.
//

#ifndef CODATAP_SEVENT_H
#define CODATAP_SEVENT_H
#include <functional>
#include <vector>

template <typename... Args>
class SEvent {
    public:
    std::vector<std::function<void(Args...)>> listeners;

    int Add(std::function<void(Args...)> function) {
        listeners.push_back(function);
        return next_id ++;
    }

    void Invoke(Args... args) {
        for (const auto& listener : listeners) {
            if (listener) {
                listener(args...);
            }
        }
    }
private:
    int next_id = 0;
};


#endif //CODATAP_SEVENT_H
