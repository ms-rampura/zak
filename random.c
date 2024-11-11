#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h> // For delay function

// Define colors (These may not work on all terminals, but should work on most UNIX-based systems)
#define RESET   "\033[0m"
#define GREEN   "\033[32m"
#define RED     "\033[31m"
#define YELLOW  "\033[33m"
#define BLUE    "\033[34m"

// Define a structure for a member
struct Member {
    char name[50];
    int batchNo;
    int rollNo;
};

// Define a structure for a society
struct Society {
    char societyName[50];
    int numMembers;
    struct Member members[100];
};

// Function prototypes
void writeDataToFile();
void readSpecificSocietyFromFile();
void deleteMemberFromSociety();
void delay(int milliseconds);

// Main function
int main() {
    int choice;

    while (1) {
        printf(GREEN "\n--- Society Management System ---\n" RESET);
        printf("1. Enter new societies and members data\n");
        printf("2. View members of a specific society\n");
        printf("3. Delete a member from a society\n");
        printf("4. Exit\n");
        printf(YELLOW "Enter your choice: " RESET);
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                writeDataToFile();
                break;
            case 2:
                readSpecificSocietyFromFile();
                break;
            case 3:
                deleteMemberFromSociety();
                break;
            case 4:
                printf(RED "Exiting the program...\n" RESET);
                exit(0);
            default:
                printf(RED "Invalid choice! Please try again.\n" RESET);
        }
    }

    return 0;
}

// Cross-platform delay function
void delay(int milliseconds) {
    clock_t start_time = clock();
    while (clock() < start_time + milliseconds * CLOCKS_PER_SEC / 1000);
}

// Function to write data to the file
void writeDataToFile() {
    struct Society society;
    FILE *file;

    file = fopen("societies.dat", "ab"); // Append mode to keep existing data
    if (file == NULL) {
        printf(RED "Error opening file for writing!\n" RESET);
        return;
    }

    printf(YELLOW "\nEnter Society Name: " RESET);
    scanf(" %[^\n]", society.societyName);

    printf(YELLOW "Enter number of members: " RESET);
    scanf("%d", &society.numMembers);

    for (int i = 0; i < society.numMembers; i++) {
        printf(BLUE "\nMember %d:\n" RESET, i + 1);
        printf(YELLOW "  Name: " RESET);
        scanf(" %[^\n]", society.members[i].name);
        printf(YELLOW "  Batch Number: " RESET);
        scanf("%d", &society.members[i].batchNo);
        printf(YELLOW "  Roll Number: " RESET);
        scanf("%d", &society.members[i].rollNo);
    }

    fwrite(&society, sizeof(struct Society), 1, file);
    printf(GREEN "\nData written to file successfully.\n" RESET);

    fclose(file);
}

// Function to read and display data of a specific society
void readSpecificSocietyFromFile() {
    struct Society society;
    char searchSocietyName[50];
    int found = 0;
    FILE *file;

    file = fopen("societies.dat", "rb");
    if (file == NULL) {
        printf(RED "Error opening file for reading!\n" RESET);
        return;
    }

    printf(YELLOW "\nEnter the Society Name to search: " RESET);
    scanf(" %[^\n]", searchSocietyName);

    while (fread(&society, sizeof(struct Society), 1, file)) {
        if (strcmp(society.societyName, searchSocietyName) == 0) {
            found = 1;
            printf(GREEN "\nSociety Name: %s\n" RESET, society.societyName);
            printf(GREEN "Number of Members: %d\n" RESET, society.numMembers);
            for (int i = 0; i < society.numMembers; i++) {
                printf(BLUE "\n  Member %d:\n" RESET, i + 1);
                printf("    Name: %s\n", society.members[i].name);
                printf("    Batch Number: %d\n", society.members[i].batchNo);
                printf("    Roll Number: %d\n", society.members[i].rollNo);
            }
            break;
        }
    }

    if (!found) {
        printf(RED "Society '%s' not found.\n" RESET, searchSocietyName);
    }

    fclose(file);
}

// Function to delete a member from a society
void deleteMemberFromSociety() {
    struct Society society;
    char searchSocietyName[50];
    int rollNumberToDelete;
    int found = 0, memberFound = 0;
    FILE *file, *tempFile;

    file = fopen("societies.dat", "rb");
    tempFile = fopen("temp.dat", "wb");
    if (file == NULL || tempFile == NULL) {
        printf(RED "Error opening file!\n" RESET);
        return;
    }

    printf(YELLOW "\nEnter the Society Name to delete a member from: " RESET);
    scanf(" %[^\n]", searchSocietyName);

    while (fread(&society, sizeof(struct Society), 1, file)) {
        if (strcmp(society.societyName, searchSocietyName) == 0) {
            found = 1;
            printf(YELLOW "Enter the Roll Number of the member to delete: " RESET);
            scanf("%d", &rollNumberToDelete);

            // Search for the member by roll number
            for (int i = 0; i < society.numMembers; i++) {
                if (society.members[i].rollNo == rollNumberToDelete) {
                    memberFound = 1;
                    // Shift members to remove the deleted one
                    for (int j = i; j < society.numMembers - 1; j++) {
                        society.members[j] = society.members[j + 1];
                    }
                    society.numMembers--;
                    printf(GREEN "Member with Roll Number %d has been deleted.\n" RESET, rollNumberToDelete);
                    break;
                }
            }
        }

        // Write the updated society data to the temporary file
        fwrite(&society, sizeof(struct Society), 1, tempFile);
    }

    if (!found) {
        printf(RED "Society '%s' not found.\n" RESET, searchSocietyName);
    } else if (!memberFound) {
        printf(RED "Member with Roll Number %d not found.\n" RESET, rollNumberToDelete);
    }

    fclose(file);
    fclose(tempFile);

    // Replace the original file with the updated data
    remove("societies.dat");
    rename("temp.dat", "societies.dat");
}
