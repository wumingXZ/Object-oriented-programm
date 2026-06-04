# 第十三周：操作符重载 — 实现 String 类

> 基于课件 `06-操作符重载.md`，参考 C++ Primer 第五版（英文版）第 14 章、第 7 章补充优化。
>
> 本文中所有代码块均支持 **Markdown Preview Enhanced** 的 `{cmd=true}` 功能，可以直接运行查看结果。

## 目录

- [一、为什么需要操作符重载？](#一、为什么需要操作符重载？)
    - [运行示例：直观感受操作符重载的好处](#运行示例直观感受操作符重载的好处)
- [二、基本概念](#二、基本概念)
    - [2.1 操作符函数的本质](#21-操作符函数的本质)
    - [2.2 成员函数还是非成员函数？— 关键判断](#22-成员函数还是非成员函数？--关键判断)
    - [2.3 运算符分类速查表](#23-运算符分类速查表)
- [三、一个简单的 String 类](#三、一个简单的-String-类)
- [四、输入运算符 `operator>>` 与友元](#四、输入运算符-operator>>-与友元)
- [五、加法运算符 `+` 和 `+=`](#五、加法运算符-+-和-+=)
- [六、自增/自减运算符 `++i` 和 `i++`](#六、自增/自减运算符-++i-和-i++)
- [七、解引用 `*` 和箭头 `->` 运算符](#七、解引用-*-和箭头-->-运算符)
- [八、类型转换操作符](#八、类型转换操作符)
- [九、类的静态成员](#九、类的静态成员)
- [十、常量数据成员和引用数据成员](#十、常量数据成员和引用数据成员)
- [十一、完整示例：一个可运行的 Str 类](#十一、完整示例一个可运行的-Str-类)
- [十二、课程总结](#十二、课程总结)
    - [运算符重载最佳实践速查表](#运算符重载最佳实践速查表)

---

## 一、为什么需要操作符重载？

之前写 `Student_info` 程序时，要把两个学生成绩相加：

```cpp
print(cout, add(data1, data2));  // 啰嗦，不直观
```

而 C++ 标准库的 `string` 让我们可以这样写：

```cpp
cout << s1 + s2;  // 清晰，自然
```

### 运行示例：直观感受操作符重载的好处

```cpp {cmd=true}
#include <iostream>
#include <string>
using namespace std;

int main() {
    string s1 = "Hello, ";
    string s2 = "world!";
    // 使用 + 运算符连接字符串，使用 << 运算符输出
    cout << "s1 + s2 = " << s1 + s2 << endl;
    // 使用 [] 运算符访问单个字符
    cout << "s1[0] = " << s1[0] << endl;
    cout << "s2.size() = " << s2.size() << endl;
    return 0;
}
```

> **来自 C++ Primer 第 14 章的核心原则**：操作符重载最有价值的时候，是当运算符的内置含义与你对类型的操作之间存在**自然的逻辑映射**时。

---

<a id="二基本概念"></a>
## 二、基本概念

### 2.1 操作符函数的本质

重载操作符就是一个**名字特殊的函数**：关键字 `operator` 后跟运算符符号。

```cpp {cmd=true}
#include <iostream>
using namespace std;

// 自定义类型：用 operator+ 作为非成员函数
struct Point {
    int x, y;
    Point(int a = 0, int b = 0) : x(a), y(b) {}
};

// 非成员函数 operator+
Point operator+(const Point& a, const Point& b) {
    return Point(a.x + b.x, a.y + b.y);
}

// 成员函数 operator+=
struct Point2 {
    int x, y;
    Point2(int a = 0, int b = 0) : x(a), y(b) {}
    Point2& operator+=(const Point2& other) {
        x += other.x; y += other.y;
        return *this;
    }
};

int main() {
    Point p1(1, 2), p2(3, 4);

    // 两个调用完全等价：
    Point sum1 = p1 + p2;                       // 表达式形式
    Point sum2 = operator+(p1, p2);             // 直接函数调用形式
    cout << "p1 + p2 = (" << sum1.x << ", " << sum1.y << ")" << endl;
    cout << "operator+(p1,p2) = (" << sum2.x << ", " << sum2.y << ")" << endl;

    // 成员函数版本的调用等价：
    Point2 m1{5, 6}, m2{7, 8};
    m1 += m2;                                    // 表达式形式
    cout << "m1 += m2: (" << m1.x << ", " << m1.y << ")" << endl;
    // m1.operator+=(m2);                        // 等价的成员函数调用
    return 0;
}
```

### 2.2 成员函数还是非成员函数？— 关键判断

**对称性操作符**（算术、相等性、关系、位运算）**通常应该定义为非成员函数**。

```cpp {cmd=true}
#include <iostream>
#include <string>
using namespace std;

int main() {
    string s = "world";
    string t = s + "!";     // OK: operator+(string, const char*)
    string u = "hi" + s;    // OK: operator+(const char*, string)
    // 如果 operator+ 是成员函数，"hi"+s 就会失败
    // 因为 "hi" 是 const char*，内置类型没有成员函数
    cout << "t = " << t << endl;
    cout << "u = " << u << endl;
    return 0;
}
```

### 2.3 运算符分类速查表

| 必须定义为成员的运算符 | 通常定义为成员的运算符 | 通常定义为非成员的运算符 |
|---|---|---|
| `=` 赋值 | `+=`, `-=`, `*=` 等 | `+`, `-`, `*`, `/` 算术 |
| `[]` 下标 | `++`, `--` 自增自减 | `==`, `!=`, `<`, `>` 关系 |
| `()` 函数调用 | `*` 解引用 | `<<`, `>>` 输入输出 |
| `->` 箭头 | | |

---

<a id="三一个简单的-string-类"></a>
## 三、一个简单的 String 类

使用 `Vec<char>` 作为底层存储来实现自己的 `Str` 类。为了简洁演示，这里用一个简化的版本：

```cpp {cmd=true}
#include <iostream>
#include <cstring>
#include <algorithm>
#include <vector>
using namespace std;

class Str {
public:
    typedef vector<char>::size_type size_type;

    // 默认构造函数：创建一个空的 Str
    Str() {}

    // 从 n 个字符 c 构造
    Str(size_type n, char c) : data(n, c) {}

    // 从 C 风格字符串构造（支持隐式类型转换）
    Str(const char* cp) {
        copy(cp, cp + strlen(cp), back_inserter(data));
    }

    // 从迭代器范围构造（模板构造函数）
    template <class In>
    Str(In b, In e) {
        copy(b, e, back_inserter(data));
    }

    size_type size() const { return data.size(); }

    // 索引运算符
    char& operator[](size_type i) { return data[i]; }
    const char& operator[](size_type i) const { return data[i]; }

private:
    vector<char> data;

    // 友元声明：允许 operator<< 访问私有成员 data
    friend ostream& operator<<(ostream& os, const Str& s);
};

// 输出运算符
ostream& operator<<(ostream& os, const Str& s) {
    for (Str::size_type i = 0; i != s.size(); ++i)
        os << s[i];
    return os;
}

int main() {
    Str s("Hello");
    Str t = " world!";
    cout << "s = \"" << s << "\"" << endl;
    cout << "s[1] = '" << s[1] << "'" << endl;

    // 隐式转换：const char* → Str
    s = "Goodbye";
    cout << "After assignment: s = \"" << s << "\"" << endl;
    return 0;
}
```

`★ Insight ─────────────────────────────────────`
`operator<<` 被声明为 `Str` 的 **friend（友元）**，因为它需要访问 `Str` 的私有成员 `data`。IO 运算符**必须是非成员函数**（若为成员函数，左操作数就必须是 `Str`，那就只能写 `s << cout` 而非 `cout << s`），所以 `friend` 是必要且常见的设计模式。
`─────────────────────────────────────────────────`

---

<a id="四输入运算符-operator-与友元"></a>
## 四、输入运算符 `operator>>` 与友元

```cpp {cmd=true}
#include <iostream>
#include <cctype>
#include <vector>
#include <algorithm>
#include <cstring>
using namespace std;

class Str {
    friend istream& operator>>(istream& is, Str& s);
    friend ostream& operator<<(ostream& os, const Str& s);

public:
    typedef vector<char>::size_type size_type;
    Str() {}
    Str(const char* cp) {
        copy(cp, cp + strlen(cp), back_inserter(data));
    }
    size_type size() const { return data.size(); }
    char& operator[](size_type i) { return data[i]; }
    const char& operator[](size_type i) const { return data[i]; }

private:
    vector<char> data;
};

// 输出运算符
ostream& operator<<(ostream& os, const Str& s) {
    for (Str::size_type i = 0; i != s.size(); ++i)
        os << s[i];
    return os;
}

// 输入运算符：跳过空白，读入一个单词
istream& operator>>(istream& is, Str& s) {
    s.data.clear();
    char c;
    while (is.get(c) && isspace(c))    // 跳过前导空白
        ;
    if (is) {
        do {
            s.data.push_back(c);
        } while (is.get(c) && !isspace(c));
        if (is) is.unget();            // 把终止的空白还给流
    }
    return is;
}

int main() {
    Str s;
    cout << "请输入一个单词: ";
    cin >> s;
    cout << "你输入的是: \"" << s << "\"" << endl;
    cout << "长度: " << s.size() << endl;
    return 0;
}
```

---

<a id="五加法运算符--和-"></a>
## 五、加法运算符 `+` 和 `+=`

```cpp {cmd=true}
#include <iostream>
#include <vector>
#include <algorithm>
#include <cstring>
using namespace std;

class Str {
    friend ostream& operator<<(ostream& os, const Str& s);
public:
    typedef vector<char>::size_type size_type;
    Str() {}
    Str(const char* cp) {
        copy(cp, cp + strlen(cp), back_inserter(data));
    }
    size_type size() const { return data.size(); }
    const char& operator[](size_type i) const { return data[i]; }

    // operator+= 是成员函数
    Str& operator+=(const Str& s) {
        copy(s.data.begin(), s.data.end(),
             back_inserter(data));
        return *this;
    }
private:
    vector<char> data;
};

// operator+ 是非成员函数，委托给 +=
Str operator+(const Str& s, const Str& t) {
    Str r = s;
    r += t;
    return r;
}

ostream& operator<<(ostream& os, const Str& s) {
    for (Str::size_type i = 0; i != s.size(); ++i)
        os << s[i];
    return os;
}

int main() {
    Str s1 = "Hello, ";
    Str s2 = "world!";
    Str s3 = s1 + s2;          // operator+(const Str&, const Str&)
    cout << "s1 + s2 = \"" << s3 << "\"" << endl;

    s1 += Str(" C++");         // operator+=(const Str&)
    cout << "s1 += \" C++\": \"" << s1 << "\"" << endl;

    // 混合类型：const char* + Str
    Str greeting = ">> " + s3;
    cout << "\">> \" + s3 = \"" << greeting << "\"" << endl;
    return 0;
}
```

**关键设计模式**：用 `+=`（成员，高效）实现 `+`（非成员，通用）。

---

<a id="六自增自减运算符-i-和-i"></a>
## 六、自增/自减运算符 `++i` 和 `i++`

```cpp {cmd=true}
#include <iostream>
using namespace std;

class Counter {
    int n;
public:
    Counter(int i = 0) : n(i) {}

    // 前缀 ++：返回引用
    Counter& operator++() { ++n; return *this; }

    // 后缀 ++：int 参数仅用于区分，返回旧值
    Counter operator++(int) {
        Counter old(n);
        ++n;          // 委托给前缀版本
        return old;
    }

    int value() const { return n; }
};

// 输出运算符
ostream& operator<<(ostream& os, const Counter& c) {
    return os << c.value();
}

int main() {
    Counter a(0);
    cout << "初始值: " << a << endl;

    cout << "++a: " << ++a << " (a变为: " << a << ")" << endl;
    cout << "a++: " << a++ << " (a变为: " << a << ")" << endl;

    // 前缀返回引用，可以链式调用
    Counter b(5);
    ++++++b;  // 连续三次前缀++
    cout << "三次 ++++b 后: " << b << endl;

    return 0;
}
```

| | 前缀 `++a` | 后缀 `a++` |
|---|---|---|
| 函数签名 | `Counter& operator++()` | `Counter operator++(int)` |
| 返回值 | 引用（`Counter&`） | 值（`Counter`） |
| 效率 | 高（无拷贝） | 低（需拷贝旧值） |

---

## 七、解引用 `*` 和箭头 `->` 运算符

```cpp {cmd=true}
#include <iostream>
#include <string>
using namespace std;

// 简化的链表节点
template <typename T>
struct ListNode {
    T data;
    ListNode* next;
    ListNode(const T& d, ListNode* n = nullptr) : data(d), next(n) {}
};

// 简化的迭代器
template <typename T>
class Iterator {
    ListNode<T>* node;
public:
    Iterator(ListNode<T>* n = nullptr) : node(n) {}

    T& operator*() const { return node->data; }
    T* operator->() const { return &(node->data); }

    Iterator& operator++() { node = node->next; return *this; }

    bool operator!=(const Iterator& other) const {
        return node != other.node;
    }
};

int main() {
    // 创建链表: 10 → 20 → 30
    ListNode<int>* head = new ListNode<int>(10,
                            new ListNode<int>(20,
                                new ListNode<int>(30)));

    Iterator<int> it(head);
    cout << "*it = " << *it << endl;

    // 用 -> 访问 string 的 size()
    ListNode<string> strNode(string("hello"));
    Iterator<string> sit(&strNode);
    cout << "sit->size() = " << sit->size() << endl;

    // 遍历
    cout << "遍历链表: ";
    for (Iterator<int> i(head); i != Iterator<int>(); ++i) {
        cout << *i << " ";
    }
    cout << endl;

    // 清理
    while (head) {
        ListNode<int>* t = head;
        head = head->next;
        delete t;
    }
    return 0;
}
```

> `operator->` 必须返回一个普通指针，或一个自己也定义了 `operator->` 的类对象。

---

## 八、类型转换操作符

```cpp {cmd=true}
#include <iostream>
#include <cmath>
using namespace std;

class Complex {
    double re, im;
public:
    Complex(double r = 0, double i = 0) : re(r), im(i) {}

    // 类型转换操作符：Complex → double（返回模）
    operator double() const {
        return sqrt(re * re + im * im);
    }

    // explicit 转换操作符（C++11）
    explicit operator bool() const {
        return re != 0 || im != 0;
    }

    friend ostream& operator<<(ostream& os, const Complex& c) {
        return os << c.re << " + " << c.im << "i";
    }
};

int main() {
    Complex c(3, 4);
    cout << "c = " << c << endl;

    // 隐式转换：Complex → double
    double magnitude = c;  // operator double()
    cout << "magnitude(c) = " << magnitude << " (即 |3+4i| = 5)" << endl;

    // explicit operator bool：只能在条件中使用
    Complex zero(0, 0);
    if (c)     cout << "c 非零" << endl;
    if (!zero) cout << "zero 为零" << endl;

    // 下面这行会编译失败（explicit 阻止了隐式转换）：
    // bool b = c;  // 错误！operator bool 是 explicit
    return 0;
}
```

> **C++ Primer 建议**：实践中类很少提供类型转换操作符，唯一的常见例外是转到 `bool`，且应声明为 `explicit`。

---

## 九、类的静态成员

```cpp {cmd=true}
#include <iostream>
#include <string>
using namespace std;

class Student {
    static int count;  // 静态数据成员：统计对象数量
    string name;
public:
    Student(const string& n) : name(n) {
        ++count;  // 每创建一个对象，计数+1
    }
    ~Student() {
        --count;  // 每销毁一个对象，计数-1
    }

    // 静态成员函数：不需要对象就能调用
    static int total() { return count; }
    string getName() const { return name; }
};

// 必须在类外部定义并初始化静态成员
int Student::count = 0;

int main() {
    cout << "初始学生数: " << Student::total() << endl;

    Student s1("Alice");
    Student s2("Bob");
    {
        Student s3("Charlie");
        cout << "创建3个学生后: " << Student::total() << endl;
    }  // s3 离开作用域，被销毁
    cout << "s3销毁后: " << Student::total() << endl;

    cout << "学生列表: " << s1.getName() << ", "
         << s2.getName() << endl;
    return 0;
}
```

> 静态成员**属于类**而非对象；通过 `ClassName::member` 访问（推荐），也可通过对象访问。

---

## 十、常量数据成员和引用数据成员

```cpp {cmd=true}
#include <iostream>
using namespace std;

class Config {
    const int maxUsers;   // 常量成员：初始化后不可修改
    int& externalRef;      // 引用成员：绑定后不可更改
public:
    Config(int max, int& ext)
        : maxUsers(max), externalRef(ext) {}  // ✅ 必须用初始化列表

    void display() const {
        cout << "maxUsers = " << maxUsers
             << ", externalRef = " << externalRef << endl;
    }

    void updateExternal(int newVal) {
        externalRef = newVal;  // OK：修改引用的对象，不是重新绑定
    }

    // void setMax(int m) { maxUsers = m; }  // ❌ 错误！const 成员不可修改
};

int main() {
    int shared = 100;
    Config cfg(1000, shared);
    cfg.display();

    cfg.updateExternal(200);
    cout << "After updateExternal(200):" << endl;
    cfg.display();
    cout << "shared现在 = " << shared << endl;  // 200 —— 引用生效
    return 0;
}
```

---

## 十一、完整示例：一个可运行的 Str 类

```cpp {cmd=true}
#include <iostream>
#include <cstring>
#include <cctype>
#include <vector>
#include <algorithm>
using namespace std;

class Str {
    friend istream& operator>>(istream&, Str&);
    friend ostream& operator<<(ostream&, const Str&);

public:
    typedef vector<char>::size_type size_type;
    Str() {}
    Str(size_type n, char c) : data(n, c) {}
    Str(const char* cp) {
        copy(cp, cp + strlen(cp), back_inserter(data));
    }
    template <class In> Str(In b, In e) {
        copy(b, e, back_inserter(data));
    }

    size_type size() const { return data.size(); }
    char& operator[](size_type i) { return data[i]; }
    const char& operator[](size_type i) const { return data[i]; }

    Str& operator+=(const Str& s) {
        copy(s.data.begin(), s.data.end(), back_inserter(data));
        return *this;
    }

private:
    vector<char> data;
};

ostream& operator<<(ostream& os, const Str& s) {
    for (Str::size_type i = 0; i < s.size(); ++i)
        os << s[i];
    return os;
}

istream& operator>>(istream& is, Str& s) {
    s.data.clear();
    char c;
    while (is.get(c) && isspace(c)) ;
    if (is) {
        do { s.data.push_back(c); }
        while (is.get(c) && !isspace(c));
        if (is) is.unget();
    }
    return is;
}

Str operator+(const Str& a, const Str& b) {
    Str r = a;
    r += b;
    return r;
}

int main() {
    Str hello = "Hello, ";
    Str world = "World!";
    Str greeting = hello + world;
    cout << greeting << endl;

    Str name;
    cout << "输入你的名字: ";
    cin >> name;
    cout << "你好, " << name << "!" << endl;

    return 0;
}
```

---

## 十二、课程总结

| 知识点 | 关键要点 |
|---|---|
| **基本概念** | 操作符函数名为 `operator符号`；成员 vs 非成员的选择取决于操作数对称性 |
| **`[]` 下标** | 必须是成员函数；提供 const 和非 const 两个版本 |
| **`<<`, `>>` IO** | 必须是非成员函数；通常需要声明为 friend |
| **`+`, `+=` 算术** | `+=` 做实际工作（成员），`+` 委托给 `+=`（非成员） |
| **`++i`, `i++`** | 后缀用 `int` 参数区分；前缀返回引用，后缀返回值 |
| **`*`, `->`** | 模拟指针行为；`->` 必须返回指针或可递归的对象 |
| **类型转换** | `operator type()` 无返回类型无参数；`explicit` 防止意外隐式转换 |
| **友元** | 单向、不可传递；主要用于 IO 运算符 |
| **静态成员** | 属于类而非对象；必须在类外初始化 |
| **const/引用成员** | 必须用初始化列表初始化 |

### 运算符重载最佳实践速查表

| 运算符 | 建议实现方式 | 返回值 |
|---|---|---|
| `=` | 成员函数 | `T&` |
| `[]` | 成员函数（const + 非 const） | `T&` / `const T&` |
| `()` | 成员函数 | 任意 |
| `->` | 成员函数 | 指针或含 `->` 的对象 |
| `+=`, `-=` 等 | 成员函数 | `T&` |
| `+`, `-`, `*` 等 | 非成员函数（委托给复合赋值） | `T`（值） |
| `==`, `!=`, `<` | 非成员函数 | `bool` |
| `<<`, `>>` | 非成员函数（通常为 friend） | `istream&` / `ostream&` |
| `++`, `--` | 成员函数 | 前缀返回 `T&`，后缀返回 `T` |

> **课后练习**：为 `Str` 类添加 `operator==`、`operator!=` 和 `operator<`。思考：`<` 的定义是否和 `==` 一致？
