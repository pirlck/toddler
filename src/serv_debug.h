#ifndef _DEBUG_FILE__
#define _DEBUG_FILE__

// 对fprintf函数简单的封装
//用于不同级别调试信息的打印

#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>

#ifdef DEBUG
#define debug(format,...) fprintf(stderr,"DEBUG %s:%d: "format" \n",__FILE__,__LINE__,##__VA_ARGS__);
#else
#define debug(format,...)
#endif

#define clean_errno() (errno == 0 ? "None":strerror(errno))

#define log_err(format,...) fprintf(stderr,"[Error] (%s:%d: errno:%s) "format" \n",__FILE__,__LINE__,clean_errno(),##__VA_ARGS__)

#define log_warn(format,...) fprintf(stderr,"[Warning] (%s:%d: warn:%s) "format" \n",__FILE__,__LINE__,clean_errno(),##__VA_ARGS__)

#define log_info(format,...) fprintf(stderr,"[Info] (%s:%d: info:%s) "format" \n",__FILE__,__LINE__,clean_errno(),##__VA_ARGS__)

#endif