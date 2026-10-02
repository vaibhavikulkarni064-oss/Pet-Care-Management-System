# 🐾 Pet Care Management System

### C++ OOP Microproject

A menu-driven **Pet Care Management System** developed using **C++ and Object-Oriented Programming (OOP)** concepts. The system manages pet owners, pets, veterinarians, appointments, medical records, and billing through separate User and Administrator interfaces.

---

## 📌 Introduction

The **Pet Care Management System** is a C++ based application designed to organize and manage important information related to pets and their owners. It provides a simple menu-driven system through which users can register pets, view pet information, receive basic care advice, book veterinary appointments, access medical records, and view bills.

The system also provides an **Administrator Interface** for managing veterinarians, owners, pets, appointments, medical records, bills, and system statistics.

The main purpose of this project is to demonstrate the practical implementation of **Object-Oriented Programming concepts in C++** while developing a useful management application.

---

## 🎯 Objectives

* To maintain pet owner information.
* To register and manage dogs and cats.
* To provide basic pet care advice.
* To manage veterinarians and appointments.
* To maintain pet medical records.
* To generate and display bills.
* To store records using file handling.
* To demonstrate important C++ OOP concepts.
* To provide separate User and Administrator interfaces.

---

## ✨ Features

### 👤 User Interface

1. Register Owner & Pet
2. Select / Change Owner
3. View My Pets & Care Advice
4. Book Veterinary Appointment
5. View My Appointments
6. View Pet Medical Records
7. View My Bills
8. Back / Exit

### 👨‍💼 Administrator Interface

1. Add Doctor
2. View Doctors
3. View Owners
4. View Pets
5. View Appointments
6. Add Medical Records
7. View Medical Records
8. Generate Bills
9. View All Bills
10. View Statistics & Revenue
11. Select / Change Owner
12. Back / Exit

---

## 🧑‍💻 OOP Concepts Used

This project demonstrates the following Object-Oriented Programming concepts:

### 1. Classes and Objects

Classes such as `Owner`, `Pet`, `Dog`, `Cat`, `Veterinarian`, `Appointment`, `MedicalRecord`, and `Bill` are used to represent real-world entities.

### 2. Inheritance

`Dog` and `Cat` inherit common properties and functions from the `Pet` base class.

### 3. Polymorphism

Virtual functions are used in the `Pet` class, and `Dog` and `Cat` provide their own implementations of functions such as `display()`, `careAdvice()`, and `type()`.

### 4. Function Overloading

The `setPetAge()` function is overloaded to accept either years or years and months.

### 5. Abstraction

The `Pet` class contains pure virtual functions and acts as an abstract base class.

### 6. Static Data Member

The `SystemCounter` class uses a static data member to count records added during the current program session.

### 7. Friend Function

The `showBillAmount()` function is declared as a friend of the `Bill` class to access the bill information.

### 8. File Handling

Binary files are used to store and retrieve project records.

---

## 💾 File Handling

The project stores information using binary data files:

```text
OWNER.DAT
DOG.DAT
CAT.DAT
DOCTOR.DAT
APPOINT.DAT
MEDICAL.DAT
BILL.DAT
```

These files are used for storing owner, pet, doctor, appointment, medical, and billing records.

---

## 🗂️ Project Structure

```text
Pet-Care-Management-System
│
├── P_T.CPP
│
├── README.md
│
├── OWNER.DAT
├── DOG.DAT
├── CAT.DAT
├── DOCTOR.DAT
├── APPOINT.DAT
├── MEDICAL.DAT
└── BILL.DAT
```

> The `.DAT` files are created/used by the program when records are stored.

---

## 🔐 Administrator Login

The current program contains administrator login credentials:

```text
Username : admin
Password : 1234
```

These credentials are defined directly in the source code for this academic microproject.

---

## ⚙️ Technologies Used

* **Programming Language:** C++
* **Programming Paradigm:** Object-Oriented Programming
* **Compiler/Environment:** Turbo C++
* **Data Storage:** Binary File Handling
* **Interface:** Console / Menu Driven

---

## ▶️ How to Run

1. Open the project in a compatible **Turbo C++** environment.
2. Open the source file:

```text
P_T.CPP
```

3. Compile the program.
4. Run the program.
5. Select either:

```text
1. User Interface
2. Administrator Interface
0. Exit
```

6. Follow the menu options displayed on the screen.

---

## 📊 Main Modules

| Module                 | Purpose                                |
| ---------------------- | -------------------------------------- |
| Owner Management       | Register and select pet owners         |
| Pet Management         | Register and view dogs and cats        |
| Doctor Management      | Add and view veterinarians             |
| Appointment Management | Book and view appointments             |
| Medical Records        | Store and view pet medical information |
| Billing                | Generate and view pet-care bills       |
| Statistics             | Display records and total revenue      |

---

## 📚 Academic Purpose

This project was developed as a **microproject for the Object-Oriented Programming (OOP) subject**. It demonstrates how OOP concepts can be applied to develop a practical management system.

---

## 👩‍💻 Project Information

**Project Name:** Pet Care Management System

**Subject:** Object-Oriented Programming (OOP)

**Language:** C++

**Project Type:** Academic Microproject

---

## 🔗 Source Code

The complete source code is available in this GitHub repository.

**GitHub Repository:**

https://github.com/vaibhavikulkarni064-oss/Pet-Care-Management-System

---

## 📄 License

This project is created for educational and academic purposes.
