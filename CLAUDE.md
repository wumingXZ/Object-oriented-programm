# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Overview

This directory contains materials for **面向对象程序设计 (Object-Oriented Programming)** at Fudan University, taught by 周雅倩. The course teaches C++ OOP following the **"Accelerated C++"** textbook (Koenig & Moo).

## Structure

- `课件（md格式）/` — Lecture notes in Markdown (Chinese), numbered 00–07
- `作业/` — Homework and training exercises
  - `hw1/` through `hw4/` — Graded homework assignments
  - `train1/` through `train3/` — Ungraded practice exercises

## Lecture notes syllabus

| # | Topic |
|---|-------|
| 00 | Course intro |
| 01 | Strings and batch data handling |
| 02 | Organizing programs and data; sequential containers |
| 03 | Generic functions and library algorithms |
| 04 | Defining new types (classes) |
| 05 | Abstract data types (template classes) |
| 06 | Operator overloading |
| 07 | Inheritance and dynamic binding |

## Homework code conventions

- All C++ code follows the **Accelerated C++** style: `#include` guards use `GUARD_xxx_h`, headers use `.h` extension (not `.hpp`), and `using namespace std;` in `.cpp` files
- Each homework builds around a "student grade" domain model: `Student_info` struct, `grade()` function, and optional `median()` helper
- Homework tasks (`hw*.txt`) describe requirements in Chinese; input/output sample files (`*_input.txt`, `*_output.txt`) are included for testing
- Header files declare the interface; implementation lives in corresponding `.cpp` files — compile by listing all `.cpp` files together (e.g., `g++ -o main *.cpp`)
