/*
 * Author: XueLong Ba
 * Date: 2023-09-05
 * Description: 
 */

#include "dxgi_utils.h"
#include <windows.h>



void WCHAR_SPLIT(WCHAR *s, WCHAR *f){

    WCHAR finalDeviceName[32];
    int findFirstDelimiter = 0;
    int findSecondDelimiter = 0;
    for(int i = 0; i <= 31; i++)
    {
        if((findFirstDelimiter == 0) && (s[i] == '\\'))
        {
            findFirstDelimiter = 1;
            continue;
        }else if((findFirstDelimiter == 0) && (s[i] != '\\')){
            continue;
        }else if((findFirstDelimiter == 1) && (s[i] != '\\')){
            findFirstDelimiter = 0;
            continue;
        }

        if((findFirstDelimiter == 1) && (s[i] == '\\'))
        {
            findSecondDelimiter = 1;
            for(int n = i + 1, m = 0; n <= 31; n++,m++){
                f[m] = s[n];
            }
            break;
        }

    }
    
}

void WCHAR_ARRAY_COPY(WCHAR *f, WCHAR *t, int size)
{
    for(int i = 0; i < size; i++){
        t[i] = f[i];
    }
}