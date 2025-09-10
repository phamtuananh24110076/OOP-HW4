#include <iostream>
#include <vector>
#include <string>
using namespace std;

// ---------------- Prescription Class ----------------
class Prescription {
private:
    vector<pair<string, string>> medicines; // <medicine, dosage>
    string notes;

public:
    void addMedicine(const string& med, const string& dosage) {
        medicines.push_back({med, dosage});
    }

    void setNotes(const string& n) { notes = n; }

    void displayPrescription() const {
        cout << "Prescription:\n";
        for (auto& m : medicines) {
            cout << " - " << m.first << " : " << m.second << endl;
        }
        if (!notes.empty()) cout << "Notes: " << notes << endl;
    }
};

// ---------------- Bill Class ----------------
class Bill {
private:
    vector<pair<string, double>> services; // <service, cost>
    bool paid;
    bool insuranceCovered;

public:
    Bill() : paid(false), insuranceCovered(false) {
        // Default consultation fee
        services.push_back({"Consultation Fee", 50.0});
    }

    void addService(const string& service, double cost) {
        services.push_back({service, cost});
    }

    double calculateTotal() const {
        double total = 0;
        for (auto& s : services) total += s.second;
        return total;
    }

    void applyInsurance() {
        insuranceCovered = true;
    }

    void markPaid() {
        paid = true;
    }

    void displayBill() const {
        cout << "----- Bill -----\n";
        for (auto& s : services) {
            cout << s.first << ": $" << s.second << endl;
        }
        double total = calculateTotal();
        cout << "Total: $" << total << endl;

        if (insuranceCovered) {
            cout << "Payment Status: Paid (Covered by Insurance)\n";
        } else if (total == 0) {
            cout << "Payment Status: Paid (No Charge)\n";
        } else {
            cout << "Payment Status: " << (paid ? "Paid" : "Unpaid") << endl;
        }
    }
};

// ---------------- Appointment Class ----------------
class Appointment {
private:
    string date;
    string time;
    string reason;
    string status; // scheduled, completed, canceled
    int patientID;
    int doctorID;
    Prescription prescription;
    Bill bill;

public:
    Appointment(string d, string t, string r, int pid, int did)
        : date(d), time(t), reason(r), patientID(pid), doctorID(did), status("Scheduled") {}

    void updateStatus(string newStatus) {
        status = newStatus;
    }

    void addPrescription(const Prescription& p) {
        prescription = p;
    }

    Prescription& getPrescription() { return prescription; }
    Bill& getBill() { return bill; }

    void displayInfo() const {
        cout << "Appointment on " << date << " at " << time
             << " | Reason: " << reason
             << " | Status: " << status
             << " | PatientID: " << patientID
             << " | DoctorID: " << doctorID << endl;
        prescription.displayPrescription();
        bill.displayBill();
    }

    int getPatientID() const { return patientID; }
    int getDoctorID() const { return doctorID; }
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

    virtual void scheduleAppointment(vector<Appointment>& appointments,
                                     string date, string time, string reason, int doctorID) {
        appointments.push_back(Appointment(date, time, reason, id, doctorID));
        cout << "Appointment scheduled for patient " << name << endl;
    }

    void updateHistory(string record) {
        medicalHistory.push_back(record);
    }

    virtual void displayInfo() const {
        cout << "Patient: " << name << " (ID: " << id << ", Age: " << age << ")\nHistory: ";
        for (auto& h : medicalHistory) cout << h << "; ";
        cout << endl;
    }

    int getID() const { return id; }
};

// ---------------- ChronicPatient Class ----------------
class ChronicPatient : public Patient {
private:
    string conditionType;
    string lastCheckupDate;

public:
    ChronicPatient(string n, int i, int a, string cond, string checkup)
        : Patient(n, i, a), conditionType(cond), lastCheckupDate(checkup) {}

    void scheduleAppointment(vector<Appointment>& appointments,
                             string date, string time, string reason, int doctorID) override {
        cout << "Chronic patient " << name << " requires frequent checkups.\n";
        Appointment app(date, time, reason + " (Chronic Care)", id, doctorID);
        app.getBill().addService("Chronic Care Fee", 20.0); // extra fee
        appointments.push_back(app);
    }

    void displayInfo() const override {
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

    void displayInfo() const {
        cout << "Doctor: " << name << " (ID: " << id << ", Specialty: " << specialty << ")" << endl;
    }

    int getID() const { return id; }
};

// ---------------- Clinic System Class ----------------
class ClinicSystem {
private:
    vector<Patient*> patients;
    vector<Doctor> doctors;
    vector<Appointment> appointments;

public:
    void addPatient(Patient* p) { patients.push_back(p); }
    void addDoctor(Doctor d) { doctors.push_back(d); }

    void createAppointment(int pid, int did, string date, string time, string reason) {
        for (auto* p : patients) {
            if (p->getID() == pid) {
                p->scheduleAppointment(appointments, date, time, reason, did);
                return;
            }
        }
        cout << "Patient not found.\n";
    }

    vector<Appointment>& getAppointments() { return appointments; }

    void cancelAppointment(int index) {
        if (index >= 0 && index < (int)appointments.size()) {
            appointments[index].updateStatus("Canceled");
        }
    }

    void showAllAppointments() const {
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

    // Add prescription + bill for Jane Doe's appointment
    Prescription pres;
    pres.addMedicine("Metformin", "500mg twice a day");
    pres.addMedicine("Insulin", "10 units daily");
    pres.setNotes("Check blood sugar regularly");

    clinic.getAppointments()[1].addPrescription(pres);
    clinic.getAppointments()[1].getBill().applyInsurance(); // covered by insurance

    // Show all appointments
    clinic.showAllAppointments();

    // Clean up
    delete p1;
    delete p2;

    return 0;
}
