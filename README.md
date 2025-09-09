# 📂 C Projects Collection

This repository contains multiple **C language projects and experiments** ranging from data structures and algorithms to system programming (sockets, threading, IPC, etc.).  
The purpose of this repo is for practice, learning, and reference.

---

## 📌 Repository Structure

```
# 📂 C Projects Collection

This repository contains a wide variety of **C programming projects** covering topics such as:

- Data Structures & Algorithms (DSA)
- System Programming (Sockets, Threads, IPC, Message Queues)
- File Handling
- Utility Programs and small experiments

The goal is to practice, explore, and build a reusable reference for future C projects.

---

## 📌 Repository Structure

```
## 📌 Repository Structure

```text
Cprojects/
├── DSA/                           # Data structures & algorithms in C
│   ├── arrays/                    # Array operations
│   ├── linkedlist/                # Linked list (single, double, circular)
│   ├── stack/                     # Stack implementation
│   ├── queue/                     # Queue and Deque
│   ├── tree/                      # Binary tree / BST
│   ├── sorting/                   # Bubble, Quick, Merge sort
│   ├── searching/                 # Linear and Binary search
│   └── misc/                      # Miscellaneous DSA practice
│
├── sockets/                       # Socket programming examples
│   ├── tcpsockets/                # TCP client-server programs
│   │   ├── tcpserver1/            # Basic TCP server
│   │   ├── tcpcharclient/         # TCP client for char streams
│   │   ├── tcperver2/             # Another TCP server version
│   │   └── tcpservermutiplex/     # Multiplexed TCP server (select)
│   ├── udp/                       # UDP client-server examples
│   └── utils/                     # Common socket utilities
│
├── threading/                     # Multithreading in C (pthreads)
│   ├── mutex_example.c            # Mutex lock usage
│   ├── condvar_example.c          # Condition variables
│   ├── thread_pool.c              # Simple thread pool implementation
│   └── misc/                      # Threading practice codes
│
├── message_queue/                 # System V / POSIX message queue examples
│   ├── sender.c                   # Message queue sender
│   ├── receiver.c                 # Message queue receiver
│   └── mq_utils.c                 # Utility functions
│
├── ipc/                           # Other Inter-Process Communication (pipes, shared memory)
│   ├── pipe_example.c
│   ├── shm_example.c
│   └── semaphores.c
│
├── file_handling/                 # Programs dealing with files
│   ├── file_read.c
│   ├── file_write.c
│   ├── file_append.c
│   └── people.txt                 # Sample data file
│
├── practice/                      # Miscellaneous practice codes
│   ├── abc.c
│   ├── reverse.c
│   ├── string_ops.c
│   └── pointer_examples.c
│
├── version4.txt                   # Version tracking file
├── version4.project               # Project metadata
└── README.md                      # Documentation

```
---

## ⚙️ Features

### 🔹 DSA Implementations
- Arrays, Linked Lists (single, double, circular)
- Stack and Queue (including deque and priority queue)
- Trees (binary tree, BST)
- Searching & Sorting algorithms

### 🔹 System Programming
- **TCP & UDP sockets**
  - Basic client-server communication
  - Multiplexed servers using `select()`
- **Threads**
  - Mutexes and condition variables
  - Thread pools
- **IPC**
  - Pipes, shared memory, semaphores
  - Message queues (System V and POSIX)

### 🔹 File Handling
- Read, write, append
- Working with structured data in files (`people.txt` etc.)

### 🔹 Practice Programs
- String manipulation
- Pointer experiments
- Small utility codes

---

## 🚀 Getting Started

### 1. Clone the repository
```bash
git clone https://github.com/harshraj1695/Cprojects.git
cd Cprojects
```

### 2. Compile a program
Use `gcc` (or `clang`) to build individual C files:

```bash
gcc sockets/tcpsockets/tcpserver1/server.c -o server
gcc sockets/tcpsockets/tcpcharclient/client.c -o client
```

Run them in different terminals:
```bash
./server
./client
```

---

## 🛠 Requirements
- GCC / Clang compiler  
- Linux / Unix environment (required for sockets, threads, IPC)  
- Basic knowledge of terminal and C compilation  

---


## 🚀 Getting Started

### 1. Clone the repository
```bash
git clone https://github.com/harshraj1695/Cprojects.git
cd Cprojects
```

### 2. Compile a program
Use `gcc` (or `clang`) to build individual C files:

```bash
gcc sockets/tcpsockets/server.c -o server
gcc sockets/tcpsockets/client.c -o client
```

Run them in different terminals:
```bash
./server
./client
```

---
