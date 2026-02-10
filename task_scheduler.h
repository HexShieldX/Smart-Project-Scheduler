#ifndef TASK_SCHEDULER_H
#define TASK_SCHEDULER_H

#include "structs.h"

class TaskScheduler {
private:
    // Adjacency List for the DAG: Predecessor ID -> List of Dependent Task IDs
    map<int, vector<int>> adj_list;
    // In-degree for Kahn's algorithm
    map<int, int> in_degree;
    // Result of the topological sort
    vector<int> topological_order;

public:
    TaskScheduler() = default;

    void build_dag();
    // Returns the execution order (or an empty vector if a cycle is detected)
    const vector<int>& get_topological_order();
};

#endif // TASK_SCHEDULER_H