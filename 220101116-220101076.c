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
<<<<<<< Updated upstream
    Student* students; // öğrencilerin listesi
    Teacher* teachers; // öğretmenlerin listesi
=======
    Student* students; // öğrencilerin listesi "düzenleyip hallettik"
    Teacher* teachers; // öğretmenlerin listesi "++"
>>>>>>> Stashed changes
    struct Course* next;
} Course;

// input string
void getUserInput(char* prompt, char* userInput, int maxLength) {
    printf("%s", prompt);
    fgets(userInput, maxLength, stdin);

    //yeni satır karakterini kaldırır
    size_t length = strlen(userInput);
    if (length > 0 && userInput[length - 1] == '\n') {
        userInput[length - 1] = '\0';
    }
}

// input integer
int getUserInputInt(char* prompt) {
    int value;
    char userInput[50];

    while (1) { // kurs kapasitesi belirlenirken sayı yerine harf girilmesi durumunda ERROR
        getUserInput(prompt, userInput, sizeof(userInput));
        if (sscanf(userInput, "%d", &value) == 1) {
            return value;
        } else {
            printf("Lutfen duzgun bir kapasite giriniz.Kapasite sayi ile ifade edilmelidir.\n");
        }
    }
}


// öğrenciyi kursa enrollamak/atamak
void addStudentToCourse(Course* course, Student* student) {
    if (course->enrolled < course->kapasite) {
        student->studentID = course->enrolled + 1; // öğrenci kimlikleri randomize değil sıralı olarak verilir ( ilk öğrenci 1 numaralı ikinci öğrenci 2 numaralı vs. )
        student->next = course->students;
        course->students = student;
        course->enrolled++;
        printf("Ogrenci %s (ID: %d) su kursa atanmistir :  %s\n", student->name, student->studentID, course->name);
    } else {
        printf("ERROR: Bu kursun kontejyani doludur.\n");
        free(student);
    }
}


// kursa yardımcı öğretmen enrollamak/atamak
void addTeacherToCourse(Course* course, Teacher* teacher) {
    // Kontrol eder++
    int teacherCount = 0;
    Teacher* currentTeacher = course->teachers;
    while (currentTeacher != NULL) {
        teacherCount++;
        currentTeacher = currentTeacher->next;
    }

    if (teacherCount < 2) { //yardımcı öğretmen sayısının 2 den fazla olmaması durumunda
        teacher->next = course->teachers;
        course->teachers = teacher;
        printf("Yardimci ogretmen %s, kursuna atanmistir: %s\n", teacher->name, course->name);
    } else {
        printf("Error: Her kursta sadece 2 tane yardimci ogretmen olabilir.\n"); //yardımcı öğretmen sayısının 2 fazla olması durumunda
        free(teacher);
    }
}

Course* addCourse(Course* head, const char* name, int kapasite, const char* teacher) {
    //aynı isimli kurs açmak istendiğinde kursun var olduğunu gösteren error
    Course* currentCourse = head;
    while (currentCourse != NULL) {
        if (strcmp(currentCourse->name, name) == 0) {
            printf("ERROR:'%s'isminde bir kurs zaten mevcuttur.\n", name);
            return head;  // listeyi değiştirmediğimiz head'e return
        }
        currentCourse = currentCourse->next;
    }

    // yeni bir kurs eklemek/listede göstermek
    Course* newCourse = (Course*)malloc(sizeof(Course));
    strcpy(newCourse->name, name);
    newCourse->kapasite = kapasite;
    newCourse->enrolled = 0;
    strcpy(newCourse->teacher, teacher);
    newCourse->students = NULL;
    newCourse->teachers = NULL;
    newCourse->next = head;

    printf("'%s' isimli kurs listeye eklenmistir !\n", name);

    return newCourse;
}


