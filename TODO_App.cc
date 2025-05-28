#include "TODO_App.h"
#include <limits>

// Task Implementation
Task::Task(int taskId, const std::string& desc, Urgency urg) 
    : id(taskId), description(desc), urgency(urg), completed(false) {
    createdAt = std::chrono::system_clock::now();
}

std::string Task::getCreatedTimeString() const {
    auto time_t = std::chrono::system_clock::to_time_t(createdAt);
    std::stringstream ss;
    ss << std::put_time(std::localtime(&time_t), "%Y-%m-%d %H:%M:%S");
    return ss.str();
}

std::string Task::getUrgencyString() const {
    return urgencyToString(urgency);
}

bool Task::operator<(const Task& other) const {
    // Sort by urgency (descending), then by creation time (ascending)
    if (urgency != other.urgency) {
        return static_cast<int>(urgency) > static_cast<int>(other.urgency);
    }
    return createdAt < other.createdAt;
}

// TodoApp Implementation
TodoApp::TodoApp(const std::string& logFile) : nextId(1), logFileName(logFile) {
    logAction("TodoApp initialized");
}

TodoApp::~TodoApp() {
    logAction("TodoApp terminated");
}

void TodoApp::addTask(const std::string& description, Urgency urgency) {
    Task newTask(nextId++, description, urgency);
    tasks.push_back(newTask);
    
    std::string logMsg = "Added task [ID: " + std::to_string(newTask.id) + 
                        "] \"" + description + "\" [" + newTask.getUrgencyString() + "]";
    logAction(logMsg);
    
    std::cout << "Task added successfully! ID: " << newTask.id << std::endl;
}

void TodoApp::removeTask(int id) {
    auto it = std::find_if(tasks.begin(), tasks.end(), 
                          [id](const Task& task) { return task.id == id; });
    
    if (it != tasks.end()) {
        std::string logMsg = "Removed task [ID: " + std::to_string(id) + 
                            "] \"" + it->description + "\"";
        tasks.erase(it);
        logAction(logMsg);
        std::cout << "Task removed successfully!" << std::endl;
    } else {
        std::cout << "Task with ID " << id << " not found!" << std::endl;
    }
}

void TodoApp::markCompleted(int id) {
    Task* task = findTaskById(id);
    if (task) {
        task->completed = true;
        std::string logMsg = "Completed task [ID: " + std::to_string(id) + 
                            "] \"" + task->description + "\"";
        logAction(logMsg);
        std::cout << "Task marked as completed!" << std::endl;
    } else {
        std::cout << "Task with ID " << id << " not found!" << std::endl;
    }
}

void TodoApp::displayTasks() const {
    if (tasks.empty()) {
        std::cout << "No tasks available." << std::endl;
        return;
    }
    
    std::cout << "\n=== ALL TASKS ===" << std::endl;
    std::cout << std::left << std::setw(5) << "ID" 
              << std::setw(40) << "Description" 
              << std::setw(12) << "Urgency" 
              << std::setw(20) << "Created" 
              << std::setw(10) << "Status" << std::endl;
    std::cout << std::string(87, '-') << std::endl;
    
    for (const auto& task : tasks) {
        std::cout << std::left << std::setw(5) << task.id
                  << std::setw(40) << task.description.substr(0, 39)
                  << std::setw(12) << task.getUrgencyString()
                  << std::setw(20) << task.getCreatedTimeString()
                  << std::setw(10) << (task.completed ? "DONE" : "PENDING") << std::endl;
    }
    std::cout << std::endl;
}

void TodoApp::displayTasksSortedByUrgency() const {
    if (tasks.empty()) {
        std::cout << "No tasks available." << std::endl;
        return;
    }
    
    std::vector<Task> sortedTasks = tasks;
    std::sort(sortedTasks.begin(), sortedTasks.end());
    
    std::cout << "\n=== TASKS SORTED BY URGENCY ===" << std::endl;
    std::cout << std::left << std::setw(5) << "ID" 
              << std::setw(40) << "Description" 
              << std::setw(12) << "Urgency" 
              << std::setw(20) << "Created" 
              << std::setw(10) << "Status" << std::endl;
    std::cout << std::string(87, '-') << std::endl;
    
    for (const auto& task : sortedTasks) {
        std::cout << std::left << std::setw(5) << task.id
                  << std::setw(40) << task.description.substr(0, 39)
                  << std::setw(12) << task.getUrgencyString()
                  << std::setw(20) << task.getCreatedTimeString()
                  << std::setw(10) << (task.completed ? "DONE" : "PENDING") << std::endl;
    }
    std::cout << std::endl;
}

std::vector<Task> TodoApp::getTasksByUrgency(Urgency urgency) const {
    std::vector<Task> filteredTasks;
    std::copy_if(tasks.begin(), tasks.end(), std::back_inserter(filteredTasks),
                [urgency](const Task& task) { return task.urgency == urgency; });
    return filteredTasks;
}

