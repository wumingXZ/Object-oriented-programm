# hw7 — 继承与多态

[← 返回目录](../作业总结.md)

## 📋 任务总结

实现学生成绩管理系统的**继承体系**，是整个课程作业的最高潮：

- **`Core` 类**（基类）：本科生，成绩 = 课程考试成绩
- **`Grad` 类**（派生类）：研究生，成绩 = min(课程成绩, 毕业论文分数)
- **`Student_info` 类**（Handle）：持有 `Core*` 指针，通过 `clone()` 实现多态拷贝

按成绩**从高到低**排序输出所有学生。

## 💡 关键算法思想

### 1. Handle 类模式（Bridge 变体）

`Student_info` 持有 `Core*` 指针，对外提供值语义（拷贝、赋值像普通对象），对内管理指针生命周期。这是 C++ 中弥合"高效多态"与"简单接口"裂痕的经典方案。

```cpp
// Student_info.h — 值语义的多态包装器
class Student_info {
public:
    Student_info();                         // cp = 0
    Student_info(std::istream&);            // 从输入流构造
    Student_info(const Student_info&);      // 拷贝：调用 clone()
    ~Student_info();                        // delete cp
    Student_info& operator=(const Student_info&);  // 赋值：先释放再 clone()

    std::string name() const;
    double grade() const;
    std::istream& read(std::istream&);      // 工厂方法：根据输入创建 Core/Grad

private:
    Core* cp;                               // 指向基类的指针——多态的关键
};
```

### 2. 原型模式——让基类不知道如何拷贝派生类

`clone()` 虚函数让每个子类负责复制自己。`Student_info` 只需调用 `cp->clone()`，不关心具体类型。关键洞察：构造函数不能是虚函数，但 `clone()` 可以——用虚函数间接实现"虚构造"。

```cpp
// Core.h — 基类的 clone
class Core {
public:
    virtual Core* clone() const { return new Core(*this); }  // 协变返回类型
};

// Grad.h — 派生类的 clone
class Grad : public Core {
public:
    Grad* clone() const { return new Grad(*this); }          // 返回 Grad*
};

// Student_info.cpp — Handle 通过 clone 实现多态拷贝
Student_info::Student_info(const Student_info& s) : cp(0) {
    if (s.cp) cp = s.cp->clone();  // 不关心是 Core 还是 Grad！
}

Student_info& Student_info::operator=(const Student_info& s) {
    if (this != &s) {
        delete cp;                  // 释放旧对象
        cp = 0;
        if (s.cp) cp = s.cp->clone();  // 多态复制新对象
    }
    return *this;
}
```

### 3. 工厂方法——运行时决定创建哪个派生类

```cpp
// Student_info.cpp — 根据输入首字符决定类型
istream& Student_info::read(istream& in) {
    delete cp;           // 先释放旧对象
    cp = 0;
    char ch;
    in >> ch;
    if (ch == 'U')
        cp = new Core;   // 本科生
    else if (ch == 'G')
        cp = new Grad;   // 研究生

    if (cp)
        cp->read(in);    // 多态调用 read()
    return in;
}
```

### 4. 静态绑定 vs 动态绑定

```cpp
// Core.h — 比较函数的两种版本
bool compare(const Core &x, const Core &y);          // 静态绑定（引用）
bool compare_grade(const Core &x, const Core &y);    // 静态绑定
bool compare_grade_ptr(const Core *x, const Core *y); // 动态绑定（指针+虚函数）
```

## 🔗 知识点分析

| 知识点 | 教程章节 |
|--------|---------|
| 面向对象编程三大基石 | [一、面向对象编程的核心思想](../教程/08-继承和动态绑定（完整教学版）.md#一面向对象编程的核心思想) |
| 保护标签 `protected` | [三、保护标签 protected](../教程/08-继承和动态绑定（完整教学版）.md#三保护标签-protected) |
| 虚函数与动态绑定 | [四、虚函数与动态绑定 — 核心机制](../教程/08-继承和动态绑定（完整教学版）.md#四虚函数与动态绑定--核心机制) |
| 继承解决学生成绩问题（完整程序） | [五、使用继承解决学生成绩问题（完整程序）](../教程/08-继承和动态绑定（完整教学版）.md#五使用继承解决学生成绩问题完整程序) |
| 虚析构函数 | [六、虚析构函数](../教程/08-继承和动态绑定（完整教学版）.md#六虚析构函数) |
| 纯虚函数和抽象类 | [七、纯虚函数和抽象类](../教程/08-继承和动态绑定（完整教学版）.md#七纯虚函数和抽象类) |
| Handle 类模式 + clone 原型模式 | [五、使用继承解决学生成绩问题（完整程序）](../教程/08-继承和动态绑定（完整教学版）.md#五使用继承解决学生成绩问题完整程序) |

📂 **文件：** `Core.cpp/h`（基类）、`Grad.cpp/h`（派生类）、`Student_info.cpp/h`（Handle）、`grade.cpp/h`、`median.h`、`Vec.h`、`main.cpp`

[← 返回目录](../作业总结.md)
