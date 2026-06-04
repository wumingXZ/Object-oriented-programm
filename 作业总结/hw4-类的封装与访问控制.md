# hw4 — 类的封装与访问控制

[← 返回目录](../作业总结.md)

## 📋 任务总结

将原来 hw2/hw3 中的 `Student_info` **结构体改造为完整的类（class）**：数据成员改为 `private`，提供公开的成员函数，支持默认构造函数和 `istream` 构造函数。这是从"面向过程"到"面向对象"的关键转折点。

```
总成绩 = 0.2 × 期中 + 0.4 × 期末 + 0.4 × 作业中值
```

## 💡 关键算法思想

### 1. 封装 = 隐藏细节 + 保持不变式

数据成员 `private` 不是形式，而是强制所有修改都经过成员函数，从而保证对象始终处于有效状态。

```cpp
// Student_info.h — struct → class 的核心变化
class Student_info {
public:
    Student_info();                    // 默认构造函数
    Student_info(std::istream&);       // 从输入流构造

    std::istream& read(std::istream&);
    double grade() const;              // const: 不修改对象状态
    std::string name() const;
    bool valid() const;

private:                               // 数据隐藏
    std::string n;
    double midterm, final;
    std::vector<double> homework;
};
```

```cpp
// Student_info.cpp — 构造函数初始化列表保证初始状态
Student_info::Student_info() : midterm(0), final(0) {}  // 初始化列表

Student_info::Student_info(istream& is) {
    read(is);      // 委托给 read 函数
}
```

### 2. const 成员函数的契约语义

`name() const` 不仅告诉编译器"我不修改对象"，更是告诉使用者"调用我不会改变对象状态"。

```cpp
// 接口明确表达：name() 只读，grade() 只读
std::string Student_info::name() const { return n; }

double Student_info::grade() const {
    return ::grade(midterm, final, homework);  // :: 调用全局 grade
}
```

## 🔗 知识点分析

| 知识点 | 教程章节 |
|--------|---------|
| 从 `struct` 到 `class` 的转变 | [一、从 struct 到 class](../教程/05-定义新类型-类（完整教学版）.md#一从-struct-到-class) |
| `struct` 与 `class` 的区别 | [二、struct vs class](../教程/05-定义新类型-类（完整教学版）.md#二struct-vs-class) |
| 构造函数（默认/带参） | [三、构造函数](../教程/05-定义新类型-类（完整教学版）.md#三构造函数) |
| `const` 成员函数 | [四、const 成员函数](../教程/05-定义新类型-类（完整教学版）.md#四const-成员函数) |
| 访问控制 `public`/`private` | [一、从 struct 到 class](../教程/05-定义新类型-类（完整教学版）.md#一从-struct-到-class) |
| 内联函数 | [五、内联函数](../教程/05-定义新类型-类（完整教学版）.md#五内联函数) |

📂 **文件：** `Student_info.cpp/h`（核心）、`grade.cpp/h`、`median.cpp/h`、`main.cpp`

[← 返回目录](../作业总结.md)
