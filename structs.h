#ifndef STRUCTS_H
#define STRUCTS_H

// --- Core Includes needed everywhere ---
#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <queue>
#include <algorithm> 
#include <iomanip> // For setw, left
#include <limits>    
#include <sstream>
#include <stdexcept> 
#include <cctype> 

using namespace std;

// --- Data Structures ---

struct Employee {
    int id = -1;
    string name = "";
    vector<string> skills;
    int current_workload = 0;
    bool is_available = true;
    int tasks_completed = 0;
};

struct Task {
    int id = -1;
    string name = "";
    int priority = 0;
    string required_skill = "";
    int duration = 0;
    int deadline = 0;
    vector<int> dependencies;

    int assigned_employee_id = -1;
    int start_time = 0;
    int end_time = 0;
    bool is_completed = false;
};

struct EmployeePriority {
    int id = -1;
    int workload = 0;
    // Overload the > operator for the std::priority_queue (Min-Heap)
    bool operator>(const EmployeePriority& other) const {
        return workload > other.workload; // Min-Heap: lower workload = higher priority
    }
};

// --- Global Data Declarations ---
extern map<int, Employee> employees_db;
extern map<int, Task> tasks_db;
extern map<string, vector<int>> skill_to_employees;

// --- Utility Function Declarations (Implemented in utility.cpp) ---
void load_data_and_build_lookup();
bool validate_data();
void add_new_data_at_runtime();
bool write_employee_to_csv(const Employee& emp);
bool write_task_to_csv(const Task& task);
vector<string> split_string(string str, char delim);

#endif // STRUCTS_H