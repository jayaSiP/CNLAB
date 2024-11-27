#include <stdio.h>

// Define the structure
struct info
{
    int roll_no;
    char name[50];
    float CGPA;
};

// Function to print the values of the structure
void printInfo(struct info *student)
{
    printf("\n--- Student Information ---\n");
    printf("Roll Number: %d\n", student->roll_no);
    printf("Name: %s\n", student->name);
    printf("CGPA: %.2f\n", student->CGPA);
}

int main()
{
    struct info student;
    printf("Enter Roll Number: ");
    scanf("%d", &student.roll_no);
    printf("Enter Name: ");
    scanf(" %[^\n]s", student.name); 
    printf("Enter CGPA: ");
    scanf("%f", &student.CGPA);
    printInfo(&student);
    return 0;
}
