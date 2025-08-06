#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE 256

typedef struct {
    char name[50];
    int age;
    char gender[10];
    char email[100];
    char phone[15];
} Person;

// Helper to remove newline from fgets
void remove_newline(char *str) {
    str[strcspn(str, "\n")] = '\0';
}

void write_to_file(const char *filename, Person p) {
    FILE *fp = fopen(filename, "a");
    if (fp == NULL) {
        perror("Unable to open file");
        exit(1);
    }
    fprintf(fp, "%s:%d:%s:%s:%s\n", p.name, p.age, p.gender, p.email, p.phone);
    fclose(fp);
}

void read_from_file(const char *filename) {
    FILE *fp = fopen(filename, "r");
    if (fp == NULL) {
        perror("Unable to open file");
        return;
    }

    char line[256];

    while (fgets(line, sizeof(line), fp)) {
        line[strcspn(line, "\n")] = '\0';

        char name[50], gender[10], email[100], phone[15];
        int age;

        int i = 0, j = 0, field = 0;
        char temp[100];

        while (line[i] != '\0') {
            if (line[i] == ':') {
                temp[j] = '\0';
                if (field == 0) sscanf(temp, "%[^\n]", name);
                else if (field == 1) sscanf(temp, "%d", &age);
                else if (field == 2) sscanf(temp, "%[^\n]", gender);
                else if (field == 3) sscanf(temp, "%[^\n]", email);
                field++;
                j = 0;
            } else {
                temp[j++] = line[i];
            }
            i++;
        }
        temp[j] = '\0';
        sscanf(temp, "%[^\n]", phone);
        printf("Name: %s, Age: %d, Gender: %s, Email: %s, Phone: %s\n",
               name, age, gender, email, phone);
    }

    fclose(fp);
}


int main() {
    Person p;
    int choice;
    char filename[] = "people.txt";
    char input[100];  

    do {
        printf("\n1. Add Person\n2. View All Records\n3. Exit\nEnter choice: ");
        fgets(input, sizeof(input), stdin);
        sscanf(input, "%d", &choice);

        if (choice == 1) {
            printf("Enter name: ");
            fgets(p.name, sizeof(p.name), stdin);
            remove_newline(p.name);

            printf("Enter age: ");
            fgets(input, sizeof(input), stdin);
            sscanf(input, "%d", &p.age);

            printf("Enter gender: ");
            fgets(p.gender, sizeof(p.gender), stdin);
            remove_newline(p.gender);

            printf("Enter email: ");
            fgets(p.email, sizeof(p.email), stdin);
            remove_newline(p.email);

            printf("Enter phone: ");
            fgets(p.phone, sizeof(p.phone), stdin);
            remove_newline(p.phone);

            write_to_file(filename, p);
            printf("Person saved!\n");
        } else if (choice == 2) {
            printf("\nAll Records:\n");
            read_from_file(filename);
        }

    } while (choice != 3);

    return 0;
}