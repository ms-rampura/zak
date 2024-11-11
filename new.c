#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
    struct Member members[100]; // Assuming a maximum of 100 members per society
};

// Function prototypes
void writeDataToFile(int numSocieties);
void readSpecificSocietyFromFile();
void deleteMemberFromSociety();

int main() {
    int choice;
    int numSocieties;

    while (1) {
        printf("\n--- Society Management System ---\n");
        printf("1. Enter new societies and members data\n");
        printf("2. View members of a specific society\n");
        printf("3. Delete a member from a society\n");
        printf("4. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                printf("Enter the number of societies: ");
                scanf("%d", &numSocieties);
                writeDataToFile(numSocieties);
                break;
            case 2:
                readSpecificSocietyFromFile();
                break;
            case 3:
                deleteMemberFromSociety();
                break;
            case 4:
                printf("Exiting the program.\n");
                exit(0);
            default:
                printf("Invalid choice. Please try again.\n");
        }
    }

    return 0;
}

// Function to write data to the file
void writeDataToFile(int numSocieties) {
    struct Society society;
    FILE *file;

    file = fopen("societies.dat", "wb");
    if (file == NULL) {
        printf("Error opening file!\n");
        exit(1);
    }

    for (int i = 0; i < numSocieties; i++) {
        printf("\n--- Enter details for Society %d ---\n", i + 1);
        printf("Enter Society Name: ");
        scanf(" %[^\n]", society.societyName);

        printf("Enter the number of members in %s: ", society.societyName);
        scanf("%d", &society.numMembers);

        for (int j = 0; j < society.numMembers; j++) {
            printf("\n--- Enter details for Member %d of %s ---\n", j + 1, society.societyName);
            printf("Enter Name: ");
            scanf(" %[^\n]", society.members[j].name);
            printf("Enter Batch Number: ");
            scanf("%d", &society.members[j].batchNo);
            printf("Enter Roll Number: ");
            scanf("%d", &society.members[j].rollNo);
        }

        fwrite(&society, sizeof(struct Society), 1, file);
    }

    printf("\nData written to file successfully.\n");
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
        printf("Error opening file!\n");
        exit(1);
    }

    printf("\nEnter the Society Name to view its members: ");
    scanf(" %[^\n]", searchSocietyName);

    while (fread(&society, sizeof(struct Society), 1, file)) {
        if (strcmp(society.societyName, searchSocietyName) == 0) {
            found = 1;
            printf("\nSociety Name: %s\n", society.societyName);
            printf("Number of Members: %d\n", society.numMembers);

            for (int j = 0; j < society.numMembers; j++) {
                printf("\n  Member %d:\n", j + 1);
                printf("  Name: %s\n", society.members[j].name);
                printf("  Batch Number: %d\n", society.members[j].batchNo);
                printf("  Roll Number: %d\n", society.members[j].rollNo);
            }
            break;
        }
    }

    if (!found) {
        printf("Society '%s' not found.\n", searchSocietyName);
    }
    fclose(file);
}

// Function to delete a member from a specific society
void deleteMemberFromSociety() {
    struct Society society;
    char searchSocietyName[50];
    int rollNumberToDelete;
    int found = 0, memberFound = 0;
    FILE *file, *tempFile;

    file = fopen("societies.dat", "rb");
    tempFile = fopen("temp.dat", "wb");
    if (file == NULL || tempFile == NULL) {
        printf("Error opening file!\n");
        exit(1);
    }

    printf("\nEnter the Society Name from which to delete a member: ");
    scanf(" %[^\n]", searchSocietyName);

    while (fread(&society, sizeof(struct Society), 1, file)) {
        if (strcmp(society.societyName, searchSocietyName) == 0) {
            found = 1;
            printf("Enter the Roll Number of the member to delete: ");
            scanf("%d", &rollNumberToDelete);

            for (int i = 0; i < society.numMembers; i++) {
                if (society.members[i].rollNo == rollNumberToDelete) {
                    memberFound = 1;
                    for (int j = i; j < society.numMembers - 1; j++) {
                        society.members[j] = society.members[j + 1];
                    }
                    society.numMembers--;
                    printf("Member with Roll Number %d has been deleted.\n", rollNumberToDelete);
                    break;
                }
            }
        }
        fwrite(&society, sizeof(struct Society), 1, tempFile);
    }

    if (!found) {
        printf("Society '%s' not found.\n", searchSocietyName);
    } else if (!memberFound) {
        printf("Member with Roll Number %d not found in '%s'.\n", rollNumberToDelete, searchSocietyName);
    }

    fclose(file);
    fclose(tempFile);

    // Replace the original file with the updated file
    remove("societies.dat");
    rename("temp.dat", "societies.dat");
}
