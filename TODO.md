- [ ] leaks
- [ ] graceful\_stop
- [ ] tests
- [ ] directory control ( . .. )

## mpuig-ma

- [ ] required directives: automate
- [ ] compare server blocks so there are no repetitions
- [ ] set defaults
- [ ] POST, large request, expect: 100 + content-length--
- [ ] chunked requests (es el EOF tambe si)
- [x] fix: CGI GATEWAY\_TIMEOUT
- [x] fix: CGI timer ident=1
- [x] fix: response to cgi timeout (segv)
- [x] fix: limit\_except ( setFlag, getFlag, AUTOINDEX ... )
- [x] timeout
- [x] CGI: not calling request\_send()
- [x] filtrar valid (allowed) methods;
- [x] rename to limit\_except
- [x] location \*.php 
- [x] check exec permissions pre-execve()
- [x] /test != /test/ != /testy != /test/other
- [x] Location /test/ == Location /test
- [x] compare listen directives, so there are no duplicate Connections
- [x] required directives: listen, root

## CLARA

- [ ] upload\_files directive to store the upload file , check upload file
- [ ] REDIRECTION HTTP - mirar q getroute li passi be els params
- [ ] mirar lu de www/ checkindex
- [ ] fer un cgi super simple amb getenv
- [ ] method o cgi -> compose\_response -> register\_send -> request\_send
- [ ] cgi\_param insert in the env
