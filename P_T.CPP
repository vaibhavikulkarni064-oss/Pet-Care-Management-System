// PET CARE MANAGEMENT SYSTEM - Turbo C++ (User + Administrator)
#include <iostream.h>
#include <conio.h>
#include <stdio.h>
#include <string.h>
#include <fstream.h>

#define OWNF "OWNER.DAT"
#define DOGF "DOG.DAT"
#define CATF "CAT.DAT"
#define DOCF "DOCTOR.DAT"
#define APPF "APPOINT.DAT"
#define MEDF "MEDICAL.DAT"
#define BILF "BILL.DAT"

// ---------- helpers ----------
void rd(char *s) { fflush(stdin); gets(s); }

void head(char *t)
{
    clrscr();
    cout << "\n==========================================";
    cout << "\n  " << t;
    cout << "\n==========================================\n";
}

void getPassword(char *p)
{
    char c; int i = 0;
    while((c = getch()) != 13)
    {
	if(c == 8) { if(i) { i--; cout << "\b \b"; } }
	else if(i < 29) { p[i++] = c; cout << '*'; }
    }
    p[i] = 0;
}

// ---------- generic file functions ----------
int count(char *f, int sz)          // number of records in file
{
    ifstream i(f, ios::binary);
    if(!i) return 0;
    i.seekg(0, ios::end);
    return (int)(i.tellg() / sz);
}

void save(char *f, void *o, int sz)
{
    ofstream out(f, ios::binary | ios::app);
    out.write((char*)o, sz);
}

void getRec(char *f, void *o, int sz, int n)
{
    ifstream in(f, ios::binary);
    in.seekg((long)n * sz);
    in.read((char*)o, sz);
}

// ---------- static class ----------
class SystemCounter
{
    static int total;
public:
    static void add() { total++; }
    static void show() { cout << "\nRecords added in this session : " << total; }
};
int SystemCounter::total = 0;

// ---------- Owner ----------
class Owner
{
    int id;
    char name[40], phone[15], address[60];
public:
    Owner() { id = 0; name[0] = phone[0] = address[0] = 0; }
    int input(int i, char *ph)
    {
	id = i; strcpy(phone, ph);
	cout << "Owner ID (auto) : " << id;
	cout << "
Owner Name      : "; rd(name);
	if(strcmp(name, "0") == 0) return 0;
	cout << "Address         : "; gets(address);
	if(strcmp(address, "0") == 0) return 0;
	return 1;
    }
    void display()
    {
	cout << "\nOwner ID : " << id << "   Name : " << name;
	cout << "\nPhone : " << phone << "   Address : " << address << "\n";
    }
    int getId() { return id; }
    char *getName() { return name; }
    char *getPhone() { return phone; }
    int getOwner() { return id; }
};

Owner currentOwner;
int currentOwnerSet = 0;

int findOwner(char *ph, Owner &o);

int selectOwner()
{
    char ph[15];
    Owner o;
    head("SELECT OWNER");
    cout << "\nEnter Owner Phone (0 = Back) : ";
    rd(ph);
    if(strcmp(ph, "0") == 0) return 0;

    if(!findOwner(ph, o))
    {
\tcout << "\nOwner not found. Please register the owner first.";
\tgetch();
\treturn 0;
    }

    currentOwner = o;
    currentOwnerSet = 1;
    cout << "\nCurrent owner : " << currentOwner.getName();
    getch();
    return 1;
}

int ensureOwner()
{
    if(currentOwnerSet) return 1;
    return selectOwner();
}

int findOwner(char *ph, Owner &o)
{
    int n = count(OWNF, sizeof(Owner)), i;
    for(i = 0; i < n; i++)
    {
	getRec(OWNF, &o, sizeof(Owner), i);
	if(strcmp(o.getPhone(), ph) == 0) return 1;
    }
    return 0;
}

