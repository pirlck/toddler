#ifndef _SERV_H_
#define _SERV_H_

#include "serv_utils.h"


//线程的状态值
enum{WORKER_INITED, WORKER_RUNNING,WORKER_DETACHING, WORKER_DETACHED,WORKER_IDEL};


//配置选项
struct conf_opts{
	char CGIRoot[128];			//CGI跟路径
	char default_file[128];		//默认文件名称
	char DocumentRoot[128];		//根文件路径
	char config_file[128];		//配置文件路径和名称
	int  listen_port;			//监听端口
	int  max_client;			//最大客户端数量
	int  time_out;				//超时时间
	int  init_client;			//初始化线程数量
};


//HTTP协议的方法 
typedef enum SHTTPD_METHOD_TYPE{
	METHOD_GET, 		//GET    方法
	METHOD_POST, 		//POST   方法
	METHOD_PUT, 		//PUT    方法
	METHOD_DELETE, 		//DELETE 方法
	METHOD_HEAD,		//HEAD   方法
	METHOD_CGI,			//CGI    方法
	METHOD_NOTSUPPORT
}SHTTPD_METHOD_TYPE;

enum {HDR_DATE, HDR_INT, HDR_STRING};	// HTTP header types

//请求方法
typedef struct shttpd_method{
	SHTTPD_METHOD_TYPE type;
	int name_index;
	
}shttpd_method;


//用于解析请求
typedef struct vec 
{
	char	*ptr;
	int			len;
	SHTTPD_METHOD_TYPE type;
}vec;


#define big_int_t long

//定义http头部结构
struct http_header {
	int		len;		// Header name length   
	int		type;		// Header type			
	size_t		offset;		// Value placeholder
	char	*name;		// Header name			
};

// 用于解析http
// 定义偏移量
#define	OFFSET(x)	offsetof(struct headers, x)

//
//用来存放解析的http结构
union variant {
	char		*v_str;
	int		v_int;
	big_int_t	v_big_int;
	time_t		v_time;
	void		(*v_func)(void);
	void		*v_void;
	struct vec	v_vec;
};


#define	URI_MAX		16384		// Default max request size	


//thanks 博客园网友@老司机
//http头部数据结构
struct headers {
	union variant	cl;		// Content-Length:		
	union variant	ct;		// Content-Type:		
	union variant	connection;	// Connection:			
	union variant	ims;		// If-Modified-Since:		
	union variant	user;		// Remote user name		
	union variant	auth;		// Authorization		
	union variant	useragent;	// User-Agent:			
	union variant	referer;	// Referer:			
	union variant	cookie;		// Cookie:			
	union variant	location;	// Location:			
	union variant	range;		// Range:			
	union variant	status;		// Status:			
	union variant	transenc;	// Transfer-Encoding:		
};

struct cgi{
	int iscgi;
	struct vec bin;
	struct vec para;	
};

struct worker_ctl;
struct worker_opts{
	pthread_t th;				//线程的ID号
	int flags;					//线程状态
	pthread_mutex_t mutex;		//线程任务互斥

	struct worker_ctl *work;	//本线程的总控结构
};

struct worker_conn ;

//请求结构
struct conn_request{
	struct vec	req;		//请求向量
	char *head;				//请求头部\0'结尾
	char *uri;				//请求URI,'\0'结尾
	char rpath[URI_MAX];	//请求文件的真实地址\0'结尾

	int 	method;			//请求类型

	//HTTP的版本信息
	unsigned long major;		//主版本
	unsigned long minor;		//副版本

	struct headers ch;			//头部结构

	struct worker_conn *conn;	//连接结构指针
	int err;
};

// 响应结构 
struct conn_response{
	struct vec	res;		//响应向量
	time_t	birth_time;		//建立时间
	time_t	expire_time;	//超时时间

	int		status;			//响应状态值
	int		cl;				//响应内容长度

	int 		fd;			//请求文件描述符
	struct stat fsate;		//请求文件状态

	struct worker_conn *conn;	//连接结构指针	
};

struct worker_conn 
{
#define K 512
	char		dreq[16*K];	//请求缓冲区
	char		dres[16*K];	//响应缓冲区

	int		cs;			//客户端套接字文件描述符
	int		to;			//客户端无响应时间超时退出时间

	struct conn_response con_res;
	struct conn_request con_req;

	struct worker_ctl *work;	//本线程的总控结构
};

struct task_ctl{
	struct task_opts opts;
	struct task_conn conn;
};

struct mine_type{
	char	*extension;
	int 			type;
	int			ext_len;
	char	*mime_type;
};

//请求类型
struct mine_type* Mine_Type(char *uri, int len, struct worker_ctl *wctl);

void init_paraments(int argc, char *argv[]);

int  resquest_parse(struct worker_ctl *wctl);

int  request_handle(struct worker_ctl* wctl);

int   run_schedule();

int   stop_schedule();

void  get_method(struct task_ctl *tctl);

void  uri_parse(char *src, int len);



#endif //_SERV_H_

