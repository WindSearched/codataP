#pragma once
#include <iostream>
#include <vector>
#include <functional>
#include <algorithm>
#include <utility>

// =====================================================
// 1. 普通模板：处理有返回值的事件
// =====================================================
template<typename ReturnType, typename... Args>
class Event {

public:
    using Handler = std::function<ReturnType(Args...)>;

    struct Listener {
        int id;
        Handler handler;
    };

    int AddListener(Handler handler) {
        int id = nextId++;
        listeners.push_back({id, std::move(handler)});
        return id;
    }

    void RemoveListener(int id) {
        listeners.erase(
            std::remove_if(
                listeners.begin(),
                listeners.end(),
                [id](const Listener& listener) {
                    return listener.id == id;
                }
            ),
            listeners.end()
        );
    }

    // 有返回值：返回最后一个有效监听者的结果
    ReturnType Invoke(Args... args) const {
        ReturnType finalResult{};

        for (const auto& listener : listeners) {
            if (listener.handler) {
                finalResult = listener.handler(args...);
            }
        }

        return finalResult;
    }

private:
    std::vector<Listener> listeners;
    inline static int nextId = 1;
};

// =====================================================
// 2. void 偏特化：处理无返回值的事件
// =====================================================
template<typename... Args>
class Event<void, Args...> {
public:
    using Handler = std::function<void(Args...)>;

    struct Listener {
        int id;
        Handler handler;
    };

    int AddListener(Handler handler) {
        int id = nextId++;
        listeners.push_back({id, std::move(handler)});
        return id;
    }

    void RemoveListener(int id) {
        listeners.erase(
            std::remove_if(
                listeners.begin(),
                listeners.end(),
                [id](const Listener& listener) {
                    return listener.id == id;
                }
            ),
            listeners.end()
        );
    }

    // 无返回值：依次调用所有监听者
    void Invoke(Args... args) const {
        for (const auto& listener : listeners) {
            if (listener.handler) {
                listener.handler(args...);
            }
        }
    }

private:
    std::vector<Listener> listeners;
    inline static int nextId = 1;
};

// =====================================================
// 3. 测试代码
// =====================================================