// ---------- Pet (abstract) , Dog , Cat ----------
class Pet
{
protected:
    int petId, ownerId, age, months;
    char petName[30], gender[10], breed[30];
public:
    Pet() { petId = ownerId = age = months = 0; petName[0] = gender[0] = breed[0] = 0; }
    void setPetAge(int y) { age = y; months = 0; }                // overloaded
    void setPetAge(int y, int m) { age = y; months = m; }
    virtual int input(int pid, int oid)
    {
	int y, m;
	petId = pid; ownerId = oid;
	cout << "
Pet ID (auto) : " << petId;
	cout << "
Pet Name (0 = Back) : "; rd(petName);
	if(strcmp(petName, "0") == 0) return 0;
	cout << "Age (years months) : "; cin >> y >> m;
	if(m == 0) setPetAge(y); else setPetAge(y, m);
	cout << "Gender : "; rd(gender);
	if(strcmp(gender, "0") == 0) return 0;
	cout << "Breed  : "; gets(breed);
	if(strcmp(breed, "0") == 0) return 0;
	return 1;
    }
    virtual void display()
    {
	cout << "\nPet ID : " << petId << "  Owner ID : " << ownerId;
	cout << "\nName : " << petName << "  Age : " << age << "y " << months << "m";
	cout << "\nGender : " << gender << "  Breed : " << breed;
    }
    virtual void careAdvice() = 0;
    virtual char *type() = 0;
    int getId() { return petId; }
    int getOwner() { return ownerId; }
    char *getName() { return petName; }
};

class Dog : public Pet
{
    char kind[30];
public:
    int input(int p, int o) { if(!Pet::input(p, o)) return 0; cout << "Dog Type (0 = Back) : "; gets(kind); if(strcmp(kind, "0") == 0) return 0; return 1; }
    void display() { Pet::display(); cout << "  Type : " << kind << " (DOG)\n"; }
    void careAdvice() { cout << "Care: daily exercise, vaccination, regular grooming.\n"; }
    char *type() { return "Dog"; }
};

class Cat : public Pet
{
    char kind[30];
public:
    int input(int p, int o) { if(!Pet::input(p, o)) return 0; cout << "Cat Type (0 = Back) : "; gets(kind); if(strcmp(kind, "0") == 0) return 0; return 1; }
    void display() { Pet::display(); cout << "  Type : " << kind << " (CAT)\n"; }
    void careAdvice() { cout << "Care: nutritious food, clean litter, vaccination.\n"; }
    char *type() { return "Cat"; }
};

Dog gd; Cat gc;   // work objects for lookup

Pet *loadPet(int id)
{
    int n, i;
    n = count(DOGF, sizeof(Dog));
    for(i = 0; i < n; i++)
    { getRec(DOGF, &gd, sizeof(Dog), i); if(gd.getId() == id) return &gd; }
    n = count(CATF, sizeof(Cat));
    for(i = 0; i < n; i++)
    { getRec(CATF, &gc, sizeof(Cat), i); if(gc.getId() == id) return &gc; }
    return 0;
}

int totalPets() { return count(DOGF, sizeof(Dog)) + count(CATF, sizeof(Cat)); }

// ask owner phone, show his pets, ask pet name -> returns pet (owner id comes automatically)
Pet *pickPet(Owner &o)
{
    char nm[30];
    int n = totalPets(), i, found = 0;
    Pet *p;

    if(!ensureOwner()) return 0;
    o = currentOwner;

    cout << "\nCurrent Owner : " << o.getName();
    cout << "\nPhone         : " << o.getPhone();
    cout << "\n\nYour pets:";

    for(i = 1; i <= n; i++)
    {
        p = loadPet(i);
        if(p && p->getOwner() == o.getId())
        {
            cout << "\n  " << p->getId() << ". " << p->getName()
                 << " (" << p->type() << ")";
            found = 1;
        }
    }

    if(!found)
    {
        cout << "\nNo pets found for this owner.";
        return 0;
    }

    cout << "\n\nSelect Pet Name (0 = Back) : ";
    rd(nm);
    if(strcmp(nm, "0") == 0) return 0;

    for(i = 1; i <= n; i++)
    {
        p = loadPet(i);
        if(p && p->getOwner() == o.getId() && strcmp(p->getName(), nm) == 0)
            return p;
    }

    cout << "\nPet not found!";
    return 0;
}

// ---------- Veterinarian ----------
class Veterinarian
{
    int id;
    char name[40], spec[40], phone[15];
public:
    int input(int i)
    {
	id = i;
	cout << "Doctor ID (auto) : " << id;
	cout << "
Name (0 = Back) : "; rd(name);
	if(strcmp(name, "0") == 0) return 0;
	cout << "Specialization : "; gets(spec);
	if(strcmp(spec, "0") == 0) return 0;
	cout << "Phone : "; gets(phone);
	if(strcmp(phone, "0") == 0) return 0;
	return 1;
    }
    void display() { cout << "\nDoctor ID : " << id << "  Dr. " << name << "\nSpecialization : " << spec << "  Phone : " << phone << "\n"; }
    char *getName() { return name; }
    int getOwner() { return id; }
};

