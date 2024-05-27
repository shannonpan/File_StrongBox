#include <Windows.h>
#include <stdio.h>
#include <string.h>

#define FOLDER_NAME "Folder"
#define PATH_MAX 260
#define PASSWORD "111"  // Define the password here

int main() {

    char currentPath[PATH_MAX];
    char normalPath[PATH_MAX];
    char specialPath[PATH_MAX];

    GetCurrentDirectory(PATH_MAX, currentPath); 
    snprintf(normalPath, PATH_MAX, "%s\\%s", currentPath, FOLDER_NAME);
    snprintf(specialPath, PATH_MAX, "\\\\?\\%s\\[Locked]%s..", currentPath, FOLDER_NAME);

    DWORD ftyp = GetFileAttributesA(specialPath);
    BOOL specialExists = (ftyp != INVALID_FILE_ATTRIBUTES) && (ftyp & FILE_ATTRIBUTE_DIRECTORY);

    ftyp = GetFileAttributesA(normalPath);
    BOOL normalExists = (ftyp != INVALID_FILE_ATTRIBUTES) && (ftyp & FILE_ATTRIBUTE_DIRECTORY);

    if (specialExists && normalExists) {
        printf("Both locked and unlocked folder exist. Please manually handle the existing folders.\n");
        MessageBeep(MB_ICONERROR);
        system("pause");
        return 1; 
    } 

    if (specialExists && !normalExists) {
        char password[100];
        while(TRUE){
            printf("Enter password to unlock the folder: ");
            fgets(password, 99, stdin);
            password[strcspn(password, "\n")] = 0;
            if (strcmp(password, PASSWORD) == 0) {
                    MoveFile(specialPath, normalPath);
                    MessageBeep(MB_ICONINFORMATION);
                    break;
            }else {
                system("cls");
                printf("Password incorrect.Please retry.\n");
                MessageBeep(MB_ICONERROR);
                continue;
            }
        }
        return 0;
    }

    if (!specialExists && normalExists) {
            MoveFile(normalPath, specialPath);
            MessageBeep(MB_ICONINFORMATION);
    }

    if (!specialExists && !normalExists) {
            CreateDirectory(normalPath, NULL);
            MessageBeep(MB_ICONINFORMATION);
    }

    return 0;
}
