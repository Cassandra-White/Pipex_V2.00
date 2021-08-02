# include <fcntl.h>
# include <unistd.h>
# include <stdlib.h>
# include <stddef.h>
# include <stdarg.h>
# include <wchar.h>
# include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>

typedef struct s_cmd
{
    const char *cmd[6];
    char * const *argv;
	char * const *envp;

}              t_cmd;


/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
//////////////                                   ////////////
//////////////              LIBFT()              ////////////
//////////////                                   ////////////
/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////




static int		words(char const *str, char c)
{
	int i;
	int words;

	words = 0;
	i = 0;
	while (str[i])
	{
		while (str[i] == c && str[i] != '\0')
			i++;
		if (str[i])
			words++;
		while (str[i] != c && str[i] != '\0')
			i++;
	}
	return (words);
}

static char		**memory_giver(char const *str, char c)
{
	char	**res;
	int		letters;
	int		i;
	int		j;

	if ((res = (char **)malloc(sizeof(char*) * (words(str, c) + 1))) == NULL)
		return (NULL);
	i = 0;
	j = 0;
	while (str[i])
	{
		letters = 0;
		while (str[i] == c && str[i])
			i++;
		while (str[i] != c && str[i] != '\0')
		{
			letters++;
			i++;
		}
		if (letters > 0)
			if ((res[j++] = (char *)malloc(sizeof(char) * letters + 1)) == NULL)
				return (NULL);
	}
	res[j] = NULL;
	return (res);
}

char			**ft_split(char const *str, char c)
{
	char	**res;
	int		i;
	int		j;
	int		str_number;
	int		size;

	if (str == NULL)
		return (NULL);
	size = words(str, c);
	res = memory_giver(str, c);
	if (res == NULL)
		return (NULL);
	i = 0;
	str_number = 0;
	while (str_number < size)
	{
		while (str[i] == c && str[i])
			i++;
		j = 0;
		while (str[i] != c && str[i])
			res[str_number][j++] = str[i++];
		res[str_number][j] = '\0';
		str_number++;
	}
	res[str_number] = 0; 
	return (res);
}

static size_t	ft_strlen(const char *s)
{
	size_t cnt;

	cnt = 0;
	while (s[cnt])
		++cnt;
	return (cnt);
}



char	*ft_strjoin(char const *s1, char const *s2)
{
	char	*str;
	char	*res;
	size_t	len;

	if (s1 == NULL || s2 == NULL)
		return (NULL);
	len = ft_strlen(s1) + ft_strlen(s2);
	if ((str = (char *)malloc(len + 1)) == NULL)
		return (NULL);
	res = str;
	while (*s1)
	{
		*str = *s1;
		str++;
		s1++;
	}
	while (*s2)
	{
		*str = *s2;
		str++;
		s2++;
	}
	*str = '\0';
	return (res);
}


/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
//////////////                                   ////////////
//////////////         FT_PARSING()              ////////////
//////////////                                   ////////////
/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////




void ft_error(char *msg)
{
    printf("%s\n", msg);
    exit(1);
}

void check_directory(char *file)
{
    int fd;

    if((fd = open(file, O_DIRECTORY, 0)) != -1)
	{

		perror(file);
		exit(1);
	}
        //ft_error("Error : Directory are invalid !\n");  
    close(fd); 
}

void check_file(char *file)
{
    int fd;

    if((fd = open(file,O_RDONLY, 0 )) <= 0)
	{

		perror(file);
		exit(1);
	}
        //ft_error("Error : file Invalide");    
    close(fd);
}


int ft_parsing(int argc)
{
    int av_fileout;

    av_fileout = argc - 1;
    if(argc < 5)
        ft_error("Error : pas assez d'arguments");
   // check_directory(argv[i]);
    //check_file(argv[i]);
   // check_directory(argv[--i]);
   // check_file(argv[i]);
    return (av_fileout);
}


/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
//////////////                                   ////////////
//////////////         PIPEX_TEST()              ////////////
//////////////                                   ////////////
/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////


