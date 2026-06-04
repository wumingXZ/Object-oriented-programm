# 面向对象程序设计（C++）— 学习笔记

基于复旦大学周雅倩老师《面向对象程序设计》课程，参考 *C++ Primer 第五版* 系统整理。

## 快速开始

### 1. 环境准备

- 安装 [VS Code](https://code.visualstudio.com/)
- 安装插件 **Markdown Preview Enhanced**（扩展 ID: `shd101wyy.markdown-preview-enhanced`）
- 确保系统有 C++ 编译器：
  - **macOS**：`xcode-select --install`（自带 `g++`/`clang++`）
  - **Windows**：安装 [MinGW-w64](https://www.mingw-w64.org/) 或 Visual Studio（带 C++ 工作负载）
  - **Linux**：`sudo apt install g++`（Ubuntu/Debian）

### 2. 打开项目

```bash
git clone <repo-url>
cd "Object-oriented programming"
code .
```

> ⚠️ **macOS 用户**：必须从终端用 `code .` 启动 VS Code，否则扩展可能找不到编译器。如果遇到 `fatal error: 'iostream' file not found`，先确认已安装 Xcode Command Line Tools。

### 3. 运行代码

在 VS Code 中打开任意 `.md` 文件，右键 → **Markdown Preview Enhanced: Open Preview**，然后：

- 光标放在代码块内，按 **`Shift + Enter`** → 运行当前代码块
- 按 **`Ctrl + Shift + Enter`** → 运行文件中所有代码块

代码块的输出会直接显示在预览窗口中。

> 首次使用需要在 VS Code 设置中启用：`"markdown-preview-enhanced.enableScriptExecution": true`（本项目 `.vscode/settings.json` 已配置）。

## 学习路径

### 主线（第 01–08 周）

按顺序阅读 `教程/` 目录下的文件：

| 文件 | 内容 |
|---|---|
| `01-批量使用数据` | Hello World、string、vector、IO |
| `02-组织程序和数据` | 函数、引用、异常、结构体、分文件编译 |
| `03-使用序列式容器和迭代器` | vector、list、map、迭代器 |
| `04-泛型函数和库算法` | 函数模板、算法、lambda |
| `05-定义新类型-类` | class、构造函数、const 成员、封装 |
| `06-定义抽象数据类型-模板类` | 类模板、new/delete、拷贝控制 |
| `07-操作符重载-实现String类` | 运算符重载、友元、类型转换 |
| `08-继承和动态绑定` | 继承、虚函数、多态、抽象类 |

### 补充（第 09–18 周）

学完主线后，按顺序阅读补充内容——填补 C++ Primer 中的系统化知识：

| 文件 | 内容 | 关联章节 |
|---|---|---|
| `补充09-变量和基本类型` | 内置类型、const、constexpr、auto、decltype | 01, 02, 04, 06 |
| `补充10-表达式和控制流` | 运算符、switch、范围 for、类型转换 | 01, 02, 04, 07 |
| `补充11-IO库和文件流` | 文件读写、字符串流、IO 状态 | 01, 03, 07, 08 |
| `补充12-关联容器深入` | set、multiset、unordered_map、容器适配器 | 03, 04, 06 |
| `补充13-智能指针` | shared_ptr、unique_ptr、weak_ptr | 06, 08 |
| `补充14-移动语义和拷贝控制进阶` | 右值引用、移动构造、=default、零法则 | 06, 07 |
| `补充15-模板进阶` | 模板推导、完美转发、可变参数模板 | 04, 06, 07 |
| `补充16-命名空间和高级异常处理` | namespace、异常层次、noexcept | 01, 02, 08 |
| `补充17-RTTI和枚举类` | dynamic_cast、typeid、enum class、tuple | 03, 04, 08 |
| `补充18-复杂度分析入门` | Big O 表示法、容器操作复杂度速查 | 全课程 |

## 目录结构

```
├── 课件（md格式）/   # 原始课件（00–07）
├── 教程/             # ★ 主学习材料（01–08 周 + 补充09–18）
├── 作业/             # 编程练习
│   ├── hw1/ ~ hw4/   # 计分作业
│   └── train1/ ~ train3/  # 练习
├── .vscode/          # VS Code 配置（MPE 代码执行已启用）
├── CLAUDE.md         # AI 辅助说明
└── README.md         # 本文件
```

## 常见问题

**Q: 代码块运行报错 `'iostream' file not found`？**
A: 未安装 C++ 编译器。macOS 运行 `xcode-select --install`，Windows 安装 MinGW，Linux 安装 `g++`。

**Q: 按 Shift+Enter 没反应？**
A: 检查 VS Code 设置中 `markdown-preview-enhanced.enableScriptExecution` 是否为 `true`。

**Q: 我的代码和笔记里的不一样，哪个是对的？**
A: 笔记中的代码均已通过 `g++ -std=c++17 -Wall` 编译验证（共 156 个代码块）。如果你的代码编译不通过，可以对照检查。
