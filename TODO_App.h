#ifndef TODO_APP_H
#define TODO_APP_H

#include <iostream>
#include <vector>
#include <string>
#include <chrono>
#include <ctime>
#include <fstream>
#include <algorithm>
#include <iomanip>
#include <sstream>

/**
 * @brief Enumeration for task urgency levels
 * 
 * Defines four levels of task urgency from lowest to highest priority.
 * Used for sorting and filtering tasks based on importance.
 */
enum class Urgency {
    LOW = 1,      ///< Low priority task
    MEDIUM = 2,   ///< Medium priority task
    HIGH = 3,     ///< High priority task
    CRITICAL = 4  ///< Critical priority task
};

/**
 * @brief Task structure to hold individual task data
 * 
 * Represents a single task with all its properties including ID, description,
 * urgency level, creation timestamp, and completion status.
 */
struct Task {
    int id;                                              ///< Unique task identifier
    std::string description;                             ///< Task description text
    Urgency urgency;                                     ///< Task urgency level
    std::chrono::system_clock::time_point createdAt;     ///< Task creation timestamp
    bool completed;                                      ///< Task completion status
    
    /**
     * @brief Constructor for Task
     * @param taskId Unique identifier for the task
     * @param desc Description of the task
     * @param urg Urgency level of the task
     * 
     * Creates a new task with the specified parameters. The creation time
     * is automatically set to the current system time, and the task is
     * initially marked as not completed.
     */
    Task(int taskId, const std::string& desc, Urgency urg);
    
    /**
     * @brief Get formatted creation time as string
     * @return Formatted date and time string (YYYY-MM-DD HH:MM:SS)
     * 
     * Converts the creation timestamp to a human-readable string format
     * suitable for display and export purposes.
     */
    std::string getCreatedTimeString() const;
    
    /**
     * @brief Get urgency as string
     * @return String representation of the urgency level
     * 
     * Converts the urgency enum value to its corresponding string
     * representation (LOW, MEDIUM, HIGH, CRITICAL).
     */
    std::string getUrgencyString() const;
    
    /**
     * @brief Comparison operator for sorting by urgency (descending)
     * @param other The other task to compare with
     * @return true if this task has higher urgency than the other
     * 
     * Implements comparison logic for sorting tasks. Primary sort is by
     * urgency (highest first), secondary sort is by creation time (oldest first).
     */
    bool operator<(const Task& other) const;
};

/**
 * @brief Main TODO Application class
 * 
 * The core class that manages all TODO application functionality including
 * task management, logging, export/import operations, and statistics.
 * Provides a complete interface for task manipulation and data persistence.
 */
class TodoApp {
private:
    std::vector<Task> tasks;    ///< Container for all tasks
    int nextId;                 ///< Next available task ID
    std::string logFileName;    ///< Name of the log file for action logging
    
    /**
     * @brief Helper function to get current timestamp string
     * @return Current date and time as formatted string
     * 
     * Private utility function that generates a timestamp string
     * in YYYY-MM-DD HH:MM:SS format for logging purposes.
     */
    std::string getCurrentTimestamp() const;
    
    /**
     * @brief Helper function to log actions
     * @param action Description of the action to log
     * 
     * Private utility function that writes timestamped action logs
     * to the specified log file for audit trail purposes.
     */
    void logAction(const std::string& action) const;

public:
    /**
     * @brief Constructor for TodoApp
     * @param logFile Name of the log file (default: "todo_log.txt")
     * 
     * Initializes the TODO application with the specified log file.
     * Sets up the initial state and logs the application startup.
     */
    TodoApp(const std::string& logFile = "todo_log.txt");
    
    /**
     * @brief Destructor for TodoApp
     * 
     * Cleans up resources and logs the application termination.
     */
    ~TodoApp();
    
    // Core functionality
    
    /**
     * @brief Add a new task to the application
     * @param description Text description of the task
     * @param urgency Urgency level of the task
     * 
     * Creates a new task with the given description and urgency level.
     * Automatically assigns a unique ID and sets the creation timestamp.
     * Logs the action and provides user feedback.
     */
    void addTask(const std::string& description, Urgency urgency);
    
    /**
     * @brief Remove a task by its ID
     * @param id Unique identifier of the task to remove
     * 
     * Searches for and removes the task with the specified ID.
     * Logs the action and provides appropriate user feedback.
     * If the task is not found, displays an error message.
     */
    void removeTask(int id);
    
    /**
     * @brief Mark a task as completed
     * @param id Unique identifier of the task to mark as completed
     * 
     * Finds the task with the specified ID and marks it as completed.
     * Logs the action and provides user feedback. If the task is not
     * found, displays an error message.
     */
    void markCompleted(int id);
    
    /**
     * @brief Display all tasks in a formatted table
     * 
     * Shows all tasks (both completed and pending) in a tabular format
     * with columns for ID, description, urgency, creation time, and status.
     * If no tasks exist, displays an appropriate message.
     */
    void displayTasks() const;
    
    /**
     * @brief Display tasks sorted by urgency level
     * 
     * Shows all tasks sorted by urgency (highest priority first),
     * with secondary sorting by creation time. Uses the same tabular
     * format as displayTasks().
     */
    void displayTasksSortedByUrgency() const;
    
    // Search and filter functions
    
    /**
     * @brief Get tasks filtered by urgency level
     * @param urgency The urgency level to filter by
     * @return Vector of tasks matching the specified urgency level
     * 
     * Returns a copy of all tasks that have the specified urgency level.
     * The returned vector can be empty if no matching tasks are found.
     */
    std::vector<Task> getTasksByUrgency(Urgency urgency) const;
    