void redirect_out(const char *file)
{

    int fd;

    if((fd = open(file, O_RDWR | O_CREAT | O_TRUNC, 0644)) < 0)
    {
		perror(file);
		exit(1);
      //  ft_error("Error : open fail");
       // close(fd);
    }
    dup2(fd, STDOUT_FILENO);
    close(fd);
}

static void cmd_init(const char *cmd, t_cmd *str)
{

    char **chunk;

    chunk = ft_split(cmd, ' ');
    str->cmd[0] = ft_strjoin("/bin/", chunk[0]);
	str->cmd[1] = ft_strjoin("/usr/local/bin/", chunk[0]);
	str->cmd[2] = ft_strjoin("/usr/bin/", chunk[0]);
	str->cmd[3] = ft_strjoin("/usr/sbin/", chunk[0]);
	str->cmd[4] = ft_strjoin("/sbin/", chunk[0]);
	str->cmd[5] = NULL;
	str->argv = (char *const *)chunk;
	str->envp = NULL;
	

}

 void run_cmd(const char *cmd, char *envp[])
 {
	
     int i;
	 int check;
	 t_cmd cmd_arg;
    
     i = 0;
	 
     cmd_init (cmd, &cmd_arg);
	  //printf("JE SUIS NULll\n");
     while(i < 5)
        check = execve( cmd_arg.cmd[i++], cmd_arg.argv, (char * const *)envp);
	if(check == -1)
		{

			
			int z;
			z = 0;
			while(cmd_arg.cmd[z] != NULL)
			{
				free((void *)cmd_arg.cmd[z]);
				z++;
			}
			int k;
			k = 0;
			while(cmd_arg.argv[k] != NULL)
			{
				free(cmd_arg.argv[k]);
				k++;
			}
			free((void *)cmd_arg.argv);
			write(2, "command not found\n", 18);
			exit(127);
			//printf("(void *)cmd_arg.argv[0]  =  (%s)\n", (void *)cmd_arg.argv[0]);
			// free((void *)cmd_arg.argv);
			//ft_error("ERROR: FREE");
		}
		 

     //perror(cmd_arg.argv[0]);
 }





void connect_pipe(int tubefd[2], int io)
{	
    dup2(tubefd[io], io);
    close(tubefd[0]);
    close(tubefd[1]);
}


void redirect_in(const char *filein)
{
    int fd;

    if((fd = open(filein, O_RDONLY)) < 0)
    {
		perror(filein);
		exit(1);
		//ft_error("Error : redirect_in - FD fail");
    }
    dup2(fd, STDIN_FILENO);
    close(fd);
}



/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
//////////////                                   ////////////
//////////////              MAIN()               ////////////
//////////////                                   ////////////
/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////


int main(int argc, char *argv[], char *envp[])
{
    int fileout;
	int filein;
	int status;
	
	//t_cmd cmd;
	// int i = 0;
	// while(envp[i++] != NULL)
	// 	printf("%s\n\n", envp[i]);
    fileout = ft_parsing(argc);
	filein = 1;
    //pipex_test(argv[1], argv[fileout], argv);
    // redirect_in(argv[1]);
    // redirect_out(argv[fileout]);

	int tubefd[2];
    pid_t pid;


    pipe(tubefd);
    pid = fork();
    if(pid == -1)
		ft_error("Error : Pid -1\n");
    else if(pid > 0)
    {
	
         waitpid(pid, &status, 0);
        if(WIFEXITED(status) == 0)
            ft_error("Error");
         else 
         {

			kill(pid, STDOUT_FILENO);
            

			redirect_out(argv[4]);
            connect_pipe(tubefd, STDIN_FILENO);
			run_cmd(argv[3], envp);
		 }
    }
    else 
    {
       	redirect_in(argv[1]);
        connect_pipe(tubefd, STDOUT_FILENO);
		run_cmd(argv[2], envp);
    }

    return (0);
}