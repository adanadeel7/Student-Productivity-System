#include <iostream>
#include <fstream>
#include <string>

using namespace std;


const int MAX_USERS = 100;
const int MAX_TASKS = 200;
const int MAX_COURSES = 50;


// Structure for User credentials
struct User {
    string username;
    string password;
};

// Structure for Task
struct Task {
    string title;
    string subject;
    int priority;        // 1 = High, 2 = Medium, 3 = Low
    string deadline;     // Format: YYYY-MM-DD
    bool isCompleted;
};

// Structure for Course (GPA)
struct Course {
    string name;
    int creditHours;
    float grade;         
};

// Global Variables 
User users[MAX_USERS];
int userCount = 0;
string currentUser = "";
bool isLoggedIn = false;

Task userTasks[MAX_TASKS];
int userTaskCount = 0;

Course userCourses[MAX_COURSES];
int userCourseCount = 0;

//Auth

void loadUsersFromFile() {
    ifstream file("users.txt");
    
    userCount = 0;
    
    for (int i = 0; i < MAX_USERS; i++) {
        users[i].username = "";
        users[i].password = "";
    }
    
    if (!file.is_open()) {
        return;
    }
    
    string line;
    while (getline(file, line) && userCount < MAX_USERS) {
        int commaPos = line.find(',');
        if (commaPos != -1) {
            users[userCount].username = line.substr(0, commaPos);
            users[userCount].password = line.substr(commaPos + 1);
            userCount++;
        }
    }
    
    file.close();
}

void saveUsersToFile() {
    ofstream file("users.txt");
    
    if (!file.is_open()) {
        cout << "Error: Cannot save users to file!" << endl;
        return;
    }
    
    for (int i = 0; i < userCount; i++) {
        file << users[i].username << "," << users[i].password << endl;
    }
    
    file.close();
}


bool userExists(string username) {
    for (int i = 0; i < userCount; i++) {
        if (users[i].username == username) {
            return true;
        }
    }
    return false;
}

bool isValidUsername(string username) {
    if (username.length() < 3) {
        return false;
    }
    
    for (int i = 0; i < username.length(); i++) {
        char c = username[i];
        if (!(c >= 'a' && c <= 'z') && !(c >= 'A' && c <= 'Z') && !(c >= '0' && c <= '9')) {
            return false;
        }
    }
    return true;
}

bool isValidPassword(string password) {
    return password.length() >= 4;
}

bool signup(string username, string password) {
    if (!isValidUsername(username)) {
        cout << "\n[ERROR] Username must be at least 3 characters and contain only letters/numbers!" << endl;
        return false;
    }
    
    if (!isValidPassword(password)) {
        cout << "\n[ERROR] Password must be at least 4 characters!" << endl;
        return false;
    }
    
    if (userExists(username)) {
        cout << "\n[ERROR] Username already exists! Please choose a different username." << endl;
        return false;
    }
    
    if (userCount >= MAX_USERS) {
        cout << "\n[ERROR] Maximum user limit reached!" << endl;
        return false;
    }
    
    users[userCount].username = username;
    users[userCount].password = password;
    userCount++;
    
    saveUsersToFile();
    
    cout << "\n========================================" << endl;
    cout << "     ACCOUNT CREATED SUCCESSFULLY!" << endl;
    cout << "========================================" << endl;
    cout << "Welcome " << username << "! You can now login." << endl;
    return true;
}

bool login(string username, string password) {
    for (int i = 0; i < userCount; i++) {
        if (users[i].username == username && users[i].password == password) {
            currentUser = username;
            isLoggedIn = true;
            cout << "\n========================================" << endl;
            cout << "     LOGIN SUCCESSFUL!" << endl;
            cout << "========================================" << endl;
            cout << "Welcome back, " << username << "!" << endl;
            return true;
        }
    }
    
    cout << "\n[ERROR] Invalid username or password!" << endl;
    return false;
}

void logout() {
    currentUser = "";
    isLoggedIn = false;
    cout << "\n[INFO] You have been logged out successfully!" << endl;
}

string getCurrentUser() {
    return currentUser;
}

// File Handling Function

string getUserTaskFilename(string username) {
    return "tasks_" + username + ".txt";
}

string getUserGPAFilename(string username) {
    return "gpa_" + username + ".txt";
}

void saveTasksToFile() {
    string filename = getUserTaskFilename(currentUser);
    ofstream file(filename.c_str());
    
    if (!file.is_open()) {
        cout << "Error: Cannot save tasks for user: " << currentUser << endl;
        return;
    }
    
    file << userTaskCount << endl;
    
    for (int i = 0; i < userTaskCount; i++) {
        file << userTasks[i].title << endl;
        file << userTasks[i].subject << endl;
        file << userTasks[i].priority << endl;
        file << userTasks[i].deadline << endl;
        file << userTasks[i].isCompleted << endl;
    }
    
    file.close();
}

