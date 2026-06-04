# 补充17：RTTI、枚举类和实用库设施

> 基于 C++ Primer 第五版（英文版）第 17 章和第 19 章补充。
>
> **前置要求：** 已完成第 01–08 周全部内容。本文介绍运行时类型识别、强类型枚举、tuple 和 bitset——C++ 工具箱中的进阶利器。
>
> **关联已有知识：** 第 08 周（多态和虚函数——RTTI 的前提）、第 03 周（map 的 pair → tuple 泛化）、第 04 周（泛型算法结合 bitset）、补充 12（关联容器与 enum class 的配合）。
>
> 本文中所有代码块均支持 **Markdown Preview Enhanced** 的 `{cmd=true}` 功能，可以直接运行查看结果。

## 目录

- [一、运行时类型识别（RTTI）](#一、运行时类型识别（RTTI）)
    - [1.2 `dynamic_cast` 用于引用](#12-dynamic_cast-用于引用)
    - [1.3 `typeid` — 获取运行时类型信息](#13-typeid---获取运行时类型信息)
- [二、强类型枚举 `enum class`（C++11）](#二、强类型枚举-enum-class（C++11）)
- [三、`tuple` — 泛化的 `pair`](#三、tuple---泛化的-pair)
    - [`tuple` vs `struct`](#tuple-vs-struct)
- [四、`bitset` — 位集合](#四、bitset---位集合)
- [五、`union` — 节省空间的联合体](#五、union---节省空间的联合体)
- [六、`mem_fn` — 将成员函数转为可调用对象](#六、mem_fn---将成员函数转为可调用对象)
- [七、补充总结](#七、补充总结)
    - [核心要点](#核心要点)
    - [何时使用 RTTI？](#何时使用-RTTI？)

---

## 一、运行时类型识别（RTTI）

> **什么是 RTTI？** RTTI（Run-Time Type Identification）是 C++ 在程序运行时判断对象真实类型的一套机制。正常情况下，一个 `Base*` 指针在编译期只知道它指向 `Base`；通过 RTTI，你可以在运行时知道它究竟指向 `Base` 还是 `Derived`。

RTTI 的两个核心运算符：`dynamic_cast`（安全的向下转型）和 `typeid`（获取类型信息）。

```cpp {cmd=true}
#include <iostream>
#include <string>
#include <vector>
using namespace std;

class Animal {
public:
    virtual ~Animal() = default;
    virtual string sound() const = 0;
};

class Dog : public Animal {
public:
    string sound() const override { return "汪汪!"; }
    void fetch() { cout << "  🐕 接飞盘!" << endl; }
};

class Cat : public Animal {
public:
    string sound() const override { return "喵喵!"; }
    void climb() { cout << "  🐈 爬树!" << endl; }
};

int main() {
    vector<Animal*> zoo{new Dog, new Cat, new Dog};

    for (auto *animal : zoo) {
        cout << "叫声: " << animal->sound() << endl;

        // dynamic_cast：尝试将基类指针转为派生类指针
        // 如果失败（类型不匹配），返回 nullptr
        if (auto *dog = dynamic_cast<Dog*>(animal)) {
            dog->fetch();  // 安全地调用 Dog 特有的方法
        } else if (auto *cat = dynamic_cast<Cat*>(animal)) {
            cat->climb();  // 安全地调用 Cat 特有的方法
        }
    }

    // 清理
    for (auto *p : zoo) delete p;
    return 0;
}
```

### 1.2 `dynamic_cast` 用于引用

```cpp {cmd=true}
#include <iostream>
#include <stdexcept>
using namespace std;

class Base {
public:
    virtual ~Base() = default;
};

class Derived : public Base {
public:
    void derived_only() { cout << "Derived 特有的方法" << endl; }
};

int main() {
    Derived d;
    Base &ref = d;

    // 对引用做 dynamic_cast：失败时抛 std::bad_cast
    try {
        auto &dr = dynamic_cast<Derived&>(ref);
        dr.derived_only();
    } catch (const bad_cast &e) {
        cerr << "dynamic_cast 失败: " << e.what() << endl;
    }

    return 0;
}
```

| | 指针版本 | 引用版本 |
|---|---|---|
| 失败行为 | 返回 `nullptr` | 抛出 `std::bad_cast` |
| 检查方式 | `if (auto *p = dynamic_cast<T*>(...))` | `try-catch` |

### 1.3 `typeid` — 获取运行时类型信息

```cpp {cmd=true}
#include <iostream>
#include <typeinfo>
using namespace std;

class Base {
public:
    virtual ~Base() = default;
};

class Derived : public Base {};

int main() {
    Derived d;
    Base *pb = &d;

    // typeid 用于多态类型时，在运行时确定动态类型
    cout << "静态类型: " << typeid(pb).name() << endl;       // Base*
    cout << "动态类型: " << typeid(*pb).name() << endl;      // Derived

    // 比较类型
    if (typeid(*pb) == typeid(Derived)) {
        cout << "*pb 是 Derived 类型" << endl;
    }

    // 非多态类型：编译期决定
    int i = 42;
    cout << "int 的类型: " << typeid(i).name() << endl;

    return 0;
}
```

> **RTTI 使用原则：**
> - 优先用虚函数代替 RTTI（多态本身就是最好的类型分发机制）
> - `dynamic_cast` 用于需要访问派生类特有接口的场景
> - `typeid` 用于调试和日志，不要用来替代虚函数分发

---

## 二、强类型枚举 `enum class`（C++11）

```cpp {cmd=true}
#include <iostream>
using namespace std;

// ❌ C 风格枚举（C++98）：作用域污染
// enum Color { Red, Green, Blue };
// enum TrafficLight { Red, Yellow, Green };  // ❌ Red 和 Green 冲突！

// ✅ C++11 强类型枚举
enum class Color { Red, Green, Blue };
enum class TrafficLight { Red, Yellow, Green };  // ✅ 无冲突！

// 可以指定底层类型
enum class Permission : unsigned char {
    Read = 1,      // 0b001
    Write = 2,     // 0b010
    Execute = 4    // 0b100
};

// 像位掩码一样使用
Permission operator|(Permission a, Permission b) {
    return static_cast<Permission>(
        static_cast<unsigned char>(a) | static_cast<unsigned char>(b)
    );
}

int main() {
    // 使用强类型枚举（需要作用域限定符）
    Color c = Color::Red;
    TrafficLight tl = TrafficLight::Green;

    // int n = c;           // ❌ 不能隐式转换
    int n = static_cast<int>(c);  // ✅ 必须显式转换

    cout << "Color: " << static_cast<int>(c) << endl;
    cout << "可以指定底层类型来节省空间" << endl;

    // 位掩码用法
    Permission p = Permission::Read | Permission::Write;
    cout << "权限掩码: " << static_cast<int>(p) << endl;

    return 0;
}
```

| | C 风格 `enum` | C++11 `enum class` |
|---|---|---|
| 作用域 | 全局污染 | 枚举名限定 `Enum::Value` |
| 隐式转 int | ✅（不安全） | ❌ 必须 `static_cast` |
| 指定底层类型 | ❌ | ✅ `enum class X : uint8_t` |
| 前置声明 | ❌ | ✅ `enum class X;` |

---

## 三、`tuple` — 泛化的 `pair`

```cpp {cmd=true}
#include <iostream>
#include <tuple>
#include <string>
using namespace std;

int main() {
    // tuple：可以存放任意数量、任意类型的元素
    tuple<string, int, double> student("Alice", 2024001, 89.5);

    // 访问元素
    cout << "姓名: " << get<0>(student) << endl;
    cout << "学号: " << get<1>(student) << endl;
    cout << "成绩: " << get<2>(student) << endl;

    // 使用 tie 解包
    string name;
    int id;
    double score;
    tie(name, id, score) = student;
    cout << "解包: " << name << ", " << id << ", " << score << endl;

    // 使用 ignore 忽略不需要的字段
    tie(name, ignore, score) = student;
    cout << "只取姓名和成绩: " << name << ", " << score << endl;

    // 元组比较（按元素顺序逐个比较）
    auto t1 = make_tuple(1, string("a"), 3.0);
    auto t2 = make_tuple(1, string("b"), 2.0);
    cout << "t1 < t2: " << boolalpha << (t1 < t2) << endl;

    return 0;
}
```

### `tuple` vs `struct`

| | `tuple` | `struct` |
|---|---|---|
| 访问方式 | `get<N>(t)` | `s.member_name` |
| 可读性 | 差（无名字） | 好（有语义化的成员名） |
| 通用性 | 高（泛型代码友好） | 低（每个 struct 是独立的类型） |
| 何时用 | 临时聚合、模板元编程 | 领域建模、长期维护的代码 |

> **经验法则：** 如果数据的含义在上下文中显而易见（如函数返回多值），用 `tuple`；如果数据需要长期维护和理解，用 `struct`。

---

## 四、`bitset` — 位集合

```cpp {cmd=true}
#include <iostream>
#include <bitset>
using namespace std;

int main() {
    // bitset：固定大小的位集合
    bitset<8> bs1;                       // 00000000
    bitset<8> bs2(42);                   // 00101010 (42 的二进制)
    bitset<8> bs3(string("1100"));       // 00001100

    cout << "bs1 = " << bs1 << endl;
    cout << "bs2 = " << bs2 << endl;
    cout << "bs3 = " << bs3 << endl;

    // 位操作
    bs1.set(3);                // 设置第3位 → 00001000
    bs1.flip(0);               // 翻转第0位 → 00001001
    cout << "bs1 操作后 = " << bs1 << endl;

    // 查询
    cout << "bs2 中置1的位数: " << bs2.count() << endl;
    cout << "bs2 第1位是否为1: " << bs2.test(1) << endl;
    cout << "bs2 是否有置1的位: " << (bs2.any() ? "是" : "否") << endl;

    // 转换
    cout << "bs2 转 unsigned long: " << bs2.to_ulong() << endl;
    cout << "bs2 转 string: " << bs2.to_string() << endl;

    // 实际应用：权限管理
    enum { READ = 0, WRITE = 1, EXECUTE = 2 };
    bitset<3> permissions;
    permissions.set(READ);
    permissions.set(WRITE);
    cout << "权限: " << permissions << " (读+写)" << endl;

    return 0;
}
```

---

## 五、`union` — 节省空间的联合体

```cpp {cmd=true}
#include <iostream>
#include <string>
using namespace std;

// C++11: union 可以包含有构造/析构函数的类型
union Token {
    int ival;
    double dval;
    char cval;
    // string sval;  // string 有非平凡构造/析构 — 需要特殊处理

    // 需要手动管理有构造/析构的成员
    Token() : ival(0) {}          // 默认初始化为 0
    ~Token() {}                   // 不自动调用 string 的析构
};

int main() {
    Token t;
    t.ival = 42;
    cout << "t.ival = " << t.ival << endl;

    t.dval = 3.14;
    cout << "t.dval = " << t.dval << endl;
    // ⚠️ t.ival 现在无效——union 所有成员共享同一块内存

    cout << "sizeof(Token) = " << sizeof(Token) << " bytes"
         << " (int=" << sizeof(int)
         << ", double=" << sizeof(double) << ")" << endl;

    return 0;
}
```

---

## 六、`mem_fn` — 将成员函数转为可调用对象

```cpp {cmd=true}
#include <iostream>
#include <vector>
#include <string>
#include <functional>
#include <algorithm>
using namespace std;

int main() {
    vector<string> words{"hello", "", "world", "", "C++"};

    // 问题：如何统计空字符串的数量？
    // 错误：不能直接传成员函数指针
    // auto cnt1 = count_if(words.begin(), words.end(), &string::empty);

    // ✅ C++11: 用 mem_fn 包装成员函数
    auto cnt = count_if(words.begin(), words.end(), mem_fn(&string::empty));
    cout << "空字符串数量: " << cnt << endl;

    // ✅ C++11: 也可以用 lambda（更现代、更推荐）
    auto cnt2 = count_if(words.begin(), words.end(),
                         [](const string &s) { return s.empty(); });
    cout << "lambda 方式: " << cnt2 << endl;

    return 0;
}
```

---

## 七、补充总结

### 核心要点

| 知识点 | 关键结论 |
|---|---|
| `dynamic_cast` | 安全的向下转型；指针版本失败返回 `nullptr`，引用版本抛 `bad_cast` |
| `typeid` | 获取运行时类型，多用于调试 |
| `enum class` | 强类型枚举——不污染作用域、不能隐式转换、可指定底层类型 |
| `tuple` | 泛化 pair；用 `get<N>()` 访问；用 `tie` 解包；`ignore` 跳过字段 |
| `bitset` | 编译期固定大小的位集合；支持位操作、计数、转换 |
| `union` | 节省空间；C++11 支持有构造/析构的成员（需手动管理） |
| `mem_fn` | 将成员函数转为可调用对象；但通常 lambda 更直观 |

### 何时使用 RTTI？

```
需要根据运行时类型调用不同行为？
├── 是 → 能用虚函数解决？
│        ├── 是 → 用虚函数（最干净）
│        └── 否 → 需要访问派生类特有接口？
│                 ├── 是 → dynamic_cast
│                 └── 否 → 重新考虑设计
└── 否 → 不需要 RTTI
```

> **C++ Core Guidelines:** 优先使用虚函数分发，RTTI 是最后的手段。过度依赖 `dynamic_cast` 和 `typeid` 通常意味着设计需要改进。