std::vector<Task> TodoApp::getCompletedTasks() const {
    std::vector<Task> completedTasks;
    std::copy_if(tasks.begin(), tasks.end(), std::back_inserter(completedTasks),
                [](const Task& task) { return task.completed; });
    return completedTasks;
}

std::vector<Task> TodoApp::getPendingTasks() const {
    std::vector<Task> pendingTasks;
    std::copy_if(tasks.begin(), tasks.end(), std::back_inserter(pendingTasks),
                [](const Task& task) { return !task.completed; });
    return pendingTasks;
}

bool TodoApp::exportToFile(const std::string& filename) const {
    std::ofstream file(filename);
    if (!file.is_open()) {
        std::cout << "Error: Could not open file " << filename << " for writing." << std::endl;
        return false;
    }
    
    file << "TODO APP EXPORT - " << getCurrentTimestamp() << std::endl;
    file << std::string(50, '=') << std::endl;
    
    for (const auto& task : tasks) {
        file << "ID: " << task.id << std::endl;
        file << "Description: " << task.description << std::endl;
        file << "Urgency: " << task.getUrgencyString() << std::endl;
        file << "Created: " << task.getCreatedTimeString() << std::endl;
        file << "Status: " << (task.completed ? "COMPLETED" : "PENDING") << std::endl;
        file << std::string(30, '-') << std::endl;
    }
    
    file.close();
    logAction("Exported tasks to file: " + filename);
    std::cout << "Tasks exported successfully to " << filename << std::endl;
    return true;
}

bool TodoApp::exportToCSV(const std::string& filename) const {
    std::ofstream file(filename);
    if (!file.is_open()) {
        std::cout << "Error: Could not open file " << filename << " for writing." << std::endl;
        return false;
    }
    
    // CSV Header
    file << "ID,Description,Urgency,Created,Status" << std::endl;
    
    for (const auto& task : tasks) {
        file << task.id << ","
             << "\"" << task.description << "\","
             << task.getUrgencyString() << ","
             << task.getCreatedTimeString() << ","
             << (task.completed ? "COMPLETED" : "PENDING") << std::endl;
    }
    
    file.close();
    logAction("Exported tasks to CSV: " + filename);
    std::cout << "Tasks exported successfully to CSV: " << filename << std::endl;
    return true;
}

bool TodoApp::exportToJSON(const std::string& filename) const {
    std::ofstream file(filename);
    if (!file.is_open()) {
        std::cout << "Error: Could not open file " << filename << " for writing." << std::endl;
        return false;
    }
    
    file << "{\n  \"tasks\": [\n";
    
    for (size_t i = 0; i < tasks.size(); ++i) {
        const auto& task = tasks[i];
        file << "    {\n";
        file << "      \"id\": " << task.id << ",\n";
        file << "      \"description\": \"" << task.description << "\",\n";
        file << "      \"urgency\": \"" << task.getUrgencyString() << "\",\n";
        file << "      \"created\": \"" << task.getCreatedTimeString() << "\",\n";
        file << "      \"completed\": " << (task.completed ? "true" : "false") << "\n";
        file << "    }";
        if (i < tasks.size() - 1) file << ",";
        file << "\n";
    }
    
    file << "  ],\n";
    file << "  \"exported_at\": \"" << getCurrentTimestamp() << "\"\n";
    file << "}\n";
    
    file.close();
    logAction("Exported tasks to JSON: " + filename);
    std::cout << "Tasks exported successfully to JSON: " << filename << std::endl;
    return true;
}

void TodoApp::clearCompleted() {
    size_t initialSize = tasks.size();
    tasks.erase(std::remove_if(tasks.begin(), tasks.end(),
                              [](const Task& task) { return task.completed; }),
                tasks.end());
    
    size_t removedCount = initialSize - tasks.size();
    if (removedCount > 0) {
        logAction("Cleared " + std::to_string(removedCount) + " completed tasks");
        std::cout << "Cleared " << removedCount << " completed tasks." << std::endl;
    } else {
        std::cout << "No completed tasks to clear." << std::endl;
    }
}

int TodoApp::getTotalTasks() const {
    return static_cast<int>(tasks.size());
}

int TodoApp::getPendingTasksCount() const {
    return static_cast<int>(std::count_if(tasks.begin(), tasks.end(),
                                         [](const Task& task) { return !task.completed; }));
}

int TodoApp::getCompletedTasksCount() const {
    return static_cast<int>(std::count_if(tasks.begin(), tasks.end(),
                                         [](const Task& task) { return task.completed; }));
}

