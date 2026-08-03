# CS6160 – Cryptology
## Assignment 1

**Submitted by:** Manoj Kumar V K  
**Roll No.:** CS26RESCH11009

---

### Environment

All programs were developed using **Visual Studio Code** on **Windows 11**.

---

### Question 1 - Frequency Analyzer (C++)

#### Compilation

```bash
g++ freq_analyzer.cpp -o out
```

#### Execution (Windows)

```bash
out.exe inputfile.txt
```

#### Execution (Linux)

```bash
./out inputfile.txt
```

> **Note:** The Linux version has not been tested.

---

### Question 2 - Breaking Vigenère Cipher using Kasiski's Method (Python)

#### Usage

##### 1. Use the built-in ciphertext

```bash
python decrypt_vigenere.py
```

Uses:
- The ciphertext embedded in the program.
- Maximum possible key length = **25**.

---

##### 2. Use a ciphertext from a file

```bash
python decrypt_vigenere.py inputcipher.txt
```

Uses:
- Ciphertext read from `inputcipher.txt`.
- Maximum possible key length = **25**.

---

##### 3. Specify the maximum possible key length

```bash
python decrypt_vigenere.py inputcipher.txt max_key_length
```

Uses:
- Ciphertext read from `inputcipher.txt`.
- Maximum possible key length = `max_key_length`.

---


> **Note:** The program outputs the estimated keylength, recovered key, and the decrypted plain text. In addition, the recovered plain text is also stored in:

```
output/decrypted_vigenere.txt
```