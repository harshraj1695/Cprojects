#include <stdio.h>
#include <string.h>
#include "function.h"
int main(int argc, char** argv)
{
    // choose between ipv4 and ipv6
    int option = 0;
label:
    printf("Enter 1 for ipv4 or 2 for ipv6 ip address ");
    scanf("%d", &option);

    if(option == 1) {
        char input[50];
        char ip[50]; // final IP to use
        printf("Enter IP (IPv4 format or 8-digit hex, e.g., 192.168.0.1 or 0xC0A80001):\n");
        scanf(" %[^\n]", input);

        // If input is hex, conversion is done into doted format
        if(is_hex(input)) {
            unsigned int hex_ip;
            sscanf(input, "%x", &hex_ip);
            hex_to_ip_string(hex_ip, ip);
            printf("Converted from HEX: %s -> IP: %s\n", input, ip);
        } else {
            strcpy(ip, input);
            ip_to_hex(ip);
        }

        int choice = 0;
        do {
            printf("\nEnter 1 to check the validation of ip address\nEnter 2: to check the class of ip address \nEnter "
                   "3: to Exit\n");
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
                if(find_class(ip) == 'X') {
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

        // for ipv6 operations
    } else if(option == 2) {
        char ipv6[50];
        printf("Enter IPV6 address \n");
        scanf(" %[^\n]", ipv6);
        if(is_valid_ipv6(ipv6)){
            printf("Entered IPv6 address %s is valid \n",ipv6);
        }else{
            printf("Entered IPv6 address %s is not valid \n",ipv6); 
        }

    } else {
        printf("Enter correct option \n");
        goto label;
    }

    return 0;
}