void TodoApp::displayStatistics() const {
    std::cout << "\n=== STATISTICS ===" << std::endl;
    std::cout << "Total Tasks: " << getTotalTasks() << std::endl;
    std::cout << "Pending Tasks: " << getPendingTasksCount() << std::endl;
    std::cout << "Completed Tasks: " << getCompletedTasksCount() << std::endl;
    
    // Count by urgency
    int critical = 0, high = 0, medium = 0, low = 0;
    for (const auto& task : tasks) {
        if (!task.completed) {
            switch (task.urgency) {
                case Urgency::CRITICAL: critical++; break;
                case Urgency::HIGH: high++; break;
                case Urgency::MEDIUM: medium++; break;
                case Urgency::LOW: low++; break;
            }
        }
    }
    
    std::cout << "\nPending Tasks by Urgency:" << std::endl;
    std::cout << "  Critical: " << critical << std::endl;
    std::cout << "  High: " << high << std::endl;
    std::cout << "  Medium: " << medium << std::endl;
    std::cout << "  Low: " << low << std::endl;
    std::cout << std::endl;
}

Task* TodoApp::findTaskById(int id) {
    auto it = std::find_if(tasks.begin(), tasks.end(),
                          [id](const Task& task) { return task.id == id; });
    return (it != tasks.end()) ? &(*it) : nullptr;
}

std::string TodoApp::getCurrentTimestamp() const {
    auto now = std::chrono::system_clock::now();
    auto time_t = std::chrono::system_clock::to_time_t(now);
    std::stringstream ss;
    ss << std::put_time(std::localtime(&time_t), "%Y-%m-%d %H:%M:%S");
    return ss.str();
}

void TodoApp::logAction(const std::string& action) const {
    std::ofstream logFile(logFileName, std::ios::app);
    if (logFile.is_open()) {
        logFile << "[" << getCurrentTimestamp() << "] " << action << std::endl;
        logFile.close();
    }
}

// Utility Functions
std::string urgencyToString(Urgency urgency) {
    switch (urgency) {
        case Urgency::LOW: return "LOW";
        case Urgency::MEDIUM: return "MEDIUM";
        case Urgency::HIGH: return "HIGH";
        case Urgency::CRITICAL: return "CRITICAL";
        default: return "UNKNOWN";
    }
}

Urgency stringToUrgency(const std::string& urgencyStr) {
    if (urgencyStr == "LOW" || urgencyStr == "1") return Urgency::LOW;
    if (urgencyStr == "MEDIUM" || urgencyStr == "2") return Urgency::MEDIUM;
    if (urgencyStr == "HIGH" || urgencyStr == "3") return Urgency::HIGH;
    if (urgencyStr == "CRITICAL" || urgencyStr == "4") return Urgency::CRITICAL;
    return Urgency::MEDIUM; // Default
}

int urgencyToInt(Urgency urgency) {
    return static_cast<int>(urgency);
}

Urgency intToUrgency(int urgencyInt) {
    switch (urgencyInt) {
        case 1: return Urgency::LOW;
        case 2: return Urgency::MEDIUM;
        case 3: return Urgency::HIGH;
        case 4: return Urgency::CRITICAL;
        default: return Urgency::MEDIUM;
    }
}

// Interactive menu functions
void displayMenu() {
    std::cout << "\n=== TODO APP MENU ===" << std::endl;
    std::cout << "1. Add Task" << std::endl;
    std::cout << "2. View All Tasks" << std::endl;
    std::cout << "3. View Tasks Sorted by Urgency" << std::endl;
    std::cout << "4. Mark Task as Completed" << std::endl;
    std::cout << "5. Remove Task" << std::endl;
    std::cout << "6. View Statistics" << std::endl;
    std::cout << "7. Export Tasks" << std::endl;
    std::cout << "8. Clear Completed Tasks" << std::endl;
    std::cout << "9. Filter Tasks by Urgency" << std::endl;
    std::cout << "0. Exit" << std::endl;
    std::cout << "Enter your choice: ";
}

void displayUrgencyMenu() {
    std::cout << "\nSelect urgency level:" << std::endl;
    std::cout << "1. Low" << std::endl;
    std::cout << "2. Medium" << std::endl;
    std::cout << "3. High" << std::endl;
    std::cout << "4. Critical" << std::endl;
    std::cout << "Enter urgency (1-4): ";
}

void displayExportMenu() {
    std::cout << "\nSelect export format:" << std::endl;
    std::cout << "1. Text file (.txt)" << std::endl;
    std::cout << "2. CSV file (.csv)" << std::endl;
    std::cout << "3. JSON file (.json)" << std::endl;
    std::cout << "Enter format (1-3): ";
}

