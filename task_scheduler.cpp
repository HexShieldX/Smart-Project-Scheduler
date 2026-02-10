#include "task_scheduler.h"

// 1. Build the DAG (Adjacency List and In-Degree Map)
void TaskScheduler::build_dag() {
    adj_list.clear();
    in_degree.clear();
    topological_order.clear();

    // C++17 structured bindings used here
    for (auto const& [id, task] : tasks_db) {
        adj_list[id]; // Ensure every task ID is a key
        in_degree[id] = 0;
    }
    cout << "DAG Built Successfully.\n";

    // C++17 structured bindings used here
    for (auto const& [id, task] : tasks_db) {
        for (int dependency_id : task.dependencies) {
            if (tasks_db.count(dependency_id)) {
                // Edge FROM dependency_id TO id.
                adj_list[dependency_id].push_back(id);
                in_degree[id]++;
            }
            else {
                cout << "Warning: Task " << id << " depends on non-existent task " << dependency_id << endl;
            }
        }
    }
}

// 2. Perform Topological Sort (Kahn's Algorithm)
const vector<int>& TaskScheduler::get_topological_order() {
    topological_order.clear();

    queue<int> q;
    // C++17 structured bindings used here
    for (auto const& [id, count] : in_degree) {
        if (count == 0) {
            q.push(id);
        }
    }

    while (!q.empty()) {
        int u = q.front();
        q.pop();
        topological_order.push_back(u);

        for (int v : adj_list[u]) {
            in_degree[v]--;
            if (in_degree[v] == 0) {
                q.push(v);
            }
        }
    }

    // Cycle Detection: If we haven't processed all tasks, there must be a cycle.
    if (topological_order.size() != tasks_db.size()) {
        cout << "Error: Cycle detected! Cannot finish project.\n";
        static const vector<int> empty_vector;
        return empty_vector;
    }

    cout << "Topological Sort Complete.\n";
    return topological_order;
}