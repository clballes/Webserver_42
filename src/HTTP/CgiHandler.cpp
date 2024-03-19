/*                                                                            */
/* CgiHandler.cpp                                                             */
/* mpuig-ma <mpuig-ma@student.42barcelona.com>                                */
/* Mon Mar  4 15:43:13 2024                                                   */


#include "HTTP.hpp"
#include "ft_stdlib.h"

#define READ 0
#define WRITE 1

char **    HTTP::init_env(std::string method, std::string target)
{
    // char **envp = NULL;

    char** env = new char*[4];
    env[0] = ft_strdup(("REQUEST_METHOD=" + method).c_str());
    env[1] = ft_strdup(("PATH_INFO=" + target).c_str());
    env[2] = ft_strdup("SERVER_PROTOCOL=HTTP/1.1");
    env[3] = nullptr;
    return env;

    //NO TENIM FETA LA FUNCIO DE REALLOC, no va aplicarho a la libft
    //ft_setenv("REQUEST_METHOD", method.c_str(), 1, &envp);
    //ft_setenv("PATH_INFO", target.c_str(), 1, &envp);
    //ft_setenv("SERVER_PROTOCOL", "HTTP/1.1", 1, &envp);
}

// Function to execute CGI script
void HTTP::executeCGI(const std::string& scriptPath, char **env)
{
    LOG("Execute CGI")

    int pipefd[2];

    if (pipe(pipefd) == -1) {
        std::cerr << "Error creating pipe: " << strerror(errno) << std::endl;
        exit(EXIT_FAILURE);
    }

    int fdopen = open("/Users/clballes/Desktop/web42/www/400.html", O_RDONLY);
    if(fdopen == -1){
        std::cerr << "ERROR OPENING THE FILE IN CGI" << std::endl;
    }

    pid_t pid = fork();

    if (pid == 0)
    {
        // Child process
        dup2(fdopen, STDIN_FILENO);
        //dup2(pipefd[WRITE], STDOUT_FILENO);
        close(pipefd[READ]);

        // Execute the CGI script with the specified environment
        execve(scriptPath.c_str(), NULL, env); //el scriptpath estabe aqui es el cgi pass
        perror("execve");
        // If execve fails
        std::cerr << "Error executing CGI script: " << strerror(errno) << std::endl;
        exit(EXIT_FAILURE);
    }
    else if(pid == -1)
    {
        // Fork fail
        std::cerr << "Error creating child process: " << strerror(errno) << std::endl;
        exit(EXIT_FAILURE);
    }

    write(pipefd[WRITE], "hola clara", 11);
    close(pipefd[WRITE]);
    char buff[1024];
   // read(pipefd[READ], &buff, 1024);
    memset(&buff,0,1024);
    std::cout << "el read es: " << read(pipefd[READ], &buff, 1024) << std::endl;
    std::cout << "buff es: "<< buff << std::endl;


    // close(pipefd[READ]);
    waitpid(pid, NULL, 0);
}

