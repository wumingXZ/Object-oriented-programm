# lab4 — 库存管理系统（类设计）

[← 返回目录](../作业总结.md)

## 📋 任务总结

设计 `goods`（商品名称+数量）和 `stock`（库存管理）两个类，支持进货（purchase）、出货（sell）、打印（print）。库存降至 0 警告，小于 0 抛异常。

## 💡 关键算法思想

### 1. 错误处理的分级策略

```cpp
// goods.h — 两级错误处理：警告 vs 异常
void sell(const goods &g) {
    for (int i = 0; i < s.size(); ++i) {
        if (s[i].name == g.name) {
            s[i].num -= g.num;
            if (s[i].num < 0) {
                throw domain_error("Error: insufficient inventory");  // 硬错误
            } else if (s[i].num == 0) {
                cout << "Warning: Inventory is 0" << endl;            // 软警告
            }
        }
    }
}
```

### 2. 进货的去重合并逻辑

```cpp
void purchase(const goods &g) {
    int i = 0;
    for (; i < s.size(); ++i) {
        if (s[i].name == g.name) {
            s[i].num += g.num;    // 已有商品：合并数量
            break;
        }
    }
    if (i == s.size()) {
        s.push_back(g);           // 新商品：直接添加
    }
}
```

### 3. Lambda 排序按数量降序

```cpp
void print() const {
    sort(s.begin(), s.end(),
         [](goods a, goods b) { return a.num > b.num; });  // 降序排列
    for (int i = 0; i < s.size(); ++i)
        cout << s[i].name << "\t" << s[i].num << endl;
}
```

## 🔗 知识点分析

| 知识点 | 教程章节 |
|--------|---------|
| 类的设计与封装 | [一、从 struct 到 class](../教程/05-定义新类型-类（完整教学版）.md#一从-struct-到-class) |
| 异常处理 `try-catch` | [二、异常处理进阶](../教程/补充16-命名空间和高级异常处理.md#二异常处理进阶) |
| `domain_error` 标准异常 | [三、自定义异常类](../教程/补充16-命名空间和高级异常处理.md#三自定义异常类) |
| Lambda 排序 | [四、Lambda 表达式 — C++11 的函数对象](../教程/04-泛型函数和库算法（完整教学版）.md#四lambda-表达式--c11-的函数对象) |

📂 **文件：** `goods.h`（两个类的完整实现）、`main.cpp`

[← 返回目录](../作业总结.md)
