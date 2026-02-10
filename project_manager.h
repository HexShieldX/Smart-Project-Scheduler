#ifndef PROJECT_MANAGER_H
#define PROJECT_MANAGER_H

#include "structs.h"

class ProjectManager {
private:
    // Tracks when an employee is next free (for resource-constrained scheduling)
    map<int, int> employee_free_time;
    // Memoization table for Earliest Finish Time
    map<int, int> dp_earliest_finish_time;

    // Helper function to calculate Earliest Finish Time 
    int calculate_earliest_finish_time(int task_id);

public:
    ProjectManager() = default;

    void generate_final_schedule(const vector<int>& execution_order);
    void print_schedule_report();
    void print_gantt_chart_visual();
    void print_workload_stats();
    void print_action_log();
    void find_task_details(int id_to_find);
    void print_longest_tasks();
};

#endif // PROJECT_MANAGER_H