    /**
     * @brief Get all completed tasks
     * @return Vector of all completed tasks
     * 
     * Returns a copy of all tasks that have been marked as completed.
     * Useful for generating completion reports or statistics.
     */
    std::vector<Task> getCompletedTasks() const;
    
    /**
     * @brief Get all pending (incomplete) tasks
     * @return Vector of all pending tasks
     * 
     * Returns a copy of all tasks that have not been completed yet.
     * Useful for focusing on remaining work or generating todo lists.
     */
    std::vector<Task> getPendingTasks() const;
    
    // Export functions
    
    /**
     * @brief Export tasks to plain text file
     * @param filename Name of the output file
     * @return true if export successful, false otherwise
     * 
     * Exports all tasks to a human-readable plain text format.
     * Includes all task details with clear formatting and separators.
     * Logs the export action upon success.
     */
    bool exportToFile(const std::string& filename) const;
    
    /**
     * @brief Export tasks to CSV format
     * @param filename Name of the output CSV file
     * @return true if export successful, false otherwise
     * 
     * Exports all tasks to CSV format suitable for spreadsheet applications.
     * Includes headers and properly quoted fields. Logs the export action
     * upon success.
     */
    bool exportToCSV(const std::string& filename) const;
    
    /**
     * @brief Export tasks to JSON format
     * @param filename Name of the output JSON file
     * @return true if export successful, false otherwise
     * 
     * Exports all tasks to JSON format suitable for web applications
     * and APIs. Includes metadata such as export timestamp.
     * Logs the export action upon success.
     */
    bool exportToJSON(const std::string& filename) const;
    
    /**
     * @brief Import tasks from file
     * @param filename Name of the input file
     * @return true if import successful, false otherwise
     * 
     * @note This function is declared but not implemented in the current version
     * 
     * Placeholder for future functionality to import tasks from
     * external files. Would support the reverse operation of export functions.
     */
    bool importFromFile(const std::string& filename);
    
    // Utility functions
    
    /**
     * @brief Clear all completed tasks
     * 
     * Removes all tasks that have been marked as completed from the
     * task list. Provides feedback on the number of tasks removed.
     * Logs the cleanup action with the count of removed tasks.
     */
    void clearCompleted();
    
    /**
     * @brief Get the total number of tasks
     * @return Total count of tasks (completed and pending)
     * 
     * Returns the total number of tasks currently in the application,
     * regardless of their completion status.
     */
    int getTotalTasks() const;
    
    /**
     * @brief Get the number of pending tasks
     * @return Count of tasks that are not completed
     * 
     * Returns the number of tasks that still need to be completed.
     * Useful for progress tracking and workload assessment.
     */
    int getPendingTasksCount() const;
    
    /**
     * @brief Get the number of completed tasks
     * @return Count of tasks that have been completed
     * 
     * Returns the number of tasks that have been marked as completed.
     * Useful for productivity tracking and progress reports.
     */
    int getCompletedTasksCount() const;
    
    /**
     * @brief Display comprehensive statistics
     * 
     * Shows detailed statistics including total, pending, and completed
     * task counts, as well as a breakdown of pending tasks by urgency level.
     * Provides a comprehensive overview of the current task status.
     */
    void displayStatistics() const;
    
    /**
     * @brief Find a task by its ID
     * @param id Unique identifier of the task to find
     * @return Pointer to the task if found, nullptr otherwise
     * 
     * Searches for a task with the specified ID and returns a pointer
     * to it if found. Returns nullptr if no task with the given ID exists.
     * The returned pointer can be used to modify the task directly.
     */
    Task* findTaskById(int id);
};

// Utility functions for urgency conversion

/**
 * @brief Convert urgency enum to string
 * @param urgency The urgency level to convert
 * @return String representation of the urgency level
 * 
 * Converts an Urgency enum value to its corresponding string representation.
 * Returns "UNKNOWN" for invalid urgency values.
 * 
 * @par Example:
 * @code
 * Urgency level = Urgency::HIGH;
 * std::string levelStr = urgencyToString(level); // Returns "HIGH"
 * @endcode
 */
std::string urgencyToString(Urgency urgency);

/**
 * @brief Convert string to urgency enum
 * @param urgencyStr String representation of urgency level
 * @return Corresponding Urgency enum value
 * 
 * Converts a string to its corresponding Urgency enum value.
 * Accepts both string names ("LOW", "MEDIUM", etc.) and numeric strings ("1", "2", etc.).
 * Returns Urgency::MEDIUM as default for unrecognized inputs.
 * 
 * @par Example:
 * @code
 * Urgency level = stringToUrgency("HIGH"); // Returns Urgency::HIGH
 * Urgency level2 = stringToUrgency("3");   // Also returns Urgency::HIGH
 * @endcode
 */
Urgency stringToUrgency(const std::string& urgencyStr);

/**
 * @brief Convert urgency enum to integer
 * @param urgency The urgency level to convert
 * @return Integer representation of the urgency level (1-4)
 * 
 * Converts an Urgency enum value to its underlying integer value.
 * Useful for numerical operations and comparisons.
 * 
 * @par Example:
 * @code
 * int priority = urgencyToInt(Urgency::CRITICAL); // Returns 4
 * @endcode
 */
int urgencyToInt(Urgency urgency);

/**
 * @brief Convert integer to urgency enum
 * @param urgencyInt Integer value to convert (1-4)
 * @return Corresponding Urgency enum value
 * 
 * Converts an integer value to its corresponding Urgency enum value.
 * Returns Urgency::MEDIUM as default for values outside the valid range (1-4).
 * 
 * @par Example:
 * @code
 * Urgency level = intToUrgency(4); // Returns Urgency::CRITICAL
 * @endcode
 */
Urgency intToUrgency(int urgencyInt);

#endif // TODO_APP_H