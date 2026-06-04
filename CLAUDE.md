# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Overview

This repository contains materials for **面向对象程序设计 (Object-Oriented Programming)** at Fudan University, taught by 周雅倩. The course teaches C++ OOP following the **"Accelerated C++"** textbook (Koenig & Moo).

## Repository structure

- `教程/` — Primary learning materials: 8 weekly chapters (01–08) + 10 supplementary chapters (补充09–18) covering C++ Primer topics
- `课件（md格式）/` — Original lecture courseware (Chinese Markdown), numbered 00–07
- `作业/` — Programming assignments (see below)
- `.vscode/settings.json` — VS Code workspace settings

## Homework assignments — architecture evolution

The homework series builds a "student grade" system that grows in sophistication. Each step introduces new C++ OOP concepts:

| HW | Concepts | Key types introduced |
|----|----------|---------------------|
| hw2–hw4 | Procedural: functions, structs, `#include` guards, separate compilation | `Student_info` (struct), `grade()`, `median()` |
| hw5 | Template classes, manual memory management (`new[]`/`delete[]`), copy control | `Vec<T>` — a custom dynamic array replacing `std::vector` |
| hw6 | Operator overloading, composition | `Str` — a string class built by wrapping `Vec<char>` with `+=`, `+`, `<<`, `>>` operators |
| hw7 | Inheritance, virtual functions, dynamic binding, handle class idiom | `Core` (base), `Grad : public Core` (derived), `Student_info` (handle holding `Core*`) |

### Labs

- **lab2–lab3**: I/O and algorithm drills (single `.cpp` files with input/output test pairs)
- **lab4**: Inventory system — `goods` struct and `stock` class managing purchases/sales with exception handling
- **lab5**: Custom linked-list (`ListNode<T>` doubly-linked, manual `new`/`delete`)
- **lab6**: `Complex` class with operator overloading (`+=`, `+`, `<<`, `>>`)

## Code conventions

- **Include guards**: `GUARD_xxx_h` style (e.g., `GUARD_CORE`, `GUARD_Student_info`)
- **Headers**: `.h` extension (not `.hpp`)
- **Implementation**: `.cpp` files mirroring header names; `using namespace std;` at the top
- **Template implementations**: defined inline in the header (e.g., `Vec.h` has full method bodies)
- **Homework tasks**: `hw*.txt` / `test*.txt` describe requirements in Chinese; code stubs have `//TODO` markers where students fill in
- **Testing pattern**: each homework includes `*_input.txt` and `*_output.txt` — compile and run, pipe input in, diff against expected output

## Compiling and testing

All homeworks compile by listing all `.cpp` files together. No Makefiles — it's single-command compilation.

```bash
# General pattern (run from the homework directory):
g++ -std=c++17 -Wall -o main *.cpp
./main < input.txt > actual.txt
diff actual.txt output.txt

# Example for hw7:
cd "作业/hw7"
g++ -std=c++17 -Wall -o main *.cpp
./main < hw7_input.txt > actual.txt
diff actual.txt hw7_output.txt
```

Some directories have `in.txt`/`out.txt` as input/output pairs instead of the `hw*_input.txt`/`hw*_output.txt` convention.

## 知识点索引

[`作业总结.md`](作业总结.md) 提供每个作业的知识点分析，并关联到 `教程/` 目录下对应章节的超链接，支持双向跳转查阅。

## Notes

- The `.gitignore` excludes compiled binaries (`*.out`, `.exe`, `*.o`, `a.out`) and `.DS_Store`
- Lab `actual.txt` files (e.g., `test3_1_actual.txt`) are student-generated output used for self-verification
- The repo tracks only the C++ source files — original files named in `.gitignore` patterns like `*.txt` (except `requirements.txt`) are gitignored so compiled output doesn't accidentally commit, but test input/output `.txt` files are tracked explicitly