// ---------- Appointment ----------
class Appointment
{
    int id, ownerId, petId, doctorId;
    char date[15], tm[10], reason[50];
public:
    int input(int i, int o, int p, int d)
    {
	id = i; ownerId = o; petId = p; doctorId = d;
	cout << "
Appointment ID (auto) : " << id;
	cout << "
Owner ID (auto)       : " << ownerId;
	cout << "
Pet ID (auto)         : " << petId;
	cout << "
Doctor ID (auto)      : " << doctorId;
	cout << "
Date (dd/mm/yyyy) (0 = Back) : "; rd(date);
	if(strcmp(date, "0") == 0) return 0;
	cout << "Time : "; gets(tm);
	if(strcmp(tm, "0") == 0) return 0;
	cout << "Reason : "; gets(reason);
	if(strcmp(reason, "0") == 0) return 0;
	return 1;
    }
    void display()
    {
	cout << "\nAppointment " << id << "  Owner " << ownerId << "  Pet " << petId << "  Doctor " << doctorId;
	cout << "\nDate : " << date << "  Time : " << tm << "  Reason : " << reason << "\n";
    }
    int getOwner() { return ownerId; }
};

// ---------- Medical record ----------
class MedicalRecord
{
    int id, ownerId, petId;
    char vacc[40], treat[60], date[15];
public:
    int input(int i, int o, int p)
    {
	id = i; ownerId = o; petId = p;
	cout << "
Record ID (auto) : " << id << "  Owner ID : " << o << "  Pet ID : " << p;
	cout << "
Vaccination (0 = Back) : "; rd(vacc);
	if(strcmp(vacc, "0") == 0) return 0;
	cout << "Treatment : "; gets(treat);
	if(strcmp(treat, "0") == 0) return 0;
	cout << "Date : "; gets(date);
	if(strcmp(date, "0") == 0) return 0;
	return 1;
    }
    void display()
    {
	cout << "\nRecord " << id << "  Owner " << ownerId << "  Pet " << petId << "  Date : " << date;
	cout << "\nVaccination : " << vacc << "\nTreatment : " << treat << "\n";
    }
    int getOwner() { return ownerId; }
};

// ---------- Bill ----------
class Bill
{
    int billNo, ownerId, petId;
    char ownerName[40], petName[30], petType[5];
    float consult, medicine, service;
public:
    void generate(int b, int oid, char *on, int pid, char *pn, char *pt)
    {
	billNo = b; ownerId = oid; petId = pid;
	strcpy(ownerName, on); strcpy(petName, pn); strcpy(petType, pt);
	if(strcmp(pt, "Dog") == 0) { consult = 500; medicine = 300; service = 200; }
	else { consult = 400; medicine = 250; service = 150; }
    }
    float total() { return consult + medicine + service; }
    void display()
    {
	cout << "\n------------- BILL No. " << billNo << " -------------";
	cout << "\nOwner : " << ownerName << " (ID " << ownerId << ")";
	cout << "\nPet   : " << petName << " (" << petType << ", ID " << petId << ")";
	cout << "\nConsultation Rs." << consult << "  Medicine Rs." << medicine << "  Service Rs." << service;
	cout << "\nTOTAL : Rs." << total() << "\n";
    }
    int getOwner() { return ownerId; }
    friend void showBillAmount(Bill b);
};

void showBillAmount(Bill b) { cout << "Amount payable (friend fn) = Rs. " << b.total() << "\n"; }

// show records of file (oid=0 -> all records, else only that owner's)
#define SHOW(F,T,oid) { T x; int n=count(F,sizeof(T)),i,f=0; \
    for(i=0;i<n;i++){ getRec(F,&x,sizeof(T),i); \
    if(!(oid)||x.getOwner()==(oid)){x.display();f++;} } \
    if(!f) cout<<"\nNo records found.\n"; }

