**🚀 SMART-PLAN: Automated Project Scheduler (DSA Semester Project)**

SMART-PLAN is a high-performance C++ project management and scheduling system built as a Data Structures & Algorithms semester project. It automates task scheduling, resolves dependencies, optimizes timelines, and balances employee workloads using advanced DSA concepts.
The project demonstrates real-world applications of graphs, heaps, dynamic programming, hashing, and stacks in an optimized and structured way.

**✨ Key Features**
🔹 Graph-Based Dependency Management
Models tasks as a Directed Acyclic Graph (DAG)
Uses Topological Sorting to ensure correct task execution order

🔹 DP-Optimized Timeline Calculation
Implements Dynamic Programming (Memoization)
Computes Earliest Finish Time (EFT) for the complete project

🔹 Smart Resource Allocation
Uses Min-Heaps (Priority Queues)
Assigns tasks to the least-burdened qualified employee

🔹 Data Persistence
Reads and manages data using CSV file handling
Dynamically loads employees and tasks from files

🔹 Console-Based Gantt Chart
Generates an ASCII Gantt chart
Visualizes task timelines directly in the terminal

🔹 Workload Monitoring
Tracks employee working hours
Classifies employees as Normal or Overloaded using Hash Maps
🔹 History Logging
Maintains a LIFO activity log
Uses Stacks to track recent task assignments

***🛠️ Technologies Used**
Language: C++ (C++17 Standard)
Core Data Structures:
Graphs (DAG) – task dependencies
Priority Queues (Min-Heaps) – workload balancing
Maps / Hashing – fast lookups (O(log n))
Stacks – action history
Vectors – dynamic task storage
IDE: Visual Studio / Visual Studio Code
Data Format: CSV (Comma-Separated Values)

**▶️ How to Run**
Download Smart-Plan-Project.zip (~150 MB)
Extract the ZIP file
Open the .sln file in Visual Studio
Ensure employees.csv and tasks.csv are present in the project root directory
Select Debug or Release configuration (x86 / x64)
Build the project using Ctrl + Shift + B
Run the program using F5

**🎓 Academic Context**

Course: Data Structures & Algorithms
Purpose: Educational implementation of non-linear data structures and optimization techniques
Focus: Practical use of DSA concepts in a real-world scheduling scenario

**📜 License**

This project is intended for academic and learning purposes only.
