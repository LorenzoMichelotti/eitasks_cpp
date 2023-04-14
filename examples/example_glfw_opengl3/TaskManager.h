#pragma once
#include <list>
#include <unordered_map>
class Task
{
public:
    bool complete {};
    int progress {};
    std::string title {};
    std::string description {};
    Task();
    Task(std::string const & _title, std::string const& _description);
};

class TaskManager
{
public:
    TaskManager();
    std::vector<int> usedIds {};
    int item_current_idx {};
    std::unordered_map<int, Task> tasks {};
    bool show_create_task_window = true;
    void CreateTaskPanel();
    void TaskListPanel(bool& taskListOpen, bool& showDetailsPanel);
    void TaskDetailsPanel(bool& taskDetailsOpen);
    void CreateTask(const char* title, const char* description);
    void SelectTask(int selectionIndex);
    void UpdateTask(const Task& updatedTask, int index);
    int GetFreeId();
};

