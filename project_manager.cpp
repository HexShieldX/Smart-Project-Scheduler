#include "project_manager.h"

// Helper function to get employee name from ID
string get_employee_name(int id) {
    if (employees_db.count(id)) {
        return employees_db[id].name;
    }
    return "Unassigned";
}

// 1. DP logic: Calculate the earliest time a task can finish
int ProjectManager::calculate_earliest_finish_time(int task_id) {
    if (dp_earliest_finish_time.count(task_id)) {
        return dp_earliest_finish_time[task_id];
    }

    Task& current_task = tasks_db[task_id];

    if (current_task.dependencies.empty()) {
        return current_task.duration;
    }

    int max_dep_finish_time = 0;
    for (int dep_id : current_task.dependencies) {
        // Use max with explicit arguments (safer than using namespace std::max directly)
        max_dep_finish_time = max(max_dep_finish_time, calculate_earliest_finish_time(dep_id));
    }

    int finish_time = max_dep_finish_time + current_task.duration;

    dp_earliest_finish_time[task_id] = finish_time;
    return finish_time;
}


// 2. Schedule Generation (Final Logic - Resource Constrained)
void ProjectManager::generate_final_schedule(const vector<int>& execution_order) {
    employee_free_time.clear();
    dp_earliest_finish_time.clear();

    // C++17 structured bindings used here
    for (auto const& [id, emp] : employees_db) {
        employee_free_time[id] = 0;
    }

    for (int task_id : execution_order) {
        Task& task = tasks_db[task_id];

        if (task.assigned_employee_id == -1) continue;

        int employee_id = task.assigned_employee_id;

        // A. Task Earliest Start Time (Dependency Constraint)
        int dependency_finish_time = 0;
        for (int dep_id : task.dependencies) {
            if (tasks_db.count(dep_id)) {
                dependency_finish_time = max(dependency_finish_time, tasks_db[dep_id].end_time);
            }
        }

        // B. Employee Earliest Start Time (Resource Constraint)
        int employee_start_time = employee_free_time[employee_id];

        // C. Final Start Time (Must wait for both dependencies and employee)
        task.start_time = max(dependency_finish_time, employee_start_time);

        // D. Final End Time
        task.end_time = task.start_time + task.duration;

        // E. Update employee's next free time
        employee_free_time[employee_id] = task.end_time;
    }

    cout << "Final Project Schedule Generated.\n";
}


// 3. Report Functions

void ProjectManager::print_schedule_report() {
    int max_completion_time = 0;
    // C++17 structured bindings used here
    for (auto const& [id, task] : tasks_db) {
        max_completion_time = max(max_completion_time, task.end_time);
    }

    vector<Task> sorted_tasks;
    // C++17 structured bindings used here
    for (auto const& [id, task] : tasks_db) {
        if (task.assigned_employee_id != -1) {
            sorted_tasks.push_back(task);
        }
    }

    // Sort by Start Time, then by ID
    sort(sorted_tasks.begin(), sorted_tasks.end(), [](const Task& a, const Task& b) {
        if (a.start_time != b.start_time) {
            return a.start_time < b.start_time;
        }
        return a.id < b.id;
        });

    cout << "\n=================================================================\n";
    // Use std::setw and std::left (requires <iomanip>)
    cout << setw(63) << left << "PROJECT SCHEDULE & ASSIGNMENT REPORT" << endl;
    cout << "=================================================================\n";
    cout << "-> Min Project Completion Time (Resource-Constrained): " << max_completion_time << endl;
    cout << "-----------------------------------------------------------------\n";
    cout << setw(5) << left << "ID"
        << setw(30) << left << "Task Name"
        << setw(15) << left << "Employee"
        << setw(8) << left << "Start"
        << setw(5) << left << "End" << endl;
    cout << "-----------------------------------------------------------------\n";

    for (const auto& task : sorted_tasks) {
        cout << setw(5) << left << task.id
            << setw(30) << left << task.name
            << setw(15) << left << get_employee_name(task.assigned_employee_id)
            << setw(8) << left << task.start_time
            << setw(5) << left << task.end_time << endl;
    }
    cout << "=================================================================\n";
}

