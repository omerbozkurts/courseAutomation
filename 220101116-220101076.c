#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Mehmet Ertuğrul ERBOĞA 220101116
// Tolga Berat ÖZTÜRK 220101076


// öğrenci structure
typedef struct Student {
    char name[50];
    int studentID;
    struct Student* next;
} Student;

// öğretmen structure
typedef struct Teacher {
    char name[50];
    struct Teacher* next;
} Teacher;

// açılıcak kursun structure
typedef struct Course {
    char name[50];
    int kapasite;
    int enrolled;
    char teacher[50];
    Student* students; // öğrencilerin listesi "--"
    Teacher* teachers; // öğretmenlerin listes "++"
    struct Course* next;
} Course;

// input string
void getUserInput(char* prompt, char* userInput, int maxLength) {
    printf("%s", prompt);
    fgets(userInput, maxLength, stdin);

    // remove the trailing newline character
    size_t length = strlen(userInput);
    if (length > 0 && userInput[length - 1] == '\n') {
        userInput[length - 1] = '\0';
    }
}

// input integer
int getUserInputInt(char* prompt) {
    char userInput[50];
    getUserInput(prompt, userInput, sizeof(userInput));

    // string -> integer
    return atoi(userInput);
}

// öğrenciyi kursa enrollamak/atamak
void addStudentToCourse(Course* course, Student* student) {
    if (course->enrolled < course->kapasite) {
        student->next = course->students;
        course->students = student;
        course->enrolled++;
        printf("Öğrenci %s (ID: %d) şu kursa atanmıştır :  %s\n", student->name, student->studentID, course->name);
    } else {
        printf("ERROR:Bu kursun kontejyanı doludur.\n"); //Kursun dolması halinde verilecek error - kurs kontejyanından fazla öğrenci eklenmeye çalışırsa
        free(student);
    }
}

// kursa öğretmen enrollamak
void addTeacherToCourse(Course* course, Teacher* teacher) {
    // ikiden fazla öğretmen atanmasını istemediğimizden ötürü  öğretmen sayısı kontrol "--"
    int teacherCount = 0;
    Teacher* currentTeacher = course->teachers;
    while (currentTeacher != NULL) {
        teacherCount++;
        currentTeacher = currentTeacher->next;
    }

    if (teacherCount < 2) {
        teacher->next = course->teachers;
        course->teachers = teacher;
        printf("Öğretmen %s şu kursa atanmıştır :  %s\n", teacher->name, course->name);
    } else {
        printf("Error: Her kursta sadece 2 tane öğretmen olabilir.\n");
        free(teacher);
    }
}

// listeye yeni bir kurs ekleme
Course* addCourse(Course* head, const char* name, int kapasite, const char* teacher) {
    Course* newCourse = (Course*)malloc(sizeof(Course));
    strcpy(newCourse->name, name);
    newCourse->kapasite = kapasite;
    newCourse->enrolled = 0;
    strcpy(newCourse->teacher, teacher);
    newCourse->students = NULL;
    newCourse->teachers = NULL;
    newCourse->next = head;
    return newCourse;
}

// listeyi göstermek ( tüm kursları )
void displayCourses(Course* head) {
    printf("\n:\n");
    Course* current = head;
    while (current != NULL) {
        printf("%s (Kapasite: %d, Atanan Öğrenci Sayısı: %d, Öğretmen(ler): %s", current->name, current->kapasite, current->enrolled, current->teacher);

        // kurslardaki öğretmenleri gösterir
        Teacher* currentTeacher = current->teachers;
        while (currentTeacher != NULL) {
            printf(", %s", currentTeacher->name);
            currentTeacher = currentTeacher->next;
        }

        printf(")\n");
        current = current->next;
    }
}

int main() {
    Course* courseList = NULL; // kursları depolama

    // menü
    int choice;
    do {
        printf("\nMenü:\n");
        printf("1. Kurs Ekle\n");
        printf("2. Kursa Öğrenci Ekle\n");
        printf("3. Kursa Öğretmen Ekle\n");
        printf("4. Kurs Listesini Göster\n");
        printf("0. Çıkış\n");

        choice = getUserInputInt("Yapmak istediğinizin numarasını giriniz: ");

        switch (choice) {
            case 1: {
                char name[50];
                getUserInput("Kurs ismi giriniz: ", name, sizeof(name));
                int kapasite = getUserInputInt("Kursun kaç kişilik olduğunu giriniz: ");
                char teacher[50];
                getUserInput("Kursun öğretmeninin ismini giriniz: ", teacher, sizeof(teacher));

                courseList = addCourse(courseList, name, kapasite, teacher);
                printf("Kursunuz listeye eklenmiştir!\n");
                break;
            }
            case 2: {
                char courseName[50];
                getUserInput("Kurs ismi giriniz: ", courseName, sizeof(courseName));

                // kursun var olup olmadığını kontrol et "--" aynı isimli kursları kontrol edemiyor ve yeniden ekliyor
                Course* currentCourse = courseList;
                while (currentCourse != NULL) {
                    if (strcmp(currentCourse->name, courseName) == 0) {
                        Student* newStudent = (Student*)malloc(sizeof(Student));
                        getUserInput("Öğrenci ismi giriniz: ", newStudent->name, sizeof(newStudent->name));
                        newStudent->studentID = rand() % 1000; // student id'yi random generate eder
                        newStudent->next = NULL;

                        addStudentToCourse(currentCourse, newStudent);
                        break;
                    }
                    currentCourse = currentCourse->next;
                }

                if (currentCourse == NULL) {
                    printf("ERROR:Böyle bir kurs bulunmamaktadır.Eğer kurs eklemek istiyorsanız menüden ekleyebilirsiniz.\n");
                }

                break;
            }
            case 3: {
                char courseName[50];
                getUserInput("Kurs ismi giriniz: ", courseName, sizeof(courseName));

                // kursun var olup olmadığını kontrol eder
                Course* currentCourse = courseList;
                while (currentCourse != NULL) {
                    if (strcmp(currentCourse->name, courseName) == 0) {
                        Teacher* newTeacher = (Teacher*)malloc(sizeof(Teacher));
                        getUserInput("Kursun öğretmeninin ismini giriniz: ", newTeacher->name, sizeof(newTeacher->name));
                        newTeacher->next = NULL;

                        addTeacherToCourse(currentCourse, newTeacher);
                        break;
                    }
                    currentCourse = currentCourse->next;
                }

                if (currentCourse == NULL) {
                    printf("ERROR:Böyle bir kurs bulunmamaktadır.Eğer kurs eklemek istiyorsanız menüden ekleyebilirsiniz.\n");
                }

                break;
            }
            case 4:
                displayCourses(courseList);
                break;
            case 0:
                printf("Çıkış yapılıyor...\n");
                break;
            default:
                printf("Böyle bir seçenek bulunamamakta.Lütfen tekrar deneyiniz.\n");
        }

    } while (choice != 0);

    // free memory
    Course* currentCourse = courseList;
    while (currentCourse != NULL) {
        Student* currentStudent = currentCourse->students;
        while (currentStudent != NULL) {
            Student* nextStudent = currentStudent->next;
            free(currentStudent);
            currentStudent = nextStudent;
        }

        Teacher* currentTeacher = currentCourse->teachers;
        while (currentTeacher != NULL) {
            Teacher* nextTeacher = currentTeacher->next;
            free(currentTeacher);
            currentTeacher = nextTeacher;
        }

        Course* nextCourse = currentCourse->next;
        free(currentCourse);
        currentCourse = nextCourse;
    }

    return 0;
}
