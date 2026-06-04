# hw6 — 运算符重载（Str 类）

[← 返回目录](../作业总结.md)

## 📋 任务总结

基于模板类 `Vec<char>` 实现**自定义字符串类 `Str`**，替代 `std::string`。要求重载：
- `operator+=` — 追加字符串/字符
- `operator+` — 三种重载：`Str+Str`、`Str+char*`、`char*+Str`
- `operator<<` / `operator>>` — 流插入/提取

程序最终输出一个带边框的问候信息。

## 💡 关键算法思想

### 1. 组合优于继承

`Str` 内部包含一个 `Vec<char> data`，而不是继承 `Vec<char>`。组合让你只复用需要的能力，不被基类的所有接口绑架。

```cpp
// Str.h — 组合：包含 Vec<char>，而非继承它
class Str {
public:
    typedef Vec<char>::size_type size_type;

    Str() {}
    Str(size_type n, char c) : data(n, c) {}     // 委托给 Vec 的构造函数
    Str(const char* cp);                           // 从 C 字符串构造

    size_type size() const { return data.size(); } // 转发给 Vec

    char& operator[](size_type i) { return data[i]; }
    const char& operator[](size_type i) const { return data[i]; }

private:
    Vec<char> data;                                // 组合，不是继承
};
```

### 2. 运算符重载的对称性设计

`operator+` 必须用非成员函数实现——`char* + Str` 的左侧是 C 字符串，不可能调用 `Str` 的成员函数。非成员函数保证了加法的交换律对混合类型也成立。

```cpp
// Str.cpp — 成员函数实现 +=（修改自身）
Str& Str::operator+=(const Str& rhs) {
    for (size_type i = 0; i != rhs.size(); ++i)
        data.push_back(rhs[i]);
    return *this;
}

// 非成员函数实现 +（返回新对象，支持混合类型）
Str operator+(const Str& lhs, const Str& rhs) {
    Str ret = lhs;
    ret += rhs;               // 委托给 +=
    return ret;
}

Str operator+(const char* lhs, const Str& rhs) {  // char* 在左边也能工作！
    Str ret(lhs);
    ret += rhs;
    return ret;
}
```

### 3. 流运算符的友元模式

```cpp
// operator<< 不需要友元（只调用 public 接口 size() 和 operator[]）
std::ostream& operator<<(std::ostream& os, const Str& s) {
    for (Str::size_type i = 0; i != s.size(); ++i)
        os << s[i];
    return os;
}

// operator>> 使用 iss pace + unget 模式读取一个单词
std::istream& operator>>(std::istream& is, Str& s) {
    s = Str();
    char c;
    while (is.get(c) && isspace(static_cast<unsigned char>(c)))  // 跳过前导空白
        ;
    if (is) {
        do { s += c; }
        while (is.get(c) && !isspace(static_cast<unsigned char>(c)));
        if (is) is.unget();  // 把不属于当前单词的字符放回去
    }
    return is;
}
```

## 🔗 知识点分析

| 知识点 | 教程章节 |
|--------|---------|
| 操作符重载基本概念 | [二、基本概念](../教程/07-操作符重载-实现String类（完整教学版）.md#二基本概念) |
| 一个简单的 String 类实现 | [三、一个简单的 String 类](../教程/07-操作符重载-实现String类（完整教学版）.md#三一个简单的-string-类) |
| 输入运算符 `>>` 与友元函数 | [四、输入运算符 operator\>\> 与友元](../教程/07-操作符重载-实现String类（完整教学版）.md#四输入运算符-operator-与友元) |
| 加法运算符 `+` 和 `+=` | [五、加法运算符 + 和 +=](../教程/07-操作符重载-实现String类（完整教学版）.md#五加法运算符--和-) |
| 基于 `Vec<char>` 的组合复用 | [一、模板类基础：Vec\<T\> 的设计](../教程/06-定义抽象数据类型-模板类（完整教学版）.md#一模板类基础vect-的设计) |

📂 **文件：** `Str.cpp/h`（核心）、`Vec.h`、`main.cpp`

[← 返回目录](../作业总结.md)
