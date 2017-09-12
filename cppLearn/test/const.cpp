#include <stdio.h>

int main()
{
    char str[50];
    const char *cstr1; //cstr1指针所指的内容为cons类型

    cstr1 = str;
    str[0] = 'a';
    // cstr1[1] = 'b';  //  assignment of read-only location ‘*(cstr + 1u)’
    printf("const str cann't be modified %s\n", str);

    const char *cstr2 = "abcd";
    // char *str2 = cstr2; // invalid conversion from ‘const char*’ to ‘char*’ [-fpermissive]

    
}