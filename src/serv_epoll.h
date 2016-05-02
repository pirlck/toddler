//
// 将epoll函数简单的疯转一下
//
#ifndef _SERV_EPOLL_H_
#define _SERV_EPOLL_H_

#include "serv_utils.h"

//Events*  ep_events;
#define OK 0
#define TIMEOUT  -1
#define MAXEVESZ 256


typedef struct epoll_event Events;

//创建epoll
int  as_epoll_create(int flag);

//添加文件描述符
void as_epoll_add(int epfd,int fs,Events* event);

//
void as_epoll_mod(int epfd,int fs,Events* event);

//等待事件触发
void as_epoll_wait(int epfd,int fs,Events* event);

//将文件描述符从集合中删除
void as_epoll_del(int epfd,int fs,Events* event);

#endif