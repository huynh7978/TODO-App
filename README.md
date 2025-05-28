# TODO-App📝
A feature-rich command-line TODO application written in C++ that helps you manage tasks with priority levels, timestamps, and multiple export formats.

## Features ✨

- Task Management: Add, remove, and mark tasks as completed  
- Priority System: Four urgency levels (Low, Medium, High, Critical)  
- Smart Sorting: Tasks sorted by urgency and creation time  
- Export Options: Export to TXT, CSV, and JSON formats  
- Statistics: View task counts and urgency breakdowns  
- Logging: All actions are logged with timestamps  
- Interactive Menu: User-friendly command-line interface  
- Filtering: Filter tasks by urgency level  

## Getting Started 🚀  
**Prerequisites**  
- C++ compiler with C++11 support (GCC, Clang, or MSVC)  
- Standard C++ libraries  

**Compilation**  
```
bash # Using g++ (recommended)    
g++ -std=c++11 -o todo_app TODO_App.cpp  
```
```
# Or using clang++  
clang++ -std=c++11 -o todo_app TODO_App.cpp  
```
**Running the Application**  
```
./todo_app  
```
## Usage 📖  
When you run the application, you'll see an interactive menu with the following options:  
**Main Menu Options**  

Add Task - Create a new task with description and urgency level  
View All Tasks - Display all tasks in a formatted table  
View Tasks Sorted by Urgency - Show tasks sorted by priority  
Mark Task as Completed - Mark a specific task as done  
Remove Task - Delete a task permanently  
View Statistics - Show task counts and urgency breakdown  
Export Tasks - Save tasks to file (TXT/CSV/JSON)  
Clear Completed Tasks - Remove all completed tasks  
Filter Tasks by Urgency - Show tasks of specific priority level  
Exit - Close the application  

**Urgency Levels**  
Low (1) - Non-urgent tasks  
Medium (2) - Normal priority tasks  
High (3) - Important tasks  
Critical (4) - Urgent, high-priority tasks  

**Example Workflow**  
1. Add a new task: "Buy groceries" with Medium urgency  
2. Add another task: "Submit report" with Critical urgency  
3. View tasks sorted by urgency (Critical tasks appear first)  
4. Mark "Buy groceries" as completed  
5. Export all tasks to CSV for backup  
6. View statistics to see task breakdown  

## Export Formats 📄  
Text Format (.txt)  
  TODO APP EXPORT - 2024-01-15 10:30:45  
  ==================================================  
  ID: 1  
  Description: Buy groceries  
  Urgency: MEDIUM  
  Created: 2024-01-15 09:15:32  
  Status: PENDING  
  ------------------------------
  
CSV Format (.csv)  
  csvID,Description,Urgency,Created,Status  
  1,"Buy groceries",MEDIUM,2024-01-15 09:15:32,PENDING  
  2,"Submit report",CRITICAL,2024-01-15 09:20:15,COMPLETED  
  
JSON Format (.json)  
```
{  
  "tasks": [  
    {  
      "id": 1,  
      "description": "Buy groceries",  
      "urgency": "MEDIUM",  
      "created": "2024-01-15 09:15:32",  
      "completed": false  
    }  
  ],  
  "exported_at": "2024-01-15 10:30:45"  
}  
```
## Class Structure 🏗️  
**Task Class**  
  Stores task information (ID, description, urgency, completion status, creation time)  
  Provides comparison operators for sorting  
  Handles time formatting and urgency string conversion  

**Todo_App Class**    
  Main application logic  
  Task management operations  
  File I/O operations  
  Logging functionality  
  Statistics calculation  

# Logging 📊  
All actions are automatically logged to todo_log.txt with timestamps:  
[2024-01-15 09:15:32] TodoApp initialized  
[2024-01-15 09:16:45] Added task [ID: 1] "Buy groceries" [MEDIUM]  
[2024-01-15 09:18:20] Completed task [ID: 1] "Buy groceries"  
[2024-01-15 09:20:10] Exported tasks to CSV: tasks_backup.csv  

# Error Handling 🛡️  
The application includes robust error handling for:  

Invalid user input  
File I/O operations  
Task ID validation  
Empty task lists  

# Contributing 🤝

Fork the repository  
Create a feature branch (git checkout -b feature/new-feature)  
Commit your changes (git commit -am 'Add new feature')  
Push to the branch (git push origin feature/new-feature)  
Create a Pull Request  

# License 📄  
This project is open source and available under the MIT License.  

#Support 💬  
If you encounter any issues or have questions:  

Check the existing issues in the repository  
Create a new issue with detailed description  
Include your system information and error messages  
