#include "CgiHandler.hpp"

CgiHandler::CgiHandler()
{
    init_env();
}

CgiHandler::~CgiHandler()
{

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
    this->_env["SCRIPT_FILENAME"] = get.path();
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