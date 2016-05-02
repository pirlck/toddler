//
// ��epoll�����򵥵ķ�תһ��
//
#ifndef _SERV_EPOLL_H_
#define _SERV_EPOLL_H_

#include "serv_utils.h"

//Events*  ep_events;
#define OK 0
#define TIMEOUT  -1
#define MAXEVESZ 256


typedef struct epoll_event Events;

//����epoll
int  as_epoll_create(int flag);

//����ļ�������
void as_epoll_add(int epfd,int fs,Events* event);

//
void as_epoll_mod(int epfd,int fs,Events* event);

//�ȴ��¼�����
void as_epoll_wait(int epfd,int fs,Events* event);

//���ļ��������Ӽ�����ɾ��
void as_epoll_del(int epfd,int fs,Events* event);

#endif