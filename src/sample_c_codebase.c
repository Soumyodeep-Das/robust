#include "sample_c_codebase.h"

User database[MAX_USERS];
int userCount = 0;

bool validate_email(const char* email) {
    if (!email) return false;
    const char* at = strchr(email, '@');
    if (!at || at == email) return false; // must have one '@' and not at start
    if (strchr(at + 1, '@')) return false; // only one '@'
    const char* dot = strchr(at + 1, '.');
    if (!dot) return false; // must have at least one '.' after '@'
    if (dot == at + 1) return false; // no '.' immediately after '@'
    if (*(dot + 1) == '\0') return false; // must be something after the dot
    if (strstr(email, "..")) return false; // no consecutive dots
    // local part (before '@') and domain part (after '@') must not be empty
    if (at - email < 1) return false;
    if (strlen(at + 1) < 3) return false; // minimum 'a.b'
    return true;
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