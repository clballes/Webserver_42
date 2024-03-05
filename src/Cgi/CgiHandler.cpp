/*                                                                            */
/* CgiHandler.cpp                                                             */
/* mpuig-ma <mpuig-ma@student.42barcelona.com>                                */
/* Tue Feb 27 14:32:20 2024                                                   */

#include "CgiHandler.hpp"

int main() {
    CgiHandler Cgi;
    // rest of the main function
    return 0;
}

CgiHandler::CgiHandler()
{
    LOG("Cgi Handler called")

    // std::string scriptPath = "/Users/clballes/Desktop/web42/src/Cgi/ubuntu_cgi_tester";
    std::string scriptPath = "/Users/clballes/Desktop/web42/src/Cgi/cgi_tester"; // fastcgi_pass instructs nginx to execute a CGI script (PHP in your case) using the FastCGI protocol.
    std::string fullPath =  "/Users/clballes/Desktop/web42/src/Cgi/cgi_tester"; //normalment es per donar extra info, en el notre cas sera el mateix q el script
    std::string query = "param1=value1&param2=value2"; //cgi_parak
    // Execute the CGI script
    char **env = init_env(scriptPath, fullPath, query);
    executeCGI(scriptPath,env);
}

CgiHandler::~CgiHandler()
{
    LOG("Cgi Destructor called")
}

// Function to execute CGI script
void CgiHandler::executeCGI(const std::string& scriptPath, char **env)
{
    int pipefd[2];
    if (pipe(pipefd) == -1) {
        std::cerr << "Error creating pipe: " << strerror(errno) << std::endl;
        exit(EXIT_FAILURE);
    }

    pid_t pid = fork();

    if (pid == 0)
    { // Child process
        close(pipefd[0]); // Close unused read end
        // Redirect stdout to the pipe
        dup2(pipefd[1], STDOUT_FILENO);

        // Execute the CGI script with the specified environment
        execve(scriptPath.c_str(), NULL, env); //el scriptpath estabe aqui es el cgi pass
        perror("execve");
        // If execve fails

        std::cerr << "Error executing CGI script: " << strerror(errno) << std::endl;
        exit(EXIT_FAILURE);
    } else if (pid > 0) { // Parent process
        close(pipefd[1]);

        // Read and print the output from the CGI script
        char buffer[BUFSIZ];
        ssize_t bytesRead;
        while ((bytesRead = read(pipefd[0], buffer, BUFSIZ)) > 0) {
            write(STDOUT_FILENO, buffer, bytesRead);
        }
        waitpid(pid, NULL, 0);

        close(pipefd[0]);
    } else { // Fork fail
        std::cerr << "Error creating child process: " << strerror(errno) << std::endl;
        exit(EXIT_FAILURE);
    }
}

char ** CgiHandler::init_env(const std::string& scriptPath, const std::string& fullPath, const std::string& query)
{
    char** env = new char*[6];
    std::string add = "HTTP/1.1";
    // std::string add2 = "CGI/1.1";

    env[0] = strdup("REQUEST_METHOD=GET");
    env[1] = strdup(("QUERY_STRING=" + query).c_str());
    env[2] = strdup(("SCRIPT_FILENAME=" + scriptPath).c_str());
    // env[2] = strdup(("SCRIPT_NAME=" + scriptPath).c_str());
    // env[3] = strdup(("PATH_INFO=" + fullPath).c_str());
    env[4] = strdup(("SERVER_PROTOCOL=" + add).c_str());
    // env[4] = strdup(("GATEWAY_INTERFACE=" + add2).c_str());
    env[5] = nullptr;
    return env;



// this->_env["REDIRECT_STATUS"] = "200"; //Security needed to execute php-cgi
// 	this->_env["GATEWAY_INTERFACE"] = "CGI/1.1";

// 	this->_env["SCRIPT_NAME"] = config.getPath();
// 	this->_env["SCRIPT_FILENAME"] = config.getPath();

// 	this->_env["PATH_INFO"] = request.getPath(); //might need some change, using config path/contentLocation
// 	this->_env["PATH_TRANSLATED"] = request.getPath(); //might need some change, using config path/contentLocation
// 	this->_env["REQUEST_METHOD"] = request.getMethod();
// 	this->_env["CONTENT_LENGTH"] = to_string(this->_body.length());
// 	this->_env["CONTENT_TYPE"] = headers["Content-Type"];
// 	this->_env["QUERY_STRING"] = request.getQuery();
// 	this->_env["REMOTEaddr"] = to_string(config.getHostPort().host);
// 	this->_env["REMOTE_IDENT"] = headers["Authorization"];
// 	this->_env["REMOTE_USER"] = headers["Authorization"];
// 	this->_env["REQUEST_URI"] = request.getPath() + request.getQuery();
// 	if (headers.find("Hostname") != headers.end())
// 		this->_env["SERVER_NAME"] = headers["Hostname"];
// 	else
    // this->_env["SERVER_SOFTWARE"];
    // this->_env["SERVER_NAME"];
    // this->_env["GATEWAY_INTERFACE"] = "CGI/1.1";

    // this->_env["SCRIPT_FILENAME"] = get.path();
    // this->_env["PATH_INFO"];
    // this->_env["SERVER_PORT"];
    // this->_env["REQUEST_METHOD"] = "GET";
    // this->_env["PATH_TRANSLATED"];
    // this->_env["REMOTE_HOST"];
    // this->_env["REMOTE_ADDR"];
    // this->_env["SCRIPT_NAME"];
    // this->_env["AUTH_TYPE"]; //request authorization
    // this->_env["REMOTE_USER"] = //request authorization
    // this->_env["CONTENT_TYPE"] = //request
    // this->_env["CONTENT_LENGTH"] = "211"; //requeest
}