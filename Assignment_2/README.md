# CS6160: Cryptology - Programming Assignment 2

**Submitted by:** Manoj Kumar V K  
**Roll Number:** CS26RESCH11009  

## Experimental Environment

| Parameter | Specification |
| :--- | :--- |
| **Machine** | ASUS ROG Zephyrus G14 GA401IH_GA401IH |
| **Processor** | AMD64 Family 23 Model 96 Stepping 1 (~2.9 GHz) |
| **Memory** | 15,790 MB |
| **Operating System** | Windows 11 Home Single Language (Build 10.0.26200) |
| **Compiler** | g++ (Rev6, Built by MSYS2 project) 16.1.0 |

## Files

* `aes.cpp` — AES-128 software and AES-NI hardware-accelerated implementations with correctness verification and performance benchmarking.
* `multiplication_table.cpp` — $GF(2^3)$ multiplication table generator using the irreducible polynomial $x^3 + x + 1$.

## Requirements

* `g++` (GCC toolchain)
* C++11 or later
* x86-64 CPU with AES-NI support (required to execute the hardware-accelerated AES implementation)

## Compilation Instructions

### 1. AES Benchmark

```bash
g++ aes.cpp -o aes -march=native -O3
```
*Note:* The `-march=native` flag automatically detects and enables the instruction sets supported by the host CPU (including AES-NI, SSE4.1, and SSSE3). This avoids having to explicitly specify individual architecture flags such as `-maes` or `-msse2`. The `-O3` flag enables aggressive compiler optimization, which is critical for establishing an accurate software performance baseline.

### 2. $GF(2^3)$ Multiplication Table

```bash
g++ multiplication_table.cpp -o multiplication_table -O3
```

## Execution Instructions

**Run the AES implementation:**
```bash
./aes
```
*Behavior:* The program first executes a 100-trial correctness verification suite using randomly generated keys and plaintexts. If verification passes, it runs a CPU warmup followed by the performance benchmark. It outputs the median throughput (MB/s) and cycles per byte (CPB) across 21 independent runs for payload sizes ranging from 1 KB to 64 KB.

**Run the multiplication table:**
```bash
./multiplication_table
```
*Behavior:* The program calculates and prints the formatted $8\times8$ multiplication table for the extension field $GF(2^3)$.