#include <stdbool.h>
#include <stdio.h>
#include <string.h>

// Check if input is hex format
bool is_hex(const char *str) {
    if (strncmp(str, "0x", 2) == 0 || strncmp(str, "0X", 2) == 0) str += 2;

    int len = strlen(str);
    if (len != 8) return false;

    for (int i = 0; i < len; i++) {
        char ch = str[i];
        if (!((ch >= '0' && ch <= '9') || (ch >= 'a' && ch <= 'f') || (ch >= 'A' && ch <= 'F'))) {
            return false;
        }
    }
    return true;
}

// Convert hex to IPv4 string C0A80001 192.168.0.1
void hex_to_ip_string(unsigned int hex_ip, char *output) {
    unsigned int a = (hex_ip >> 24) & 0xFF;
    unsigned int b = (hex_ip >> 16) & 0xFF;
    unsigned int c = (hex_ip >> 8) & 0xFF;
    unsigned int d = hex_ip & 0xFF;
    sprintf(output, "%u.%u.%u.%u", a, b, c, d);
}

// Validate dotted-decimal IP with no leading zeros
bool validate(const char* ip) {
    int a, b, c, d;
    char dummy;

    // Check for leading zeros manually
    int i = 0;
    while (ip[i] != '\0') {
        if (ip[i] == '.') {
            if (ip[i + 1] == '0' && ip[i + 2] >= '0' && ip[i + 2] <= '9') {
                return false;  // leading zero detected
            }
        }
        i++;
    }

    if (sscanf(ip, "%d.%d.%d.%d%c", &a, &b, &c, &d, &dummy) != 4)
        return false;

    if (a < 0 || a > 255 || b < 0 || b > 255 || c < 0 || c > 255 || d < 0 || d > 255)
        return false;

    return true;
}

// Determine class function 
char find_class(const char *ip) {
    int a, b, c, d;
    sscanf(ip, "%d.%d.%d.%d", &a, &b, &c, &d);
    if (a >= 0 && a <= 127)
        return 'A';
    else if (a >= 128 && a <= 191)
        return 'B';
    else if (a >= 192 && a <= 223)
        return 'C';
    else if (a >= 224 && a <= 239)
        return 'D';
    else if (a >= 240 && a <= 255)
        return 'E';
    else
        return 'X';
}

// conversion function of ip to hexadecimal no
void ip_to_hex(const char *ip) {
    int a, b, c, d;
    sscanf(ip, "%d.%d.%d.%d", &a, &b, &c, &d);
    unsigned int ip_hex = (a << 24) | (b << 16) | (c << 8) | d;
    printf("Hexadecimal Fromat for given Ip address %s is 0x%08X\n",ip, ip_hex);
}

int main() {
    char input[50];
    char ip[50];  // final IP to use
    printf("Enter IP (IPv4 format or 8-digit hex, e.g., 192.168.0.1 or 0xC0A80001):\n");
    scanf(" %[^\n]", input);

    // If input is hex, conversion is done into doted format
    if (is_hex(input)) {
        unsigned int hex_ip;
        sscanf(input, "%x", &hex_ip);
        hex_to_ip_string(hex_ip, ip);
        printf("Converted from HEX: %s -> IP: %s\n",input, ip);
    } else {
        strcpy(ip, input);
        ip_to_hex(ip);
    }

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