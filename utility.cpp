#include "structs.h"
#include <fstream>
#include <sstream>

// Global DBs and File Paths (DEFINITION matching extern declaration in structs.h)
map<int, Employee> employees_db;
map<int, Task> tasks_db;
map<string, vector<int>> skill_to_employees;

// Hardcoded paths (Files must be in the same folder as the executable)
string employee_file_path = "employees.csv";
string task_file_path = "tasks.csv";

// --- Helper: Split string by delimiter ---
vector<string> split_string(string str, char delim) {
    vector<string> tokens;
    stringstream ss(str);
    string token;
    while (getline(ss, token, delim)) {
        if (!token.empty()) {
            tokens.push_back(token);
        }
    }
    return tokens;
}

// --- Write Employee to CSV (APPEND) ---
bool write_employee_to_csv(const Employee& emp) {
    ofstream file(employee_file_path, ios::app);
    if (!file.is_open()) { return false; }
    string skills_csv;
    for (size_t i = 0; i < emp.skills.size(); ++i) {
        skills_csv += emp.skills[i];
        if (i < emp.skills.size() - 1) skills_csv += ";";
    }
    file << "\n" << emp.id << "," << emp.name << "," << skills_csv;
    file.close();
    return true;
}

// --- Write Task to CSV (APPEND) ---
bool write_task_to_csv(const Task& task) {
    ofstream file(task_file_path, ios::app);
    if (!file.is_open()) { return false; }
    string deps_csv;
    for (size_t i = 0; i < task.dependencies.size(); ++i) {
        deps_csv += to_string(task.dependencies[i]);
        if (i < task.dependencies.size() - 1) deps_csv += ";";
    }
    file << "\n" << task.id << "," << task.name << "," << task.priority
        << "," << task.required_skill << "," << task.duration << "," << task.deadline
        << "," << deps_csv;
    file.close();
    return true;
}

// --- Load Employees from CSV ---
bool load_employees_from_csv(string filename) {
    employees_db.clear();
    skill_to_employees.clear();
    ifstream file(filename);
    if (!file.is_open()) { cout << "Cannot open employee data file: " << filename << endl; return false; }
    string line;
    getline(file, line); // skip header
    while (getline(file, line)) {
        if (line.empty()) continue;
        stringstream ss(line);
        vector<string> row;
        string temp;
        while (getline(ss, temp, ',')) row.push_back(temp);
        if (row.size() < 3) continue;
        Employee emp;
        try {
            if (row[0].empty()) throw std::runtime_error("Empty ID field");
            emp.id = stoi(row[0]);
            emp.name = row[1];
            emp.skills = split_string(row[2], ';');
            employees_db[emp.id] = emp;
            for (const string& s : emp.skills) skill_to_employees[s].push_back(emp.id);
        }
        catch (const exception&) { // Changed from (const exception& e)
            cout << "Error parsing employee line (Skipped): " << line << endl;
            continue;
        }
    }
    return true;
}

// --- Load Tasks from CSV ---
bool load_tasks_from_csv(string filename) {
    tasks_db.clear();
    ifstream file(filename);
    if (!file.is_open()) { cout << "Cannot open task data file: " << filename << endl; return false; }
    string line;
    getline(file, line); // skip header
    while (getline(file, line)) {
        if (line.empty()) continue;
        stringstream ss(line);
        vector<string> row;
        string temp;
        while (getline(ss, temp, ',')) row.push_back(temp);
        if (row.size() < 6) continue;
        Task t;
        try {
            if (row[0].empty() || row[2].empty() || row[4].empty() || row[5].empty())
                throw std::runtime_error("Missing essential task field");
            t.id = stoi(row[0]);
            t.name = row[1];
            t.priority = stoi(row[2]);
            t.required_skill = row[3];
            t.duration = stoi(row[4]);
            t.deadline = stoi(row[5]);
            if (row.size() > 6 && !row[6].empty()) {
                for (string d : split_string(row[6], ';')) {
                    if (!d.empty()) t.dependencies.push_back(stoi(d));
                }
            }
            tasks_db[t.id] = t;
        }
        catch (const exception&) { // Changed from (const exception& e)
            cout << "Error parsing task line (Skipped): " << line << endl;
            continue;
        }
    }
    return true;
}

