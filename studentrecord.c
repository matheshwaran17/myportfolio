#include <stdio.h>
#include <string.h>
#include <stdlib.h>

struct student {
    int roll;
    char name[50];
    float marks;
};

void addStudent();
void viewStudents();
void searchStudent();
void updateStudent();
void deleteStudent();
void saveToFile();
void loadFromFile();

int main() {
    int choice;
    
    while(1) {
        printf("\nStudent Record Management System\n");
        printf("1. Add Student\n");
        printf("2. View Students\n");
        printf("3. Search Student\n");
        printf("4. Update Student\n");
        printf("5. Delete Student\n");
        printf("6. Save to File\n");
        printf("7. Load from File\n");
        printf("8. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        
        switch(choice) {
            case 1: addStudent(); break;
            case 2: viewStudents(); break;
            case 3: searchStudent(); break;
            case 4: updateStudent(); break;
            case 5: deleteStudent(); break;
            case 6: saveToFile(); break;
            case 7: loadFromFile(); break;
            case 8: exit(0);
            default: printf("Invalid choice!\n");
        }
    }
    return 0;
}

void addStudent() {
    struct student s;
    printf("Enter Roll Number: ");
    scanf("%d", &s.roll);
    printf("Enter Name: ");
    scanf("%s", s.name);
    printf("Enter Marks: ");
    scanf("%f", &s.marks);
    
    FILE *file = fopen("students.dat", "ab+");
    fwrite(&s, sizeof(struct student), 1, file);
    fclose(file);
    printf("Student added successfully!\n");
}

void viewStudents() {
    struct student s;
    FILE *file = fopen("students.dat", "rb");
    
    if(file == NULL) {
        printf("No records found!\n");
        return;
    }
    
    printf("\nRoll\tName\tMarks\n");
    while(fread(&s, sizeof(struct student), 1, file)) {
        printf("%d\t%s\t%.2f\n", s.roll, s.name, s.marks);
    }
    fclose(file);
}

// Implement other functions similarly...
