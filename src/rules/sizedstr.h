
#ifndef _SIZEDSTR_H
#define _SIZEDSTR_H

//
// This struct is used to support strings containing null chars. The length of 
// the string is stored along the string data. However the string data is also
// terminated with a null char.
//

typedef struct _SIZED_STRING
{
    int length;
    char c_string[1];  

} SIZED_STRING;

#endif

