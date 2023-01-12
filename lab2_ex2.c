#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <stdio.h>
#include <unistd.h>

int main(int argc, char* argv[])
{
	char buf[4096];
	size_t read_size;
	int fd1 = open(argv[1], O_RDONLY);
	if (fd1 < 0)
	{
		perror(NULL);
		return errno;
	}
	
	int fd2 = open(argv[2], O_WRONLY | O_CREAT, 0666);
	if (fd2<0)
	{
		perror(NULL);
		return errno;
	}
	
	while(read_size = read(fd1, buf, sizeof(buf)))
	{
		if(read_size < 0)
		{
			perror(NULL);
			return errno;
		}
		
		if(write(fd2,buf,read_size) < 0)
		{
			perror(NULL);
			return errno;
		}
	}
	
	if(close(fd1) <0)
	{
		perror(NULL);
		return errno;
	}
	
	if(close(fd2) <0)
	{
		perror(NULL);
		return errno;
	}
	
	return 0;
}