void loadTasksFromFile() {
    string filename = getUserTaskFilename(currentUser);
    ifstream file(filename.c_str());
    
    if (!file.is_open()) {
        userTaskCount = 0;
        return;
    }
    
    file >> userTaskCount;
    file.ignore();
    
    for (int i = 0; i < userTaskCount && i < MAX_TASKS; i++) {
        getline(file, userTasks[i].title);
        getline(file, userTasks[i].subject);
        file >> userTasks[i].priority;
        file.ignore();
        getline(file, userTasks[i].deadline);
        file >> userTasks[i].isCompleted;
        file.ignore();
    }
    
    file.close();
}

void saveCoursesToFile() {
    string filename = getUserGPAFilename(currentUser);
    ofstream file(filename.c_str());
    
    if (!file.is_open()) {
        cout << "Error: Cannot save GPA data for user: " << currentUser << endl;
        return;
    }
    
    file << userCourseCount << endl;
    
    for (int i = 0; i < userCourseCount; i++) {
        file << userCourses[i].name << endl;
        file << userCourses[i].creditHours << endl;
        file << userCourses[i].grade << endl;
    }
    
    file.close();
}

void loadCoursesFromFile() {
    string filename = getUserGPAFilename(currentUser);
    ifstream file(filename.c_str());
    
    if (!file.is_open()) {
        userCourseCount = 0;
        return;
    }
    
    file >> userCourseCount;
    file.ignore();
    
    for (int i = 0; i < userCourseCount && i < MAX_COURSES; i++) {
        getline(file, userCourses[i].name);
        file >> userCourses[i].creditHours;
        file.ignore();
        file >> userCourses[i].grade;
        file.ignore();
    }
    
    file.close();
}

//Task Management

void addTask() {
    cout << "\n--- ADD NEW TASK ---" << endl;
    if(userTaskCount >= MAX_TASKS) {
        cout << "Task limit reached!" << endl;
        return;
    }
    
    cout << "Task Title: ";
    getline(cin, userTasks[userTaskCount].title);
    cout << "Subject: ";
    getline(cin, userTasks[userTaskCount].subject);
    cout << "Priority (1=High, 2=Medium, 3=Low): ";
    cin >> userTasks[userTaskCount].priority;
    cin.ignore();
    cout << "Deadline (YYYY-MM-DD): ";
    getline(cin, userTasks[userTaskCount].deadline);
    userTasks[userTaskCount].isCompleted = false;
    userTaskCount++;
    
    cout << "\n[SUCCESS] Task added successfully!" << endl;
    saveTasksToFile();
}

void viewTasks() {
    cout << "\n--- ALL TASKS ---" << endl;
    if(userTaskCount == 0) {
        cout << "No tasks found. Add some tasks first!" << endl;
        return;
    }
    
    for(int i = 0; i < userTaskCount; i++) {
        cout << "\nTask #" << i+1 << endl;
        cout << "  Title: " << userTasks[i].title << endl;
        cout << "  Subject: " << userTasks[i].subject << endl;
        cout << "  Priority: ";
        if(userTasks[i].priority == 1) cout << "High";
        else if(userTasks[i].priority == 2) cout << "Medium";
        else cout << "Low";
        cout << endl;
        cout << "  Deadline: " << userTasks[i].deadline << endl;
        cout << "  Status: " << (userTasks[i].isCompleted ? "Completed" : "Pending") << endl;
    }
}

void markTaskCompleted() {
    int taskNum;
    viewTasks();
    if(userTaskCount > 0) {
        cout << "Enter task number to mark as completed: ";
        cin >> taskNum;
        if(taskNum >= 1 && taskNum <= userTaskCount) {
            userTasks[taskNum-1].isCompleted = true;
            cout << "Task marked as completed!" << endl;
            saveTasksToFile();
        } else {
            cout << "Invalid task number!" << endl;
        }
    }
}

void deleteTask() {
    int taskNum;
    viewTasks();
    if(userTaskCount > 0) {
        cout << "Enter task number to delete: ";
        cin >> taskNum;
        if(taskNum >= 1 && taskNum <= userTaskCount) {
            for(int i = taskNum-1; i < userTaskCount-1; i++) {
                userTasks[i] = userTasks[i+1];
            }
            userTaskCount--;
            cout << "Task deleted successfully!" << endl;
            saveTasksToFile();
        } else {
            cout << "Invalid task number!" << endl;
        }
    }
}

