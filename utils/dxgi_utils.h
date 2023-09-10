/*
 * Author: XueLong Ba
 * Date: 2023-09-05
 * Description: 
 */

#pragma once

#include <Windows.h>
#include "../capture-output-priv.h"

// 定义一个函数类型
typedef void* (*GetFrameL)(CaptureOutputL *captureOutput, BYTE*, int, int, int, int, int*, int);

void WCHAR_SPLIT(WCHAR *s, WCHAR *f);
void WCHAR_ARRAY_COPY(WCHAR *f, WCHAR *t, int size);

