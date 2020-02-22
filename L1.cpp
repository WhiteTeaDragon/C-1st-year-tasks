#include<iostream>
#include<map>
#include<algorithm>
#include<vector>

using namespace std;

template<typename T>
class Polynomial {
private:
    map<int, T> coeffs;

public:
    int Degree() const {
        if (coeffs.size() == 0) {
            return -1;
        }
        return (*coeffs.rbegin()).first;
    }

    Polynomial(const vector<T>& c) {
        for (size_t i = 0; i != c.size(); ++i) {
            if (c[i] != static_cast<T>(0)) {
                coeffs[i] = c[i];
            }
        }
    }

    Polynomial(const T& x = T()) {
        if (x != static_cast<T>(0)) {
            coeffs[0] = x;
        }
    }

    template<typename Iter>
    Polynomial(Iter first, Iter last) {
        int ind = 0;
        for (auto it = first; it != last; ++it) {
            if (*it != static_cast<T>(0)) {
                coeffs[ind] = *it;
            }
            ++ind;
        }
    }

    Polynomial(const T& x, const int& power) {
        coeffs[power] = x;
    }

    T operator[] (int ind) const {
        if (coeffs.find(ind) != coeffs.end()) {
            return coeffs.at(ind);
        }
        return static_cast<T>(0);
    }

    void NormalizePol() {
        int deg = (*this).Degree();
        if (deg == -1) {
            return;
        }
        T to_div = coeffs[deg];
        for (auto& elem : coeffs) {
            elem.second /= to_div;
        }
    }

    Polynomial<T> operator-() const {
        Polynomial<T> ans = (*this);
        T m_one = static_cast<T>(-1);
        for (auto& item : ans.coeffs) {
            item.second = m_one * item.second;
        }
        return ans;
    }

    T operator() (const T& x) const {
        int power = 0;
        T ans = static_cast<T>(0), pow_x = static_cast<T>(1);
        for (const auto& elem : coeffs) {
            while (power != elem.first) {
                pow_x *= x;
                ++power;
            }
            ans += elem.second * pow_x;
        }
        return ans;
    }

    typename map<int, T>::const_iterator begin() const {
        return coeffs.cbegin();
    }

    typename map<int, T>::const_iterator end() const {
        return coeffs.cend();
    }

    typename map<int, T>::const_reverse_iterator rbegin() const {
        return coeffs.crbegin();
    }

    typename map<int, T>::const_reverse_iterator rend() const {
        return coeffs.crend();
    }

    friend bool operator == (const Polynomial<T>& first, const Polynomial<T>& second) {
        return first.coeffs == second.coeffs;
    }

    friend bool operator != (const Polynomial<T>& first, const Polynomial<T>& second) {
        return !(first == second);
    }

    friend Polynomial<T> operator + (const Polynomial<T>& first, const Polynomial<T>& second) {
        int f_size = first.Degree();
        int s_size = second.Degree();
        if (f_size == -1) {
            return second;
        }
        if (s_size == -1) {
            return first;
        }
        int size = max(f_size, s_size) + 1;
        vector<T> ans(size);
        int i = 0, j = 0, k = 0;
        while (i <= f_size && j <= s_size) {
            ans[k] = first[i] + second[j];
            ++i;
            ++j;
            ++k;
        }
        while (i <= f_size) {
            ans[k] = first[i];
            ++i;
            ++k;
        }
        while (j <= s_size) {
            ans[k] = second[j];
            ++j;
            ++k;
        }
        return Polynomial<T>(ans);
    }

    friend Polynomial<T> operator - (const Polynomial<T>& first, const Polynomial<T>& second) {
        return first + (-second);
    }

    friend Polynomial<T> operator * (const Polynomial<T>& first, const Polynomial<T>& second) {
        int f_size = first.Degree(), s_size = second.Degree();
        if (f_size == -1 || s_size == -1) {
            return Polynomial<T>();
        }
        vector<T> ans(f_size + s_size + 1);
        int i, j;
        for (const auto& elem1 : first.coeffs) {
            i = elem1.first;
            for (const auto& elem2 : second.coeffs) {
                j = elem2.first;
                ans[i + j] += elem1.second * elem2.second;
            }
        }
        return Polynomial<T>(ans);
    }

    Polynomial<T>& operator += (const Polynomial<T>& second) {
        *this = *this + second;
        return *this;
    }

    Polynomial<T>& operator -= (const Polynomial<T>& second) {
        *this = *this - second;
        return *this;
    }

    Polynomial<T>& operator *= (const Polynomial<T>& second) {
        *this = *this * second;
        return *this;
    }

    friend Polynomial<T> operator & (const Polynomial<T>& first, const Polynomial<T>& second) {
        int f_size = first.Degree(), power = 0;
        if (f_size == -1) {
            return first;
        }
        Polynomial<T> ans, pow_s(static_cast<T>(1));
        for (const auto& elem : first) {
            while (power != elem.first) {
                pow_s *= second;
                ++power;
            }
            ans += elem.second * pow_s;
        }
        return ans;
    }

