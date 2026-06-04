# lab2 — 字符串处理与栈计算器

[← 返回目录](../作业总结.md)

## 📋 任务总结

三个独立的编程练习：
1. **split 函数**：将字符串按空白符分割为单词列表
2. **frame 函数**：为字符串列表加上星号边框
3. **逆波兰表达式（RPN）计算器**：用 `vector` 模拟栈求值

## 💡 关键算法思想

### 1. RPN 后缀表达式——用栈消除优先级

中缀 `(3+4)×5` 需要括号和优先级，后缀 `3 4 + 5 ×` 只需要从左到右扫描 + 一个栈。遇到数字压栈，遇到运算符弹两个、算完压回去。O(n) 时间复杂度。

```cpp
// lab2/test2_3.cpp — RPN 求值的核心逻辑
void calcu() {
    vector<double> exp;
    string s;
    while (cin >> s) {
        if (!isop(s)) {
            exp.push_back(stod(s));           // 数字：压栈
        } else {
            double right = exp.back();        // 弹右操作数
            exp.pop_back();
            double left = exp.back();         // 弹左操作数
            exp.pop_back();
            double ret = myop(left, right, s); // 计算
            exp.push_back(ret);               // 结果压回栈
        }
    }
    cout << exp.front() << endl;              // 栈顶即答案
}
```

### 2. 操作分发的函数表模式

```cpp
// 用 if-else 链分发运算符——简洁但可扩展为 map<string, function>
double myop(const double & a, const double & b, const string & op) {
    if (op == "+") return a + b;
    if (op == "-") return a - b;
    if (op == "*") return a * b;
    if (op == "/") {
        if (b == 0) throw runtime_error("除数不能为0");
        return a / b;
    }
    throw "无效操作符";
}
```

## 🔗 知识点分析

| 知识点 | 教程章节 |
|--------|---------|
| `string` 类型的基本操作 | [三、字符串类型 std::string](../教程/01-批量使用数据（完整教学版）.md#三字符串类型-stdstring) |
| `vector` 容器用作栈 | [四、批量数据处理 — vector 容器](../教程/01-批量使用数据（完整教学版）.md#四批量数据处理--vector-容器) |
| 栈（stack）数据结构原理 | [4.1 stack — 栈](../教程/补充12-关联容器深入.md#41-stack--栈) |

📂 **文件：** `test2/test2_1.cpp`、`test2/test2_2.cpp`、`test2/test2_3.cpp`

[← 返回目录](../作业总结.md)
