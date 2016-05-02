#ifndef _SERV_H_
#define _SERV_H_

#include "serv_utils.h"


//�̵߳�״ֵ̬
enum{WORKER_INITED, WORKER_RUNNING,WORKER_DETACHING, WORKER_DETACHED,WORKER_IDEL};


//����ѡ��
struct conf_opts{
	char CGIRoot[128];			//CGI��·��
	char default_file[128];		//Ĭ���ļ�����
	char DocumentRoot[128];		//���ļ�·��
	char config_file[128];		//�����ļ�·��������
	int  listen_port;			//�����˿�
	int  max_client;			//���ͻ�������
	int  time_out;				//��ʱʱ��
	int  init_client;			//��ʼ���߳�����
};


//HTTPЭ��ķ��� 
typedef enum SHTTPD_METHOD_TYPE{
	METHOD_GET, 		//GET    ����
	METHOD_POST, 		//POST   ����
	METHOD_PUT, 		//PUT    ����
	METHOD_DELETE, 		//DELETE ����
	METHOD_HEAD,		//HEAD   ����
	METHOD_CGI,			//CGI    ����
	METHOD_NOTSUPPORT
}SHTTPD_METHOD_TYPE;

enum {HDR_DATE, HDR_INT, HDR_STRING};	// HTTP header types

//���󷽷�
typedef struct shttpd_method{
	SHTTPD_METHOD_TYPE type;
	int name_index;
	
}shttpd_method;


//���ڽ�������
typedef struct vec 
{
	char	*ptr;
	int			len;
	SHTTPD_METHOD_TYPE type;
}vec;


#define big_int_t long

//����httpͷ���ṹ
struct http_header {
	int		len;		// Header name length   
	int		type;		// Header type			
	size_t		offset;		// Value placeholder
	char	*name;		// Header name			
};

// ���ڽ���http
// ����ƫ����
#define	OFFSET(x)	offsetof(struct headers, x)

//
//������Ž�����http�ṹ
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


//thanks ����԰����@��˾��
//httpͷ�����ݽṹ
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
	pthread_t th;				//�̵߳�ID��
	int flags;					//�߳�״̬
	pthread_mutex_t mutex;		//�߳����񻥳�

	struct worker_ctl *work;	//���̵߳��ܿؽṹ
};

struct worker_conn ;

//����ṹ
struct conn_request{
	struct vec	req;		//��������
	char *head;				//����ͷ��\0'��β
	char *uri;				//����URI,'\0'��β
	char rpath[URI_MAX];	//�����ļ�����ʵ��ַ\0'��β

	int 	method;			//��������

	//HTTP�İ汾��Ϣ
	unsigned long major;		//���汾
	unsigned long minor;		//���汾

	struct headers ch;			//ͷ���ṹ

	struct worker_conn *conn;	//���ӽṹָ��
	int err;
};

// ��Ӧ�ṹ 
struct conn_response{
	struct vec	res;		//��Ӧ����
	time_t	birth_time;		//����ʱ��
	time_t	expire_time;	//��ʱʱ��

	int		status;			//��Ӧ״ֵ̬
	int		cl;				//��Ӧ���ݳ���

	int 		fd;			//�����ļ�������
	struct stat fsate;		//�����ļ�״̬

	struct worker_conn *conn;	//���ӽṹָ��	
};

struct worker_conn 
{
#define K 512
	char		dreq[16*K];	//���󻺳���
	char		dres[16*K];	//��Ӧ������

	int		cs;			//�ͻ����׽����ļ�������
	int		to;			//�ͻ�������Ӧʱ�䳬ʱ�˳�ʱ��

	struct conn_response con_res;
	struct conn_request con_req;

	struct worker_ctl *work;	//���̵߳��ܿؽṹ
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

//��������
struct mine_type* Mine_Type(char *uri, int len, struct worker_ctl *wctl);

void init_paraments(int argc, char *argv[]);

int  resquest_parse(struct worker_ctl *wctl);

int  request_handle(struct worker_ctl* wctl);

int   run_schedule();

int   stop_schedule();

void  get_method(struct task_ctl *tctl);

void  uri_parse(char *src, int len);



#endif //_SERV_H_

