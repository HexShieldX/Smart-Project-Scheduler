#pragma once
#ifndef EMPLOYEE_MANAGER_H
#define EMPLOYEE_MANAGER_H

#include "structs.h"

class EmployeeManager {
private:
    // Min-Heap (Priority Queue) to get the least busy employee first
    priority_queue<EmployeePriority, vector<EmployeePriority>, greater<EmployeePriority>> employee_pq;

    // Helper to score employees (lower score = higher priority)
    int calculate_priority_score(const Task& task, const Employee& employee);

    // Builds the PQ for the current task based on skill match
    void build_employee_pq_for_task(const Task& task);

public:
    EmployeeManager() = default;

    int assign_employee_to_task(int task_id);
};

#endif // EMPLOYEE_MANAGER_H