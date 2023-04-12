#include "TaskManager.h"
#include <imgui.h>
#include <string>


Task::Task(std::string const& _title, std::string  const& _description) :
    complete{ false },
    progress{ 0 },
    title{ _title },
    description{ _description } { };

void Task::ToggleCompletion() {
    complete = !complete;
}

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
            if (ImGui::Selectable(tasks.at(n).title.c_str(), is_selected)) {
                SelectTask(n);
                showDetailsPanel = true;
            }

            // Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
            if (is_selected)
                ImGui::SetItemDefaultFocus();
        }
        ImGui::EndListBox();
    }

    ImGui::End();
}

void TaskManager::TaskDetailsPanel(bool& isOpen) const {
    if (tasks.empty() || item_current_idx == 0) return;

    Task currentTask = tasks.at(item_current_idx);

    static bool complete = false;
    if (currentTask.complete != complete) {
        currentTask.ToggleCompletion();
    }

    ImGui::Begin("Task Details", &isOpen);

    ImGui::Text(currentTask.title.c_str()); ImGui::SameLine();
    ImGui::ProgressBar(static_cast<float>(currentTask.progress), ImVec2(0.0f,0.0f)); ImGui::SameLine();
    ImGui::Checkbox("##Complete", &complete); ImGui::SameLine();
    ImGui::Text(currentTask.description.c_str());

    ImGui::End();
}

void TaskManager::CreateTask(const char* title, const char* description) {
    auto task = Task(title, description);
    tasks.insert({tasks.size()+1, task });
}

void TaskManager::SelectTask(int selectionIndex) {
    item_current_idx = selectionIndex;
}
