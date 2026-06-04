# lab3 — 关联容器与 Lambda 表达式

[← 返回目录](../作业总结.md)

## 📋 任务总结

三个练习聚焦 STL 容器和算法的综合应用：
1. **单词计数**：`map<string, int>` 统计词频
2. **交叉引用表**：`map<string, set<int>>` 记录行号
3. **Lambda 与 STL 算法**：`transform`、`copy_if`、`sort`、`for_each`、`accumulate`

## 💡 关键算法思想

### 1. Map-Reduce 模式

单词计数 = `map` 分组 + 算法累加。核心思想：用关联容器将逐条处理转为按 key 聚合。

```cpp
// lab3/test3_3.cpp — 算法链：transform → copy_if → sort → accumulate
void task1() {
    vector<int> numbers = {3, 5, 2, 8, 1, 10};

    // transform: 每个元素 ×2
    vector<int> doubled;
    transform(numbers.begin(), numbers.end(),
              back_inserter(doubled),
              [](int x) { return 2 * x; });

    // copy_if: 过滤 >5
    vector<int> filtered;
    copy_if(numbers.begin(), numbers.end(),
            back_inserter(filtered),
            [](int x) { return x > 5; });

    // sort: 按与5的差值排序
    sort(numbers.begin(), numbers.end(),
         [](int a, int b) { return abs(a - 5) < abs(b - 5); });
}
```

### 2. Lambda 作为"当场写的函数对象"

捕获列表让 Lambda 能访问外部变量，编译器自动生成匿名函数对象。

```cpp
void task2() {
    vector<Student> students = {{"Alice", 85}, {"Bob", 72}, {"Charlie", 90}};

    // Lambda 作为 sort 的比较谓词
    sort(students.begin(), students.end(),
         [](const Student &a, const Student &b) { return a.score > b.score; });

    // Lambda 作为 for_each 的过滤逻辑
    for_each(students.begin(), students.end(),
             [](const Student &a) { if (a.score > 80) cout << a.name << endl; });
}
```

### 3. accumulate 的自定义二元操作

```cpp
void task3() {
    vector<int> numbers = {3, 5, 2, 8, 1, 10};
    // accumulate: 累积偶数乘积（初始值 1 是乘法单位元）
    int pdt = accumulate(numbers.begin(), numbers.end(), 1,
                         [](int acc, int x) { return (x % 2 == 0) ? acc * x : acc; });
}
```

## 🔗 知识点分析

| 知识点 | 教程章节 |
|--------|---------|
| `map` 关联容器 | [四、map — 关联容器](../教程/03-使用序列式容器和迭代器（完整教学版）.md#四map--关联容器) |
| `set` 不重复集合 | [一、set — 不重复元素的集合](../教程/补充12-关联容器深入.md#一set--不重复元素的集合) |
| Lambda 表达式语法与捕获 | [四、Lambda 表达式 — C++11 的函数对象](../教程/04-泛型函数和库算法（完整教学版）.md#四lambda-表达式--c11-的函数对象) |
| `copy_if` / `remove_if` 条件过滤 | [3.2 remove_copy_if 和 remove_if](../教程/04-泛型函数和库算法（完整教学版）.md#32-remove_copy_if-和-remove_if--条件删除) |
| `transform` 转换算法 | [3.3 transform — 转换](../教程/04-泛型函数和库算法（完整教学版）.md#33-transform--转换) |
| `accumulate` 累积求和 | [3.4 accumulate — 累积求和](../教程/04-泛型函数和库算法（完整教学版）.md#34-accumulate--累积求和) |

📂 **文件：** `test3_1.cpp`、`test3_2.cpp`、`test3_3.cpp`

[← 返回目录](../作业总结.md)
