# hw5 — 模板类 Vec 与动态内存

[← 返回目录](../作业总结.md)

## 📋 任务总结

在 hw4 的基础上，实现**模板类 `Vec<T>`** 来替代 `std::vector`。`Vec` 是简化版动态数组，需手动管理内存（`new[]`/`delete[]`）、实现拷贝控制（三法则）、提供迭代器接口。

同时增加字母等级评定：A(90-100)、B(80-89)、C(70-79)、D(60-69)、F(<60)。

## 💡 关键算法思想

### 1. RAII（资源获取即初始化）

构造函数分配内存，析构函数释放内存。对象生命周期 = 资源持有期。编译器自动在作用域结束时调用析构函数——这是 C++ 无 GC 却能避免内存泄漏的核心机制。

```cpp
// Vec.h — RAII 的完整体现
template<class T>
class Vec {
public:
    Vec() { elements = 0; sz = 0; space = 0; }   // 构造：初始化为空

    Vec(const Vec& v) {                             // 拷贝构造：深拷贝
        sz = v.sz;
        space = v.sz;
        elements = new T[space];                    // 分配新内存
        for (size_type i = 0; i < sz; ++i)
            elements[i] = v.elements[i];
    }

    ~Vec() { delete[] elements; }                   // 析构：自动释放
```

### 2. 三法则（Rule of Three）

一旦自定义了析构函数（管理动态内存），必须同时自定义拷贝构造和赋值运算符。浅拷贝会导致 double-free 崩溃。

```cpp
    Vec& operator=(const Vec& v) {
        if (this == &v) return *this;               // 自赋值检查——经典细节！
        delete[] elements;                           // 释放旧内存
        sz = v.sz;
        space = v.sz;
        elements = new T[space];                    // 分配新内存，深拷贝
        for (size_type i = 0; i < sz; ++i)
            elements[i] = v.elements[i];
        return *this;
    }
```

### 3. 指数增长策略的均摊 O(1)

`push_back` 时 capacity 翻倍（×2），虽然某次插入触发 O(n) 扩容复制，但 n 次插入总复杂度 O(n)，均摊每次 O(1)。

```cpp
    void push_back(const T& val) {
        if (sz == space) {
            size_type new_space = (space == 0) ? 1 : space * 2;  // 翻倍增长
            T* new_elements = new T[new_space];
            for (size_type i = 0; i < sz; ++i)
                new_elements[i] = elements[i];
            delete[] elements;
            elements = new_elements;
            space = new_space;
        }
        elements[sz++] = val;
    }
```

## 🔗 知识点分析

| 知识点 | 教程章节 |
|--------|---------|
| 模板类基础 `Vec<T>` 的设计 | [一、模板类基础：Vec\<T\> 的设计](../教程/06-定义抽象数据类型-模板类（完整教学版）.md#一模板类基础vect-的设计) |
| `new` 和 `delete` 动态内存 | [二、new 和 delete — C++ 的动态内存](../教程/06-定义抽象数据类型-模板类（完整教学版）.md#二new-和-delete--c-的动态内存) |
| 拷贝控制 — 三法则 | [三、拷贝控制 — 三个关键操作](../教程/06-定义抽象数据类型-模板类（完整教学版）.md#三拷贝控制--三个关键操作) |
| `explicit` 防止隐式转换 | [四、explicit 关键字](../教程/06-定义抽象数据类型-模板类（完整教学版）.md#四explicit-关键字) |
| 迭代器 `iterator`/`const_iterator` | [一、从 vector 到迭代器](../教程/03-使用序列式容器和迭代器（完整教学版）.md#一从-vector-到迭代器) |

📂 **文件：** `Vec.h`（核心，模板类完整实现）、`Student_info.cpp/h`、`grade.cpp/h`、`median.h`、`main.cpp`

[← 返回目录](../作业总结.md)
