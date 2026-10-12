#include <vector>
#include <functional>
#include <algorithm>

// ================= 1. 基础模板：处理【有返回值】的事件 =================
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
        listeners.push_back({id, handler});
        return id;
    }

    void RemoveListener(int id) {
        listeners.erase(
            std::remove_if(listeners.begin(), listeners.end(),
                [id](const Listener& l) { return l.id == id; }),
            listeners.end()
        );
    }

    // 有返回值时：Invoke 返回 ReturnType (采用 C# 默认规则：返回最后一个订阅者的值)
    ReturnType Invoke(Args... args) const {
        if (listeners.empty()) return ReturnType(); // 没人订阅则返回默认值

        ReturnType finalResult;
        for (const auto& listener : listeners) {
            if (listener.handler) {
                finalResult = listener.handler(args...);
            }
        }
        return finalResult;
    }

private:
    std::vector<Listener> listeners;
    inline static int nextId = 0;
};

// ================= 2. 偏特化模板：处理【无返回值 (void)】的事件 =================
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
        listeners.push_back({id, handler});
        return id;
    }

    void RemoveListener(int id) {
        listeners.erase(
            std::remove_if(listeners.begin(), listeners.end(),
                [id](const Listener& l) { return l.id == id; }),
            listeners.end()
        );
    }

    // 无返回值时：Invoke 返回 void
    void Invoke(Args... args) const {
        for (const auto& listener : listeners) {
            if (listener.handler) {
                listener.handler(args...);
            }
        }
    }

private:
    std::vector<Listener> listeners;
    inline static int nextId = 0;
};
