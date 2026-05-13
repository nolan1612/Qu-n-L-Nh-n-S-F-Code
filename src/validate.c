#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <ctype.h>

#include "../includes/validate.h"
#include "../includes/utils.h"

StudentIdValidationResult validateStudentID(const char id[]) {
    if (strlen(id) == 0 || id == NULL) return STUDENT_ID_EMPTY;
    if (strlen(id) < 8) return STUDENT_ID_TOO_SHORT;
    if (strlen(id) > 12) return STUDENT_ID_TOO_LONG;
    if(id[0] != 'S' && id[1] != 'E') return FIRST_SECOND_DIGIT_INVALID;
    for(size_t i = 0; i < strlen(id); i++){
        if(!isdigit(id[i]) && !(i == 0 && id[i] == 'S') && !(i == 1 && id[i] == 'E')) return STUDENT_ID_HAS_NON_DIGIT;
    }
    return STUDENT_ID_VALID;
}
const char *getStudentIDErrorMessage(StudentIdValidationResult result){
    switch(result){
        case STUDENT_ID_EMPTY:
            return "Student ID cannot be empty!";
        case STUDENT_ID_TOO_SHORT:
            return "Student ID must be at least 8 characters long!";
        case STUDENT_ID_TOO_LONG:
            return "Student ID cannot be longer than 12 characters!";
        case STUDENT_ID_HAS_NON_DIGIT:
            return "Student ID can only contain digits!";
        case FIRST_SECOND_DIGIT_INVALID:
            return "Invalid Student ID format! Student ID must start with 'S' followed by 'E'!";
        default:
            return "Unknown error!";

    }
}

NameValidationResult validateName(const char name[]){
    if(strlen(name) == 0 || name == NULL) return NAME_EMPTY;
    if(strlen(name) > 20) return NAME_TOO_LONG;
    for(size_t i = 0; i < strlen(name); i++){
        if(!isalpha(name[i]) && name[i] != ' '){
            return NAME_HAS_INVALID_CHAR;
        }
    }
    return NAME_VALID;
}

const char *getNameErrorMessage(NameValidationResult result){
    switch(result){
        case NAME_EMPTY:
            return "Name cannot be empty!";
        case NAME_TOO_LONG:
            return "Name cannot be longer than 20 characters!";
        case NAME_HAS_INVALID_CHAR:
            return "Name can only contain letters and spaces!";
        default:
            return "Unknown error!";
    }
}

EmailValidationResult validateEmail(const char email[]){
    if(email == NULL || strlen(email) == 0){
        return EMAIL_EMPTY;
    }

    int atCount = 0;
    int atIndex = -1;

    for(int i = 0; email[i] != '\0'; i++){
        if(email[i] == '@'){
            atCount++;
            atIndex = i;
        }
    }

    if(atCount == 0){
        return EMAIL_MISSING_AT;
    }

    if(atCount > 1){
        return EMAIL_MULTIPLE_AT;
    }

    if(atIndex == 0){
        return EMAIL_INVALID_FORMAT;
    }

    const char *domain = email + atIndex + 1;

    if(strncmp(domain, "gmail", 5) != 0){
        return EMAIL_MISSING_GMAIL;
    }

    if(strcmp(domain, "gmail.com") != 0){
        return EMAIL_MISSING_DOT_COM;
    }

    return EMAIL_VALID;
}


const char *getEmailErrorMessage(EmailValidationResult result){
    switch(result){
        case EMAIL_EMPTY:
            return "Email cannot be empty!";
        case EMAIL_MISSING_AT:
            return "Email must contain @!";
        case EMAIL_MULTIPLE_AT:
            return "Email cannot contain more than one @!";
        case EMAIL_MISSING_GMAIL:
            return "Email must use gmail!";
        case EMAIL_MISSING_DOT_COM:
            return "Email must end with gmail.com!";
        case EMAIL_INVALID_FORMAT:
            return "Invalid email format!";
        case EMAIL_VALID:
        default:
            return "";
    }
}


PhoneValidationResult validatePhone(const char phone[]){
    if(phone == NULL || strlen(phone) == 0) return PHONE_EMPTY;

    size_t length = strlen(phone);

    if(length < 10) return PHONE_TOO_SHORT;
    if(length > 11) return PHONE_TOO_LONG;

    for(size_t i = 0; i < length; i++){
        if(!isdigit((unsigned char)phone[i])){
            return PHONE_INVALID_FORMAT;
        }
    }

    if(phone[0] != '0') return FIRST_DIGIT_INVALID;

    if(phone[1] != '2' && phone[1] != '3' && phone[1] != '5' &&
       phone[1] != '7' && phone[1] != '8' && phone[1] != '9'){
        return SECOND_DIGIT_INVALID;
    }

    return PHONE_VALID;
}

const char *getPhoneErrorMessage(PhoneValidationResult result){
    switch(result){
        case PHONE_EMPTY:
            return "Phone number cannot be empty!";
        case PHONE_TOO_SHORT:
            return "Phone number must be at least 10 characters long!";
        case PHONE_TOO_LONG:
            return "Phone number cannot be longer than 11 characters!";
            case FIRST_DIGIT_INVALID:
            return "Invalid phone number format! Phone number must start with '0'!";
        case SECOND_DIGIT_INVALID:
            return "Invalid phone number format! The second digit of the phone number must be one of the following: '2', '3', '5', '7', '8', or '9'!";
        case PHONE_INVALID_FORMAT:
            return "Phone number can only contain digits";
            case PHONE_VALID:
            return "";
        default:
            return "Unknown error!";
    }
}