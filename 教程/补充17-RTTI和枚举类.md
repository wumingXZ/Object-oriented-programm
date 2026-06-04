# 补充17：RTTI、枚举类和实用库设施

> 基于 C++ Primer 第五版（英文版）第 17 章和第 19 章补充。
>
> **前置要求：** 已完成第 01–08 周全部内容。本文介绍运行时类型识别、强类型枚举、tuple 和 bitset——C++ 工具箱中的进阶利器。
>
> **关联已有知识：** 第 08 周（多态和虚函数——RTTI 的前提）、第 03 周（map 的 pair → tuple 泛化）、第 04 周（泛型算法结合 bitset）、补充 12（关联容器与 enum class 的配合）。
>

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

> **什么时候使用 `dynamic_cast`？** 当虚函数不足以表达你的类型分派逻辑时。这里的例子：`Animal` 有虚函数 `sound()`，但 `Dog::fetch()` 和 `Cat::climb()` 不是虚函数——它们是各自子类独有的接口。通过基类指针调用这些方法前，你必须先确认对象的真实类型。`dynamic_cast` 就是干这个的。

```cpp
class Animal {
public:
    virtual ~Animal() = default;
    virtual string sound() const = 0;
};

class Dog : public Animal {
public:
    string sound() const override { return "汪汪!"; }
    void fetch() { }
};

class Cat : public Animal {
public:
    string sound() const override { return "喵喵!"; }
    void climb() { }
};

vector<Animal*> zoo{new Dog, new Cat, new Dog};

for (auto *animal : zoo) {
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
```

> **`dynamic_cast` 的工作原理：** 它在运行时检查 `animal` 的实际类型（通过虚函数表信息），判断是否可以安全转型。如果可以，返回有效的派生类指针；如果不行，返回 `nullptr`。这听起来很强大，但需要付出运行时开销——`dynamic_cast` 比普通的 `static_cast` 慢（需要遍历继承体系）。
>
> **为什么不总是用 `dynamic_cast`？** 设计良好的面向对象程序应该优先考虑用**虚函数**来处理类型相关的行为差异。如果发现代码中大量使用 `dynamic_cast`，通常意味着继承体系的设计需要重构——可以把子类特有操作抽象为新的虚函数。

### 1.2 `dynamic_cast` 用于引用

```cpp
class Base {
public:
    virtual ~Base() = default;
};

class Derived : public Base {
public:
    void derived_only() { }
};

Derived d;
Base &ref = d;

// 对引用做 dynamic_cast：失败时抛 std::bad_cast
try {
    auto &dr = dynamic_cast<Derived&>(ref);
    dr.derived_only();
} catch (const bad_cast &e) {
    // dynamic_cast 失败
}
```

| | 指针版本 | 引用版本 |
|---|---|---|
| 失败行为 | 返回 `nullptr` | 抛出 `std::bad_cast` |
| 检查方式 | `if (auto *p = dynamic_cast<T*>(...))` | `try-catch` |

### 1.3 `typeid` — 获取运行时类型信息

> **`typeid` 返回一个 `type_info` 对象，它包含了类型运行时的完整信息。** 关键区别：`typeid(pb)` 返回的是指针本身的类型（`Base*`，编译期决定），而 `typeid(*pb)` 返回的是指针所指对象的实际类型（在运行时通过虚表决定）。这对多态类型的判断至关重要。

```cpp
class Base {
public:
    virtual ~Base() = default;
};

class Derived : public Base {};

Derived d;
Base *pb = &d;

// typeid 用于多态类型时，在运行时确定动态类型
// typeid(pb)      // 静态类型 Base*
// typeid(*pb)     // 动态类型 Derived

// 比较类型
if (typeid(*pb) == typeid(Derived)) {
    // *pb 是 Derived 类型
}

// 非多态类型：编译期决定
int i = 42;
// typeid(i).name()
```

> **`typeid` 和 `dynamic_cast` 的选择：**
> - 如果你只需要"检查对象是不是某种类型"，用 `typeid` 更简洁
> - 如果你需要"把这对象当作某种类型来操作"，用 `dynamic_cast`（它会同时检查和转型）
> - 如果你在写调试日志，用 `typeid(*p).name()`——但注意 `name()` 返回的实现定义的名字（不同编译器可能不同，例如 GCC 返回 `4Derived` 这样的 mangled name）

> **RTTI 使用原则：**
> - 优先用虚函数代替 RTTI（多态本身就是最好的类型分发机制）
> - `dynamic_cast` 用于需要访问派生类特有接口的场景
> - `typeid` 用于调试和日志，不要用来替代虚函数分发

---

## 二、强类型枚举 `enum class`（C++11）

> **为什么 C++11 要引入 `enum class`？** C++98 的 `enum` 有两大问题：**（1）名字污染**——`enum Color { Red, Green, Blue }` 中的 `Red` 直接进入外层作用域，你不能在同一个作用域中再定义另一个 `Red`；**（2）类型不安全**——`enum` 可以隐式转为 `int`，你可以在不需要枚举的地方意外使用它，编译器不会报错。C++11 的 `enum class` 同时解决了这两个问题。

```cpp
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

// 使用强类型枚举（需要作用域限定符）
Color c = Color::Red;
TrafficLight tl = TrafficLight::Green;

// int n = c;           // ❌ 不能隐式转换
int n = static_cast<int>(c);  // ✅ 必须显式转换

// 位掩码用法
Permission p = Permission::Read | Permission::Write;
```

