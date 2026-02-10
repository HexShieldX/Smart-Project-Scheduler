#include "structs.h" 
#include "task_scheduler.h"
#include "employee_manager.h"
#include "project_manager.h"

// Function to handle the interactive report menu
void run_report_menu(ProjectManager& proj_manager) {
    int choice = -1;
    while (choice != 0) {
        // Explicitly using std::cout to prevent potential ambiguity if 'using namespace std;' fails in structs.h
        std::cout << "\n\n================= REPORT MENU =================\n";
        std::cout << "Select a Report Feature:\n";
        std::cout << "1. Project Schedule Table (Report)\n";
        std::cout << "2. Gantt Chart Visualization\n";
        std::cout << "3. Workload & Completion Statistics\n";
        std::cout << "4. Action Log (Last 5 assignments)\n";
        std::cout << "5. Find Task Details (by ID)\n";
        std::cout << "6. Top 3 Longest Tasks\n";
        std::cout << "0. EXIT Program\n";
        std::cout << "Enter choice: ";

        if (!(std::cin >> choice)) {
            std::cout << "Invalid input. Please enter a number.\n";
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            continue;
        }

        switch (choice) {
        case 1:
            proj_manager.print_schedule_report();
            break;
        case 2:
            proj_manager.print_gantt_chart_visual();
            break;
        case 3:
            proj_manager.print_workload_stats();
            break;
        case 4:
            proj_manager.print_action_log();
            break;
        case 5: {
            int id_to_find;
            std::cout << "Enter Task ID to search: ";
            if (std::cin >> id_to_find) {
                proj_manager.find_task_details(id_to_find);
            }
            else {
                std::cout << "Invalid Task ID.\n";
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            }
            break;
        }
        case 6:
            proj_manager.print_longest_tasks();
            break;
        case 0:
            std::cout << "Exiting Report Menu.\n";
            return;
        default:
            std::cout << "Invalid choice. Please select from 0 to 6.\n";
            break;
        }
    }
}

// --- Main Execution (Integration) ---

int main() {
    std::cout << "--- Smart Project Scheduler Initialization ---\n";

    // 1. Data Handling and Validation
    load_data_and_build_lookup();

    add_new_data_at_runtime();
    add_new_data_at_runtime();

    if (!validate_data()) {
        std::cout << "Project cannot proceed due to data validation failure (missing skill match).\n";
        return 1;
    }

    // 2. Dependency Management and Topological Sort
    TaskScheduler scheduler;
    scheduler.build_dag();

    const vector<int>& execution_order = scheduler.get_topological_order();

    // Check if topological sort failed (i.e., a cycle was detected)
    if (execution_order.empty() && !tasks_db.empty()) {
        std::cout << "Project cannot be scheduled due to circular dependencies. Please check tasks.csv.\n";
        return 1; // Exit if cycle detected
    }

    // 3. Employee Assignment (Priority Queue Logic)
    EmployeeManager emp_manager;
    for (int task_id : execution_order) {
        emp_manager.assign_employee_to_task(task_id);
    }

    // 4. Optimization and Final Schedule Generation (Dynamic Programming)
    ProjectManager proj_manager;
    proj_manager.generate_final_schedule(execution_order);

    // 5. Interactive Report Menu
    run_report_menu(proj_manager);

    std::cout << "\n\n";

    // Top Separator: = (80 characters)
    std::cout << "================================================================================\n";
    // Second Separator: # (80 characters)
    std::cout << "################################################################################\n";

    // Empty Space Line (76 spaces between ##)
    std::cout << "##                                                                            ##\n";

    // Line 1: S M A R T (Centered)
    std::cout << "##                               S M A R T                                    ##\n";

    // Line 2: P R O J E C T (Centered)
    std::cout << "##                             P R O J E C T                                  ##\n";

    // Line 3: S C H E D U L E R (Centered)
    std::cout << "##                           S C H E D U L E R                                ##\n";

    // Empty Space Line
    std::cout << "##                                                                            ##\n";

    // Closing Separator: # (80 characters)
    std::cout << "################################################################################\n";
    // Bottom Separator: = (80 characters)
    std::cout << "================================================================================\n";

    // Print Newlines for spacing
    std::cout << "\n\n";

    // System Message (Centered)
    std::cout << "##         >>> Bye. Hope you Enjoyed. <<<         ##\n";

    // Print Newlines for spacing
    std::cout << "\n\n";
    return 0;
}