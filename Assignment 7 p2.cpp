#include <iostream>
#include <fstream>
#include <list>
#include <iomanip>
#include <sstream>
#include <limits>
using namespace std;

struct Patient {
    string id;
    string name;
    string severity;
    int waitTime;
    string status;
};

void loadPatients(list<Patient>& patients) {
    ifstream fin("patients.txt");
    if (!fin) return;
    string line;
    while (getline(fin, line)) {
        if (line.empty()) continue;
        stringstream ss(line);
        string id, name, severity, wait, status;
        getline(ss, id, ',');
        getline(ss, name, ',');
        getline(ss, severity, ',');
        getline(ss, wait, ',');
        getline(ss, status, ',');
        Patient p;
        p.id = id;
        p.name = name;
        p.severity = severity;
        p.waitTime = wait.empty() ? 0 : stoi(wait);
        p.status = status.empty() ? "Waiting" : status;
        patients.push_back(p);
    }
}

void savePatients(const list<Patient>& patients) {
    ofstream fout("patients.txt");
    for (const auto& p : patients) {
        fout << p.id << "," << p.name << "," << p.severity << ","
            << p.waitTime << "," << p.status << "\n";
    }
}

void registerPatient(list<Patient>& patients) {
    Patient p;
    cout << "Enter patient ID: ";
    cin >> p.id;
    cout << "Enter name: ";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    getline(cin, p.name);
    cout << "Enter condition severity: ";
    getline(cin, p.severity);
    cout << "Enter estimated wait time: ";
    cin >> p.waitTime;
    p.status = "Waiting";
    patients.push_back(p);
    cout << "Patient registered successfully!\n";
}

void displayQueue(const list<Patient>& patients) {
    cout << "=========== ER WAITING LIST ===========\n";
    cout << left << setw(8) << "ID"
        << setw(18) << "Name"
        << setw(10) << "Severity"
        << right << setw(10) << "Wait(min)"
        << setw(10) << "Status" << "\n";
    cout << "------------------------------------------------------\n";
    for (const auto& p : patients) {
        cout << left << setw(8) << p.id
            << setw(18) << p.name.substr(0, 17)
            << setw(10) << p.severity.substr(0, 9)
            << right << setw(10) << p.waitTime
            << setw(10) << p.status << "\n";
    }
}

void admitPatient(list<Patient>& patients) {
    for (auto it = patients.begin(); it != patients.end(); ++it) {
        cout << "Patient " << it->id << " has been admitted to the ER.\n";
        it->status = "Admitted";
        return;
    }
    cout << "No patients in queue.\n";
}

void viewSummary(const list<Patient>& patients) {
    int total = 0;
    int admitted = 0;
    int waiting = 0;
    long long waitSum = 0;
    int waitingCount = 0;
    for (const auto& p : patients) {
        total++;
        if (p.status == "Admitted") admitted++;
        else { waiting++; waitSum += p.waitTime; waitingCount++; }
    }
    double avg = waitingCount ? (double)waitSum / waitingCount : 0.0;
    cout << "========== ER SUMMARY ==========\n";
    cout << "Total Patients: " << total << "\n";
    cout << "Admitted: " << admitted << "\n";
    cout << "Waiting: " << waiting << "\n";
    cout << "Avg Wait Time (Waiting): " << fixed << showpoint << setprecision(2) << avg << " minutes\n";
    cout << "================================\n";
}

int main() {
    list<Patient> patients;
    loadPatients(patients);
    int choice;
    do {
        cout << "\n****** Welcome to Emergency Room Queue Manager ******\n";
        cout << "1. Register New Patient\n2. Display Patient Queue\n3. Admit Patient\n4. View ER Summary\n5. Exit\n ";
        cin >> choice;
        switch (choice) {
        case 1: registerPatient(patients); break;
        case 2: displayQueue(patients); break;
        case 3: admitPatient(patients); break;
        case 4: viewSummary(patients); break;
        case 5: savePatients(patients);
            cout << "Patient queue saved to patients.txt. Have a safe shift!\n";
            break;
        default: cout << "Invalid option. Try again.\n";
        }
    } while (choice != 5);
    return 0;
}
