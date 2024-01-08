# Configuration File's Structure

<!-- Adapted from NGINX's beginners guide -->

webserv is controlled by directives specified in the configuration file.
There are simple directives and block directives. 

## directives

A simple directive consists of the name and parameters separated by spaces and ends with a semicolor (;).

A block directive has the same structure as a simple directive, but instead of the semicolon it endss with a set of additional instructions surrounded by braces.

## comments

The rest of a line after the # sign is considered a comment.

## server

*server* blocks are distinguished by ports on which they listen to and by server names.

Once webserv decides which *server* processes a request, it tests the URI specified in the request's header against the parameters of the *location* directives defined inside the *server* block.

### location

If there are several matching *location* blocks, nginx selects the one with the longest prefix. 

Note that the *root* directive is placed in the *server* context. Such *root* directive is used when the *location* block selected for serving a request does not include its own *root* directive.
