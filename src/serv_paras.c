//
//
//
#include "serv_utils.h"





static char *l_opt_arg;
static char *shortopts = "c:d:f:ho:l";
/*
struct option{
     char *name;
     int has_arg;
     int *flag;
     int val;
};
*/
static struct option longopts[] = {
	{"CGIRoot", 		required_argument, NULL, 'c'},
	{"config_file", 		required_argument, NULL, 'f'},	
	{"default_file", 	required_argument, NULL, 'd'},	
	{"DocumentRoot",	required_argument, NULL, 'o'},
	{"listen_port", 		required_argument, NULL, 'l'},
	{"max_client", 		required_argument, NULL, 'm'},	
	{"time_out",		required_argument, NULL, 't'},
	{"Help",			no_argument, NULL, 'h'},
	{0, 0, 0, 0},
};


extern struct conf_opts conf_para;
static void display_usage(void)
{
	printf("sHTTPD -l number  -m number      -o path    -c  path   -d  filename   -t seconds  -o filename\n");
	printf("sHTTPD --listen_port number\n");
	printf("       --max_client number\n");
	printf("       --DocumentRoot) path\n");
	printf("       --default_file) filename\n");
	printf("       --CGIRoot path \n");
	printf("       --default_file filename\n");
	printf("       --time_out seconds\n");
	printf("       --config_file filename\n");
}

static void display_para()
{
	printf("sHTTPD listen_port: %d\n",conf_para.listen_port);
	printf("       max_client: %d\n", conf_para.max_client);
	printf("       DocumentRoot: %s\n",conf_para.DocumentRoot);
	printf("       default_file:%s\n",conf_para.default_file);
	printf("       CGIRoot:%s \n",conf_para.CGIRoot);
	printf("       default_file:%s\n",conf_para.default_file);
	printf("       time_out:%d\n",conf_para.time_out);
	printf("       config_file:%s\n",conf_para.config_file);
}

static int conf_readline(int fd, char *buff, int len)
{
	int n = -1;
	int i = 0;
	int begin = 0;

	/*清缓冲区*/
	memset(buff, 0, len);
	for(i =0; i<len;begin?i++:i)/*当开头部分不为'\r'或者'\n'时i计数*/
	{
		n = read(fd, buff+i, 1);/*读一个字符*/
		if(n == 0)/*文件末尾*/{
			*(buff+i) = '\0';
			break;
		}else if(*(buff+i) == '\r' ||*(buff+i) == '\n'){/*回车换行*/
			if(begin){/*为一行*/
				*(buff+i) = '\0';	
				break;
			}
		}else{
			begin = 1;
		}
	}

	return i;
}


static int Para_CmdParse(int argc, char *argv[])
{
	int c;
	int len;
	int value;

	//遍历输入参数,设置配置参数
	while ((c = getopt_long (argc, argv, shortopts, longopts, NULL)) != -1)
	{
		switch (c)
		{
			case 'c': 	//CGI根路径
				l_opt_arg = optarg;
				if(l_opt_arg && l_opt_arg[0]!=':'){
					len = strlen(l_opt_arg);
					memcpy(conf_para.CGIRoot, l_opt_arg, len +1);
				}
				
				break;
			case 'd': 	//默认文件名称
				l_opt_arg = optarg;
				if(l_opt_arg && l_opt_arg[0]!=':'){
					len = strlen(l_opt_arg);
					memcpy(conf_para.default_file, l_opt_arg, len +1);
				}
				
				break;
			case 'f': 	//配置文件名称和路径
				l_opt_arg = optarg;
				if(l_opt_arg && l_opt_arg[0]!=':'){
					len = strlen(l_opt_arg);
					memcpy(conf_para.config_file, l_opt_arg, len +1);
				}
				
				break;
			case 'o': //根文件路径
				l_opt_arg = optarg;
				if(l_opt_arg && l_opt_arg[0]!=':'){
					len = strlen(l_opt_arg);
					memcpy(conf_para.DocumentRoot, l_opt_arg, len +1);
				}
				
				break;
			case 'l': 	//侦听端口
				l_opt_arg = optarg;
				if(l_opt_arg && l_opt_arg[0]!=':'){
					len = strlen(l_opt_arg);
					value = strtol(l_opt_arg, NULL, 10);
					if(value != LONG_MAX && value != LONG_MIN)
						conf_para.listen_port = value;
				}
				
				break;
			case 'm': //最大客户端数量
				l_opt_arg = optarg;
				if(l_opt_arg && l_opt_arg[0]!=':'){
					len = strlen(l_opt_arg);
					value = strtol(l_opt_arg, NULL, 10);
					if(value != LONG_MAX && value != LONG_MIN)
						conf_para.max_client= value;
				}				
				
				break;
			case 't': 	//超时时间
				l_opt_arg = optarg;
				if(l_opt_arg && l_opt_arg[0]!=':'){
					printf("TIMEOUT\n");
					len = strlen(l_opt_arg);
					value = strtol(l_opt_arg, NULL, 10);
					if(value != LONG_MAX && value != LONG_MIN)
						conf_para.time_out = value;
				}
				
				break;
			case '?'://错误参数
				printf("Invalid para\n");
			case 'h':	//帮助
				display_usage();
				break;
		}
	}
	return 0;
}