    friend Polynomial<T> operator / (const Polynomial<T>& first, const Polynomial<T>& second) {
        int f_size = first.Degree(), s_size = second.Degree();
        int f_i = f_size;
        vector<T> ans(max(0, f_size - s_size + 1));
        Polynomial<T> first_copy = first;
        for (int i = f_size - s_size; i >= 0; --i) {
            ans[i] = first_copy[f_i] / second[s_size];
            Polynomial<T> curr(ans[i], i);
            first_copy -= curr * second;
            --f_i;
        }
        return Polynomial<T>(ans);
    }

    friend Polynomial<T> operator % (const Polynomial<T>& first, const Polynomial<T>& second) {
        return first - second * (first / second);
    }

    friend Polynomial<T> operator , (const Polynomial<T>& first, const Polynomial<T>& second) {
        Polynomial<T> first_copy = first, second_copy = second;
        while (first_copy.Degree() != -1) {
            Polynomial<T> tmp = first_copy;
            first_copy = second_copy % first_copy;
            second_copy = tmp;
        }
        second_copy.NormalizePol();
        return second_copy;
    }
};

template<typename T>
ostream& operator<< (ostream& out, const Polynomial<T>& c) {
    int deg = c.Degree(), i;
    if (deg == -1) {
        out << 0;
        return out;
    }
    T zero = static_cast<T>(0), one = static_cast<T>(1), m_one = static_cast<T>(-1), curr;
    for (auto it = c.rbegin(); it != c.rend(); ++it) {
        curr = (*it).second;
        i = (*it).first;
        if (curr > zero && i != deg) {
            out << '+';
        } else if (curr < zero && curr == m_one && i != 0) {
            out << '-';
        }
        if (i == 0 || (curr != one && curr != m_one)) {
            out << curr;
        }
        if (i > 0) {
            if (curr != one && curr != m_one) {
                out << '*';
            }
            out << 'x';
            if (i > 1) {
                out << '^' << i;
            }
        }
    }
    return out;
}

int main() {
    Polynomial<int> a({ 1, 2, 3, 3, 0 });
    cout << a[0] << " " << a[1] << " " << a[2] << " " << a[3] << '\n';
    Polynomial<int> b(-1);
    cout << b[0] << '\n';
    vector<int> cc = { 1, 2, 7 };
    Polynomial<int> c(cc.begin(), cc.end());
    cout << c[0] << " " << c[1] << " " << c[2] << '\n';
    Polynomial<int> d = a + b, e = a - c;
    cout << d[0] << " " << d[1] << " " << d[2] << " " << d[3] << '\n';
    cout << e[0] << " " << e[1] << " " << e[2] << " " << e[3] << '\n';
    a += b;
    cout << a[0] << " " << a[1] << " " << a[2] << " " << a[3] << '\n';
    a -= b;
    cout << a[0] << " " << a[1] << " " << a[2] << " " << a[3] << '\n';
    cout << boolalpha << (a + b == d) << " " << (a == a) << " " << (a != b) << " " << (a != a) << " " << (a == b) << '\n';
    cout << a.Degree() << "\n";
    for (auto& elem : a) {
        cout << elem.first << " " << elem.second << '\n';
    }
    Polynomial<int> i({ 1, 2, 3 }), f({ 7, 51, 0, -6 }), h({ -6, 0, 51, 7 });
    Polynomial<int> g = i * f;
    cout << "G :\n";
    for (auto& elem : g) {
        cout << elem.first << " " << elem.second << '\n';
    }
    h *= i;
    cout << "H :\n";
    for (auto& elem : h) {
        cout << elem.first << " " << elem.second << '\n';
    }
    cout << i(0) << " " << i(1) << " " << i(-1) << '\n';
    cout << a << '\n';
    cout << b << '\n';
    cout << c << '\n';
    cout << d << '\n';
    cout << e << '\n';
    cout << f << '\n';
    cout << g << '\n';
    cout << h << '\n';
    cout << i << '\n';
    cout << "----\n";
    cout << (a & b) << '\n';
    cout << (b & a) << '\n';
    Polynomial<int> j(0);
    cout << (a & j) << '\n';
    cout << (j & a) << '\n';
    cout << (b & j) << '\n';
    cout << (j & b) << '\n';
    cout << "----\n";
    Polynomial<int> k(vector<int>{7, 1}), m({ 1, 5, 1 });
    cout << (k & m) << '\n';
    cout << (m & k) << '\n';
    cout << "----\n";
    Polynomial<int> n(vector<int>{-1, 1}), p({ 1, -2, 1 }), q(vector<int>{ -8, 1 }), r(1);
    cout << (p / n) << '\n';
    cout << (p % n) << '\n';
    cout << (p, n) << '\n';
    cout << "----\n";
    cout << (q / r) << '\n';
    cout << (q % r) << '\n';
    cout << (q, r) << '\n';
    return 0;
}