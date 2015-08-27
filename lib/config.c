#include   <stdio.h>
#include   <stdlib.h>
#include   <ctype.h>
#include   <string.h>
#include   "config.h"

static int   isremark(const char* line);
static int   isjoint(const char* line);
static char* readline(char* line, int n, FILE *stream);
static int   getsection(const char* line, char* section, unsigned int seclen);
static int   getkeyname(const char* line, char* keyname, unsigned int keylen);
static int   getkeyvalue(const char* line, char* keyvalue, unsigned int valen);
static int   getkeyadd(const char* line, char* keyvalue, unsigned int valen);


static char* readline(char* line, int n, FILE *stream)
{   
    static char* rs;

    rs = fgets(line, n, stream);
    if (strlen(line) >=  1) {   
        line[strlen(line)-1] = line[strlen(line)];
    }   

    return   rs;   
}   


static int isjoint(const char* line)   
{   
    unsigned int i;   
    unsigned int sign = 0;   

    for (i = 0; i < strlen(line); i++) {   
        if (line[i] == ' ' || line[i] == '\t' || line[i] == '\\') {   
            if (line[i] == ' ' || line[i] == '\t') {   
                if (sign < 1) {
                    sign++;   
                }
            } else {   
                if (sign > 0) {
                    sign++;
                } else {
                    return 0;
                }
            }   
        } else {   
            return 0;   
        }   
    }   

    if (sign == 2) { 
        return 1;   
    } else {
        return 0;
    }
}   


static int isremark(const char* line) 
{   
    unsigned int i; 

    for (i = 0; i < strlen(line); i++) { 
        if (isgraph(line[i])) {
            if ( line[i] == '#' ) {
                return 1;   
            } else {
                return 0;
            }
        }   
    }   

    return 1;   
}   


static int getsection(const char* line, char* section, unsigned int seclen)   
{   
    unsigned int start, mid, end;   

    for (start = 0; start < strlen(line); start++) {   
        if (isgraph(line[start])) {   
            if ( line[start] != '[' ) {
                return 0;   
            } else {
                break;
            }
        }   
    }   

    if (start >= strlen(line)) { 
        return 0;
    }

    for (end = strlen(line); end > 1; end--) {   
        if (isgraph(line[end])) {
            if (line[end] != ']') {
                return 0;
            } else {
                break;
            }
        }
    }

    if (end <= 1) {
        return 0;
    }

    if (end - start < 2) {
        return 0;
    }

    for (mid = start + 1; mid < end; mid++) {
        if (!isalnum(line[mid])) {
            return 0;
        }
    }

    if (seclen > end - start - 1) { 
        memcpy(section, &line[start + 1], end - start - 1); 
        section[end - start - 1] = '\0'; 
        return (end - start - 1); 
    } else {   
        memcpy(section, &line[start + 1], seclen - 1);   
        section[seclen - 1] = '\0';   
        return (seclen -1);   
    }
}   


static int getkeyname(const char* line, char* keyname, unsigned int keylen)   
{   
    unsigned int start, mid, end;   

    for (start = 0; start < strlen(line); start++) {   
        if ( isgraph(line[start]) ) {   
            if ( !isalnum(line[start]) ) {
                return 0;   
            } else {
                break;
            }
        }   
    }   

    if (start >= strlen(line)) {
        return 0;
    }

    for (end = start; line[end] != '=' && end < strlen(line); end++) {
        if ( !isalnum(line[end]) && line[end] != ' ' && line[end] != '\t' ) {
            return 0;
        }
    }   

    if (end >= strlen(line)) {
        return 0;
    }

    /* 到这里end是=的索引 */


    /* 去掉=号前面的非空格和数字字符 */
    for (end = end - 1; !isalnum(line[end]); end--) {   
    }   

    for (mid = start; mid <= end; mid++) {   
        if ( line[mid] == ' ' || line[mid] == '\t' ) {    
            return 0;
        }
    }   


    if (keylen > end - start + 1) {   
        memcpy(keyname, &line[start], end - start + 1);   
        keyname[end - start + 1] = '\0';   
        return (end - start + 1);   
    } else {   
        memcpy(keyname, &line[start], keylen - 1);   
        keyname[keylen - 1] = '\0';   
        return (keylen -1);   
    }   

}   