Urgency getUserUrgency() {
    int choice;
    while (true) {
        displayUrgencyMenu();
        if (std::cin >> choice && choice >= 1 && choice <= 4) {
            std::cin.ignore(); // Clear the newline
            return intToUrgency(choice);
        } else {
            std::cout << "Invalid input! Please enter a number between 1-4." << std::endl;
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
    }
}

std::string getUserInput(const std::string& prompt) {
    std::string input;
    std::cout << prompt;
    std::getline(std::cin, input);
    return input;
}

int getUserChoice() {
    int choice;
    while (true) {
        if (std::cin >> choice) {
            std::cin.ignore(); // Clear the newline
            return choice;
        } else {
            std::cout << "Invalid input! Please enter a number: ";
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
    }
}

void handleAddTask(TodoApp& app) {
    std::string description = getUserInput("Enter task description: ");
    if (description.empty()) {
        std::cout << "Task description cannot be empty!" << std::endl;
        return;
    }
    
    Urgency urgency = getUserUrgency();
    app.addTask(description, urgency);
}

void handleMarkCompleted(TodoApp& app) {
    if (app.getTotalTasks() == 0) {
        std::cout << "No tasks available!" << std::endl;
        return;
    }
    
    app.displayTasks();
    std::cout << "Enter task ID to mark as completed: ";
    int id = getUserChoice();
    app.markCompleted(id);
}

void handleRemoveTask(TodoApp& app) {
    if (app.getTotalTasks() == 0) {
        std::cout << "No tasks available!" << std::endl;
        return;
    }
    
    app.displayTasks();
    std::cout << "Enter task ID to remove: ";
    int id = getUserChoice();
    app.removeTask(id);
}

void handleExportTasks(TodoApp& app) {
    if (app.getTotalTasks() == 0) {
        std::cout << "No tasks to export!" << std::endl;
        return;
    }
    
    int choice;
    displayExportMenu();
    choice = getUserChoice();
    
    std::string filename = getUserInput("Enter filename (without extension): ");
    if (filename.empty()) {
        filename = "todo_export";
    }
    
    bool success = false;
    switch (choice) {
        case 1:
            success = app.exportToFile(filename + ".txt");
            break;
        case 2:
            success = app.exportToCSV(filename + ".csv");
            break;
        case 3:
            success = app.exportToJSON(filename + ".json");
            break;
        default:
            std::cout << "Invalid choice!" << std::endl;
            return;
    }
    
    if (!success) {
        std::cout << "Export failed!" << std::endl;
    }
}

void handleFilterByUrgency(TodoApp& app) {
    if (app.getTotalTasks() == 0) {
        std::cout << "No tasks available!" << std::endl;
        return;
    }
    
    Urgency urgency = getUserUrgency();
    std::vector<Task> filteredTasks = app.getTasksByUrgency(urgency);
    
    if (filteredTasks.empty()) {
        std::cout << "No tasks found with " << urgencyToString(urgency) << " urgency." << std::endl;
        return;
    }
    
    std::cout << "\n=== TASKS WITH " << urgencyToString(urgency) << " URGENCY ===" << std::endl;
    std::cout << std::left << std::setw(5) << "ID" 
              << std::setw(40) << "Description" 
              << std::setw(20) << "Created" 
              << std::setw(10) << "Status" << std::endl;
    std::cout << std::string(75, '-') << std::endl;
    
    for (const auto& task : filteredTasks) {
        std::cout << std::left << std::setw(5) << task.id
                  << std::setw(40) << task.description.substr(0, 39)
                  << std::setw(20) << task.getCreatedTimeString()
                  << std::setw(10) << (task.completed ? "DONE" : "PENDING") << std::endl;
    }
    std::cout << std::endl;
}

// Interactive main function
int main() {
    std::cout << "=== Welcome to Interactive TODO App ===" << std::endl;
    std::cout << "Your tasks will be logged to 'todo_log.txt'" << std::endl;
    
    TodoApp app("todo_log.txt");
    int choice;
    
    while (true) {
        displayMenu();
        choice = getUserChoice();
        
        switch (choice) {
            case 1:
                handleAddTask(app);
                break;
            case 2:
                app.displayTasks();
                break;
            case 3:
                app.displayTasksSortedByUrgency();
                break;
            case 4:
                handleMarkCompleted(app);
                break;
            case 5:
                handleRemoveTask(app);
                break;
            case 6:
                app.displayStatistics();
                break;
            case 7:
                handleExportTasks(app);
                break;
            case 8:
                app.clearCompleted();
                break;
            case 9:
                handleFilterByUrgency(app);
                break;
            case 0:
                std::cout << "Thank you for using TODO App! Goodbye!" << std::endl;
                return 0;
            default:
                std::cout << "Invalid choice! Please select 0-9." << std::endl;
                break;
        }
        
        // Pause before showing menu again
        std::cout << "\nPress Enter to continue...";
        std::cin.get();
    }
    
    return 0;
}