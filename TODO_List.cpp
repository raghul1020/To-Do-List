#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using namespace std;

class Todo
{
    int id;
    string task;

public:
    // Constructors
    Todo() : id(0), task("") {}
    Todo(int id, string task) : id(id), task(task) {}

    // Getters
    int getId() const { return id; }
    string getTask() const { return task; }

    // Setters
    void setTask(string newTask) { task = newTask; }

    // Save to file
    void save(ofstream &outFile) const
    {
        outFile << id << endl;
        outFile << task << endl;
    }

    // Load from file
    static Todo load(ifstream &inFile)
    {
        int id;
        string task;
        inFile >> id;
        inFile.ignore();
        getline(inFile, task);
        return Todo(id, task);
    }
};

class TodoList
{
    vector<Todo> todos;
    int currentId;

public:
    TodoList() : currentId(0) { loadFromFile(); }

    void addTodo(string task)
    {
        currentId++;
        todos.emplace_back(currentId, task);
        saveToFile();
    }

    void viewTodos() const
    {
        if (todos.empty())
        {
            cout << "No tasks found." << endl;
            return;
        }
        for (const auto &todo : todos)
        {
            cout << "ID: " << todo.getId() << " | Task: " << todo.getTask() << endl;
        }
    }

    void deleteTodo(int id)
    {
        auto it = remove_if(todos.begin(), todos.end(),
                            [id](const Todo &todo)
                            { return todo.getId() == id; });
        if (it != todos.end())
        {
            todos.erase(it, todos.end());
            saveToFile();
            cout << "Task deleted successfully." << endl;
        }
        else
        {
            cout << "Task not found." << endl;
        }
    }

    void searchTodo(int id) const
    {
        for (const auto &todo : todos)
        {
            if (todo.getId() == id)
            {
                cout << "ID: " << todo.getId() << " | Task: " << todo.getTask() << endl;
                return;
            }
        }
        cout << "Task not found." << endl;
    }

private:
    void loadFromFile()
    {
        ifstream inFile("todo.txt");
        if (!inFile)
        {
            currentId = 0;
            return;
        }
        todos.clear();
        while (!inFile.eof())
        {
            Todo todo = Todo::load(inFile);
            if (!inFile.fail())
            {
                todos.push_back(todo);
                currentId = max(currentId, todo.getId());
            }
        }
    }

    void saveToFile() const
    {
        ofstream outFile("todo.txt");
        for (const auto &todo : todos)
        {
            todo.save(outFile);
        }
    }
};

int main()
{
    TodoList todoList;
    int choice;

    do
    {
        cout << "\n1. Add Task\n2. View Tasks\n3. Delete Task\n4. Search Task\n5. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice)
        {
        case 1:
        {
            cin.ignore();
            string task;
            cout << "Enter the task: ";
            getline(cin, task);
            todoList.addTodo(task);
            break;
        }
        case 2:
            todoList.viewTodos();
            break;
        case 3:
        {
            int id;
            cout << "Enter task ID to delete: ";
            cin >> id;
            todoList.deleteTodo(id);
            break;
        }
        case 4:
        {
            int id;
            cout << "Enter task ID to search: ";
            cin >> id;
            todoList.searchTodo(id);
            break;
        }
        case 5:
            cout << "Exiting..." << endl;
            break;
        default:
            cout << "Invalid choice. Try again!" << endl;
        }
    } while (choice != 5);

    return 0;
}
