#include <iostream>
#include <vector>
#include <string>
using namespace std;

// ---------------- Appointment Class ----------------
class Appointment {
private:
    string date;
    string time;
    string reason;
    string status; // scheduled, completed, canceled
    int patientID;
    int doctorID;

public:
    Appointment(string d, string t, string r, int pid, int did)
        : date(d), time(t), reason(r), patientID(pid), doctorID(did), status("Scheduled") {}

    void updateStatus(string newStatus) {
        status = newStatus;
    }

    void displayInfo() {
        cout << "Appointment on " << date << " at " << time
             << " | Reason: " << reason
             << " | Status: " << status
             << " | PatientID: " << patientID
             << " | DoctorID: " << doctorID << endl;
    }

    int getPatientID() { return patientID; }
    int getDoctorID() { return doctorID; }
};

// ---------------- Patient Class ----------------
class Patient {
protected:
    string name;
    int id;
    int age;
    vector<string> medicalHistory;

public:
    Patient(string n, int i, int a) : name(n), id(i), age(a) {}

    virtual void scheduleAppointment(vector<Appointment>& appointments, string date, string time, string reason, int doctorID) {
        appointments.push_back(Appointment(date, time, reason, id, doctorID));
        cout << "Appointment scheduled for patient " << name << endl;
    }

    void updateHistory(string record) {
        medicalHistory.push_back(record);
    }

    virtual void displayInfo() {
        cout << "Patient: " << name << " (ID: " << id << ", Age: " << age << ")\nHistory: ";
        for (auto& h : medicalHistory) cout << h << "; ";
        cout << endl;
    }

    int getID() { return id; }
};

// ---------------- ChronicPatient Class ----------------
class ChronicPatient : public Patient {
private:
    string conditionType;
    string lastCheckupDate;

public:
    ChronicPatient(string n, int i, int a, string cond, string checkup)
        : Patient(n, i, a), conditionType(cond), lastCheckupDate(checkup) {}

    void scheduleAppointment(vector<Appointment>& appointments, string date, string time, string reason, int doctorID) override {
        cout << "Chronic patient " << name << " requires frequent checkups.\n";
        appointments.push_back(Appointment(date, time, reason + " (Chronic Care)", id, doctorID));
    }

    void displayInfo() override {
        Patient::displayInfo();
        cout << "Condition: " << conditionType << " | Last Checkup: " << lastCheckupDate << endl;
    }
};

// ---------------- Doctor Class ----------------
class Doctor {
private:
    string name;
    int id;
    string specialty;

public:
    Doctor(string n, int i, string s) : name(n), id(i), specialty(s) {}

    void displayInfo() {
        cout << "Doctor: " << name << " (ID: " << id << ", Specialty: " << specialty << ")" << endl;
    }

    int getID() { return id; }
};

// ---------------- Clinic System Class ----------------
class ClinicSystem {
private:
    vector<Patient*> patients;
    vector<Doctor> doctors;
    vector<Appointment> appointments;

public:
    void addPatient(Patient* p) {
        patients.push_back(p);
    }

    void addDoctor(Doctor d) {
        doctors.push_back(d);
    }

    void createAppointment(int pid, int did, string date, string time, string reason) {
        for (auto* p : patients) {
            if (p->getID() == pid) {
                p->scheduleAppointment(appointments, date, time, reason, did);
                return;
            }
        }
        cout << "Patient not found.\n";
    }

    void cancelAppointment(int index) {
        if (index >= 0 && index < appointments.size()) {
            appointments[index].updateStatus("Canceled");
        }
    }

    void showAllAppointments() {
        for (auto& a : appointments) a.displayInfo();
    }
};

// ---------------- Main Function ----------------
int main() {
    ClinicSystem clinic;

    // Add doctors
    clinic.addDoctor(Doctor("Dr. Smith", 1, "Cardiology"));
    clinic.addDoctor(Doctor("Dr. Alice", 2, "General"));

    // Add patients
    Patient* p1 = new Patient("John Doe", 101, 30);
    Patient* p2 = new ChronicPatient("Jane Doe", 102, 45, "Diabetes", "2025-06-01");

    clinic.addPatient(p1);
    clinic.addPatient(p2);

    // Create appointments
    clinic.createAppointment(101, 1, "2025-09-10", "10:00", "Routine Checkup");
    clinic.createAppointment(102, 2, "2025-09-15", "14:00", "Diabetes Follow-up");

    // Show all appointments
    clinic.showAllAppointments();

    // Clean up
    delete p1;
    delete p2;

    return 0;
}
