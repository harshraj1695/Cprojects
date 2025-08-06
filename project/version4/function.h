
/*
    Structures:
    - Person:
        Holds information about an individual including:
            - id       : Unique identifier
            - name     : Full name (supports spaces)
            - age      : Integer age
            - salary   : Integer salary
            - email    : Email address string

    - Node:
        A node in a singly linked list that stores a Person structure
        and a pointer to the next node.

    Functions:
    - add_person(filename, p, head):
        Adds a new person `p` to both the linked list and appends the data to the given file.

    - read_from_file(filename):
        Reads and displays all person records directly from the file.

    - read_list(head):
        Traverses and prints all person data from the linked list.

    - load_list(filename):
        Loads all data from the file into a linked list and returns the head node.

    - idsize(head):
        Returns the total number of nodes (people) in the linked list.

    - search(head, idd):
        Searches the linked list for a person with a given ID and returns a pointer to the Person (or NULL if not
   found).

    - deletedata(head, filename, id_to_delete):
        Deletes a person with the given ID from both the linked list and the file. Returns the updated head of the list.
*/

#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * @struct Person
 * @brief Holds personal information.
 *
 * @var Person::id
 *      Unique ID of the person.
 * @var Person::name
 *      Full name of the person.
 * @var Person::age
 *      Age of the person.
 * @var Person::salary
 *      Salary of the person.
 * @var Person::email
 *      Email address.
 */

typedef struct {
    int id;
    char name[50];
    int age;
    int salary;
    char email[100];
} Person;

/**
 * @struct Node
 * @brief Singly linked list node for storing a person.
 *
 * @var Node::p
 *      Person data.
 * @var Node::next
 *      Pointer to the next node in the list.
 */
typedef struct Node {
    Person p;
    struct Node* next;
} Node;

/**
 * @brief Adds a person to both the file and the linked list.
 *
 * @param filename File to which the person will be appended.
 * @param p The person to add.
 * @param head Current head of the linked list.
 * @return Updated head of the linked list.
 */
Node* add_person(const char* filename, Person p, Node* head);

/**
 * @brief Reads and displays all person records from the file.
 *
 * @param filename Name of the file to read.
 */
void read_from_file(const char* filename);

/**
 * @brief Displays all persons in the linked list.
 *
 * @param head Pointer to the head of the list.
 */
void read_list(Node* head);

/**
 * @brief Loads all person records from a file into a linked list.
 *
 * @param filename File containing the records.
 * @return Head of the newly created linked list.
 */
Node* load_list(const char* filename);

/**
 * @brief Counts the number of nodes (persons) in the linked list.
 *
 * @param head Head of the list.
 * @return Number of nodes in the list.
 */
int idsize(Node* head);

/**
 * @brief Searches for a person by ID in the linked list.
 *
 * @param head Head of the list.
 * @param idd ID of the person to search.
 * @return Pointer to the found Person, or NULL if not found.
 */
Person* search(Node* head, int idd);

/**
 * @brief Deletes a person from both the list and the file based on ID.
 *
 * @param head Head of the linked list.
 * @param filename File storing the records.
 * @param id_to_delete ID of the person to delete.
 * @return Updated head of the linked list.
 */
Node* deletedata(Node* head, const char* filename, int id_to_delete);
