# 第十三周：操作符重载 — 实现 String 类

> 基于课件 `06-操作符重载.md`，参考 C++ Primer 第五版（英文版）第 14 章、第 7 章补充优化。
>

## 目录

- [一、为什么需要操作符重载？](#一为什么需要操作符重载)
- [二、基本概念](#二基本概念)
    - [2.1 操作符函数的本质](#21-操作符函数的本质)
    - [2.2 成员函数还是非成员函数？— 关键判断](#22-成员函数还是非成员函数关键判断)
    - [2.3 运算符分类速查表](#23-运算符分类速查表)
- [三、一个简单的 String 类](#三一个简单的-string-类)
- [四、输入运算符 `operator>>` 与友元](#四输入运算符-operator-与友元)
- [五、加法运算符 `+` 和 `+=`](#五加法运算符--和-)
- [六、自增/自减运算符 `++i` 和 `i++`](#六自增自减运算符-i-和-i)
- [七、解引用 `*` 和箭头 `->` 运算符](#七解引用--和箭头---运算符)
- [八、类型转换操作符](#八类型转换操作符)
- [九、类的静态成员](#九类的静态成员)
- [十、常量数据成员和引用数据成员](#十常量数据成员和引用数据成员)
- [十一、完整示例：一个可运行的 Str 类](#十一完整示例一个可运行的-str-类)
- [十二、课程总结](#十二课程总结)
    - [运算符重载最佳实践速查表](#运算符重载最佳实践速查表)

---

<a id="一为什么需要操作符重载"></a>
## 一、为什么需要操作符重载？

> 在之前的 `Student_info` 程序中，如果我们想把两个学生的成绩相加，免不了写 `add(data1, data2)` 和 `print(cout, result)` 这样啰嗦的调用。但 C++ 标准库的 `string` 类型可以写 `cout << s1 + s2`——直观、自然、和内置类型一样简洁。
>
> **操作符重载（operator overloading）** 的目的就是：让用户自定义的类型也能使用 C++ 内置的操作符语法，使其行为像内置类型一样自然。但这有一个重要前提——**运算符的内置含义与你对类型的操作之间存在自然的逻辑映射时，才值得重载**（C++ Primer 第 14 章）。不要为了"看起来酷"而把 `+` 重载成减法。

```cpp
// 没有重载：只能用函数调用的方式
print(cout, add(data1, data2));  // 啰嗦，不直观

// 有重载：用操作符表达
cout << s1 + s2;  // 清晰，自然
```

> **来自 C++ Primer 第 14 章的核心原则**：操作符重载最有价值的时候，是当运算符的内置含义与你对类型的操作之间存在**自然的逻辑映射**时。

---

<a id="二基本概念"></a>
## 二、基本概念

### 2.1 操作符函数的本质

> 重载操作符就是一个**名字特殊的函数**：关键字 `operator` 后跟运算符符号。编译器看到 `a + b` 这样的表达式，会尝试将之解析为 `operator+(a, b)` 的调用——无论 `+` 是不是成员函数。

```cpp
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

// 使用示例
Point p1(1, 2), p2(3, 4);

// 两个调用完全等价：
Point sum1 = p1 + p2;                       // 表达式形式
Point sum2 = operator+(p1, p2);             // 直接函数调用形式

// 成员函数版本的调用等价：
Point2 m1{5, 6}, m2{7, 8};
m1 += m2;                                    // 表达式形式
// m1.operator+=(m2);                        // 等价的成员函数调用
```

> 注意上面的代码展示了两种不同的实现方式——非成员函数形式的 `operator+` 和成员函数形式的 `operator+=`。为什么选择不同方式？这不是随意的——**选择成员还是非成员是由操作符的语义性质决定的**。

### 2.2 成员函数还是非成员函数？— 关键判断

> 这是操作符重载中最容易出错的问题。**核心判断标准是"对称性"**：如果操作符的操作数在语义上是对称的，应该用非成员函数；如果有一个操作数明显是"主导"对象（被修改的那个），应该用成员函数。

```cpp
string s = "world";
string t = s + "!";     // OK: operator+(string, const char*)
string u = "hi" + s;    // OK: operator+(const char*, string)
// 如果 operator+ 是成员函数，"hi"+s 就会失败
// 因为 "hi" 是 const char*，内置类型没有成员函数
```

> **为什么对称性操作符必须是非成员函数？**
>
> 考虑 `"hi" + s`：如果 `operator+` 是 `string` 的成员函数，那它等价于 `"hi".operator+(s)`——但 `"hi"` 是 `const char[3]`，没有成员函数！所以编译失败。
>
> 如果 `operator+` 是非成员函数，编译器会尝试匹配 `operator+(const char*, string)` 通过隐式转换将 `const char*` 转为 `string`——编译成功。
>
> **核心规则总结**：
>
> ① **必须定义为成员的运算符**：`=`（赋值）、`[]`（下标）、`()`（函数调用）、`->`（箭头）。C++ 标准强制要求。理由是这些操作符在语义上必然和"左侧对象"绑定——赋值就是修改 `*this`，下标就是访问 `*this` 的某个位置，函数调用就是视对象为函数。
>
> ② **通常定义为成员的运算符**：`+=`、`-=`、`*=` 等复合赋值，`++`、`--`。这些操作符"修改左侧对象"，以成员定义自然。
>
> ③ **通常定义为非成员的运算符**：`+`、`-`、`*`、`/` 等算术，`==`、`!=`、`<` 等关系，`<<`、`>>` IO。这些操作符操作数对称——不应该因为操作数在左边还是右边而有不同行为。
>
> ④ **友元（friend）在这个场景中为什么必要？** 非成员函数不能访问类的私有成员——而 `operator<<` 和 `operator>>` 通常需要直接读写私有数据成员。这种情况下必须将外部函数声明为类的 `friend`。

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

> 下面实现一个简单的 `Str` 类，使用 `std::vector<char>` 作为底层存储——这样我们不用亲自管理内存，可以专注于运算符重载的语法。
>
> 注意这个版本的 `Str` 采用了**隐式转换**——从 `const char*` 构造没有加 `explicit`，意味着 `"hello"` 可以隐式转为 `Str`。这在字符串类中通常是合适的（因为字符串字面量和 `Str` 在语义上等价），但在别的场景中需要谨慎（见 `explicit` 关键字的相关讨论）。

```cpp
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
```

> **逐点解释上述代码**：
>
> **① 为什么 `operator<<` 必须是 friend？** 因为它需要访问 `Str` 的 `data` 成员。`operator<<` 的左侧是 `ostream&`（不是 `Str`），所以它**不能是成员函数**。作为非成员函数，它无法访问 `private: data`——除非被声明为 `friend`。
>
> **② `operator[]` 为什么有 const 和非 const 两个版本？** const 版本供 const 对象调用（只读），非 const 版本供普通对象（读写）。如果只有一个版本且是 `char&`，则 const 对象不能调用 `operator[]`，使 `const Str` 变得几乎无用。
>
> **③ 非成员函数 `operator<<` 返回 `ostream&`——为什么？** 为了链式调用：`cout << a << b` 等价于 `operator<<(operator<<(cout, a), b)`。第一个 `<<` 返回 `cout`（的引用），第二个 `<<` 复用同一个流对象。如果返回值是 `void`，链式调用不可能。

`★ Insight ─────────────────────────────────────`
`operator<<` 被声明为 `Str` 的 **friend（友元）**，因为它需要访问 `Str` 的私有成员 `data`。IO 运算符**必须是非成员函数**（若为成员函数，左操作数就必须是 `Str`，那就只能写 `s << cout` 而非 `cout << s`），所以 `friend` 是必要且常见的设计模式。
`─────────────────────────────────────────────────`

---

<a id="四输入运算符-operator-与友元"></a>
## 四、输入运算符 `operator>>` 与友元

> 输入运算符是 `operator<<` 的镜像操作——它从输入流读取字符，构造出一个 `Str` 对象。同样需要定义为非成员函数 + friend，原因和输出运算符一样（左操作数是 `istream&` 不是 `Str`）。

```cpp
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
```

> **输入运算符的设计考量**：
>
> ① 为什么先 `clear()`? `>>` 是**覆盖**旧内容而不是追加。不 clear 的话，旧字符串和新读入的字符会混在一起。
>
> ② 为什么跳过空白？和内置 `>>` 对 `string` 的行为一致——读入一个"单词"（空白分隔的连续非空白字符）。
>
> ③ 为什么用 `do { ... } while`？因为读入第一个字符后已经确定"有数据可读"，用 `do-while` 避免重复读取逻辑。
>
> ④ `is.unget()` 的作用：读入遇到了空白符号后停止，但空白符号属于下一个输入操作——所以要把这个空白符号"放回"流中。不放回的话，下一个 `>>` 会丢失这个空白。
>
> **朋友规则：** `friend` 声明出现在类内部（可以在任意访问标签下），但**不是成员函数**——它只是在允许外部函数访问私有成员。`friend` 关系是**单向的**（`Str` 授权给 `operator>>`，但 `operator>>` 不能反过来访问 `Str` 的其他 friend）且**不可传递的**（`A` 是 `B` 的 friend，`B` 是 `C` 的 friend ≠ `A` 是 `C` 的 friend）。

---

<a id="五加法运算符--和-"></a>
## 五、加法运算符 `+` 和 `+=`

> 这是一个重要的设计模式：**`+=` 做实际工作（成员函数），`+` 委托给 `+=`（非成员函数）。** 原因：`+=` 需要修改左侧对象（自然应该用成员函数），而 `+` 不应修改操作数（应该返回一个新对象），并且要求对称性。

```cpp
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

// 使用示例
Str s1 = "Hello, ";
Str s2 = "world!";
Str s3 = s1 + s2;          // operator+(const Str&, const Str&)

s1 += Str(" C++");         // operator+=(const Str&)

// 混合类型：const char* + Str
Str greeting = ">> " + s3;
```

> **为什么 `+` 和 `+=` 要分开定义？**
>
> 很多初学者会问：一个 `operator+` 做"连接并返回新对象"不就行了？为什么还需要 `operator+=`？因为**语义不同**：
>
> | 操作符 | 对自身的影响 | 返回值 | 效率 |
> |--------|------------|--------|------|
> | `+=` | 修改自身 | 自身的引用 | 高（不创建新对象） |
> | `+` | 不修改自身 | 新对象（值） | 较低（需拷贝） |
>
> 如果只定义 `+` 不定义 `+=`，写 `s1 += s2` 也行吗？可以——你可以实现 `+=` 为 `s1 = s1 + s2`，但这就多了一次不必要的拷贝（先创建临时对象再赋值）。**最佳实践：`+=` 直接操作自身，`+` 委托给 `+=`**——这样两者都正确且高效。
>
> **为什么 `operator+` 必须是**非成员**？** 回顾对称性原则：`"hi" + s` 合法需要 `operator+` 接受左操作数为 `const char*`。如果 `+` 是成员函数，左侧必须是 `Str`，`"hi" + s` 就编译失败了。

**关键设计模式**：用 `+=`（成员，高效）实现 `+`（非成员，通用）。

---

<a id="六自增自减运算符-i-和-i"></a>
## 六、自增/自减运算符 `++i` 和 `i++`

> 前缀和后缀自增是 C++ 中最微妙的重载之一。后缀版本有个哑元 `int` 参数——它的唯一作用是和前缀版本**区分函数签名**。两者的**返回值差异**（引用 vs 值）对应了它们在 C++ 中不同的语义。

```cpp
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

// 使用示例
Counter a(0);
++a;    // 前缀 ++
a++;    // 后缀 ++

// 前缀返回引用，可以链式调用
Counter b(5);
++++++b;  // 连续三次前缀++
```

> **为什么返回值不同？——这是语义决定的不是随意的**
>
> **前缀 `++a`：返回自增后的值**。这就像 `++a = 10` 在 C++ 中合法（`a` 是一个左值，可以出现在赋值左侧）。所以返回 `Counter&`（引用），允许继续修改。
>
> **后缀 `a++`：返回自增前的值**。表达式 `a++` 的值是 `a` 修改**之前**的值。要实现这个语义，你必须保存旧值的副本！所以返回 `Counter`（值）。返回引用也不可能——因为引用的对象已经被修改了。
>
> | | 前缀 `++a` | 后缀 `a++` |
> |---|---|---|
> | 函数签名 | `Counter& operator++()` | `Counter operator++(int)` |
> | 返回值 | 引用（`Counter&`） | 值（`Counter`） |
> | 语义 | 先自增，返回自身 | 保存旧值，自增，返回旧值 |
> | 效率 | 高（无拷贝） | 低（需拷贝旧值） |
> | 链式调用 | 支持（`++++++b`） | 不支持 |
>
> **经验法则**：**优先用前缀 `++`**，除非你真的需要后缀的"自增前的值"。在泛型代码/迭代器中，前缀 `++` 避免了不必要的拷贝，通常更高效。

---

<a id="七解引用--和箭头---运算符"></a>
## 七、解引用 `*` 和箭头 `->` 运算符

> `*` 和 `->` 通常一起重载来实现**迭代器（iterator）**和**智能指针**。`operator*` 返回"指向的对象"，`operator->` 返回"指向的对象指针"——注意 `->` 必须返回一个普通指针或一个自己也定义了 `operator->` 的对象。

```cpp
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

// 使用示例
ListNode<int>* head = new ListNode<int>(10,
                        new ListNode<int>(20,
                            new ListNode<int>(30)));

Iterator<int> it(head);
// *it == 10
// it-> 访问 ListNode<int>::data
```

> **关于 `->` 重载的底层机制**：
>
> 当写 `it->member` 时，C++ 会这样解析：
> ```
> // 1. 检查 it 的 operator-> 是否存在
> // 2. 调用 it.operator->()，得到 T*（普通指针）
> // 3. 普通指针调用内置 ->，访问 member
> ```
>
> 所以 `operator->` 的返回值要么是**一个普通指针**，要么是**另一个定义了 `operator->` 的对象**（递归直到得到普通指针）。这种机制支持了"多层代理"类。

---

<a id="八类型转换操作符"></a>
## 八、类型转换操作符

> 类型转换操作符允许你的类**隐式地**转换为另一种类型。它的语法非常特殊：`operator type()`——**没有返回类型声明**（因为 `type` 本身就是返回类型），**没有参数**（因为转换的是 `*this`）。**这种隐式转换非常危险**——它让编译器在你不注意的地方"自作聪明"，往往导致难以发现的 bug。

```cpp
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

// 使用示例
Complex c(3, 4);

// 隐式转换：Complex → double
double magnitude = c;  // operator double()

// explicit operator bool：只能在条件中使用
Complex zero(0, 0);
if (c)     { /* c 非零 */ }
if (!zero) { /* zero 为零 */ }

// bool b = c;  // 错误！operator bool 是 explicit
```

> **类型转换操作符的危险性**：
>
> ① **意想不到的转换**：写 `if (c)` 时你期望 `c` 被判断为 bool —— 但如果写了 `operator double()`，编译器也可能把 `c` 转成 `double`（非零即为 true）。多个隐式转换路径会引起歧义。
>
> ② **意外的函数匹配**：如果你有一个接受 `double` 的函数 `f(double)`，调用 `f(c)` 会在你不知情时计算复数的模并传入——这可能完全不是你想要的行为。
>
> ③ **C++11 的 `explicit operator bool()` 解决了大部分问题**：只有在条件语境中（`if`、`while`、`&&`、`||`、`!`、三元运算符 `?:`）才会触发转换，避免了隐式转数字或参与算术运算。
>
> **C++ Primer 强烈建议**：实践中类很少提供类型转换操作符，唯一的常见例外是转到 `bool`，且应声明为 `explicit`。
>
> | 类型转换操作符 | 行为 | 建议 |
> |---------------|------|------|
> | `operator bool()` | 用于条件判断 | ✅ 加 `explicit` 使用 |
> | `operator double()` | 转成数值 | ❌ 极少情况下才用 |
> | `operator string()` | 转成字符串 | ⚠️ 有 `toString()` 就用成员函数 |

---

<a id="九类的静态成员"></a>
## 九、类的静态成员

> 静态成员属于**类**而不是某个对象。所有对象共享同一个静态成员变量，通过 `ClassName::member` 访问。常用于全局计数器、工厂方法、配置常量等。

```cpp
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

// 使用示例
Student s1("Alice");
Student s2("Bob");
{
    Student s3("Charlie");
    // 创建3个学生后，Student::total() == 3
}  // s3 离开作用域，被销毁
// s3销毁后，Student::total() == 2
```

> **静态成员的关键区别**：
>
> ① **不属于任何对象**：静态成员不占用对象的内存空间，它在程序的数据段中有一份单独存储。`sizeof(Student)` 不包含 `count`。
>
> ② **必须在类外定义**：`int Student::count = 0;` 这条语句必须出现在一个 `.cpp` 文件中（通常不是在头文件中），否则链接时可能产生重复定义错误。
>
> ③ **静态成员函数没有 `this` 指针**：因为静态函数不属于某个对象，所以不能访问非静态成员、不能是 `const`、不能是 `virtual`。

---

<a id="十常量数据成员和引用数据成员"></a>
## 十、常量数据成员和引用数据成员

> `const` 成员和引用成员一旦初始化就不能改变。这个"一旦初始化"的时机**必须是初始化列表**——不能在构造函数体内赋值。这是 C++ 的硬性语法规则。

```cpp
class Config {
    const int maxUsers;   // 常量成员：初始化后不可修改
    int& externalRef;      // 引用成员：绑定后不可更改
public:
    Config(int max, int& ext)
        : maxUsers(max), externalRef(ext) {}  // ✅ 必须用初始化列表

    void updateExternal(int newVal) {
        externalRef = newVal;  // OK：修改引用的对象，不是重新绑定
    }
    // void setMax(int m) { maxUsers = m; }  // ❌ 错误！const 成员不可修改
};

// 使用示例
int shared = 100;
Config cfg(1000, shared);
cfg.updateExternal(200);
// shared 变为 200 —— 引用生效
```

> **为什么必须用初始化列表？**
>
> 回忆第五章的"对象创建过程"：
> ```
> 1. 分配内存
> 2. 用初始化列表初始化所有成员（按声明顺序）
> 3. 执行构造函数体
> ```
>
> 如果不给 `maxUsers` 提供初始值，**步骤 2** 会试图默认初始化它——但 `const int` 没有默认值！然后**步骤 3** 再试图 `maxUsers = max`——但 const 成员不能赋值！
>
> 引用同理：引用必须在创建时绑定到一个对象。如果在初始化列表中不给 `externalRef` 提供绑定的对象，它就是未绑定的悬挂引用——在构造函数体中再试图初始化已经太晚了。
>
> **规则**：只要类中包含了 `const` 成员或引用成员，**每个构造函数**都必须通过初始化列表给它们赋值。没有例外。

---

<a id="十一完整示例一个可运行的-str-类"></a>
## 十一、完整示例：一个可运行的 Str 类

> 将前面所有概念整合成一个完整的 `Str` 类——支持构造、索引、输入输出、拼接。注意追踪**每个操作符选择成员/非成员的原因**。

```cpp
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

// 使用示例
Str hello = "Hello, ";
Str world = "World!";
Str greeting = hello + world;
```

> **完整 Str 类的设计回顾**：

| 操作符 | 为什么是这个形式 | 替代方案会怎样 |
|--------|----------------|-------------|
| `operator[]` — 成员 | `s[i]` 左侧必须是 `Str`，语义显然 | 非成员也可行但无意义 |
| `operator<<` — 非成员 friend | 左操作数是 `ostream&`，需要访问 `private data` | 如果是成员，「cout << s」写成「s << cout」 |
| `operator>>` — 非成员 friend | 同上，左操作数是 `istream&` | 同上 |
| `operator+=` — 成员 | 修改自身，左侧必须是 `Str` | 非成员也可，但必须 friend 才能访问 data |
| `operator+` — 非成员 | 对称性 + 委托给 `+=` | 如果是成员，「"hi"+s」编译失败 |

---

<a id="十二课程总结"></a>
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
