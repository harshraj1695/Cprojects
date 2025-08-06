/*
    This program manages a list of people using a linked list and file storage.
    It supports adding, reading, searching, and deleting person records.
    I have declared all the required function in a different header file i.e function.h file
     and their implentation in function.c file
*/
#define _GNU_SOURCE
#include "function.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main()
{
    Person p;
    Node* head = NULL;
    int choice;
    char filename[] = "people.txt";

    head = load_list(filename);
    int siz = idsize(head);
    int deleteid = 0;
    int searchid = 0;
    do {
        printf("\n1. Add Person\n2. View All from File\n3. View All from List\n4. Search if an id exist or not\n"
               "5. Enter to delete the informaton\n6 Exit\nChoice: ");
        scanf("%d", &choice);
        switch(choice) {
        case 1:
            printf("Enter ID Which should be Greater than %d and in Sequence: ", siz);
            scanf("%d", &p.id);
            printf("Enter Name: ");
            scanf(" %[^\n]", p.name);
            printf("Enter Age: ");
            scanf("%d", &p.age);
            printf("Enter Salary: ");
            scanf("%d", &p.salary);
            printf("Enter Email: ");
            scanf("%s", p.email);

            head = add_person(filename, p, head);
            siz++;
            break;

        case 2:
            read_from_file(filename);
            break;

        case 3:
            read_list(head);
            break;

        case 4:
            printf("Enter the id of person to search: ");
            scanf("%d", &searchid);
            Person* result = search(head, searchid);
            if(result) {
                printf("Found: ID=%d, Name=%s, Age=%d, Salary=%d, Email=%s\n", result->id, result->name, result->age,
                       result->salary, result->email);
            } else {
                printf("Person with ID %d not found.\n", searchid);
            }
            break;
        case 5:
            printf("Enter the id whose imformation you want to delete: ");
            scanf("%d", &deleteid);
            if(search(head, deleteid) == NULL) {
                printf("Deletion is not possible as the id donot exit... ");
            } else {
                head = deletedata(head, filename, deleteid);
            }
        case 6:
            printf("Exiting...\n");
            break;

        default:
            printf("Invalid choice.\n");
        }
    } while(choice != 6);

    return 0;
}
