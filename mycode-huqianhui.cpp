#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>

char command_str[256]; //存储输入的命今串
char param_list[20][256];//储存分析好的命令串

int ReadCommand(void);
int ParseCommand(void);
int ExcuteCommand(void);
void pwd(void);
void list(void);
void modifydir(void);
int makedir(void);
void deldir(void);
void renameFile(void);
void copyFile(void);
void findFile(char* dir_name, char* file_name);

int ReadCommand(void)//读取命令
{
	char c;
	int i=0;
	while (scanf("%c",&c)!=EOF && c!='\n'&&i<256)//从输入串中逐个读取字符，当遇到结尾或换行或读取的字符已达到字符串的总长度时结束
	{
		command_str[i]=c;
		i++;
	}
	command_str[i]=0;//字符串后加“0”表示结束
	return 0;
}

int ParseCommand(void)//对输入的命令进行解析
{
	unsigned int i=0,j=0,n=0;
	int blank_flag=1;//初始化为前面有分割符，为0时表示前面没有分割符
	for (i=0;i<strlen(command_str);i++) //依次扫描命令串的每个字符
	{
		if (command_str[i]==' '|| command_str[i]=='\t')//分割符出现
		{
			if (blank_flag==0) //此字符前面的字符不是分隔符
			{
				param_list[n][j]=0;//在此字符后加上“0”表示字符结束
				n++;
				j=0;
				blank_flag=1;
			}
		}
		else //字符不为结束符
		{
			param_list[n][j]=command_str[i];//把字符储存起来
			j++;
			blank_flag=0;
		}
	}
	if(blank_flag==1)		//当输入的字符串最后一个字符为分制符时会出现的特殊情况
		param_list[n][0]=0;//标识param_list的结束
	else
	{
		param_list[n][j]=0;
		n++;
		param_list[n][0]=0;
	} 
	return 1;
}

int ExcuteCommand(void)//执行命令
{
	if(strcmp("pwd",param_list[0])==0)  //如果是pwd
	{
		pwd();
		return 0;
	}
	if(strcmp("ls",param_list[0])==0)  //如果是list
	{
		list();
		return 0;
	}
	if(strcmp("modifydir",param_list[0])==0)  //如果是modifydir
	{
		modifydir();
		return 0;
	}
	if(strcmp("makedir",param_list[0])==0)  //如果是makedir
	{
		makedir();
		return 0;
	}
	if(strcmp("deldir",param_list[0])==0)  //如果是deldir
	{
		deldir();
		return 0;
	}
	if(strcmp("rename",param_list[0])==0)  //如果是rename
	{
		renameFile();
		return 0;
	}
	if(strcmp("copy",param_list[0])==0)  //如果是copy
	{
		copyFile();
		return 0;
	}
	if(strcmp("find",param_list[0])==0)  //如果是copy
	{
		findFile(param_list[1], param_list[3]);
		return 0;
	}
	if(strcmp("exit",param_list[0])==0)	//如果是exit
		exit(0);
 } 

void pwd(void)//显示当前所在目录的路径名
{
	char path[256];
	if (getcwd(path, sizeof(path)) != NULL) 
	{
		printf("%s\n", path);
	} 
	else 
	{
		printf("获取目录失败!\n");
	}
}

void list(void)//列出指定目录名中的所有目录及文件
{
	DIR*dirp;
	struct dirent *ent;
	if (strcmp("",param_list[1])==0)//命令不带参数
		dirp=opendir(".");//打开当前工作目录
	else
		dirp=opendir(param_list[1]);//打开目录
	if (dirp==NULL) //如果打开目录失败
	{
		printf("打开目录失败!\n");
		return;
	}
	while((ent =readdir(dirp))!=NULL)	//依次读取日录的信息
	{
		if (ent->d_name[0]!='.')
			printf("%s\t",ent->d_name);//%s表示数据输出类型为字符串
	}
	printf("\n");
	closedir(dirp);//关闭目录
}

void modifydir(void)//改变当前工作目录
{
	if(strcmp("",param_list[1])==0)
		return;
	if (chdir(param_list[1])==-1) //改变目录失败
	{
		printf("目录不存在!\n");
	}
	else
		printf("改变目录成功！\n"); 
}

