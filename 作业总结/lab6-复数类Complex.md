# lab6 — 复数类 Complex（运算符重载）

[← 返回目录](../作业总结.md)

## 📋 任务总结

实现复数类 `Complex`（`double real, img`），支持构造函数默认参数、`operator+=`（成员）、`operator+`（支持混合类型）、`operator<<`/`>>`（友元）。

## 💡 关键算法思想

### 1. 成员 vs 非成员运算符的选择

`operator+=` 用成员（修改自身），`operator+` 用非成员（需要左右对称转换）。关键原则：如果二元运算符需要对两个操作数都做隐式类型转换，必须用非成员函数——`3 + c` 需要把 `3` 转为 `Complex(3,0)`。

```cpp
// Complex.h
class Complex {
private:
    double real, img;
public:
    Complex(double r = 0.0, double i = 0.0) : real(r), img(i) {}

    Complex operator+(const Complex & c);         // 成员：Complex + Complex
    Complex operator+(int a);                     // 成员：Complex + int
    Complex & operator+=(const Complex & c);      // 成员：修改自身

    friend Complex operator+(int a, const Complex & c);  // 友元：int + Complex
    friend ostream & operator<<(ostream & o, const Complex & c);
    friend istream & operator>>(istream & i, Complex & c);
};
```

```cpp
// Complex.cpp — 三种 operator+ 各司其职
Complex Complex::operator+(const Complex & c) {
    return Complex(real + c.real, img + c.img);
}

Complex Complex::operator+(int a) {
    return Complex(real + static_cast<double>(a), img);
}

// 友元：int 在左边时，成员函数无法处理，必须用非成员
Complex operator+(int a, const Complex & c) {
    return Complex(c.real + static_cast<double>(a), c.img);
}
```

### 2. `a+bi` 字符串解析的状态机

`operator>>` 需要逐字符解析：跳过空白 → 读实部 → 识别 `+`/`-` → 读虚部 → 确认 `i` 结尾。

```cpp
// 解析 "3.5+2.1i" 格式的辅助函数
static vector<double> getc(string s) {
    double d = 0.0;
    vector<double> D;
    string word = "";
    for (size_t i = 0; i < s.length(); ++i) {
        if (s[i] == '+' || s[i] == 'i' || s[i] == 'I') {
            d = stod(word);
            D.push_back(d);
            d = 0.0;
            word.clear();
        } else if (s[i] == ' ') {
            continue;                         // 跳过空格
        } else {
            word.push_back(s[i]);             // 累积数字字符
        }
    }
    return D;
}

istream & operator>>(istream & i, Complex & c) {
    string w = "";
    getline(i, w);
    vector<double> D = getc(w);
    if (D.size() != 2) {
        cout << "无效输入" << endl;
        return i;
    }
    c.real = D[0];
    c.img = D[1];
    return i;
}
```

### 3. `operator<<` 的条件格式化

```cpp
ostream & operator<<(ostream & o, const Complex & c) {
    if (c.img > 0) {
        o << c.real << "+" << c.img << "i";    // 3+2i
    } else if (c.img == 0) {
        o << c.real;                           // 3（纯实数）
    } else {
        o << c.real << c.img << "i";           // 3-2i（负号自带）
    }
    return o;
}
```

## 🔗 知识点分析

| 知识点 | 教程章节 |
|--------|---------|
| 操作符重载基本概念 | [二、基本概念](../教程/07-操作符重载-实现String类（完整教学版）.md#二基本概念) |
| 加法运算符 `+` 和 `+=` | [五、加法运算符 + 和 +=](../教程/07-操作符重载-实现String类（完整教学版）.md#五加法运算符--和-) |
| 输入运算符与友元函数 | [四、输入运算符 operator\>\> 与友元](../教程/07-操作符重载-实现String类（完整教学版）.md#四输入运算符-operator-与友元) |
| 构造函数默认参数 | [三、构造函数](../教程/05-定义新类型-类（完整教学版）.md#三构造函数) |
| 二元运算符的对称性设计 | [二、基本概念](../教程/07-操作符重载-实现String类（完整教学版）.md#二基本概念) |

📂 **文件：** `Complex.cpp/h`、`main.cpp`

[← 返回目录](../作业总结.md)
