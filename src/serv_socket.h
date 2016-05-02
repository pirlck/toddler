#ifndef __SERV_SOCKET__
#define __SERV_SOCKET__

// 对基本的套接字相关函数封装
// 便于调用

#include "serv_utils.h"
#include "serv_debug.h"


int creat_sockfd(unsigned short port)
{
	// start a socket
	int servfd;
	struct sockaddr_in servaddr;
	socklen_t socklen;
	
	memset(&servaddr,0,sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port = htons(port);

	servfd = socket(PF_INET,SOCK_STREAM,0);
	if(servfd == -1)
	{
		log_err("creaete sock error!");
		exit(-1);
	}

    socklen = sizeof(servaddr);
    int status;

    if((status = bind(servfd,(struct sockaddr*)&servaddr,socklen)) < 0 )
        return -1;

    //set fd non blocking
    if( (status = listen(servfd,5) ) == -1)
        return -1;
    log_info("create servfd! success !");
    return servfd;
}

//设置套接字的属性
int set_socket_nonblock(int sock_fd)
{
    int ctl,status;
    //cmd:get such kind control
    ctl = fcntl(sock_fd,F_GETFL);
    if(ctl == -1)
    {
        log_err("GET file CTL failed !");
        return -1;
    }
    ctl |= O_NONBLOCK;
    status = fcntl(sock_fd,ctl);
    if(status == -1)
    {
        log_err("SET file CTL failed !");
        return -1;
    }

    return 0;
}

int tcp_listen()
{
	struct sockaddr_in server;
	int ss = -1;
	int err = -1;
	int reuse = 1;
	int ret = -1;

	// 初始化服务器地址
	memset(&server, 0, sizeof(server));
	server.sin_family = AF_INET;
	server.sin_addr.s_addr=htonl(INADDR_ANY);
	server.sin_port = htons(conf_para.ListenPort);   
	
	//信号截取函数 
	signal(SIGINT,  sig_int);
	signal(SIGPIPE, sig_pipe);

	// 生成套接字文件描述符 
	ss = socket (AF_INET, SOCK_STREAM, 0);
	if (ss == -1)
	{
		log_err("socket() error\n");
		ret = -1;
		goto EXITshttpd_listen;
	}

	// 设置套接字地址和端口复用
	err = setsockopt(ss, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse));
	if (err == -1)
	{
		log_err("setsockopt SO_REUSEADDR failed\n");
	}

	// 绑定IP和套接字描述符
	err = bind (ss, (struct sockaddr*)  &server, sizeof(server));
	if (err == -1)
	{
		log_err("bind() error\n");
		ret = -2;
		goto EXITshttpd_listen;
	}

	// 设置服务器侦听队列长度 
	err = listen(ss, conf_para.MaxClient*2);
	if (err)
	{
		log_err ("listen() error\n");
		ret = -3;
		goto EXITshttpd_listen;
	}

	ret = ss;
EXITshttpd_listen:
	return ret;
}


#endif

