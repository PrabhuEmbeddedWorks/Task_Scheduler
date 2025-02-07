#include <iostream>
#include <thread>
#include <chrono>
#include <unordered_map>

// Task structure
struct Task {
    int taskID;
    int priority;
    int executionTime;
    std::string taskName;
    Task* next;

    Task(int id, int pri, int execTime, std::string name)
        : taskID(id), priority(pri), executionTime(execTime), taskName(name), next(nullptr) {}
};

// Linked list to manage tasks
class LinkedList {
public:
    Task* head;

    LinkedList() : head(nullptr) {}

    // Insert task into the list based on priority
    void insert(Task* newTask) {
        if (!head || head->priority < newTask->priority) {
            newTask->next = head;
            head = newTask;
        } else {
            Task* current = head;
            while (current->next && current->next->priority >= newTask->priority) {
                current = current->next;
            }
            newTask->next = current->next;
            current->next = newTask;
        }
    }

    // Remove and return the highest priority task
    Task* remove() {
        if (!head) return nullptr;
        Task* temp = head;
        head = head->next;
        return temp;
    }

    // Check if the list is empty
    bool isEmpty() {
        return head == nullptr;
    }
};

// Scheduler class to manage tasks
class Scheduler {
private:
    LinkedList taskList;
    std::unordered_map<int, Task*> taskMap; // Hash table to store tasks by ID

public:
    // Add task to the scheduler
    void addTask(int id, int priority, int executionTime, const std::string& name) {
        if (taskMap.find(id) != taskMap.end()) {
            std::cout << "Task with ID " << id << " already exists.\n";
            return;
        }
       
        Task* newTask = new Task(id, priority, executionTime, name);
        taskList.insert(newTask);
        taskMap[id] = newTask; // Add to hash table
    }

    // Execute tasks based on priority
    void executeTasks() {
        while (!taskList.isEmpty()) {
            Task* currentTask = taskList.remove();
            taskMap.erase(currentTask->taskID); // Remove from hash table
            std::cout << "Executing Task ID: " << currentTask->taskID
                      << " | Name: " << currentTask->taskName
                      << " | Priority: " << currentTask->priority
                      << " | Execution Time: " << currentTask->executionTime << " seconds." << std::endl;

            std::this_thread::sleep_for(std::chrono::seconds(currentTask->executionTime));
            delete currentTask;
        }
    }

    // Find task by ID
    Task* findTask(int id) {
        if (taskMap.find(id) != taskMap.end()) {
            return taskMap[id];
        }
        return nullptr;
    }
};

// Main function
int main() {
    Scheduler scheduler;
    int choice;

    do {
        std::cout << "1. Add Task\n2. Execute Tasks\n3. Exit\nEnter your choice: ";
        std::cin >> choice;

        if (choice == 1) {
            int id, priority, executionTime;
            std::string name;

            std::cout << "Enter Task ID: ";
            std::cin >> id;
            std::cout << "Enter Task Priority: ";
            std::cin >> priority;
            std::cout << "Enter Task Execution Time (in seconds): ";
            std::cin >> executionTime;
            std::cout << "Enter Task Name: ";
            std::cin.ignore(); // To ignore the newline character left in the input buffer
            std::getline(std::cin, name);

            scheduler.addTask(id, priority, executionTime, name);
        } else if (choice == 2) {
            scheduler.executeTasks();
        }

    } while (choice != 3);

    return 0;
}
