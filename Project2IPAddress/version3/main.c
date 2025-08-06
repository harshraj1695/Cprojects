#include <stdio.h>
#include "function.h"
int main(int argc, char** argv)
{
    char ip[50];
    printf("Enter the string for Processessing ...(format should follow 127.22.3.4 structure)\n");
    scanf(" %[^\n]", ip);
    printf("IP you hav entered is: %s\n", ip);
    int choice = 0;
    do {
        printf("\nEnter 1 to check the validation of ip address\nEnter 2: to check the class of ip address \nEnter 3: to Exit\n");
        scanf("%d", &choice);
        switch(choice) {

        case 1:
            if(validate(ip)) {
                printf("IP %s is valid\n", ip);

            } else {
                printf("IP %s is not valid\n", ip);
            }
            break;

        case 2:
            if(find_class(ip)=='X') {
                printf("IP %s is not valid\n", ip);
        
            } else {
                printf("IP address %s belongs to Class %c\n", ip, find_class(ip));
            }
            break;
        case 3:
            printf("Exiting...\n");
            break;
        default:
            printf("enter the valid choice \n");
        }
    } while(choice != 3);
    return 0;
}
