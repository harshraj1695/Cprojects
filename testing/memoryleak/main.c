#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Define a struct for Student
typedef struct {
    char *name;
    int age;
    int *marks;   // dynamic array of marks
} Student;

int main() {
    // Allocate memory for 3 students
    Student *students = (Student *)malloc(3 * sizeof(Student));
    if (!students) {
        perror("malloc failed");
        return 1;
    }

    // Initialize students
    for (int i = 0; i < 3; i++) {
        // Allocate memory for name
        students[i].name = (char *)malloc(20 * sizeof(char));
        snprintf(students[i].name, 20, "Student_%d", i + 1);

        students[i].age = 18 + i;

        // Allocate memory for marks (5 subjects each)
        students[i].marks = (int *)calloc(5, sizeof(int));
        for (int j = 0; j < 5; j++) {
            students[i].marks[j] = (i + 1) * (j + 10);
        }
    }

    // Print student info
    for (int i = 0; i < 3; i++) {
        printf("Name: %s, Age: %d, Marks:", students[i].name, students[i].age);
        for (int j = 0; j < 5; j++) {
            printf(" %d", students[i].marks[j]);
        }
        printf("\n");
    }

    // Free memory carefully
    for (int i = 0; i < 3; i++) {
        free(students[i].marks);

        // INTENTIONAL LEAK: we will NOT free students[1].name
        if (i != 1) {
            free(students[i].name);
        }
    }

    // Free array of students
    free(students);

    return 0;
}
