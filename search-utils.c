#include "search-utils.h"
#include <ctype.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

int hash_string(char *str){
    int ret = 0;
    for (int i = 0; *(str+i) != '\0'; ++i){
        ret += (int) *(str+i);
    }
    return ret;
}

int count_char(char *str, char ch){
    int i, count;
    count = 0;
    for (i = 0; *(str+i) != '\0'; ++i){
        if (ch == *(str+i)){
            ++count;
        }
    }
    return count;
}

int strip(char *str, char *ret){
    int left;
    for (left = 0; *(str+left) == ' '; ++left){
        ;
    }
    int x;
    int right = 0;
    int l = len(str);
    for (x = l-1; *(str+x) == ' '; --x){
        ++right;
    }
    //printf("Left->%d,Right->%d\n",left,right);
    int i, j;
    for (i = left, j = 0; i < l-right; ++i, ++j){
        *(ret+j) = *(str+i);
    }
    *(ret+j) = '\0';
    return 0;
}

int in(char s, char *li){
    int c;
    for (c = 0; *(li+c) != '\0'; ++c){
        if (s == *(li+c)){
            return 1;
        }
    }
    return 0;
}

int* in_where(int *l, int x, int length){
    int i, j;
    j = 0;
    int* loc = (int*) malloc(1000*sizeof(int));
    for (i = 0; i < length; ++i){
        if (l[i] == x){
            loc[j] = i;
            ++j;
        }
        else{
            continue;
        }
    }
    if (j == 0){
        loc[0] = -1;
        return loc;
    }
    loc[j] = -1;
    return loc;
}

int swap(float **x, int i, int j){
    //Swaps two elements of 2D array
    //printf("SWAP START!\n");
    float temp1 = x[i][0];
    float temp2 = x[i][1];
    x[i][0] = x[j][0];
    x[i][1] = x[j][0];
    x[j][0] = temp1;
    x[j][1] = temp2;
    //printf("SWAP SUCCESS!\n");
    return 0;
}

int in_str_list(char *str, char **l, int len_l){
    for (int i = 0; i < len_l; ++i){
        if (str_equal(str, l[i])){
            return 1;
        }
        else{
            continue;
        }
    }
    return 0;
}

int bubble2dsort(float** unsorted, int l){
    //Sorts based on the 2nd axis
    int n = l;
    int i, j;
    //printf("the length of the array that is being sorted is %d\n",n);
    for (i = 0; i < n-1; ++i){
        for (j = 0; j < n-i-1; ++j){
            //printf("i = %d, j = %d\n",i,j);
            if (unsorted[j][1] > unsorted[j+1][1]){
                swap(unsorted, j, j+1);
            }
        }
    }
    //printf("SORT SUCCESS!\n");
    return 0;
}

int str_equal(char *s1, char *s2){
    int i = len(s1);
    int j = len(s2);
    if (i != j){
        return 0;
    }
    else{
        for (int x = 0; x < i; ++x){
            if (s1[x] != s2[x]){
                return 0;
            }
        }
    }
    return 1;
}

int sanitize(char *str, char *ret){
    int c, i;
    c = i = 0;
    while (*(str+c) != '\0'){
        //printf("%d\n",c);
        if ((*(str+c) <= 'Z' && *(str+c) >= 'A') || (*(str+c) <= 'z' && *(str+c) >= 'a')){
            //printf("character!\n");
            *(ret+i) = *(str+c);
            ++i;
        }
        else if (in(*(str+c),"áÁàÀâÂäÄãÃåÅæÆ")){
            *(ret+i) = 'a';
            ++i;
        }
        else if (in(*(str+c),"çÇ")){
            *(ret+i) = 'c';
            ++i;
        }
        else if (in(*(str+c),"éÉèÈêÊëË")){
            *(ret+i) = 'e';
            ++i;
        }
        else if (in(*(str+c),"íÍìÌîÎïÏ")){
            *(ret+i) = 'i';
            ++i;
        }
        else if (in(*(str+c),"ñÑ")){
            *(ret+i) = 'n';
            ++i;
        }
        else if (in(*(str+c),"óÓòÒôÔöÖõÕøØœŒ")){
            *(ret+i) = 'o';
            ++i;
        }
        else if (in(*(str+c),"úÚùÙûÛüÜ")){
            *(ret+i) = 'u';
            ++i;
        }
        else if (*(str+c) >= '0' && *(str+c) <= '9'){
            //printf("Number!\n");
            *(ret+i) = *(str+c);
            ++i;
        }
        ++c;
    }
    *(ret+i) = '\0';
    lower(ret,ret);
    return 0;
}

int copy_str(char *str, char *copy){
    int i;
    for (i = 0; *(str+i) != '\0'; ++i){
        *(copy+i) = *(str+i);
    }
    *(copy+i) = '\0';
    return 0;
}

int strong_sanitize(char *str, char *ret){
    int c, i;
    c = i = 0;
    while (*(str+c) != '\0'){
        //printf("%d\n",c);
        if ((*(str+c) <= 'Z' && *(str+c) >= 'A') || (*(str+c) <= 'z' && *(str+c) >= 'a')){
            //printf("character!\n");
            *(ret+i) = *(str+c);
            ++i;
        }
        else if (in(*(str+c),"áÁàÀâÂäÄãÃåÅæÆ")){
            *(ret+i) = 'a';
            ++i;
        }
        else if (in(*(str+c),"çÇ")){
            *(ret+i) = 'c';
            ++i;
        }
        else if (in(*(str+c),"éÉèÈêÊëË")){
            *(ret+i) = 'e';
            ++i;
        }
        else if (in(*(str+c),"íÍìÌîÎïÏ")){
            *(ret+i) = 'i';
            ++i;
        }
        else if (in(*(str+c),"ñÑ")){
            *(ret+i) = 'n';
            ++i;
        }
        else if (in(*(str+c),"óÓòÒôÔöÖõÕøØœŒ")){
            *(ret+i) = 'o';
            ++i;
        }
        else if (in(*(str+c),"úÚùÙûÛüÜ")){
            *(ret+i) = 'u';
            ++i;
        }
        ++c;
    }
    *(ret+i) = '\0';
    lower(ret,ret);
    return 0;
}

int lower(char *str, char *ret){
    int c;
    for (c = 0; *(str+c) != '\0'; ++c){
        *(ret+c) = tolower(*(str+c));
    }
    *(ret+c) = '\0';
    return 0;
}

int len(char *str){
    int c;
    for (c = 0; *(str+c) != '\0'; ++c){
        ;
    }
    return c;
}

int str_split(char *str, char delim, char** l){
    int i, j, k;
    j = k = 0;
    for (i = 0; i < len(str); ++i){
        if (*(str+i) == delim){
            l[k][j] = '\0';
            ++k;
            j = 0;
        }
        else{
            l[k][j] = str[i];
            ++j;
        }
    }
    return k+1;
}