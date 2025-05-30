<h1 align="center">🧠 CSE 310: Compiler Design</h1>
<p align="center"><em>Bangladesh University of Engineering and Technology (BUET)</em></p>
<p align="center"><code>Author: 2005033</code></p>

---

<div align="center">
  <img src="https://upload.wikimedia.org/wikipedia/commons/1/1b/Flex_bison.svg" width="200" alt="Flex and Bison Logo" />
</div>

---

## 🚀 Overview

Welcome to the **Compiler Universe!** 🌌  
This repository is a journey through the four essential phases of compiler design. Each phase is implemented in a separate offline assignment as part of the **CSE 310** course at **BUET CSE**.

> 🧬 From symbol tables to intermediate code, this repo builds a miniature compiler — step by step.

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
