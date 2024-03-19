/*                                                                            */
/* CgiHandler.cpp                                                             */
/* mpuig-ma <mpuig-ma@student.42barcelona.com>                                */
/* Tue Feb 27 14:32:20 2024                                                   */

#include "CgiHandler.hpp"
#define READ 0
#define WRITE 1

CgiHandler::CgiHandler( HTTP request)
{
    LOG("Cgi Handler called")

    // std::string scriptPath = "/Users/clballes/Desktop/web42/src/Cgi/ubuntu_cgi_tester";
    std::string scriptPath = "/Users/clballes/Desktop/web42/src/Cgi/cgi_tester"; // fastcgi_pass instructs nginx to execute a CGI script (PHP in your case) using the FastCGI protocol.
    std::string fullPath =  "/Users/clballes/Desktop/web42/src/Cgi/cgi_tester"; //normalment es per donar extra info, en el notre cas sera el mateix q el script
    std::string query = "param1=value1&param2=value2"; //cgi_param ?????
    // Execute the CGI script
    char **env = init_env(scriptPath, fullPath, query, request);
    executeCGI(scriptPath,env);
}

CgiHandler::~CgiHandler()
{
    LOG("Cgi Destructor called")
}

// // Function to execute CGI script
// void CgiHandler::executeCGI(const std::string& scriptPath, char **env)
// {
//     LOG("Execute CGI")

//     int pipefd[2];

//     if (pipe(pipefd) == -1) {
//         std::cerr << "Error creating pipe: " << strerror(errno) << std::endl;
//         exit(EXIT_FAILURE);
//     }

//     int fdopen = open("/Users/clballes/Desktop/web42/www/400.html", O_RDONLY);
//     if(fdopen == -1){
//         std::cerr << "ERROR OPENING THE FILE IN CGI" << std::endl;
//     }

//     pid_t pid = fork();

//     if (pid == 0)
//     {
//         // Child process
//         dup2(fdopen, STDIN_FILENO);
//         //dup2(pipefd[WRITE], STDOUT_FILENO);
//         close(pipefd[READ]);

//         // Execute the CGI script with the specified environment
//         execve(scriptPath.c_str(), NULL, env); //el scriptpath estabe aqui es el cgi pass
//         perror("execve");
//         // If execve fails
//         std::cerr << "Error executing CGI script: " << strerror(errno) << std::endl;
//         exit(EXIT_FAILURE);
//     }
//     else if(pid == -1)
//     {
//         // Fork fail
//         std::cerr << "Error creating child process: " << strerror(errno) << std::endl;
//         exit(EXIT_FAILURE);
//     }

//     write(pipefd[WRITE], "hola clara", 11);
//     close(pipefd[WRITE]);
//     char buff[1024];
//    // read(pipefd[READ], &buff, 1024);
//     memset(&buff,0,1024);
//     std::cout << "el read es: " << read(pipefd[READ], &buff, 1024) << std::endl;
//     std::cout << "buff es: "<< buff << std::endl;

//     // close(pipefd[READ]);
//     waitpid(pid, NULL, 0);
// }

