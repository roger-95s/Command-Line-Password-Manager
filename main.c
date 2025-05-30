#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// Function prototypes
void add_password();
void retrieve_password();
void delete_password();
void encrypt_password(char *password, char *encrypted_password);
void decrypt_password(char *encrypted_password, char *decrypted_password);
void save_to_file(char *service_name, char *username, char *encrypted_password);
int find_in_file(char *service_name, char *encrypted_password);
int remove_from_file(char *service_name);

// Main Function: Manages the Menu
int main() {
    int choice;
    while (1) {
        // Display Menu Options
        printf("\nPassword Manager\n");
        printf("1. Add password\n");
        printf("2. Retrieve password\n");
        printf("3. Delete password\n");
        printf("4. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        // Handle the user's choice
        switch (choice) {
            case 1: 
                add_password();
                break;
            case 2: 
                retrieve_password();
                break;
            case 3: 
                delete_password();
                break;
            case 4: 
                printf("Exiting...\n");
                exit(0);  // Exit the program
                break;
            default: 
                printf("Invalid choice\n");
        }
    }
    return 0;
}

// Function to Add a New Password
void add_password() {
    char service_name[50], username[50], password[50], encrypted_password[50];
    
    // Get user input for service name, username, and password
    printf("Enter service name: ");
    scanf("%s", service_name);
    printf("Enter username: ");
    scanf("%s", username);
    printf("Enter password: ");
    scanf("%s", password);

    // Encrypt the password
    encrypt_password(password, encrypted_password);
    
    // Save the service_name, username, and encrypted password to file
    save_to_file(service_name, username, encrypted_password);
}

// Function to Retrieve a Password
void retrieve_password() {
    char service_name[50], encrypted_password[50], decrypted_password[50];
    
    // Prompt user for the service name
    printf("Enter service name to retrieve password: ");
    scanf("%s", service_name);
    
    // Search for the service_name in the file
    if (find_in_file(service_name, encrypted_password)) {
        // If found, decrypt the password
        decrypt_password(encrypted_password, decrypted_password);
        
        // Display the decrypted password
        printf("Decrypted Password: %s\n", decrypted_password);
    } else {
        printf("No entry found for the service name: %s\n", service_name);
    }
}

// Function to Delete a Password
void delete_password() {
    char service_name[50];
    
    // Prompt user for the service name to delete
    printf("Enter service name to delete: ");
    scanf("%s", service_name);
    
    // Remove the entry from the file
    if (remove_from_file(service_name)) {
        printf("Password for %s deleted successfully.\n", service_name);
    } else {
        printf("No entry found for the service name: %s\n", service_name);
    }
}

// Encryption and Decryption functions (using XOR for simplicity)
void encrypt_password(char *password, char *encrypted_password) {
    int key = 5;  // Simple XOR key
    for (int i = 0; password[i] != '\0'; i++) {
        encrypted_password[i] = password[i] ^ key;  // XOR encryption
    }
    encrypted_password[strlen(password)] = '\0';  // Null-terminate the string
}

void decrypt_password(char *encrypted_password, char *decrypted_password) {
    int key = 5;  // Same XOR key used in encryption
    for (int i = 0; encrypted_password[i] != '\0'; i++) {
        decrypted_password[i] = encrypted_password[i] ^ key;  // XOR decryption
    }
    decrypted_password[strlen(encrypted_password)] = '\0';  // Null-terminate the string
}

// Save the password data to a file (for simplicity, assume a text file)
void save_to_file(char *service_name, char *username, char *encrypted_password) {
    FILE *file = fopen("passwords.txt", "a");  // Open file in append mode
    if (file != NULL) {
        fprintf(file, "%s %s %s\n", service_name, username, encrypted_password);
        fclose(file);
    } else {
        printf("Error saving password to file.\n");
    }
}

// Find the password in the file based on service_name
int find_in_file(char *service_name, char *encrypted_password) {
    FILE *file = fopen("passwords.txt", "r");
    char file_service_name[50], username[50];
    if (file != NULL) {
        while (fscanf(file, "%s %s %s", file_service_name, username, encrypted_password) != EOF) {
            if (strcmp(service_name, file_service_name) == 0) {
                fclose(file);
                return 1;  // Entry found
            }
        }
        fclose(file);
    }
    return 0;  // Entry not found
}

// Remove the password entry from the file
int remove_from_file(char *service_name) {
    FILE *file = fopen("passwords.txt", "r");
    FILE *temp_file = fopen("temp.txt", "w");
    char file_service_name[50], username[50], encrypted_password[50];
    int found = 0;

    if (file != NULL && temp_file != NULL) {
        while (fscanf(file, "%s %s %s", file_service_name, username, encrypted_password) != EOF) {
            if (strcmp(service_name, file_service_name) != 0) {
                fprintf(temp_file, "%s %s %s\n", file_service_name, username, encrypted_password);
            } else {
                found = 1;  // Mark the entry for deletion
            }
        }
        fclose(file);
        fclose(temp_file);

        // Replace original file with the updated one
        remove("passwords.txt");
        rename("temp.txt", "passwords.txt");
    } else {
        printf("Error: Could not open file.\n");
        return 0;
    }
    
    return found;  // Return 1 if the password was deleted, 0 if not found
}
