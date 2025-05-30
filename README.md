<h1 align="center">🧠 CSE 310: Compiler Sessional</h1> 
<p align="center"><em>Bangladesh University of Engineering and Technology (BUET)</em></p>
<p align="center"><code>Author: Somik Dasgupta</code></p>

---

## 📘 Introduction

This repository showcases my journey through the **CSE 310 Compiler Sessional** — a core course in the BUET CSE curriculum that dives deep into the inner workings of modern compilers. Through a series of four thoughtfully structured assignments, I explored the theoretical foundations and practical implementation of each major phase in the compilation pipeline.

From building a robust **symbol table** and crafting a **lexer** with Flex, to developing a complete **parser** with Bison and generating **intermediate assembly code**, each assignment brought me one step closer to understanding how high-level code is transformed into machine-readable instructions.

This compilation of work reflects not only technical skills in C++, Flex, and Bison, but also an appreciation for the elegance and complexity of compiler design.

Know more about the course from [outline](CSE-310_outline.pdf)

---

## 📦 Assignments Breakdown

### 🧱 OFFLINE_1 — *Symbol Table Implementation*
> “Where the compiler's memory begins...”

🔹 Implemented a hierarchical symbol table in C++  
🔹 Supports nested scopes and function/variable definitions  
🔹 Demonstrates chaining for collision handling

📄 Files:
- `2005033_SymbolTable.hpp`
- `2005033_main.cpp`
- `input.txt`

📘 [📄 Specification](OFFLINE_1/Symbol%20Table%20Implementation.pdf)

---

### 🔍 OFFLINE_2 — *Lexical Analysis*
> “The compiler learns to read...”

🔸 Built with **Flex**  
🔸 Tokenizes source code into identifiers, numbers, operators, etc.  
🔸 Clean token stream ready for parsing

📄 Files:
- `2005033.l`
- `2005033.hpp`
- `input.txt`

📘 [📄 Specification](OFFLINE_2/spec.pdf)

---

### 🧠 OFFLINE_3 — *Syntax and Semantic Analysis*
> “Understanding meaning, not just words…”

🔸 Combined **Bison** and **Flex** for parsing  
🔸 Captures syntax and semantic errors  
🔸 Employs grammar rules and production actions

📄 Files:
- `2005033.l`
- `2005033.y`
- `2005033.hpp`
- `noerror.c`

📘 [📄 Specification](OFFLINE_3/CSE310_July_2023_YACC_Assignment_Spec.pdf)

---

### ⚙️ OFFLINE_4 — *Intermediate Code Generation (ICG)*
> “The birth of assembly...”

🔸 Generates **8086 Assembly Code**  
🔸 Translates high-level constructs into low-level TAC  
🔸 Intermediate step toward full code generation

📄 Files:
- `2005033.y`
- `2005033.l`
- `input.c`

📘 [📄 Specification](OFFLINE_4/CSE_310_July_2023_ICG_Spec.pdf)

---

## 🛠️ Running Instructions

```bash
cd OFFLINE_1            # Change to desired offline folder
chmod +x 2005033.sh     # Make the script executable
./2005033.sh            # Run the script
