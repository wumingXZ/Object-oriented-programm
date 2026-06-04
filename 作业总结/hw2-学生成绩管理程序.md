# hw2 — 学生成绩管理程序（基础版）

[← 返回目录](../作业总结.md)

## 📋 任务总结

编写管理学生成绩的程序。从标准输入读取学生记录（姓名、期中成绩、期末成绩、家庭作业成绩），按字母顺序排列学生姓名，并区分及格与不及格学生分别输出。

**成绩计算公式：**

```
总成绩 = 0.2 × 期中 + 0.4 × 期末 + 0.4 × (0.6 × 作业平均分 + 0.4 × 作业中值)
```

程序使用 `list` 容器存储学生数据，涉及结构体定义、排序、中值/平均值计算、异常处理等基础概念。

## 💡 关键算法思想

### 1. 加权中值抵御极端值

成绩公式中 `0.6 × 平均分 + 0.4 × 中值` 的设计是一个经典策略——中值对离群点（一次极低作业分）不敏感，平均值反映整体趋势，两者结合比单纯平均更鲁棒。

```cpp
// grade.cpp — 最终的加权成绩计算
double grade(double midterm, double final, const std::vector<double> & hw) {
    if (hw.size() == 0) {
        throw domain_error("Student doesn't finish any homework.");
    }
    double fhw = 0.4 * median(hw) + 0.6 * average(hw);  // 中值+均值混合
    return grade(midterm, final, fhw);
}
```

### 2. 为什么选 `list` 而非 `vector`？

区分及格/不及格学生时需要从容器中间删除元素。`vector` 删除中间元素是 O(n)（后续元素全部前移），而 `list` 是 O(1)（只改两个指针）。这是"为操作选容器"的 STL 核心哲学。

```cpp
// main.cpp — list 的 O(1) 删除 vs vector 的 O(n)
list<Student_info> extract_fails(list<Student_info> & students) {
    list<Student_info> fail;
    auto iter = students.begin();
    while (iter != students.end()) {
        if (fgrade(*iter)) {
            fail.push_back(*iter);
            iter = students.erase(iter);  // list: O(1), 返回下一个有效迭代器
        } else {
            ++iter;
        }
    }
    return fail;
}
```

### 3. 分文件编译：接口与实现分离

`.h` 声明接口（what），`.cpp` 实现细节（how）。修改实现只需重新编译一个 `.cpp`，接口不变则其他文件无需重编译。

```cpp
// Student_info.h — 只声明接口
struct Student_info {
    std::string name;
    double midterm, final;
    std::vector<double> homework;
    bool operator<(const Student_info &n) const {
        return name < n.name;
    }
};
std::istream & read(std::istream & is, Student_info &s);
```

## 🔗 知识点分析

| 知识点 | 教程章节 |
|--------|---------|
| `struct` 组织学生数据 | [五、结构体：组织相关数据](../教程/02-组织程序和数据（完整教学版）.md#五结构体组织相关数据) |
| `list` 容器的高效插入删除 | [三、list — 高效插入和删除](../教程/03-使用序列式容器和迭代器（完整教学版）.md#三list--高效插入和删除) |
| `sort` 排序与自定义比较谓词 | [五、结构体：组织相关数据](../教程/02-组织程序和数据（完整教学版）.md#五结构体组织相关数据) |
| `median` 中值计算 | [1.3 查找中值的函数](../教程/02-组织程序和数据（完整教学版）.md#13-查找中值的函数) |
| 异常处理 `try-catch`、`domain_error` | [二、异常处理 try-catch](../教程/02-组织程序和数据（完整教学版）.md#二异常处理-try-catch) |
| 分文件编译（.h / .cpp） | [六、分文件编译](../教程/02-组织程序和数据（完整教学版）.md#六分文件编译) |

📂 **文件：** `median.cpp/h`、`grade.cpp/h`、`Student_info.cpp/h`、`main.cpp`

[← 返回目录](../作业总结.md)
