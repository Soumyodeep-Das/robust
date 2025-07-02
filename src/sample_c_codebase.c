#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include <time.h>

#define MAX_USERS 100
#define MAX_NAME_LEN 50
#define MAX_EMAIL_LEN 100

// -------------------------- Data Structures -----------------------------
typedef struct {
    int day, month, year;
} Date;

typedef struct {
    int id;
    char name[MAX_NAME_LEN];
    char email[MAX_EMAIL_LEN];
    Date dob;
    bool isActive;
    float balance;
} User;

User database[MAX_USERS];
int userCount = 0;

// -------------------------- Utility Functions -----------------------------
bool validate_email(const char* email) {
    const char* at = strchr(email, '@');
    const char* dot = strrchr(email, '.');
    return at && dot && at < dot;
}

bool is_leap_year(int year) {
    return (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
}

bool validate_date(Date d) {
    if (d.year < 1900 || d.year > 2100 || d.month < 1 || d.month > 12 || d.day < 1)
        return false;
    int days_in_month[] = {0,31,28,31,30,31,30,31,31,30,31,30,31};
    if (is_leap_year(d.year)) days_in_month[2] = 29;
    return d.day <= days_in_month[d.month];
}

// -------------------------- Core Logic -----------------------------
int add_user(const char* name, const char* email, Date dob, float balance) {
    if (userCount >= MAX_USERS) return -1;
    if (!validate_email(email) || !validate_date(dob)) return -2;

    User u;
    u.id = userCount + 1;
    strncpy(u.name, name, MAX_NAME_LEN);
    strncpy(u.email, email, MAX_EMAIL_LEN);
    u.dob = dob;
    u.isActive = true;
    u.balance = balance;

    database[userCount++] = u;
    return u.id;
}

User* get_user_by_id(int id) {
    for (int i = 0; i < userCount; ++i) {
        if (database[i].id == id) return &database[i];
    }
    return NULL;
}

bool deactivate_user(int id) {
    User* u = get_user_by_id(id);
    if (!u || !u->isActive) return false;
    u->isActive = false;
    return true;
}

float deposit(int id, float amount) {
    User* u = get_user_by_id(id);
    if (!u || amount < 0) return -1;
    u->balance += amount;
    return u->balance;
}

float withdraw(int id, float amount) {
    User* u = get_user_by_id(id);
    if (!u || amount < 0 || u->balance < amount) return -1;
    u->balance -= amount;
    return u->balance;
}

// -------------------------- Debug / Display -----------------------------
void print_user(const User* u) {
    if (!u) return;
    printf("ID: %d\nName: %s\nEmail: %s\nDOB: %02d-%02d-%04d\nActive: %s\nBalance: $%.2f\n\n",
           u->id, u->name, u->email, u->dob.day, u->dob.month, u->dob.year,
           u->isActive ? "Yes" : "No", u->balance);
}

void list_all_users() {
    printf("--- USER DATABASE ---\n");
    for (int i = 0; i < userCount; ++i) {
        print_user(&database[i]);
    }
}

// -------------------------- Main (for demo) -----------------------------
#ifdef DEMO
int main() {
    Date d1 = {12, 10, 1990};
    Date d2 = {30, 2, 2000};

    printf("Add User 1: %d\n", add_user("Alice", "alice@example.com", d1, 100.0));
    printf("Add User 2 (invalid): %d\n", add_user("Bob", "bob_at_email", d2, 50.0));

    deposit(1, 150.0);
    withdraw(1, 50.0);
    deactivate_user(1);

    list_all_users();
    return 0;
}
#endif
