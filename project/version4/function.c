/*

 * Functions:
 * - add_person: Adds a new person to both the file and linked list.
 * - read_from_file: Reads and displays all person records from the file.
 * - read_list: Displays all person records from the linked list.
 * - load_list: Loads all person records from the file into a linked list.
 * - idsize: Returns the number of nodes (people) in the list.
 * - search: Searches for a person by ID in the list and returns a pointer to the record.
 * - deletedata: Deletes a person record by ID from both the list and file.
 */

#define _GNU_SOURCE
#include "function.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

Node* add_person(const char* filename, Person p, Node* head)
{
    FILE* fp = fopen(filename, "a");
    if(!fp) {
        perror("File open failed");
        return head;
    }

    fprintf(fp, "%d:%s:%d:%d:%s\n", p.id, p.name, p.age, p.salary, p.email);
    fclose(fp);

    Node* new_node = malloc(sizeof(Node));
    new_node->p = p;
    new_node->next = NULL;

    if(!head)
        return new_node;

    Node* temp = head;
    while(temp->next)
        temp = temp->next;
    temp->next = new_node;

    return head;
}

// Read all from file
void read_from_file(const char* filename)
{
    FILE* fp = fopen(filename, "r");
    if(!fp) {
        perror("File open failed");
        return;
    }

    char* line = NULL;
    size_t len = 0;

    while(getline(&line, &len, fp) != -1) {
        line[strcspn(line, "\n")] = '\0'; // remove newline here it will fnd first occurence of \n and change it with \0
        Person p1;
        int i = 0, j = 0, field = 0;
        char temp[100] = { 0 };

        while(1) {
            if(line[i] == ':' || line[i] == '\0') {
                temp[j] = '\0';

                if(field == 0)
                    p1.id = atoi(temp);
                else if(field == 1)
                    strncpy(p1.name, temp, sizeof(p1.name) - 1);
                else if(field == 2)
                    p1.age = atoi(temp);
                else if(field == 3)
                    p1.salary = atoi(temp);
                else if(field == 4)
                    strncpy(p1.email, temp, sizeof(p1.email) - 1);

                field++;
                j = 0;
                if(line[i] == '\0')
                    break;
            } else {
                temp[j++] = line[i];
            }
            i++;
        }

        printf("ID: %d | Name: %s | Age: %d | Salary: %d | Email: %s\n", p1.id, p1.name, p1.age, p1.salary, p1.email);
    }

    free(line);
    fclose(fp);
}
// Read from linked list
void read_list(Node* head)
{
    if(!head) {
        printf("List is empty.\n");
        return;
    }

    while(head) {
        Person p = head->p;
        printf("ID: %d | Name: %s | Age: %d | Salary: %d | Email: %s\n", p.id, p.name, p.age, p.salary, p.email);
        head = head->next;
    }
}

// Load list from file
Node* load_list(const char* filename)
{
    FILE* fp = fopen(filename, "r");
    if(!fp)
        return NULL;

    char* line = NULL;
    size_t len = 0;
    Node *head = NULL, *tail = NULL;

    while(getline(&line, &len, fp) != -1) {
        Person p;
        int i = 0, j = 0, field = 0;
        char temp[100] = { 0 };
        line[strcspn(line, "\n")] = '\0';

        while(1) {
            if(line[i] == ':' || line[i] == '\0') {
                temp[j] = '\0';

                if(field == 0)
                    p.id = atoi(temp);
                else if(field == 1)
                    strncpy(p.name, temp, sizeof(p.name) - 1);
                else if(field == 2)
                    p.age = atoi(temp);
                else if(field == 3)
                    p.salary = atoi(temp);
                else if(field == 4)
                    strncpy(p.email, temp, sizeof(p.email) - 1);

                field++;
                j = 0;
                if(line[i] == '\0')
                    break;
            } else {
                temp[j++] = line[i];
            }
            i++;
        }

        Node* new_node = malloc(sizeof(Node));
        new_node->p = p;
        new_node->next = NULL;

        if(!head)
            head = tail = new_node;
        else {
            tail->next = new_node;
            tail = new_node;
        }
    }

    free(line);
    fclose(fp);
    return head;
}

int idsize(Node* head)
{
    if(head == NULL)
        return 0;
    int size = 0;
    while(head != NULL) {
        size++;
        head = head->next;
    }
    return size;
}
Person* search(Node* head, int idd)
{
    if(!head)
        return NULL;
    while(head) {
        if(head->p.id == idd)
            return &(head->p);
        head = head->next;
    }
    return NULL;
}

Node* deletedata(Node* head, const char* filename, int id_to_delete)
{
    // 1. Delete from linked list
    Node *curr = head, *prev = NULL;

    while(curr) {
        if(curr->p.id == id_to_delete) {
            if(prev == NULL) {
                head = curr->next;
                free(curr);
            } else {
                prev->next = curr->next;
                free(curr);
            }
            printf("ID %d deleted from list.\n", id_to_delete);
            break;
        }
        prev = curr;
        curr = curr->next;
    }

    // 2. Rewrite file with all remaining nodes
    FILE* fp = fopen("temp.txt", "w");
    if(!fp) {
        perror("Failed to open temp file");
        return head;
    }

    Node* temp = head;
    while(temp) {
        Person p = temp->p;
        fprintf(fp, "%d:%s:%d:%d:%s\n", p.id, p.name, p.age, p.salary, p.email);
        temp = temp->next;
    }
    fclose(fp);

    remove(filename);
    rename("temp.txt", filename);
    printf("ID %d deleted from file.\n", id_to_delete);
    return head;
}
