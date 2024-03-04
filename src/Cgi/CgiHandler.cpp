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
    init_env();
    // Parse request to get script path, full path, and query

    std::string scriptPath = "/path/to/your/cgi-handler.cgi";
    std::string fullPath = "/path/to/your/script.php";
    std::string query = "param1=value1&param2=value2";
    // Execute the CGI script
    executeCGI(scriptPath, fullPath, query);
}

CgiHandler::~CgiHandler()
{

}

// Function to execute CGI script
void CgiHandler::executeCGI(const std::string& scriptPath, const std::string& fullPath, const std::string& query) {
    // Set environment variables for CGI script
    setenv("REQUEST_METHOD", "GET", 1); // or "POST"
    setenv("QUERY_STRING", query.c_str(), 1);
    setenv("SCRIPT_FILENAME", scriptPath.c_str(), 1);
    setenv("PATH_INFO", fullPath.c_str(), 1);

    // Create a pipe for communication with CGI script
    int pipefd[2];
    if (pipe(pipefd) == -1) {
        std::cerr << "Error creating pipe: " << strerror(errno) << std::endl;
        exit(EXIT_FAILURE);
    }

    pid_t pid = fork();

    if (pid == 0) { // Child process
        close(pipefd[0]); // Close unused read end

        // Redirect stdout to the pipe
        dup2(pipefd[1], STDOUT_FILENO);

        // Execute the CGI script
        execve(scriptPath.c_str(), NULL, NULL);

        // If execve fails
        std::cerr << "Error executing CGI script: " << strerror(errno) << std::endl;
        exit(EXIT_FAILURE);
    } else if (pid > 0) { // Parent process
        close(pipefd[1]); // Close unused write end

        // Read and print the output from the CGI script
        char buffer[BUFSIZ];
        ssize_t bytesRead;
        while ((bytesRead = read(pipefd[0], buffer, BUFSIZ)) > 0) {
            write(STDOUT_FILENO, buffer, bytesRead);
        }

        // Wait for the child process to finish
        waitpid(pid, NULL, 0);

        close(pipefd[0]); // Close read end
    } else { // Fork failed
        std::cerr << "Error creating child process: " << strerror(errno) << std::endl;
        exit(EXIT_FAILURE);
    }
}

void CgiHandler::init_env()
{
    //set for all the requests
    this->_env["SERVER_SOFTWARE"];
    this->_env["SERVER_NAME"];
    this->_env["GATEWAY_INTERFACE"] = "CGI/1.1";

    this->_env["SERVER_PROTOCOL"] = "HTTP/1.1";
    this->_env["REQUEST_METHOD"];
    this->_env["QUERY_STRING"]; 
    // this->_env["SCRIPT_FILENAME"] = get.path();
    this->_env["PATH_INFO"];
    this->_env["SERVER_PORT"];
    this->_env["REQUEST_METHOD"] = "GET";
    this->_env["PATH_TRANSLATED"];
    this->_env["REMOTE_HOST"];
    this->_env["REMOTE_ADDR"];
    this->_env["SCRIPT_NAME"];
    this->_env["AUTH_TYPE"]; //request authorization
    this->_env["REMOTE_USER"] = //request authorization
    this->_env["CONTENT_TYPE"] = //request
    this->_env["CONTENT_LENGTH"] = "211"; //requeest
}