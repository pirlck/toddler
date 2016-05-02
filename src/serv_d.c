// 
// �Ѿ���ɴ����ܣ���������������
// ����CGI������GET����
// �Ż�������ṹ
#include "serv_d.h"
#include "serv_debug.h"

struct conf_opts conf_para={
	/*CGIRoot*/		"/usr/local/var/www/cgi-bin/",
	/*DefaultFile*/	"index.html",
	/*DocumentRoot*/"/usr/local/var/www/",
	/*ConfigFile*/	"/etc/sHTTPd.conf",
	/*ListenPort*/	8080,
	/*MaxClient*/	4,	
	/*TimeOut*/		3,
	/*InitClient*/		2
};

struct vec _shttpd_methods[] = {
	{"GET",		3, METHOD_GET},
	{"POST",		4, METHOD_POST},
	{"PUT",		3, METHOD_PUT},
	{"DELETE",	6, METHOD_DELETE},
	{"HEAD",		4, METHOD_HEAD},
	{NULL,		0}
};

//SIGINT�źŽ�ȡ����
static void sig_int(int num)
{
	Worker_ScheduleStop();

	return;
}


//SIGPIPE�źŽ�ȡ����
static void sig_pipe(int num)
{
	return;
}


//���е�������
int main(int argc, char *argv[])
{
	signal(SIGINT, sig_int);

	inti_paraments(argc,argv);

	int fd = tcp_listen();

	run_schedule(fd);

	return 0;
}