// ======================= USER INTERFACE =======================
void registerPet()
{
    Owner o; Dog d; Cat c;
    char ph[15]; int ch, pid, ok;

    head("REGISTER OWNER & PET");

    if(currentOwnerSet)
    {
        o = currentOwner;
        cout << "\nCurrent Owner : " << o.getName();
        cout << "\nPhone         : " << o.getPhone();
    }
    else
    {
        cout << "\nOwner Phone (0 = Back) : ";
        rd(ph);
        if(strcmp(ph, "0") == 0) return;

        if(findOwner(ph, o))
        {
            cout << "\nExisting owner found: " << o.getName()
                 << " (ID " << o.getId() << ")";
        }
        else
        {
            if(!o.input(count(OWNF, sizeof(Owner)) + 1, ph))
            {
                cout << "\nRegistration cancelled.";
                getch();
                return;
            }
            save(OWNF, &o, sizeof(Owner));
            SystemCounter::add();
            cout << "\nOwner registered successfully.";
        }

        currentOwner = o;
        currentOwnerSet = 1;
    }

    pid = totalPets() + 1;
    cout << "\n\n1. Dog  2. Cat  0. Back : ";
    cin >> ch;
    if(ch == 0) return;

    if(ch == 1)
    {
        ok = d.input(pid, o.getId());
        if(!ok) { cout << "\nPet registration cancelled."; getch(); return; }
        save(DOGF, &d, sizeof(Dog));
    }
    else if(ch == 2)
    {
        ok = c.input(pid, o.getId());
        if(!ok) { cout << "\nPet registration cancelled."; getch(); return; }
        save(CATF, &c, sizeof(Cat));
    }
    else
    {
        cout << "\nInvalid choice.";
        getch();
        return;
    }

    SystemCounter::add();
    cout << "\n\nSaved! Owner ID = " << o.getId()
         << ", Pet ID = " << pid;
    cout << "\nOwner information will NOT be asked again while this owner is selected.";
    getch();
}

void myPets()
{
    Owner o; Pet *p; int n = totalPets(), i, found = 0;
    head("MY PETS");

    if(!ensureOwner()) return;
    o = currentOwner;

    o.display();
    for(i = 1; i <= n; i++)
    {
        p = loadPet(i);
        if(p && p->getOwner() == o.getId())
        {
            p->display();
            p->careAdvice();
            found = 1;
        }
    }
    if(!found) cout << "\nNo pets found.";

    getch();
}

void bookAppointment()
{
    Owner o; Pet *p; Appointment a; Veterinarian v;
    int nd, id;
    head("BOOK APPOINTMENT");
    nd = count(DOCF, sizeof(Veterinarian));
    if(nd == 0) { cout << "\nNo doctor available. Contact administrator."; getch(); return; }
    p = pickPet(o);
    if(p)
    {
	id = count(APPF, sizeof(Appointment)) + 1;
	getRec(DOCF, &v, sizeof(Veterinarian), (id - 1) % nd);   // doctor auto-assigned
	if(!a.input(id, o.getId(), p->getId(), (id - 1) % nd + 1))
	{
	    cout << "
Appointment cancelled.";
	    getch();
	    return;
	}
	save(APPF, &a, sizeof(Appointment)); SystemCounter::add();
	cout << "

Booked with Dr. " << v.getName();
    }
    getch();
}

void myRecords(int what)
{
    Owner o; int oid;
    head(what == 1 ? "MY APPOINTMENTS" : what == 2 ? "MY PET MEDICAL RECORDS" : "MY BILLS");

    if(!ensureOwner()) return;
    o = currentOwner;
    oid = o.getId();

    cout << "\nOwner : " << o.getName();
    if(what == 1) SHOW(APPF, Appointment, oid)
    else if(what == 2) SHOW(MEDF, MedicalRecord, oid)
    else SHOW(BILF, Bill, oid)

    getch();
}

void userMenu()
{
    int ch;
    do {
        head("USER MENU");
        if(currentOwnerSet)
            cout << "\nCurrent Owner : " << currentOwner.getName()
                 << " (" << currentOwner.getPhone() << ")";

        cout << "\n\n1. Register Owner & Pet";
        cout << "\n2. Select / Change Owner";
        cout << "\n3. My Pets & Care Advice";
        cout << "\n4. Book Appointment";
        cout << "\n5. My Appointments";
        cout << "\n6. My Pet Medical Records";
        cout << "\n7. My Bills";
        cout << "\n0. Back";
        cout << "\n\nChoice : ";
        cin >> ch;

        switch(ch)
        {
            case 1: registerPet(); break;
            case 2: selectOwner(); break;
            case 3: myPets(); break;
            case 4: bookAppointment(); break;
            case 5: myRecords(1); break;
            case 6: myRecords(2); break;
            case 7: myRecords(3); break;
            case 0: break;
            default: cout << "\nInvalid choice!"; getch();
        }
    } while(ch != 0);
}

// ===================== ADMINISTRATOR INTERFACE =====================
int adminLogin()
{
    char u[30], p[30]; int i;
    for(i = 1; i <= 3; i++)
    {
	head("ADMIN LOGIN");
	cout << "\nUsername (0 = Back) : "; cin >> u;
	if(strcmp(u, "0") == 0) return 0;
	cout << "Password : "; getPassword(p);
	if(strcmp(u, "admin") == 0 && strcmp(p, "1234") == 0) return 1;
	cout << "\n\nWrong! Attempts left : " << 3 - i; getch();
    }
    return 0;
}