static int getkeyvalue(const char* line, char* keyvalue, unsigned int valen)   
{   
    unsigned int start, end;   
    int len;   

    for (start = 0; line[start] != '=' && start < strlen(line); start++) {
    }   

    for (start = start + 1; !isgraph(line[start]) && start < strlen(line); start++) {
    }   

    if ( start >= strlen(line) ) {
        return 0;
    }

    for (end = start; line[end] != ' ' && line[end] != '\t' && end < strlen(line); end++) {
    }   

    if ( valen > end - start ) { 
        memcpy(keyvalue, &line[start], end - start); 
        keyvalue[end - start] = '\0'; 
        len = end - start; 
    } else {   
        memcpy(keyvalue, &line[start], valen - 1); 
        keyvalue[valen - 1] = '\0'; 
        len = valen - 1;   
    } 

    if ( end >= strlen(line) ) {
        return len;   
    }else {   
        if ( isjoint(&line[end]) ) {
            return -len;
        }
        return len;
    }   
}   


static int getkeyadd(const char* line, char* keyvalue, unsigned int valen)   
{   
    unsigned int start, end;   
    int len;   

    for (start = 0; !isgraph(line[start]) && start < strlen(line); start++) {   /* 定位到第一个可打印字符 */
    }   

    if ( start >= strlen(line) )   
        return 0;   

    for (end = start; line[end] != ' ' && line[end] != '\t' && end < strlen(line); end++) {
    }   

    if ( valen > end - start ) {   
        memcpy(keyvalue, &line[start], end - start);   
        keyvalue[end - start] = '\0';   
        len = end - start;   
    } else {   
        memcpy(keyvalue, &line[start], valen - 1);   
        keyvalue[valen - 1] = '\0';   
        len = valen - 1;   
    }   

    if ( end >= strlen(line) ) {
        return len;   
    } else {   
        if ( isjoint(&line[end]) ) {   
            return -len;
        }
        return len;
    }
}   



int getconfigstr(const char* section,   
                   const char* keyname,   
                   char* keyvalue,   
                   unsigned int len,   
                   const char* filename)   
{   
    int step = 0;   
    int rs = 0;   
    int ra = 0;   
    FILE* stream;   
    char line[LINE_LEN];   
    char sec[LINE_LEN];   
    char ken[LINE_LEN];   
    char kev[LINE_LEN];   

    if( (stream = fopen(filename, "r") ) == NULL )   
        return 0x2;   

    while ( !feof(stream) ) { 
        if ( readline(line, 100, stream) == NULL ) { 
            fclose(stream); 
            return 0x3;   
        } 

        if ( !isremark(line) ) {   
            if ( step == 0 ) {   
                if ( getsection(line, sec, LINE_LEN) ) {   
                    if ( strcmp(sec, section) == 0 )   
                        step = 1;   
                }   
            } else if (step == 1) {   
                if ( getkeyname(line, ken, LINE_LEN) ) {   
                    if ( strcmp(ken, keyname) == 0 )   {   
                        rs = getkeyvalue(line, kev, LINE_LEN);   
                        if ( rs > 0 ) {   
                            strncpy(keyvalue, kev, len);   
                            fclose(stream);   
                            return   0x0;   
                        } else if ( rs < 0 ) {   
                            step = 2;   
                            rs = abs(rs);   
                        } else {   
                            fclose(stream);   
                            return 0x4;   
                        } 
                    } 
                } 
            } else {   
                ra = getkeyadd(line, kev + rs, LINE_LEN - rs); 
                if ( ra > 0 ) { 
                    strncpy(keyvalue, kev, len); 
                    fclose(stream); 
                    return   0x0; 
                } else if ( ra < 0 ) { 
                    rs += abs(ra);   
                } else {   
                    fclose(stream);   
                    return 0x4;   
                }   
            }   
        }   
    }   

    fclose(stream);       
    return 0x5;   

}   


int getconfigint(const char* section,   
                   const char* keyname,   
                   int* keyvalue,   
                   const char* filename)   
{   
    int rs;   
    char kev[12];   

    memset(kev, 0, 12);   
    rs = getconfigstr(section, keyname, kev, 12, filename);   
    if ( rs == 0 ) {  
        *keyvalue = atoi(kev);
    }
    return rs; 
}