> **`enum class` 三个核心优点详解：**
> 1. **作用域限定：** `Color::Red` 和 `TrafficLight::Red` 互不冲突——你必须显式写出枚举名作为前缀
> 2. **无隐式转换：** `if (c == 0)` 会编译错误——你必须写 `c == Color::Red` 或在必要时用 `static_cast<int>(c)` 显式转换
> 3. **指定底层类型：** `enum class Permission : unsigned char` 指定用 1 字节存储（默认是 `int`），在内存和序列化场景中非常重要。C++98 的枚举无法指定底层类型

| | C 风格 `enum` | C++11 `enum class` |
|---|---|---|
| 作用域 | 全局污染 | 枚举名限定 `Enum::Value` |
| 隐式转 int | ✅（不安全） | ❌ 必须 `static_cast` |
| 指定底层类型 | ❌ | ✅ `enum class X : uint8_t` |
| 前置声明 | ❌ | ✅ `enum class X;` |

| | C 风格 `enum` | C++11 `enum class` |
|---|---|---|
| 作用域 | 全局污染 | 枚举名限定 `Enum::Value` |
| 隐式转 int | ✅（不安全） | ❌ 必须 `static_cast` |
| 指定底层类型 | ❌ | ✅ `enum class X : uint8_t` |
| 前置声明 | ❌ | ✅ `enum class X;` |

---

## 三、`tuple` — 泛化的 `pair`

```cpp
// tuple：可以存放任意数量、任意类型的元素
tuple<string, int, double> student("Alice", 2024001, 89.5);

// 访问元素
get<0>(student);  // 姓名
get<1>(student);  // 学号
get<2>(student);  // 成绩

// 使用 tie 解包
string name;
int id;
double score;
tie(name, id, score) = student;

// 使用 ignore 忽略不需要的字段
tie(name, ignore, score) = student;

// 元组比较（按元素顺序逐个比较）
auto t1 = make_tuple(1, string("a"), 3.0);
auto t2 = make_tuple(1, string("b"), 2.0);
t1 < t2;
```

### `tuple` vs `struct`

| | `tuple` | `struct` |
|---|---|---|
| 访问方式 | `get<N>(t)` | `s.member_name` |
| 可读性 | 差（无名字） | 好（有语义化的成员名） |
| 通用性 | 高（泛型代码友好） | 低（每个 struct 是独立的类型） |
| 何时用 | 临时聚合、模板元编程 | 领域建模、长期维护的代码 |

> **`tuple` 的典型应用场景——函数返回多个值：**
> ```cpp
> // 不用 tuple（需要额外定义 struct）
> struct DivisionResult { int quotient; int remainder; };
> DivisionResult divide(int a, int b) { return {a / b, a % b}; }
>
> // 用 tuple（省去定义 struct 的步骤）
> tuple<int, int> divide2(int a, int b) { return {a / b, a % b}; }
> auto [q, r] = divide2(10, 3);  // C++17 structured binding
> ```
>
> C++17 的 structured binding（结构化绑定）让 `tuple` 的使用体验大幅提升——`auto [q, r] = ...` 直接把元组元素赋给命名变量。
>
> **经验法则：** 如果数据的含义在上下文中显而易见（如函数返回多值），用 `tuple`；如果数据需要长期维护和理解，用 `struct`。

---

## 四、`bitset` — 位集合

```cpp
// bitset：固定大小的位集合
bitset<8> bs1;                       // 00000000
bitset<8> bs2(42);                   // 00101010 (42 的二进制)
bitset<8> bs3(string("1100"));       // 00001100

// 位操作
bs1.set(3);                // 设置第3位
bs1.flip(0);               // 翻转第0位

// 查询
bs2.count();               // 置1的位数
bs2.test(1);               // 第1位是否为1
bs2.any();                 // 是否有置1的位

// 转换
bs2.to_ulong();
bs2.to_string();

// 实际应用：权限管理
enum { READ = 0, WRITE = 1, EXECUTE = 2 };
bitset<3> permissions;
permissions.set(READ);
permissions.set(WRITE);
```

---

## 五、`union` — 节省空间的联合体

> **`union` 的核心思想：** 所有成员共享同一块内存——同一时刻只有一个成员是"活跃的"。`union` 的大小等于其最大成员的大小。这种设计在内存受限的场景（嵌入式系统、协议解析）中非常有用，但同时也带来了手动管理的负担——你必须自己记录当前哪个成员是活跃的。

```cpp
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

Token t;
t.ival = 42;
// ⚠️ t.ival 现在无效——union 所有成员共享同一块内存
t.dval = 3.14;
```

> **使用 `union` 的三个关键注意事项：**
> 1. **读取非活跃成员是未定义行为**——如果你写入了 `t.ival = 42` 然后读取 `t.dval`，结果是不可预测的（虽然很多编译器不会报错）
> 2. **C++11 允许非平凡类型的成员**（如 `string`），但需要你手动调用其构造和析构函数——这在实践中非常容易出错
> 3. **现代 C++ 倾向用 `std::variant`（C++17）替代 `union`**——`variant` 自动追踪当前活跃成员，类型安全
>
> 除非你在写底层系统代码或需要极致的内存节省，否则请优先考虑 `std::variant` 或继承体系而不是 `union`。

## 六、`mem_fn` — 将成员函数转为可调用对象

---

## 六、`mem_fn` — 将成员函数转为可调用对象

```cpp
vector<string> words{"hello", "", "world", "", "C++"};

// 问题：如何统计空字符串的数量？
// 错误：不能直接传成员函数指针
// auto cnt1 = count_if(words.begin(), words.end(), &string::empty);

// ✅ C++11: 用 mem_fn 包装成员函数
auto cnt = count_if(words.begin(), words.end(), mem_fn(&string::empty));

// ✅ C++11: 也可以用 lambda（更现代、更推荐）
auto cnt2 = count_if(words.begin(), words.end(),
                     [](const string &s) { return s.empty(); });
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
