#include <iostream>
#include <queue>
#include <string>
#include<stack>
using namespace std;

struct Task {
    string title;
    string des;
    string due_date;
    string priority;
    Task* next;
    Task* previous;
};

struct List {
    int s;
    Task* head;
    Task* tail;
};
struct Action {
    enum ActionType { Add, Edit, Delete };
    ActionType type;
    int position;
    Task originalTask;
};

List* CreateEmptyTask() {
    List* t = new List;
    t->s = 0;
    t->head = nullptr;
    t->tail = nullptr;
    return t;
}

void AddBeginTask(List* t,string title1, string description, string due_datee, string priority1) {
    Task* ts = new Task;
    ts->title = title1;
    ts->des = description;
    ts->due_date = due_datee;
    ts->priority = priority1;
    ts->next = nullptr;
    ts->previous = nullptr;

    if (t->s == 0) {
        t->head = ts;
        t->tail = ts;
        t->s = t->s + 1;
    } else {
        ts->next = t->head;
        t->head->previous = ts;
        t->head = ts;
        t->s = t->s + 1;
    }
}

void DisplayTask(List* t) {
    Task* p = t->head;
    int i = 1;
    while (p != nullptr) {
        cout << "============Task " << i << "===============" << endl;
        cout << "Title: " << p->title << endl;
        cout << "Description: " << p->des << endl;
        cout << "Due Date: " << p->due_date << endl;
        cout << "Priority level: " << p->priority << endl;
        p = p->next;
        i++;
    }
    cout << endl;
}

struct Task_priority {
    bool operator()(const Task& task1, const Task& task2) const {
        if (task1.priority != task2.priority) {
            return task1.priority > task2.priority;
        } else {
            return task1.due_date > task2.due_date;
        }
    }
};


void Delete_First_task(List* t) {
    if (t->s == 0) {
        cout << "No Task to delete!" << endl;
    } else {
        Task* p = t->head;
        t->head = t->head->next;
        delete p;
    }
}

void Delete_Last_task(List* t) {
    if (t->s == 0) {
        cout << "No Task to delete!" << endl;
    } else if (t->s == 1) {
        delete t->head;
        t->head = t->tail = nullptr;
    } else {
        Task* p = t->head;
        while (p->next != t->tail) {
            p = p->next;
        }
        delete t->tail;
        t->tail = p;
        t->tail->next = nullptr;
    }
}
void UpdateAndDisplayPriorityQueue(priority_queue<Task, vector<Task>, Task_priority>& taskQueue, List* t) {

    while (!taskQueue.empty()) {
        taskQueue.pop();
    }

    Task* current = t->head;
    while (current != nullptr) {
        taskQueue.push(*current);
        current = current->next;
    }
    cout << "Updated Priority Queue:" << endl;
    while (!taskQueue.empty()) {
        Task topTask = taskQueue.top();
        cout << "Title: " << topTask.title << "\tDue Date: " << topTask.due_date << "\tPriority: " << topTask.priority << endl;
        taskQueue.pop();
    }
}


void Delete_Position(List* t, int poss,priority_queue<Task, vector<Task>, Task_priority>& taskQueue) {
    if (poss < 1 || poss > t->s) {
        cout << "Invalid position to delete!" << endl;
        return;
    }

    if (poss == 1) {
        Delete_First_task(t);
        cout << "Deleted the first task successfully!" << endl;
    } else if (poss == t->s) {
        Delete_Last_task(t);
        cout << "Deleted the last task successfully!" << endl;
    } else {
        Task* current = t->head;
        Task* previous = nullptr;
        int i = 1;

        while (current != nullptr && i < poss) {
            previous = current;
            current = current->next;
            i++;
        }

        previous->next = current->next;
        delete current;
        t->s--;
    }
     UpdateAndDisplayPriorityQueue(taskQueue, t);
}