void addDoctor()
{
    Veterinarian v;
    head("ADD DOCTOR");
    if(!v.input(count(DOCF, sizeof(Veterinarian)) + 1))
    {
        cout << "\nOperation cancelled.";
        getch();
        return;
    }
    save(DOCF, &v, sizeof(Veterinarian)); SystemCounter::add();
    cout << "\nDoctor saved!"; getch();
}

void addMedical()
{
    Owner o; Pet *p; MedicalRecord m;
    head("ADD MEDICAL RECORD");
    p = pickPet(o);
    if(p)
    {
	if(!m.input(count(MEDF, sizeof(MedicalRecord)) + 1, o.getId(), p->getId()))
	{
	    cout << "
Medical record cancelled.";
	    getch();
	    return;
	}
	save(MEDF, &m, sizeof(MedicalRecord)); SystemCounter::add();
	cout << "
Record saved!";
    }
    getch();
}

void makeBill()
{
    Owner o; Pet *p; Bill b;
    head("GENERATE BILL");
    p = pickPet(o);
    if(p)
    {
	b.generate(count(BILF, sizeof(Bill)) + 1, o.getId(), o.getName(), p->getId(), p->getName(), p->type());
	b.display(); showBillAmount(b);
	save(BILF, &b, sizeof(Bill)); SystemCounter::add();
    }
    getch();
}

void stats()
{
    Bill b; float rev = 0; int n = count(BILF, sizeof(Bill)), i;
    head("STATISTICS");
    for(i = 0; i < n; i++) { getRec(BILF, &b, sizeof(Bill), i); rev += b.total(); }
    cout << "\nOwners       : " << count(OWNF, sizeof(Owner));
    cout << "\nDogs         : " << count(DOGF, sizeof(Dog));
    cout << "\nCats         : " << count(CATF, sizeof(Cat));
    cout << "\nDoctors      : " << count(DOCF, sizeof(Veterinarian));
    cout << "\nAppointments : " << count(APPF, sizeof(Appointment));
    cout << "\nMedical Recs : " << count(MEDF, sizeof(MedicalRecord));
    cout << "\nBills        : " << n << "   Total Revenue : Rs." << rev;
    SystemCounter::show();
    getch();
}

void adminMenu()
{
    int ch, z = 0;
    if(!adminLogin()) { cout << "\n\nAccess denied."; getch(); return; }

    do {
        head("ADMINISTRATOR MENU");

        if(currentOwnerSet)
            cout << "\nCurrent Owner : " << currentOwner.getName();

        cout << "\n\n1. Add Doctor";
        cout << "\n2. View Doctors";
        cout << "\n3. View Owners";
        cout << "\n4. View Pets";
        cout << "\n5. View Appointments";
        cout << "\n6. Add Medical Record";
        cout << "\n7. View Medical Records";
        cout << "\n8. Generate Bill";
        cout << "\n9. View All Bills";
        cout << "\n10. Statistics & Revenue";
        cout << "\n11. Select / Change Owner";
        cout << "\n0. Back";
        cout << "\n\nChoice : ";
        cin >> ch;

        switch(ch)
        {
            case 1: addDoctor(); break;
            case 2:
                head("VIEW DOCTORS");
                SHOW(DOCF, Veterinarian, z)
                getch();
                break;
            case 3:
                head("VIEW OWNERS");
                SHOW(OWNF, Owner, z)
                getch();
                break;
            case 4:
                head("VIEW PETS");
                SHOW(DOGF, Dog, z)
                SHOW(CATF, Cat, z)
                getch();
                break;
            case 5:
                head("VIEW APPOINTMENTS");
                SHOW(APPF, Appointment, z)
                getch();
                break;
            case 6: addMedical(); break;
            case 7:
                head("VIEW MEDICAL RECORDS");
                SHOW(MEDF, MedicalRecord, z)
                getch();
                break;
            case 8: makeBill(); break;
            case 9:
                head("VIEW ALL BILLS");
                SHOW(BILF, Bill, z)
                getch();
                break;
            case 10: stats(); break;
            case 11: selectOwner(); break;
            case 0: break;
            default: cout << "\nInvalid choice!"; getch();
        }
    } while(ch != 0);
}

void main()
{
    int ch;
    do {
	head("PET CARE MANAGEMENT SYSTEM");
	cout << "\n1. User Interface\n2. Administrator Interface\n0. Exit\n\nChoice : ";
	cin >> ch;
	if(ch == 1) userMenu();
	else if(ch == 2) adminMenu();
    } while(ch != 0);
}
