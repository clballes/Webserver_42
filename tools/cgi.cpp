


// If the request is data intended for an external application, then the server needs to use CGI to run that application. For example, the client's request might be to search a database. The CGI application takes the search criteria, searches the database, then sends the results back to the client.

// When a server receives a request that must be handled by an external application (a CGI request) that server creates a copy of itself.

// create a copy of the server -- cgi process que busca la comunicacio entre el ./cgi i el server
//CGI process knows

        // Which remote host made the request (if your server uses DNS, this is the user's host name; otherwise, this is the user's IP address).
        // Whether that host was authorized with server access control or not.
        // What browser software the remote user is running. HOST PORT

        // TAKE THE DATA AND PUT IT INTO ENV VARIABLES


    // _cgi_pass
    // _cgi_param