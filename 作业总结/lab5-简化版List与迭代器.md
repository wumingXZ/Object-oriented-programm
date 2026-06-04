# lab5 — 简化版 List 与迭代器

[← 返回目录](../作业总结.md)

## 📋 任务总结

编写简化版 `myList<T>` 模板类及 `Iterator<T>` 模板类。基于双向循环链表，支持 `push_back`、`print`、`sort`（冒泡），手动管理内存。

## 💡 关键算法思想

### 1. 哨兵节点——让边界条件消失

双向循环链表中，哨兵节点（senti）既是头也是尾。空链表 = 哨兵指向自己，所有操作统一为"在某个节点前后插入/删除"，不再需要 `if (head == nullptr)` 的特殊判断。

```cpp
// lab5/main.cpp — 哨兵节点初始化
template <class T>
class myList {
private:
    ListNode<T> senti;       // 哨兵节点，不存数据
    size_t length;

public:
    myList() : length(0) {
        senti.prev = &senti;  // 空链表：哨兵指向自己
        senti.next = &senti;
    }

    Iterator<T> begin() { return Iterator<T>(senti.next); }
    Iterator<T> end()   { return Iterator<T>(&senti); }     // end = 哨兵本身
```

### 2. push_back 的指针操作

```cpp
    void push_back(const T & val) {
        ListNode<T>* N = new ListNode<T>;
        N->data = val;
        senti.prev->next = N;   // 原尾节点 → 新节点
        N->prev = senti.prev;   // 新节点 ← 原尾节点
        N->next = &senti;       // 新节点 → 哨兵
        senti.prev = N;         // 哨兵 ← 新节点
        length++;
    }
```

### 3. 迭代器——容器与算法的粘合剂

`Iterator` 封装 `ListNode*` 指针，让 `for (auto it = lst.begin(); it != lst.end(); ++it)` 的遍历语法与 `vector` 完全一致。

```cpp
// 迭代器类——把指针操作抽象为 ++ / * / !=
template <typename T>
class Iterator {
private:
    ListNode<T>* node;
public:
    Iterator(ListNode<T>* n = NULL) : node(n) {}

    T& operator*() { return node->data; }
    Iterator& operator++() { node = node->next; return *this; }
    Iterator& operator--() { node = node->prev; return *this; }
    bool operator!=(const Iterator& other) { return node != other.node; }
};
```

### 4. 析构函数释放整个链表

```cpp
    ~myList() {
        ListNode<T> * del = senti.next;
        ListNode<T> * temp = del;
        for (int i = 0; i < length; ++i) {
            del = del->next;
            delete temp;           // 逐个释放节点
            temp = del;
        }
    }
```

## 🔗 知识点分析

| 知识点 | 教程章节 |
|--------|---------|
| 模板类设计 | [一、模板类基础：Vec\<T\> 的设计](../教程/06-定义抽象数据类型-模板类（完整教学版）.md#一模板类基础vect-的设计) |
| 自定义迭代器 | [一、从 vector 到迭代器](../教程/03-使用序列式容器和迭代器（完整教学版）.md#一从-vector-到迭代器) |
| `new`/`delete` 动态内存 | [二、new 和 delete — C++ 的动态内存](../教程/06-定义抽象数据类型-模板类（完整教学版）.md#二new-和-delete--c-的动态内存) |
| 运算符重载 `*`、`++`、`--`、`!=` | [六、自增/自减运算符 ++i 和 i++](../教程/07-操作符重载-实现String类（完整教学版）.md#六自增自减运算符-i-和-i) |
| `list` 底层数据结构 | [三、list — 高效插入和删除](../教程/03-使用序列式容器和迭代器（完整教学版）.md#三list--高效插入和删除) |

📂 **文件：** `main.cpp`（包含 ListNode、Iterator、myList 及测试）

[← 返回目录](../作业总结.md)
