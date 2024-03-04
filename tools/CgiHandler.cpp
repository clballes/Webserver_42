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
    this->_env["REQUEST_METHOD"];
    this->_env["QUERY_STRING"]; 
    this->_env["SCRIPT_FILENAME"]; 
    this->_env["PATH_INFO"];
    this->_env["SERVER_SOFTWARE"];
    this->_env["SERVER_NAME"];
    this->_env["GATEWAY_INTERFACE"];
    this->_env["SERVER_PROTOCOL"];
    this->_env["SERVER_PORT"];
    this->_env["REQUEST_METHOD"];
    this->_env["PATH_TRANSLATED"];
    this->_env["REMOTE_HOST"];
    this->_env["REMOTE_ADDR"];
    this->_env["SCRIPT_NAME"];
    this->_env["AUTH_TYPE"];
    this->_env["REMOTE_USER"];
    this->_env["CONTENT_TYPE"];
    this->_env["CONTENT_LENGTH"];
}