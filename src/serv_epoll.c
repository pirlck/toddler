#include "serv_epoll.h"
#include "serv_debug.h"

//创建epoll
int as_epoll_create(int flag)
{
	int epfd = epoll_create1(flag);
	if(epfd <= 0)
	{
		log_err("create epoll err !");
		exit(-1);
	}
	event = (Events*)malloc(MAXEVESZ*sizeof(Events));
	return epfd;
}

//将文件描述符加入监听集合
void as_epoll_add(int epfd,int fd,Events* event)
{
	int status = epoll_ctl(epfd,EPOLL_CTL_ADD,fd,event);
	if(status != OK)
	{
		log_err("add event err !");
		exit(-1);
	}
}

//http://bbs.csdn.net/topics/390808838
//修改已经注册的fd的监听事件；
void as_epoll_mod(int epfd,int fd,Events* event)
{
	int status = epoll_ctl(epfd,EPOLL_CTL_MOD,fd,event);
	if(status != OK)
	{
		log_err("change registered fd !");
	}
}

//删除文件描述符
void as_epoll_del(int epfd,int fd,Events* event)
{
	int status = epoll_ctl(epfd,EPOLL_CTL_DEL,fd,event);
	if(status != OK)
	{
		log_err("delete event err !");
		exit(-1);
	}
}

//epoll 等待时间触发
int as_epoll_wait(int epfd,Events* events,int maxevents ,int timeout )
{
    //MAXEVESZ,TIMEOUT
	int timeout = -1;
	int n = epoll_wait(epfd,events,maxevents,timeout);
	if( n < 0)
	{
		log_err("wait epoll error !\n");
		exit(-1);
	}
	return n;
}

