#pragma once

template <class T>
class Singleton {
public:
    static T& getInstance(void) {
        static T instance;
        return instance;
    };
    void operator=(const Singleton<T>&) = delete;
    Singleton(const Singleton<T>&) = delete;

private:
    Singleton(void){};
};
