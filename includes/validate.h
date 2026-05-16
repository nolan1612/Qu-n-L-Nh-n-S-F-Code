#ifndef VALIDATE_H
#define VALIDATE_H


#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

typedef enum {
    STUDENT_ID_VALID = 0,
    STUDENT_ID_EMPTY,
    STUDENT_ID_TOO_SHORT,
    STUDENT_ID_TOO_LONG,
    STUDENT_ID_HAS_NON_DIGIT,
    FIRST_SECOND_DIGIT_INVALID
} StudentIdValidationResult;


typedef enum{
   NAME_VALID = 0,
   NAME_EMPTY,
   NAME_TOO_LONG,
   NAME_HAS_INVALID_CHAR
} NameValidationResult;


typedef enum {
    EMAIL_VALID = 0,
    EMAIL_EMPTY,
    EMAIL_MULTIPLE_AT,
    EMAIL_MISSING_AT,
    EMAIL_MISSING_GMAIL,
    EMAIL_MISSING_DOT_COM,
    EMAIL_INVALID_FORMAT
} EmailValidationResult;


typedef enum{
    PHONE_VALID = 0,
    PHONE_EMPTY,
    PHONE_INVALID_FORMAT,
    PHONE_TOO_SHORT,
    PHONE_TOO_LONG,
    FIRST_DIGIT_INVALID,
    SECOND_DIGIT_INVALID
} PhoneValidationResult;

StudentIdValidationResult validateStudentID(const char id[]);
const char *getStudentIDErrorMessage(StudentIdValidationResult result);

NameValidationResult validateName(const char name[]);
const char *getNameErrorMessage(NameValidationResult result);

EmailValidationResult validateEmail(const char email[]);
const char *getEmailErrorMessage(EmailValidationResult result);

PhoneValidationResult validatePhone(const char phone[]);
const char *getPhoneErrorMessage(PhoneValidationResult result);
#endif