void Display_Position(List* t,int edit) {
    int i = 1;
    Task* e = t->head;

    while (e != nullptr && i < edit) {
        e = e->next;
        i++;
    }

    if (i == edit && e != nullptr) {
        cout << "1. Title: " << e->title << endl;
        cout << "2. Description: " << e->des << endl;
        cout << "3. Due Date: " << e->due_date << endl;
        cout << "4. Priority level: " << e->priority << endl;
    } else {
        cout << "Task not found." << endl;
    }

}

void EditTask(List* t,priority_queue<Task, vector<Task>, Task_priority>& taskQueue) {
    string Newtitle, New_description, New_due_date, New_priority;
    Task* temp = t->head;
    int position;
    int edit;
    cout << "Enter the task you want to edit: ";
    cin >> edit;
    int i = 1;

    while (temp != nullptr && i < edit) {
        temp = temp->next;
        i++;
    }

    Display_Position(t, edit);
    cout << "Enter the position you want to edit: ";
    cin >> position;
    cin.ignore();

    if (position == 1) {
        cout << "Enter a new title: ";
        getline(cin, Newtitle);
        temp->title = Newtitle;
        cout << "Update Successfully!" << endl;
    } else if (position == 2) {
        cout << "Enter new description: ";
        getline(cin, New_description);
        temp->des = New_description;
        cout << "Update Successfully!" << endl;
    } else if (position == 3) {
        cout << "Enter new due date: ";
        cin >> New_due_date;
        temp->due_date = New_due_date;
        cout << "Update Successfully!" << endl;
    } else if (position == 4) {
        cout << "Enter a new priority: ";
        cin >> New_priority;
        temp->priority = New_priority;
        cout << "Update Successfully!" << endl;
    } else {
        cout << "Position not found";
    }
     UpdateAndDisplayPriorityQueue(taskQueue, t);
}

int main() {
    int choice;
    int edit;
    Task*temp;
    cout << "Welcome to the Smart Task Manager!" << endl;
    cout << "===================================" << endl;

    List* Mytask = CreateEmptyTask();
    string title, description, due_date, priority;
    priority_queue<Task, vector<Task>, Task_priority> taskQueue;
     stack<Action> undoStack;

    cout << "Main Menu:" << endl;
    cout << "1. Add Task" << endl;
    cout << "2. View Task" << endl;
    cout << "3. Edit Task" << endl;
    cout << "4. Delete Task" << endl;
    cout << "5. Undo" << endl;
    cout << "6. Exit" << endl;
    cout << "===================================" << endl;

    do {
        cout << "Enter your choice: ";
        cin >> choice;

        if (choice == 1) {
            cin.ignore();
            cout << "Enter a title: ";
            getline(cin, title);
            cout << "Enter a description: ";
            getline(cin, description);
            cout << "Enter the deadline (DD-MM-YYYY): ";
            cin >> due_date;
            cout << "Enter a priority: ";
            cin >> priority;

            AddBeginTask(Mytask, title, description, due_date, priority);

            Task task;
            task.title = title;
            task.des = description;
            task.due_date = due_date;
            task.priority = priority;
            taskQueue.push(task);


            cout << "Task added successfully!" << endl;
        } else if (choice == 2) {
            cout << "Task List:" << endl;
            DisplayTask(Mytask);
        } else if (choice == 3) {
            int t;
            EditTask(Mytask,taskQueue);
            cout << "Task updated in double-linked list" << endl;
            DisplayTask(Mytask);

        }else if(choice==4) {

    int poss;
    DisplayTask(Mytask);
    cout << "Enter the task you want to delete: ";
    cin >> poss;
    cin.ignore();

    // Delete the task from the doubly linked list
    Delete_Position(Mytask, poss,taskQueue);
    cout << "Task deleted from double-linked list.\n" << endl;
    DisplayTask(Mytask);
    cout<<"Task deleted from priority queue\n";

        }

        else if(choice == 5) {


     }}while (choice != 6);

    return 0;
}