void displayTaskMenu() {
    int choice;
    do {
        cout << "\n========== TASK MANAGER ==========" << endl;
        cout << "1. Add Task" << endl;
        cout << "2. View All Tasks" << endl;
        cout << "3. Mark Task as Completed" << endl;
        cout << "4. Delete Task" << endl;
        cout << "5. Back to Main Menu" << endl;
        cout << "==================================" << endl;
        cout << "Choice: ";
        cin >> choice;
        cin.ignore();
        
        switch(choice) {
            case 1:
                addTask();
                break;
            case 2:
                viewTasks();
                break;
            case 3:
                markTaskCompleted();
                break;
            case 4:
                deleteTask();
                break;
            case 5:
                cout << "Returning to main menu..." << endl;
                break;
            default:
                cout << "Invalid choice!" << endl;
        }
    } while(choice != 5);
}

// GPA CALCULATOR FUNCTIONS 

void addCourse() {
    cout << "\n--- ADD NEW COURSE ---" << endl;
    if(userCourseCount >= MAX_COURSES) {
        cout << "Course limit reached!" << endl;
        return;
    }
    
    cout << "Course Name: ";
    getline(cin, userCourses[userCourseCount].name);
    cout << "Credit Hours: ";
    cin >> userCourses[userCourseCount].creditHours;
    
    char gradeLetter;
    cout << "Grade (A, B, C, D, F): ";
    cin >> gradeLetter;
    
    switch(gradeLetter) {
        case 'A':
        case 'a':
            userCourses[userCourseCount].grade = 4.0;
            break;
        case 'B':
        case 'b':
            userCourses[userCourseCount].grade = 3.0;
            break;
        case 'C':
        case 'c':
            userCourses[userCourseCount].grade = 2.0;
            break;
        case 'D':
        case 'd':
            userCourses[userCourseCount].grade = 1.0;
            break;
        case 'F':
        case 'f':
            userCourses[userCourseCount].grade = 0.0;
            break;
        default:
            cout << "Invalid grade! Setting to F." << endl;
            userCourses[userCourseCount].grade = 0.0;
    }
    
    cin.ignore();
    userCourseCount++;
    
    cout << "\n[SUCCESS] Course added successfully!" << endl;
    saveCoursesToFile();
}

float calculateGPA() {
    float totalPoints = 0;
    int totalCredits = 0;
    
    for(int i = 0; i < userCourseCount; i++) {
        totalPoints = totalPoints + (userCourses[i].grade * userCourses[i].creditHours);
        totalCredits = totalCredits + userCourses[i].creditHours;
    }
    
    if(totalCredits == 0) return 0.0;
    return totalPoints / totalCredits;
}

void displayGPA() {
    cout << "\n--- ALL COURSES ---" << endl;
    if(userCourseCount == 0) {
        cout << "No courses added yet!" << endl;
        return;
    }
    
    for(int i = 0; i < userCourseCount; i++) {
        cout << "\nCourse #" << i+1 << endl;
        cout << "  Name: " << userCourses[i].name << endl;
        cout << "  Credit Hours: " << userCourses[i].creditHours << endl;
        cout << "  Grade Points: " << userCourses[i].grade << endl;
    }
    
    cout << "\n=====================================" << endl;
    cout << "Semester GPA: " << calculateGPA() << endl;
    cout << "=====================================" << endl;
}

void displayGPAMenu() {
    int choice;
    do {
        cout << "\n========== GPA CALCULATOR ==========" << endl;
        cout << "1. Add Course" << endl;
        cout << "2. View All Courses" << endl;
        cout << "3. Calculate Semester GPA" << endl;
        cout << "4. Back to Main Menu" << endl;
        cout << "=====================================" << endl;
        cout << "Choice: ";
        cin >> choice;
        cin.ignore();
        
        switch(choice) {
            case 1:
                addCourse();
                break;
            case 2:
                displayGPA();
                break;
            case 3:
                cout << "\nYour Semester GPA: " << calculateGPA() << endl;
                break;
            case 4:
                cout << "Returning to main menu..." << endl;
                break;
            default:
                cout << "Invalid choice!" << endl;
        }
    } while(choice != 4);
}


