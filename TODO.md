- [ ] upload\_files directive to store the upload file , check upload file
- [ ] chunked requests (es el EOF tambe si)

## mpuig-ma

- [ ] CGI: not calling request\_send()
- [ ] timeout
- [ ] tests
- [ ] set defaults
- [ ] directory control ( . .. )
- [ ] required directives: automate
- [ ] server\_name listening to localhost should match server listening to 0.0.0.0 ?
- [ ] graceful\_stop
- [ ] POST, large request, expect: 100 + content-length--
- [ ] compare server blocks so there are no repetitions
- [ ] leaks
- [x] filtrar valid (allowed) methods;
- [x] rename to limit\_except
- [x] location \*.php 
- [x] check exec permissions pre-execve()
- [x] /test != /test/ != /testy != /test/other
- [x] Location /test/ == Location /test
- [x] compare listen directives, so there are no duplicate Connections
- [x] required directives: listen, root

## CLARA

- [ ] REDIRECTION HTTP - mirar q getroute li passi be els params
- [ ] mirar lu de www/ checkindex
- [ ] fer un cgi super simple amb getenv
- [ ] method o cgi -> compose\_response -> register\_send -> request\_send
- [ ] cgi\_param insert in the env
