# AGENTS.md — C++ 作业项目 hw6

> 本文件供 AI 编程助手阅读。项目根目录为 `hw6/`。

---

## 项目概述

本项目是 C++ 课程的第六次作业（hw6），目标是**基于已提供的模板类 `Vec`，实现一个自定义的 `Str` 字符串类**，使得 `main.cpp` 中的程序能够正确编译运行并输出指定格式的问候框。

项目目录结构非常简单，所有源码文件均位于根目录，无子模块、无外部依赖包管理器。

## 技术栈

- **语言**：C++（代码风格偏向 C++98/03，使用 `std::allocator`、`std::uninitialized_copy`、`std::uninitialized_fill` 等早期标准接口）
- **构建方式**：无配置好的构建系统（无 Makefile、CMakeLists.txt 等），需手动编译
- **标准库依赖**：`<iostream>`、`<algorithm>`、`<memory>`

## 文件结构

```
hw6/
├── Vec.h            # 已实现的泛型动态数组模板类（类似 std::vector）
├── Str.h            # Str 类声明（目前只有 //TODO，需要补充）
├── Str.cpp          # Str 类实现（目前只有 //TODO，需要补充）
├── main.cpp         # 主程序入口（只读，不要修改）
├── hw6_input.txt    # 测试输入数据（内容为 "XiaoMing"）
├── hw6_output.txt   # 预期输出（用于对比验证）
└── hw6.txt          # 作业要求说明（GBK 编码的中文文本）
```

### 关键文件说明

- **Vec.h**：提供了模板类 `Vec<T>`，支持动态扩容、迭代器、拷贝构造、赋值运算符等。`Str` 类应基于 `Vec<char>` 实现。
- **main.cpp**：程序运行时先读取一个名字，然后构造问候语 `"Hello, " + name + "!"`，并以星号边框打印。对 `Str` 类提出了以下接口需求：
  - 默认构造函数
  - 接受 `(size_t n, char c)` 的构造函数
  - `operator+`（至少支持 `Str + Str`、`Str + const char*`、`const char* + Str`）
  - `operator<<`（输出到 `ostream`）
  - `operator>>`（从 `istream` 输入）
  - `size()` 成员函数
- **hw6.txt**：原始作业要求，说明需补全 `Str.cpp` 和 `Str.h`，使程序完整，并使用提供的测试数据验证。

## 构建与运行

由于项目中没有 Makefile 或 CMake 配置，请使用以下手动编译命令：

```bash
# 使用 g++ 编译（建议开启 -Wall -Wextra）
g++ -Wall -Wextra -o main main.cpp Str.cpp

# 运行并输入测试数据
./main < hw6_input.txt

# 对比输出
diff <(./main < hw6_input.txt | head -n 6) hw6_output.txt
```

> 注：`hw6_output.txt` 最后一行包含 `"Press any key to continue"`，这是 Windows 环境下 IDE 的暂停提示，实际 Linux/macOS 运行输出可能不包含该行。对比时请留意平台差异。

## 代码风格与约定

- 代码中现有注释为 `//TODO`，完成作业后请将这些占位符替换为实际实现代码。
- 保持与 `Vec.h` 一致的命名风格：类名大驼峰，成员函数小驼峰，使用 `_` 开头的宏保护（如 `_STRING_H`、`__VEC_H`）。
- `Str` 类建议以私有继承或成员对象方式复用 `Vec<char>`，避免重复实现内存管理逻辑。

## 测试策略

1. **功能测试**：将 `hw6_input.txt` 重定向到标准输入，观察输出是否与 `hw6_output.txt` 一致。
2. **边界测试**：考虑空字符串输入、超长名字输入，确保 `Vec::grow()` 的扩容机制正常工作。
3. **内存安全测试**：由于 `Vec` 使用了手动内存管理（`allocate` / `deallocate` / `construct` / `destroy`），请使用 Valgrind 或 AddressSanitizer 检查泄漏：
   ```bash
   g++ -g -fsanitize=address -o main main.cpp Str.cpp
   ./main < hw6_input.txt
   ```

## 提交要求

根据 `hw6.txt` 的说明，最终需要将所有 `.cpp` 和 `.h` 文件打包压缩后提交。打包命令示例：

```bash
zip hw6_submit.zip Str.h Str.cpp Vec.h main.cpp
```

## 安全与注意事项

- 不要修改 `main.cpp`，作业评分会以原始 `main.cpp` 为准。
- `Vec.h` 中使用了较老式的 `std::allocator` 接口（`construct`、`destroy`、`allocate` 带两个参数等），这些在 C++17 中已被移除。如需在现代编译器下编译，可能需要调整 `Vec.h` 或指定 `-std=c++11`/`-std=c++14` 标准。
- `hw6.txt` 采用 GBK 编码，如需在终端直接查看中文内容，请确保编码转换正确：
  ```bash
  iconv -f GBK -t UTF-8 hw6.txt
  ```
