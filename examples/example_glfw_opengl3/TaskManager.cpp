#include "TaskManager.h"
#include <imgui.h>
#include <string>
#include <algorithm>

Task::Task() = default;

Task::Task(std::string const& _title, std::string  const& _description) :
    complete{ false },
    progress{ 0 },
    title{ _title },
    description{ _description } { };

TaskManager::TaskManager() : item_current_idx{ 0 } {};

void TaskManager::CreateTaskPanel() {
    ImGui::Begin("Create task");   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
    ImGui::PushItemWidth(ImGui::GetWindowWidth() * 0.94f);
    ImGui::Text("Create a new task");
    static char titleBuffer[25] = "";
    ImGui::InputTextWithHint("##title", "title", titleBuffer, IM_ARRAYSIZE(titleBuffer));
    static char descriptionBuffer[250] = "";
    ImGui::InputTextMultiline("##description", descriptionBuffer, IM_ARRAYSIZE(descriptionBuffer));
    if (ImGui::Button("Create")) {
        CreateTask(titleBuffer, descriptionBuffer);
        memset(titleBuffer, 0, IM_ARRAYSIZE(titleBuffer));
        memset(descriptionBuffer, 0, IM_ARRAYSIZE(descriptionBuffer));
    }
    ImGui::End();
}

void TaskManager::TaskListPanel(bool& isOpen, bool& showDetailsPanel) {
    ImGui::Begin("Task List", &isOpen);
    ImGui::Text(" ");
    if (ImGui::BeginListBox("##listbox 2", ImVec2(-FLT_MIN, ImGui::GetWindowHeight() * 0.65f)))
    {
        for (int n = 1; n <= tasks.size(); n++)
        {
            const bool is_selected = (item_current_idx == n);
            auto it = tasks.find(n);
            if (it != tasks.end()) {
                Task currentTask = it->second;
                if (ImGui::Selectable(("[" + std::to_string(currentTask.progress) + "%] - " + currentTask.title).c_str(), is_selected)) {
                    SelectTask(n);
                    showDetailsPanel = true;
                }

                // Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
                if (is_selected)
                    ImGui::SetItemDefaultFocus();
            }
        }
        ImGui::EndListBox();
    }

    ImGui::End();
}

void TaskManager::TaskDetailsPanel(bool& isOpen) {
    if (tasks.empty() || item_current_idx == 0) return;

    auto it = tasks.find(item_current_idx);
    if (it != tasks.end()) {
        Task currentTask = it->second;

        ImGui::Begin("Task Details", &isOpen);

        ImGui::Text(currentTask.title.c_str());
        ImGui::SameLine();

        if (ImGui::Button("Delete")) {
            tasks.erase(item_current_idx);
            item_current_idx = 0;
        }

        ImGui::ProgressBar(static_cast<float>(currentTask.progress), ImVec2(0.0f, 0.0f)); ImGui::SameLine();
        if (ImGui::Checkbox("##Complete", &currentTask.complete)) {
            currentTask.progress = currentTask.complete ? 100 : 0;
            UpdateTask(currentTask, item_current_idx);
        }

        ImGui::SameLine();
        ImGui::Text(currentTask.description.c_str());

        ImGui::End();
    }
}

void TaskManager::CreateTask(const char* title, const char* description) {
    if (title == nullptr || title[0] == '\0') return;
    auto task = Task(title, description);
    tasks.insert({tasks.size()+1, task });
}

void TaskManager::SelectTask(int selectionIndex) {
    item_current_idx = selectionIndex;
}

void TaskManager::UpdateTask(const Task& updatedTask, int index) {
    tasks[index] = updatedTask;
}

int TaskManager::GetFreeId() {
    if (usedIds.size() <= 0) {
        usedIds.push_back(1);
        return 1;
    }
    int a = (*std::max_element(usedIds.begin(), usedIds.end())) + 1;
    usedIds.push_back(a);
    return a;
}
