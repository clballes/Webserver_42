#include <iostream>
#include <cstring>
#include <cstdlib>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <dirent.h>

// Function to execute CGI script
void executeCGI(const std::string& scriptPath, const std::string& fullPath, const std::string& query) {
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

void set_env()
{
    std::map<std::string, std::string>	_env;
}


// Function to handle HTTP requests

// POST /your/path/to/resource HTTP/1.1
// Host: your-server.com
// Content-Type: application/x-www-form-urlencoded
// Content-Length: 25
// param1=value1&param2=value2

void handleRequest(const std::string& request) {
    // Parse request to get script path, full path, and query

    std::string scriptPath = "/path/to/your/cgi-handler.cgi";
    std::string fullPath = "/path/to/your/script.php";
    std::string query = "param1=value1&param2=value2";
    set_env();
    // Execute the CGI script
    executeCGI(scriptPath, fullPath, query);
}

int main() {
    std::string request;
    handleRequest(request);
    return 0;
}