//
// 解析配置文件参数
//
void Para_FileParse(char *file)
{
#define LINELENGTH 256
	char line[LINELENGTH];
	char *name = NULL, *value = NULL;
	int fd = -1;
	int n = 0;

	fd = open(file, O_RDONLY);
	if(fd == -1)
	{
		goto EXITPara_FileParse;
	}
	
	/*
	*命令格式如下:
	*[#注释|[空格]关键字[空格]=[空格]value]
	*/
	while( (n = conf_readline(fd, line, LINELENGTH)) !=0)
	{
		char *pos = line;	
		/* 跳过一行开头部分的空格 */
		while(isspace(*pos)){
			pos++;
		}
		/*注释?*/
		if(*pos == '#'){
			continue;
		}
		
		/*关键字开始部分*/
		name = pos;
		/*关键字的末尾*/
		while(!isspace(*pos) && *pos != '=')
		{
			pos++;
		}
		*pos = '\0';/*生成关键字字符串*/

		/*value部分前面空格*/
		while(isspace(*pos))
		{
			pos++;
		}
		/*value开始*/
		value = pos;
		/*到结束*/
		while(!isspace(*pos) && *pos != '\r' && *pos != '\n')
		{
			pos++;
		}
		*pos = '\0';/*生成值的字符串*/


		/*根据关键字部分，获得value部分的值*/
		int ivalue;
		/*"CGIRoot","default_file","DocumentRoot","listen_port","max_client","time_out"*/
		if(strncmp("CGIRoot", name, 7))	{/*CGIRoot部分*/
			memcpy(conf_para.CGIRoot, value, strlen(value)+1);
		}else if(strncmp("default_file", name, 11)){/*DefaultFile部分*/
			memcpy(conf_para.default_file, value, strlen(value)+1);
		}else if(strncmp("DocumentRoot", name, 12)){/*DocumentRoot部分*/
			memcpy(conf_para.DocumentRoot, value, strlen(value)+1);
		}else if(strncmp("listen_port", name, 10)){/*ListenPort部分*/
			ivalue = strtol(value, NULL, 10);
			conf_para.listen_port = ivalue;
		}else if(strncmp("max_client", name, 9)){/*MaxClient部分*/
			ivalue = strtol(value, NULL, 10);
			conf_para.max_client = ivalue;
		}else if(strncmp("time_out", name, 7)){/*TimeOut部分*/
			ivalue = strtol(value, NULL, 10);
			conf_para.time_out = ivalue;
		}		
	}
	close(fd);

EXIT:
	return;	
}


void init_paras(int argc, char *argv[])
{
	//解析命令行输入参数
	parse_cmd(argc, argv);

	//解析配置文件配置参数
	if(strlen(conf_para.config_file))
		paese_config_file(conf_para.config_file);

	display_paras();

	//返回配置参数
	return ;
}