// listeyi göstermek ( tüm kursları )
void displayCourses(Course* head) {
    printf("\nKurslar:\n");
    Course* current = head;
    while (current != NULL) {
        printf("%s (Kapasite: %d, Kursu Alan Ogrenci Sayisi: %d, Sorumlu Bas Ogretmen: %s", current->name, current->kapasite, current->enrolled, current->teacher);

        // Atanmış öğrencileri gösterir
        printf(", Ogrenci Listesi: ");
        if (current->students == NULL) {
            printf("Suanda bu kursda ogrenci bulunmamaktadir."); //öğrenci enrollanmama durumunda
        } else {
            Student* currentStudent = current->students;
            while (currentStudent != NULL) {
                printf("%s (%d), ", currentStudent->name, currentStudent->studentID);
                currentStudent = currentStudent->next;
            }
        }

        // Dersin/kursun yardımcı öğretmenlerini gösterir
        printf(", Sorumlu Yardimci Ogretmenlerin Listesi: ");
        if (current->teachers == NULL) {
            printf("Henuz bu kursa Sorumlu Yardimci Ogretmen atanmamistir."); //yardımcı öğretmen enrollanmama durumunda
        } else {
            Teacher* currentTeacher = current->teachers;
            while (currentTeacher != NULL) {
                printf("%s, ", currentTeacher->name);
                currentTeacher = currentTeacher->next;
            }
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
        printf("\nMenu:\n");
        printf("1. Kurs Ekle\n");
        printf("2. Kursa Ogrenci Ekle\n");
        printf("3. Kursa Yardimci Ogretmen Ekle\n");
        printf("4. Kurs Listesini Goster\n");
        printf("0. Cikis Yap\n");

        choice = getUserInputInt("Uygulamak istediginiz secenegin numarasini giriniz: ");

        switch (choice) {
           case 1: {
    char name[50];
    getUserInput("Kurs ismi giriniz: ", name, sizeof(name));

    // aynı isimli kursun mevcut olup olmadığına bak
    Course* currentCourse = courseList;
    while (currentCourse != NULL) {
        if (strcmp(currentCourse->name, name) == 0) {
            printf("ERROR:'%s'isminde bir kurs zaten mevcuttur.\n", name);
            break;  // kursun mevcut olması durumunda loopdan çıkar
        }
        currentCourse = currentCourse->next;
    }

    // kursun mevcut olmaması durumunda loop'a devam eder
    if (currentCourse == NULL) {
        int kapasite = getUserInputInt("Kursun kac kisilik oldugunu giriniz: ");
        char teacher[50];
        getUserInput("Kursun Sorumlu Bas Ogretmeninin ismini giriniz: ", teacher, sizeof(teacher));

        courseList = addCourse(courseList, name, kapasite, teacher);
        printf("Kursunuz listeye eklenmistir!\n");
    }

    break;
}

            case 2: {
                char courseName[50];
                getUserInput("Kurs ismi giriniz: ", courseName, sizeof(courseName));

                // kursun var olup olmadığını kontrol et "++" fixlendi
                Course* currentCourse = courseList;
                while (currentCourse != NULL) {
                    if (strcmp(currentCourse->name, courseName) == 0) {
                        Student* newStudent = (Student*)malloc(sizeof(Student));
                        getUserInput("Ogrenci ismi giriniz: ", newStudent->name, sizeof(newStudent->name));
                        newStudent->next = NULL;

                        addStudentToCourse(currentCourse, newStudent);
                        break;
                    }
                    currentCourse = currentCourse->next;
                }

                if (currentCourse == NULL) {
                    printf("ERROR:Boyle bir kurs bulunmamaktadir.Eger kurs eklemek istiyorsaniz menuden ekleyebilirsiniz.\n");
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
                        getUserInput("Kursun yardimci ogretmeninin ismini giriniz: ", newTeacher->name, sizeof(newTeacher->name));
                        newTeacher->next = NULL;

                        addTeacherToCourse(currentCourse, newTeacher);
                        break;
                    }
                    currentCourse = currentCourse->next;
                }

                if (currentCourse == NULL) {
                    printf("ERROR:Boyle bir kurs bulunmamaktadir.Eger kurs eklemek istiyorsaniz menuden ekleyebilirsiniz.\n");
                }

                break;
            }
            case 4:
                displayCourses(courseList);
                break;
            case 0:
                printf("Cikis yapiliyor...\n");
                break;
            default:
                printf("Boyle bir secenek bulunamamaktadir.Lutfen tekrar deneyiniz.\n");
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