// --- Load all data and build lookup (NO USER INPUT FOR PATHS) ---
void load_data_and_build_lookup() {
    cout << "Loading Data...\n";

    if (!load_employees_from_csv(employee_file_path)) {
        cout << "CRITICAL ERROR: Failed to load employees from " << employee_file_path << endl;
    }
    if (!load_tasks_from_csv(task_file_path)) {
        cout << "CRITICAL ERROR: Failed to load tasks from " << task_file_path << endl;
    }
    cout << "Data Loaded Successfully.\n";

    if (tasks_db.empty()) {
        cout << "WARNING: No tasks found in tasks.csv. Schedule will be empty.\n";
    }
}

// --- Validate tasks against skills ---
bool validate_data() {
    if (tasks_db.empty()) {
        return true;
    }
    // C++17 structured binding is used here.
    for (auto const& p : tasks_db) {
        if (skill_to_employees.find(p.second.required_skill) == skill_to_employees.end()) {
            cout << "No employee found for skill: " << p.second.required_skill << " (Task " << p.first << ")\n";
            return false;
        }
    }
    return true;
}

// --- Runtime Data Input Logic ---
void add_new_data_at_runtime() {
    char choice;
    cout << "\nDo you want to add new data (E/T/N - Employee/Task/None)? ";

    if (!(cin >> choice)) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        return;
    }
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    if (toupper(choice) == 'E') {
        Employee emp;
        emp.id = employees_db.empty() ? 1 : employees_db.rbegin()->first + 1;
        cout << "--- Adding New Employee (ID: " << emp.id << ") ---\n";
        cout << "Enter Name: ";
        getline(cin, emp.name);
        string skill_input;
        cout << "Enter Skills (e.g., C++;Python;Java): ";
        getline(cin, skill_input);
        emp.skills = split_string(skill_input, ';');
        if (write_employee_to_csv(emp)) {
            employees_db[emp.id] = emp;
            for (const string& s : emp.skills) skill_to_employees[s].push_back(emp.id);
            cout << "Employee " << emp.name << " added to DB and CSV.\n";
        }
        else {
            cout << "Error writing employee to CSV. Check file permissions.\n";
        }
    }
    else if (toupper(choice) == 'T') {
        Task t;
        t.id = tasks_db.empty() ? 101 : tasks_db.rbegin()->first + 1;
        cout << "--- Adding New Task (ID: " << t.id << ") ---\n";
        cout << "Enter Task Name: ";
        getline(cin, t.name);
        cout << "Enter Required Skill: ";
        getline(cin, t.required_skill);
        cout << "Enter Duration (Hours): ";
        while (!(cin >> t.duration) || t.duration <= 0) {
            cout << "Invalid duration. Enter a positive number: ";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
        cout << "Enter Deadline (Time Unit): ";
        while (!(cin >> t.deadline) || t.deadline <= 0) {
            cout << "Invalid deadline. Enter a positive number: ";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
        cout << "Enter Priority (1-5): ";
        while (!(cin >> t.priority) || t.priority < 1 || t.priority > 5) {
            cout << "Invalid priority. Enter a number between 1 and 5: ";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        string dep_input;
        cout << "Enter Dependency IDs (e.g., 101;102 or leave blank): ";
        getline(cin, dep_input);
        vector<string> dep_tokens = split_string(dep_input, ';');
        for (const string& token : dep_tokens) {
            try {
                if (!token.empty()) t.dependencies.push_back(stoi(token));
            }
            catch (const exception&) {}
        }
        if (write_task_to_csv(t)) {
            tasks_db[t.id] = t;
            cout << "Task " << t.name << " added to DB and CSV.\n";
        }
        else {
            cout << "Error writing task to CSV. Check file permissions.\n";
        }
    }
}