int makedir(void)//新建目录
{
	if (strcmp("", param_list[1]) == 0) {
		printf("请输入要创建的目录名!\n");
		return -1;
	}
	if (mkdir(param_list[1], 0777) == -1) {
		printf("创建目录失败！\n");
		return -1;
	}
	else
		printf("创建目录成功！\n"); 
}

void deldir(void)//删除目录
{
	if(strcmp("",param_list[1])==0)		//命令不带参数 
	{
		printf("请输入要删除的目录名！\n");
		return;
	}
	if (rmdir(param_list[1])==-1) //删除目录失败
	{
		printf("删除目录失败!\n");
	}
	else
		printf("删除目录成功！\n"); 
}

void renameFile(void)//重命名文件
{
    if(strcmp("",param_list[1])==0 || strcmp("",param_list[2])==0)
    {
        printf("请输入原文件名和新文件名！\n");
        return;
    }
    if(rename(param_list[1],param_list[2])!=0)
    {
        printf("重命名失败!\n");
    }
	else
		printf("重命名成功！\n"); 
}

void copyFile(void) {	// 复制文件
    const char* source_file = param_list[1];
    const char* dest_file = param_list[2];
    FILE* src = fopen(source_file, "rb");    // 打开源文件和目标文件
    if (!src) {
        printf("错误：源文件不存在！\n");
        return;
    }
    FILE* dest = fopen(dest_file, "wb");
    if (!dest) {
        printf("错误：无法创建目标文件！\n");
        fclose(src);
        return;
    }
    char buffer[1024];
    size_t bytes_read;
    while ((bytes_read = fread(buffer, 1, sizeof(buffer), src)) > 0) {
        fwrite(buffer, 1, bytes_read, dest);
    }
    fclose(src);
    fclose(dest);
    printf("文件复制成功！\n");
}


void findFile(char* dir_name, char* file_name) {
    DIR* dir;
    struct dirent* entry;
    struct stat file_info;
    dir = opendir(dir_name);
    if (dir == NULL) {
        return;
    }
    while ((entry = readdir(dir)) != NULL) {
        sprintf(command_str, "%s/%s", dir_name, entry->d_name);
        if (stat(command_str, &file_info) == -1) {
            continue;
        }
        if (S_ISDIR(file_info.st_mode)) {
            if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
                continue;
            }
            findFile(command_str, file_name);
        } else if (strcmp(entry->d_name, file_name) == 0) {
            printf("%s/%s\n", dir_name, entry->d_name);
        }   
    }
    closedir(dir);
}

int main(int argc,char*argv[] )
{
	printf("*****************************************************************************************************************\n");
	printf("*******************                      在Linux环境下模拟实现命令解释器                      *******************\n");
	printf("*******************                                 21计科2班                                 *******************\n");
	printf("*******************                               202110089053                                *******************\n");
	printf("*******************                                 huqianhui                                 *******************\n");
	printf("*****************************************************************************************************************\n");
	printf("* 欢迎使用简单命令解释器!                                                                                       *\n");
	printf("* 支持命令有:                                                                                                   *\n");
	printf("* 1. pwd---显示当前所在目录的路径名                                                                             *\n");
	printf("* 2. ls <目录名>---列出指定目录名中的所有目录及文件                                                             *\n");
	printf("* 3. modifydir <目录名或路径>---改变当前工作目录                                                                *\n");
	printf("* 4. makedir <目录名>---新建目录                                                                                *\n");
	printf("* 5. deldir <目录名>---删除目录                                                                                 *\n");
	printf("* 6. exit---退出命令解释程序                                                                                    *\n");
	printf("* 7. rename <旧文件名> <新文件名>---重命名一个文件或目录                                                        *\n");
	printf("* 8. copy <已存在的文件名> <副本文件名或路径>---复制一个已存在的文件                                            *\n");
	printf("* 9. find <目录> -name <待查找的文件名> ---在指定的目录及其子目录中查找指定的文件，并输出查找到的文件的绝对路径 *\n");
	printf("*****************************************************************************************************************\n");
	while (1)
	{
		printf("huqianhui@");
		ReadCommand();
		ParseCommand();
		ExcuteCommand();
	}
	return 0;
}
