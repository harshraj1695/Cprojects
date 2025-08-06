#define _GNU_SOURCE
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

    char *line = NULL;
    size_t len = 0;

    while (getline(&line, &len, fp) != -1) {
        line[strcspn(line, "\n")] = '\0'; // remove newline here it will fnd first occurence of \n and change it with \0
        Person p1;
        int i = 0, j = 0, field = 0;
        char temp[100] = {0};

        while (1) {
            if (line[i] == ':' || line[i] == '\0') {
                temp[j] = '\0';  

                if (field == 0)
                    strncpy(p1.name, temp, sizeof(p1.name) - 1);
                else if (field == 1)
                    p1.age = atoi(temp);
                else if (field == 2)
                    strncpy(p1.gender, temp, sizeof(p1.gender) - 1);
                else if (field == 3)
                    strncpy(p1.email, temp, sizeof(p1.email) - 1);
                else if (field == 4)
                    strncpy(p1.phone, temp, sizeof(p1.phone) - 1);

                field++;
                j = 0;  
                if (line[i] == '\0') break; 
            } else {
                temp[j++] = line[i];
            }
            i++;
        }

        printf("Name: %s, Age: %d, Gender: %s, Email: %s, Phone: %s\n",
              p1.name, p1.age, p1.gender, p1.email, p1.phone);
    }

    free(line);
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
            remove_newline(p.name);  // fgets receive \n in buffer space after clicking new line

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
        }else if(choice<0 ||choice >3){
            printf("\n \n Enter valid choice from below options\n");
        }

    } while (choice != 3);

    return 0;
}