void studyTimer() {
    int minutes;
    cout << "\n--- STUDY TIMER (Pomodoro Style) ---" << endl;
    cout << "Enter study duration in minutes (recommended: 25): ";
    cin >> minutes;
    
    cout << "\n[INFO] Timer started for " << minutes << " minutes!" << endl;
    cout << "[INFO] Focus on your studies." << endl;
    cout << "[INFO] Press Enter when you finish your study session..." << endl;
    
    cout << "\nPress Enter when time is up or to check remaining time..." << endl;
    
    time_t startTime = time(NULL);
    time_t currentTime;
    int elapsedMinutes = 0;
    
    cin.ignore();
    
    while (elapsedMinutes < minutes) {
        cout << "\rTime elapsed: " << elapsedMinutes << " / " << minutes << " minutes. Press Enter to continue..." << flush;
        
        cin.clear();
        
        currentTime = time(NULL);
        elapsedMinutes = (currentTime - startTime) / 60;
        
        if (elapsedMinutes >= minutes) {
            break;
        }
        
        for(int i = 0; i < 1000000; i++) {
        }
    }
    
    cout << "\n\n========================================" << endl;
    cout << "   TIME'S UP! Great job staying focused!" << endl;
    cout << "   Take a 5-minute break now." << endl;
    cout << "========================================" << endl;
}


void displayAuthMenu() {
    int choice;
    string username, password;
    
    do {
        cout << "\n========================================" << endl;
        cout << "   STUDENT PRODUCTIVITY TRACKER" << endl;
        cout << "========================================" << endl;
        cout << "   1. Login" << endl;
        cout << "   2. Sign Up" << endl;
        cout << "   3. Exit" << endl;
        cout << "========================================" << endl;
        cout << "   Enter your choice: ";
        cin >> choice;
        cin.ignore();
        
        switch(choice) {
            case 1:
                cout << "\n--- LOGIN ---" << endl;
                cout << "Username: ";
                getline(cin, username);
                cout << "Password: ";
                getline(cin, password);
                login(username, password);
                break;
                
            case 2:
                cout << "\n--- SIGN UP ---" << endl;
                cout << "Username (min 3 chars, letters/numbers only): ";
                getline(cin, username);
                cout << "Password (min 4 chars): ";
                getline(cin, password);
                signup(username, password);
                break;
                
            case 3:
                cout << "\n========================================" << endl;
                cout << "   Thank you for using Student Productivity Tracker!" << endl;
                cout << "========================================" << endl;
                exit(0);
                break;
                
            default:
                cout << "\n[ERROR] Invalid choice! Please enter 1, 2, or 3." << endl;
        }
    } while (!isLoggedIn);
}

void displayMainMenu() {
    cout << "\n========================================" << endl;
    cout << "   MAIN MENU" << endl;
    cout << "   Logged in as: " << currentUser << endl;
    cout << "========================================" << endl;
    cout << "   1. Task Manager" << endl;
    cout << "   2. GPA Calculator" << endl;
    cout << "   3. Study Timer" << endl;
    cout << "   4. Logout" << endl;
    cout << "   5. Exit Program" << endl;
    cout << "========================================" << endl;
    cout << "   Enter your choice: ";
}

void loadAllUserData() {
    loadTasksFromFile();
    loadCoursesFromFile();
    cout << "\n[INFO] Data loaded successfully for: " << currentUser << endl;
}

void saveAllUserData() {
    saveTasksToFile();
    saveCoursesToFile();
    cout << "\n[INFO] All data saved successfully for: " << currentUser << endl;
}


int main() {
    // Load all registered users from file
    loadUsersFromFile();
    
    // Display welcome message
    cout << "\n========================================" << endl;
    cout << "   STUDENT PRODUCTIVITY TRACKER v1.0" << endl;
    cout << "========================================" << endl;
    cout << "   A complete solution for students" << endl;
    cout << "   - Task Management" << endl;
    cout << "   - GPA Calculator" << endl;
    cout << "   - Study Timer" << endl;
    cout << "========================================" << endl;
    
    // Show authentication menu (Login/Signup)
    displayAuthMenu();
    
    // After successful login, load user's data
    loadAllUserData();
    
    int choice;
    bool running = true;
    
    // Main application loop
    while (running && isLoggedIn) {
        displayMainMenu();
        cin >> choice;
        cin.ignore();
        
        switch(choice) {
            case 1:
                displayTaskMenu();
                break;
                
            case 2:
                displayGPAMenu();
                break;
                
            case 3:
                studyTimer();
                break;
                
            case 4:
                saveAllUserData();
                logout();
                displayAuthMenu();
                if(isLoggedIn) {
                    loadAllUserData();
                }
                break;
                
            case 5:
                cout << "\n[INFO] Saving your data before exit..." << endl;
                saveAllUserData();
                cout << "\n========================================" << endl;
                cout << "   Thank you for using Student Productivity Tracker!" << endl;
                cout << "   Goodbye, " << currentUser << "!" << endl;
                cout << "========================================" << endl;
                running = false;
                break;
                
            default:
                cout << "\n[ERROR] Invalid choice! Please enter 1-5." << endl;
        }
    }
    
    return 0;
}