// FIX: Renamed function for consistency
void ProjectManager::print_gantt_chart_visual() {
    int max_time = 0;
    // C++17 structured bindings used here
    for (auto const& [id, task] : tasks_db) {
        max_time = max(max_time, task.end_time);
    }

    if (max_time == 0) {
        cout << "\n================= GANTT CHART (Timeline) =================\n";
        cout << "Time: 0\n |---|\n";
        cout << "============================================================\n";
        return;
    }

    vector<Task> chart_tasks;
    // C++17 structured bindings used here
    for (auto const& [id, task] : tasks_db) {
        if (task.assigned_employee_id != -1) {
            chart_tasks.push_back(task);
        }
    }
    sort(chart_tasks.begin(), chart_tasks.end(), [](const Task& a, const Task& b) {
        return a.id < b.id;
        });

    cout << "\n================= GANTT CHART (Timeline) =================\n";


    

        cout << "Time: ";
    for (int t = 0; t <= max_time; ++t) {
        cout << setw(2) << left << t % 10;
    }
    cout << endl;

    cout << "      ";
    for (int t = 0; t <= max_time; ++t) {
        cout << (t % 5 == 0 ? "| " : "- ");
    }
    cout << endl;

    for (const auto& task : chart_tasks) {
        cout << "T" << task.id << " | ";

        for (int t = 0; t < task.start_time; ++t) {
            cout << "  ";
        }

        for (int t = 0; t < task.duration; ++t) {
            cout << "##";
        }

        cout << " (" << get_employee_name(task.assigned_employee_id) << ")\n";
    }

    cout << "============================================================\n";
}

void ProjectManager::print_workload_stats() {
    vector<pair<int, const Employee*>> employees_info;
    // C++17 structured bindings used here
    for (auto const& [id, emp] : employees_db) {
        employees_info.push_back({ id, &emp });
    }

    sort(employees_info.begin(), employees_info.end(), [](const pair<int, const Employee*>& a, const pair<int, const Employee*>& b) {
        return a.second->current_workload > b.second->current_workload;
        });

    cout << "\n================= WORKLOAD & COMPLETION REPORT =================\n";
    cout << setw(15) << left << "Employee"
        << setw(10) << left << "Hours"
        << setw(12) << left << "Tasks Done"
        << setw(10) << left << "Status" << endl;
    cout << "-----------------------------------------------------------------\n";

    for (const auto& item : employees_info) {
        const Employee& emp = *item.second;
        // Simple heuristic: Normal if <= 8 hours, Overloaded if > 8 hours
        string status = (emp.current_workload > 8) ? "Overloaded" : "Normal";

        cout << setw(15) << left << emp.name
            << setw(10) << left << emp.current_workload
            << setw(12) << left << emp.tasks_completed
            << setw(10) << left << status << endl;
    }
    cout << "=================================================================\n";
}

void ProjectManager::print_action_log() {
    vector<Task> assigned_tasks;
    // Collect assigned tasks
    // C++17 structured bindings used here
    for (auto const& [id, task] : tasks_db) {
        if (task.assigned_employee_id != -1) {
            assigned_tasks.push_back(task);
        }
    }

    // Sort tasks by ID descending to show "latest" assignments 
    sort(assigned_tasks.begin(), assigned_tasks.end(), [](const Task& a, const Task& b) {
        return a.id > b.id;
        });

    cout << "\n=========== ACTION LOG (Latest Assignments) ===========\n";

    int count = 0;
    for (const auto& task : assigned_tasks) {
        if (count >= 5) break;
        cout << "Task " << task.id << " assigned to " << get_employee_name(task.assigned_employee_id) << endl;
        count++;
    }

    if (assigned_tasks.empty()) {
        cout << "No tasks have been assigned yet.\n";
    }

    cout << "=======================================================\n";
}

void ProjectManager::find_task_details(int id_to_find) {
    cout << "\n=========== SEARCH TASK ===========\n";
    if (tasks_db.count(id_to_find)) {
        const Task& task = tasks_db[id_to_find];

        cout << "Task Found!\n";
        cout << "ID: " << task.id << endl;
        cout << "Name: " << task.name << endl;
        cout << "Duration: " << task.duration << " Hours" << endl;

        string status = (task.assigned_employee_id != -1)
            ? "Assigned to " + get_employee_name(task.assigned_employee_id)
            : "Unassigned";
        cout << "Status: " << status << endl;

    }
    else {
        cout << "Task ID " << id_to_find << " not found.\n";
    }
    cout << "===================================\n";
}

void ProjectManager::print_longest_tasks() {
    vector<Task> all_tasks;
    // C++17 structured bindings used here
    for (auto const& [id, task] : tasks_db) {
        all_tasks.push_back(task);
    }

    // Sort tasks by duration (descending)
    sort(all_tasks.begin(), all_tasks.end(), [](const Task& a, const Task& b) {
        return a.duration > b.duration;
        });

    cout << "\n=========== LONGEST TASKS ===========\n";
    cout << setw(30) << left << "Task Name" << "|" << setw(10) << right << "Duration" << endl;
    cout << "------------------------------------------\n";

    int count = 0;
    for (const auto& task : all_tasks) {
        if (count >= 3) break;
        cout << setw(30) << left << task.name << "|" << setw(10) << right << task.duration << " Hours" << endl;
        count++;
    }

    if (all_tasks.empty()) {
        cout << "No tasks available.\n";
    }

    cout << "=====================================\n";
}