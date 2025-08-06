#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//#define MAX_LINE 256

typedef struct {
    char name[50];
    int age;
    char gender[10];
    char email[100];
    char phone[15];
} Person;


// struct for linked list
typedef struct Node {
    char name[50];
    int age;
    char gender[10];
    char email[100];
    char phone[15];
    struct Node *next;
} Node;
// Helper to remove newline from fgets
void remove_newline(char *str) {
    str[strcspn(str, "\n")] = '\0';
}

Node* write_to_file(const char *filename, Person p,Node* head) {
    FILE *fp = fopen(filename, "a");
    if (fp == NULL) {
        perror("Unable to open file");
        exit(1);
    }
    
    fprintf(fp, "%s:%d:%s:%s:%s\n", p.name, p.age, p.gender, p.email, p.phone);
    Node *new_node = malloc(sizeof(Node));
        if (!new_node) {
            perror("Memory allocation failed");
        }
        strcpy(new_node->name, p.name);
        new_node->age = p.age;
        strcpy(new_node->gender, p.gender);
        strcpy(new_node->email, p.email);
        strcpy(new_node->phone, p.phone);
        new_node->next = NULL;
       if (!head) {
          head = new_node;
       } else {
       Node *tail = head;
       while (tail->next != NULL) {
        tail = tail->next;
       }
    tail->next = new_node;
}

    fclose(fp);
    return head;
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



//makig list form gettig file
Node* make_list_new(const char *filename, Node *head) {
    FILE *fp = fopen(filename, "r");
    if (fp == NULL) {
        perror("Unable to open file");
        return NULL;
    }
    char *line = NULL;
    size_t len = 0;
    while (getline(&line, &len, fp) != -1) {
        line[strcspn(line, "\n")] = '\0'; 
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
        Node *new_node = malloc(sizeof(Node));
        if (!new_node) {
            perror("Memory allocation failed");
            continue;
        }
        strcpy(new_node->name, p1.name);
        new_node->age = p1.age;
        strcpy(new_node->gender, p1.gender);
        strcpy(new_node->email, p1.email);
        strcpy(new_node->phone, p1.phone);
        new_node->next = NULL;
       if (!head) {
          head = new_node;
       } else {
       Node *tail = head;
       while (tail->next != NULL) {
        tail = tail->next;
       }
        tail->next = new_node;
       }
    }

    free(line);
    fclose(fp);
    return head;
}


void read_list(Node* head){
    if(head==NULL){
 printf("List is empty");
 return;
}else{
    while(head!=NULL){
        printf("Name: %s, Age: %d, Gender: %s, Email: %s, Phone: %s\n",
              head->name, head->age, head->gender, head->email, head->phone);
              head=head->next;
    }
}
    
}
int main() {
    Person p;
    int choice;
    char filename[] = "people.txt";
    char input[100];  
    Node* head=NULL;
    head=make_list_new(filename,head);
    do {
        printf("\n1. Add Person\n2. View All Records\n3. Read data from list\n4. Exit\nEnter choice: ");
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

            head=write_to_file(filename, p,head);
            printf("Person saved!\n");
        } else if (choice == 2) {
            printf("\nAll Records:\n");
            read_from_file(filename);
        }else if(choice==3){
            read_list(head);
        }
        else if(choice<0 ||choice >4){
            printf("\n \n Enter valid choice from below options\n");
        }

    } while (choice != 4);

    return 0;
}