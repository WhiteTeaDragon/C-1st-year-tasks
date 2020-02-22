#include <cstdlib>
#include <iostream>
#include <utility>

using namespace std;

template<typename T>
class Vector {
private:
    T* arr;
    size_t sz, cap;

public:
    Vector() {
        arr = nullptr;
        sz = 0;
        cap = 0;
    }

    Vector(size_t n) {
        arr = static_cast<T*>(malloc(n * sizeof(T)));
        for (size_t i = 0; i != n; ++i) {
            *(arr + i) = T();
        }
        sz = n;
        cap = n;
    }

    Vector(const Vector<T>& other) {
        arr = static_cast<T*>(malloc(other.sz * sizeof(T)));
        for (size_t i = 0; i != other.sz; ++i) {
            *(arr + i) = *(other.arr + i);
        }
        sz = other.sz;
        cap = sz;
    }

    Vector<T>& operator= (const Vector<T>& other) {
        Vector<T> tmp(other);
        (*this).swap(tmp);
        return *this;
    }

    void reserve(size_t new_cap) {
        if (new_cap <= cap) {
            return;
        }
        T* tmp = nullptr;
        if (tmp = static_cast<T*>(realloc(arr, new_cap * sizeof(T)))) {
            arr = tmp;
            cap = new_cap;
        }
    }

    void resize(size_t new_size) {
        if (new_size <= sz) {
            sz = new_size;
        } else {
            T* tmp = nullptr;
            if (new_size > cap) {
                if (tmp = static_cast<T*>(realloc(arr, new_size * sizeof(T)))) {
                    arr = tmp;
                }
            }
            if (new_size <= cap || tmp) {
                for (size_t i = sz; i != new_size; ++i) {
                    *(arr + i) = T();
                }
                sz = new_size;
                cap = max(cap, new_size);
            }
        }
    }

    void clear() {
        sz = 0;
    }

    void swap(const Vector<T>& other) noexcept {
        swap(sz, other.sz);
        swap(cap, other.cap);
        T* tmp = arr;
        arr = other.arr;
        other.arr = tmp;
    }

    void push_back(const T& elem) {
        T* tmp = nullptr;
        if (cap == 0) {
            if (tmp = static_cast<T*>(malloc(sizeof(T)))) {
                arr = tmp;
                cap = 1;
            }
        } else if (sz + 1 > cap) {
            if (tmp = static_cast<T*>(realloc(arr, cap * 2 * sizeof(T)))) {
                arr = tmp;
                cap *= 2;
            }
        }
        if (sz + 1 <= cap || tmp) {
            *(arr + sz) = elem;
            ++sz;
        }
    }

    void push_back(T&& elem) {
        T* tmp = nullptr;
        if (cap == 0) {
            if (tmp = static_cast<T*>(malloc(sizeof(T)))) {
                arr = tmp;
                cap = 1;
            }
        } else if (sz + 1 > cap) {
            if (tmp = static_cast<T*>(realloc(arr, cap * 2 * sizeof(T)))) {
                arr = tmp;
                cap *= 2;
            }
        }
        if (sz + 1 <= cap || tmp) {
            *(arr + sz) = move(elem);
            ++sz;
        }
    }

    void pop_back() {
        if (sz != 0) {
            --sz;
        }
    }

    T* begin() {
        return arr;
    }

    const T* begin() const {
        return arr;
    }

    T* end() {
        return arr + sz;
    }

    const T* end() const {
        return arr + sz;
    }

    size_t size() const noexcept {
        return sz;
    }

    size_t capacity() const noexcept {
        return cap;
    }

    T& operator[] (size_t pos) {
        return *(arr + pos);
    }

    const T& operator[] (size_t pos) const {
        return *(arr + pos);
    }

    ~Vector() {
        free(arr);
        arr = nullptr;
    }
};

int main() {
    return 0;
}