// char ** CgiHandler::init_env(const std::string& scriptPath, const std::string& fullPath, const std::string& query, HTTP request)
// {
//     // std::cout << " - -  - - - - - - -  - - - - - - -  - - - - - - - -  - -"  << std::endl;
//     // LOG("Cgi Handler init environment variables")
//     // //   4.1.1.  AUTH_TYPE. . . . . . . . . . . . . . . . . . . .  11
//     // //   4.1.2.  CONTENT_LENGTH . . . . . . . . . . . . . . . . .  12
//     // //   4.1.3.  CONTENT_TYPE . . . . . . . . . . . . . . . . . .  12
//     // //   4.1.4.  GATEWAY_INTERFACE. . . . . . . . . . . . . . . .  13
//     // //   4.1.5.  PATH_INFO. . . . . . . . . . . . . . . . . . . .  13
//     // //   4.1.6.  PATH_TRANSLATED. . . . . . . . . . . . . . . . .  14
//     // //   4.1.7.  QUERY_STRING . . . . . . . . . . . . . . . . . .  15
//     // //   4.1.8.  REMOTE_ADDR. . . . . . . . . . . . . . . . . . .  15
//     // //   4.1.9.  REMOTE_HOST. . . . . . . . . . . . . . . . . . .  16
//     // //   4.1.10. REMOTE_IDENT . . . . . . . . . . . . . . . . . .  16
//     // //   4.1.11. REMOTE_USER. . . . . . . . . . . . . . . . . . .  16
//     // //   4.1.12. REQUEST_METHOD . . . . . . . . . . . . . . . . .  17
//     // //   4.1.13. SCRIPT_NAME. . . . . . . . . . . . . . . . . . .  17
//     // //   4.1.14. SERVER_NAME. . . . . . . . . . . . . . . . . . .  17
//     // //   4.1.15. SERVER_PORT. . . . . . . . . . . . . . . . . . .  18
//     // //   4.1.16. SERVER_PROTOCOL. . . . . . . . . . . . . . . . .  18
//     // //   4.1.17. SERVER_SOFTWARE. . . . . . . . . . . . . . . . .  19
//     // std::string target;
// 	// target.append( request._server._root );
// 	// target.append( request._request.target );

//     // this->_env["AUTH_TYPE"] = "";
//     // this->_env["CONTENT_LENGTH"] = request._headers["content-length"];
//     // this->_env["CONTENT_TYPE"] = "";
//     // this->_env["GATEAWAY_INTERFACE"] = "CGI/1.1";
//     // this->_env["PATH_INFO"] = target;
//     // this->_env["PATH_TRANSLATED"] = target;
//     // this->_env["QUERY_STRING"] = "";
//     // this->_env["REMOTE_ADDR"] = "";
//     // this->_env["REMOTE_HOST"] = "";
//     // this->_env["REMOTE_IDENT"] = request._headers["Authorization"];
//     // this->_env["REMOTE_USER"] = request._headers["Authorization"];
//     // this->_env["REQUEST_METHOD"] = request._request.method->method;
//     // this->_env["SCRIPT_NAME"] = request._server._cgi_pass;
//     // this->_env["SERVER_NAME"] = "";
//     // this->_env["SERVER_PORT"] = "";
//     // this->_env["SERVER_PROTOCOL"] = "HTTP/1.1";
//     // this->_env["SERVER_SOFTWARE"] = "";
    
//     // std::cout << "request method: " << request._request.method->method << std::endl;
//     // std::cout << "request method: " << request._request.http_version << std::endl;
//     // std::cout << "request authorization: " << request._headers["Authorization"] << std::endl;
//     // std::cout << "request len: " <<  request._response_headers["content-length"] << std::endl;
//     // std::cout << "request type: " <<  request._response_headers["content-type"] << std::endl;
//     // std::cout << "request cgipass: " <<  request._server._cgi_pass << std::endl;
//     // std::cout << " - -  - - - - - - -  - - - - - - -  - - - - - - - -  - -"  << std::endl;
//     // (void)fullPath;
//     // (void)scriptPath;
//     // (void)query;
//     // char** env = new char*[4];
//     // env[0] = strdup("REQUEST_METHOD=GET");
//     // env[1] = strdup(("PATH_INFO=" + target).c_str());
//     // env[2] = strdup("SERVER_PROTOCOL=HTTP/1.1");
//     // // env[1] = strdup(("QUERY_STRING=" + query).c_str());
//     // // env[2] = strdup(("SCRIPT_FILENAME=" + scriptPath).c_str());
//     // // env[2] = strdup(("SCRIPT_NAME=" + scriptPath).c_str());
//     // // env[3] = strdup("GATEWAY_INTERFACE=CGI/1.1");
//     // env[3] = nullptr;
//     // return env;
// }
