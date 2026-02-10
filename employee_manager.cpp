#include "employee_manager.h"

// Helper function to calculate a score for employee selection
int EmployeeManager::calculate_priority_score(const Task& task, const Employee& employee) {
    // Score based primarily on current workload (lower is better)
    return employee.current_workload;
}

// 1. Build the Min-Heap (Priority Queue) with only skill-matched employees
void EmployeeManager::build_employee_pq_for_task(const Task& task) {
    // Clear the queue for the new task
    while (!employee_pq.empty()) {
        employee_pq.pop();
    }

    string required_skill = task.required_skill;

    if (skill_to_employees.count(required_skill)) {
        for (int emp_id : skill_to_employees[required_skill]) {
            // Get reference to the employee
            Employee& emp = employees_db[emp_id];

            EmployeePriority ep = { emp.id, calculate_priority_score(task, emp) };
            employee_pq.push(ep);
        }
    }
}

// 2. Assign the best employee to the task
int EmployeeManager::assign_employee_to_task(int task_id) {
    if (!tasks_db.count(task_id)) return -1;

    // Get reference to the task
    Task& task = tasks_db[task_id];

    build_employee_pq_for_task(task);

    if (employee_pq.empty()) {
        cout << "Error: No employee available or matched for Task " << task_id << " (" << task.required_skill << ").\n";
        return -1;
    }

    EmployeePriority best_emp_prio = employee_pq.top();
    int best_emp_id = best_emp_prio.id;

    // Get reference to the employee
    Employee& employee = employees_db[best_emp_id];

    // 1. Assign the task to the employee
    task.assigned_employee_id = best_emp_id;

    // 2. Update employee's workload and completed tasks
    employee.current_workload += task.duration;
    employee.tasks_completed++;

    // 3. Log the action
    cout << "-> Assigned Task " << task_id << " to " << employee.name
        << " (New Workload: " << employee.current_workload << ")\n";

    return best_emp_id;
}