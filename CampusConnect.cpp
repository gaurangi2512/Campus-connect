#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>
#include <queue>
#include <iomanip>
#include <limits>
#include <cctype>
using namespace std;

struct Student {
    int id = 0;
    string name, course, email, role;
    int semester = 1, experience = 0;
    double proficiency = 0, availability = 0, interest = 0;
    vector<string> skills;
};

struct Role {
    string name;
    int membersNeeded = 1;
    vector<string> requiredSkills;
};

struct Project {
    int id = 0;
    string name, description;
    vector<Role> roles;
};

struct Recommendation {
    Student student;
    double skillMatch = 0, score = 0;
};

struct CompareRecommendation {
    bool operator()(const Recommendation& a, const Recommendation& b) const {
        if (a.score != b.score) return a.score < b.score;
        if (a.skillMatch != b.skillMatch) return a.skillMatch < b.skillMatch;
        return a.student.name > b.student.name;
    }
};


string lowerText(string value) {
    transform(value.begin(), value.end(), value.begin(),
        [](unsigned char ch) { return static_cast<char>(tolower(ch)); });
    return value;
}

bool sameText(const string& a, const string& b) {
    return lowerText(a) == lowerText(b);
}

int readInt(const string& prompt, int minimum, int maximum = numeric_limits<int>::max()) {
    int value;
    while (true) {
        cout << prompt;
        if (cin >> value && value >= minimum && value <= maximum) {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            return value;
        }
        cout << "Invalid input. Please enter a number from " << minimum
             << " to " << maximum << ".\n";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
}

double readPercent(const string& prompt) {
    double value;
    while (true) {
        cout << prompt;
        if (cin >> value && value >= 0 && value <= 100) {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            return value;
        }
        cout << "Enter a value between 0 and 100.\n";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
}

string readLine(const string& prompt) {
    string value;
    cout << prompt;
    getline(cin >> ws, value);
    return value;
}

bool containsText(const vector<string>& values, const string& target) {
    for (const string& value : values)
        if (sameText(value, target)) return true;
    return false;
}

int findStudentIndex(const vector<Student>& students, int id) {
    for (int i = 0; i < static_cast<int>(students.size()); ++i)
        if (students[i].id == id) return i;
    return -1;
}

int findProjectIndex(const vector<Project>& projects, int id) {
    for (int i = 0; i < static_cast<int>(projects.size()); ++i)
        if (projects[i].id == id) return i;
    return -1;
}

int nextStudentId(const vector<Student>& students) {
    int largest = 0;
    for (const Student& s : students) largest = max(largest, s.id);
    return largest + 1;
}

int nextProjectId(const vector<Project>& projects) {
    int largest = 0;
    for (const Project& p : projects) largest = max(largest, p.id);
    return largest + 1;
}

// ======================= STUDENT FILE HANDLING =======================
// std::quoted preserves spaces in names, courses, emails and skills.
void saveStudents(const vector<Student>& students) {
    ofstream out("students.txt");
    if (!out) {
        cout << "Warning: could not save students.txt\n";
        return;
    }
    for (const Student& s : students) {
        out << s.id << ' ' << quoted(s.name) << ' ' << quoted(s.course) << ' '
            << s.semester << ' ' << quoted(s.email) << ' ' << quoted(s.role) << ' '
            << s.experience << ' ' << s.proficiency << ' '
            << s.availability << ' ' << s.interest << ' ' << s.skills.size();
        for (const string& skill : s.skills) out << ' ' << quoted(skill);
        out << '\n';
    }
}

void loadStudents(vector<Student>& students) {
    ifstream in("students.txt");
    if (!in) return;
    Student s;
    while (in >> s.id >> quoted(s.name) >> quoted(s.course)
              >> s.semester >> quoted(s.email) >> quoted(s.role)
              >> s.experience >> s.proficiency
              >> s.availability >> s.interest) {
        size_t count;
        if (!(in >> count)) break;
        s.skills.clear();
        for (size_t i = 0; i < count; ++i) {
            string skill;
            if (!(in >> quoted(skill))) break;
            s.skills.push_back(skill);
        }
        students.push_back(s);
    }
}

// ======================= MODULE 1: STUDENT PROFILE MANAGEMENT =======================
void printStudent(const Student& s) {
    cout << "\nID: " << s.id << "\nName: " << s.name
         << "\nCourse: " << s.course << "\nSemester: " << s.semester
         << "\nEmail: " << s.email << "\nPreferred role: " << s.role
         << "\nExperience (years): " << s.experience
         << "\nProficiency: " << s.proficiency << "%"
         << "\nAvailability: " << s.availability << "%"
         << "\nInterest: " << s.interest << "%\nSkills: ";
    if (s.skills.empty()) cout << "None";
    for (size_t i = 0; i < s.skills.size(); ++i) {
        if (i) cout << ", ";
        cout << s.skills[i];
    }
    cout << "\n";
}

void addStudent(vector<Student>& students) {
    Student s;
    s.id = nextStudentId(students);
    cout << "\n--- Add Student Profile ---\n";
    s.name = readLine("Name: ");
    s.course = readLine("Course: ");
    s.semester = readInt("Semester: ", 1, 20);
    s.email = readLine("Email: ");
    s.role = readLine("Preferred role: ");
    s.experience = readInt("Experience in years: ", 0, 100);
    s.proficiency = readPercent("Skill proficiency (0-100): ");
    s.availability = readPercent("Availability (0-100): ");
    s.interest = readPercent("Interest in project/domain (0-100): ");
    int count = readInt("Number of skills: ", 0, 100);
    for (int i = 0; i < count; ++i)
        s.skills.push_back(readLine("Enter skill " + to_string(i + 1) + ": "));
    students.push_back(s);
    saveStudents(students);
    cout << "Student added. Assigned ID: " << s.id << "\n";
}

void displayStudents(const vector<Student>& students) {
    if (students.empty()) { cout << "No student records found.\n"; return; }
    for (const Student& s : students) {
        cout << "\n------------------------------";
        printStudent(s);
    }
}

void searchStudent(const vector<Student>& students) {
    int id = readInt("Enter student ID: ", 1);
    int index = findStudentIndex(students, id);
    if (index == -1) cout << "Student not found.\n";
    else printStudent(students[index]);
}

void updateStudent(vector<Student>& students) {
    int id = readInt("Enter student ID to update: ", 1);
    int index = findStudentIndex(students, id);
    if (index == -1) { cout << "Student not found.\n"; return; }
    Student& s = students[index];
    cout << "Enter updated details:\n";
    s.name = readLine("Name: ");
    s.course = readLine("Course: ");
    s.semester = readInt("Semester: ", 1, 20);
    s.email = readLine("Email: ");
    s.role = readLine("Preferred role: ");
    s.experience = readInt("Experience in years: ", 0, 100);
    s.proficiency = readPercent("Skill proficiency (0-100): ");
    s.availability = readPercent("Availability (0-100): ");
    s.interest = readPercent("Interest in project/domain (0-100): ");
    s.skills.clear();
    int count = readInt("Number of skills: ", 0, 100);
    for (int i = 0; i < count; ++i)
        s.skills.push_back(readLine("Enter skill " + to_string(i + 1) + ": "));
    saveStudents(students);
    cout << "Student profile updated.\n";
}

void deleteStudent(vector<Student>& students) {
    int id = readInt("Enter student ID to delete: ", 1);
    int index = findStudentIndex(students, id);
    if (index == -1) { cout << "Student not found.\n"; return; }
    students.erase(students.begin() + index);
    saveStudents(students);
    cout << "Student deleted.\n";
}

// ======================= MODULE 2: PROJECT & ROLE MANAGEMENT =======================
void createProject(vector<Project>& projects) {
    Project p;
    p.id = nextProjectId(projects);
    cout << "\n--- Create Project ---\n";
    p.name = readLine("Project name: ");
    p.description = readLine("Project description: ");
    int roleCount = readInt("Number of required roles: ", 1, 50);
    for (int i = 0; i < roleCount; ++i) {
        Role r;
        cout << "\nRole " << i + 1 << '\n';
        r.name = readLine("Role name: ");
        r.membersNeeded = readInt("Number of members needed for this role: ", 1, 100);
        int skillCount = readInt("Number of required skills: ", 0, 100);
        for (int j = 0; j < skillCount; ++j)
            r.requiredSkills.push_back(readLine("Required skill " + to_string(j + 1) + ": "));
        p.roles.push_back(r);
    }
    projects.push_back(p);
    cout << "Project created. Project ID: " << p.id
         << " (projects are stored for this session only).\n";
}

void displayProjects(const vector<Project>& projects) {
    if (projects.empty()) { cout << "No projects created yet.\n"; return; }
    for (const Project& p : projects) {
        cout << "\nProject ID: " << p.id << "\nName: " << p.name
             << "\nDescription: " << p.description << "\nRoles:\n";
        for (const Role& r : p.roles) {
            cout << "  " << r.name << " (members needed: " << r.membersNeeded << ")\n  Skills: ";
            for (size_t i = 0; i < r.requiredSkills.size(); ++i) {
                if (i) cout << ", ";
                cout << r.requiredSkills[i];
            }
            cout << '\n';
        }
    }
}

// ======================= MODULE 3: STUDENT SEARCHING =======================
void searchStudentsByRole(const vector<Student>& students) {
    string role = readLine("Enter role to search: ");
    bool found = false;
    for (const Student& s : students)
        if (sameText(s.role, role)) { printStudent(s); found = true; }
    if (!found) cout << "No students found for that role.\n";
}

void searchStudentsBySkill(const vector<Student>& students) {
    string skill = readLine("Enter skill to search: ");
    bool found = false;
    for (const Student& s : students)
        if (containsText(s.skills, skill)) { printStudent(s); found = true; }
    if (!found) cout << "No students found with that skill.\n";
}

// ======================= MODULES 4 & 5: SORTING AND PRIORITY QUEUE =======================
double calculateSkillMatch(const Student& s, const Role& role) {
    if (role.requiredSkills.empty()) return 0;
    int matched = 0;
    for (const string& required : role.requiredSkills)
        if (containsText(s.skills, required)) ++matched;
    return 100.0 * matched / role.requiredSkills.size();
}

double calculateRecommendationScore(const Student& s, double skillMatch) {
    return skillMatch * 0.40 + s.proficiency * 0.25
         + s.availability * 0.20 + s.interest * 0.15;
}

void printRecommendations(const vector<Recommendation>& results) {
    if (results.empty()) { cout << "No candidates available.\n"; return; }
    cout << "\n" << left << setw(6) << "Rank" << setw(22) << "Name"
         << setw(14) << "Role" << setw(14) << "Skill match" << "Recommendation\n";
    cout << string(72, '-') << '\n';
    int rank = 1;
    for (const Recommendation& r : results)
        cout << left << setw(6) << rank++ << setw(22) << r.student.name
             << setw(14) << r.student.role << setw(14) << fixed << setprecision(1)
             << r.skillMatch << fixed << setprecision(2) << r.score << "%\n";
}

bool selectProjectRole(const vector<Project>& projects, Role& selectedRole) {
    if (projects.empty()) { cout << "Create a project first.\n"; return false; }
    displayProjects(projects);
    int projectId = readInt("\nEnter project ID: ", 1);
    int pIndex = findProjectIndex(projects, projectId);
    if (pIndex == -1) { cout << "Project not found.\n"; return false; }
    const Project& p = projects[pIndex];
    int roleNumber = readInt("Select role number (1-" + to_string(p.roles.size()) + "): ",
                             1, static_cast<int>(p.roles.size()));
    selectedRole = p.roles[roleNumber - 1];
    return true;
}

void sortCandidatesForRole(const vector<Student>& students, const vector<Project>& projects) {
    Role role;
    if (!selectProjectRole(projects, role)) return;
    vector<Recommendation> results;
    for (const Student& s : students) {
        Recommendation r;
        r.student = s;
        r.skillMatch = calculateSkillMatch(s, role);
        r.score = calculateRecommendationScore(s, r.skillMatch);
        results.push_back(r);
    }
    sort(results.begin(), results.end(), [](const Recommendation& a, const Recommendation& b) {
        if (a.score != b.score) return a.score > b.score;
        if (a.skillMatch != b.skillMatch) return a.skillMatch > b.skillMatch;
        return a.student.name < b.student.name;
    });
    cout << "\nCandidates sorted by recommendation score for role: " << role.name << '\n';
    printRecommendations(results);
}

void priorityQueueRecommendations(const vector<Student>& students, const vector<Project>& projects) {
    Role role;
    if (!selectProjectRole(projects, role)) return;
    priority_queue<Recommendation, vector<Recommendation>, CompareRecommendation> pq;
    for (const Student& s : students) {
        Recommendation r;
        r.student = s;
        r.skillMatch = calculateSkillMatch(s, role);
        r.score = calculateRecommendationScore(s, r.skillMatch);
        pq.push(r);
    }
    cout << "\n===== PRIORITY QUEUE RECOMMENDATIONS: " << role.name << " =====\n";
    int rank = 1;
    while (!pq.empty()) {
        Recommendation top = pq.top();
        cout << "\nRank: " << rank++ << "\nName: " << top.student.name
             << "\nRole: " << top.student.role
             << "\nSkill match: " << fixed << setprecision(2) << top.skillMatch << "%"
             << "\nRecommendation score: " << top.score << "%\n";
        pq.pop();
    }
}

// ======================= MODULE 6: TEAM & SKILL GAP ANALYSIS =======================
void teamSkillGapAnalysis(const vector<Student>& students, const vector<Project>& projects) {
    Role role;
    if (!selectProjectRole(projects, role)) return;
    if (students.empty()) { cout << "Add students first.\n"; return; }

    cout << "\nAvailable students:\n";
    for (const Student& s : students) cout << "ID " << s.id << ": " << s.name << '\n';

    int teamSize = readInt("Enter team size: ", 1, static_cast<int>(students.size()));
    vector<int> teamIds;
    for (int i = 0; i < teamSize; ++i) {
        while (true) {
            int id = readInt("Enter student ID for team member " + to_string(i + 1) + ": ", 1);
            if (find(teamIds.begin(), teamIds.end(), id) != teamIds.end())
                cout << "That student is already selected. Try again.\n";
            else if (findStudentIndex(students, id) == -1)
                cout << "Student ID not found. Try again.\n";
            else { teamIds.push_back(id); break; }
        }
    }

    vector<string> teamSkills;
    cout << "\n===== FINAL TEAM MEMBERS =====\n";
    for (int id : teamIds) {
        const Student& s = students[findStudentIndex(students, id)];
        cout << s.name << " (ID " << s.id << ")\n";
        for (const string& skill : s.skills)
            if (!containsText(teamSkills, skill)) teamSkills.push_back(skill);
    }

    cout << "\n===== TEAM AVAILABLE SKILLS =====\n";
    if (teamSkills.empty()) cout << "No skills recorded.\n";
    for (const string& skill : teamSkills) cout << "- " << skill << '\n';

    vector<string> missingSkills;
    for (const string& required : role.requiredSkills)
        if (!containsText(teamSkills, required)) missingSkills.push_back(required);

    cout << "\n===== SKILL GAP ANALYSIS FOR " << role.name << " =====\n";
    if (missingSkills.empty()) cout << "No skill gap found. The team has all required skills.\n";
    else {
        cout << "Missing skills:\n";
        for (const string& skill : missingSkills) cout << "- " << skill << '\n';
    }
}

// ======================= MAIN MENU =======================
int main() {
    vector<Student> students;
    vector<Project> projects; // Projects remain in memory for this program run.
    loadStudents(students);

    int choice;
    do {
        cout << "\n========== CAMPUS CONNECT ==========\n"
             << "1.  Add student profile\n"
             << "2.  Display all students\n"
             << "3.  Search student by ID\n"
             << "4.  Update student profile\n"
             << "5.  Delete student profile\n"
             << "6.  Create project and required roles\n"
             << "7.  Display all projects\n"
             << "8.  Search students by role\n"
             << "9.  Search students by skill\n"
             << "10. Sort candidates for a project role\n"
             << "11. Priority queue recommendations\n"
             << "12. Team formation and skill gap analysis\n"
             << "0.  Save and exit\n";
        choice = readInt("Enter your choice: ", 0, 12);

        switch (choice) {
            case 1: addStudent(students); break;
            case 2: displayStudents(students); break;
            case 3: searchStudent(students); break;
            case 4: updateStudent(students); break;
            case 5: deleteStudent(students); break;
            case 6: createProject(projects); break;
            case 7: displayProjects(projects); break;
            case 8: searchStudentsByRole(students); break;
            case 9: searchStudentsBySkill(students); break;
            case 10: sortCandidatesForRole(students, projects); break;
            case 11: priorityQueueRecommendations(students, projects); break;
            case 12: teamSkillGapAnalysis(students, projects); break;
            case 0:
                saveStudents(students);
                cout << "Student records saved. Program closed.\n";
                break;
        }
    } while (choice != 0);
    return 0;
}