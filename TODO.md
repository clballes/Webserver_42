- [ ] upload\_files directive to store the upload file , check upload file
- [ ] chunked requests (es el EOF tambe si)

## mpuig-ma

- [ ] filtrar valid (allowed) methods
- [ ] location \*.php 
- [ ] timeout

- [ ] tests
- [ ] set defaults
- [ ] directory control ( . .. )
- [x] /test != /test/ != /testy != /test/other
- [x] Location /test/ == Location /test
- [x] compare listen directives, so there are no duplicate Connections
- [ ] compare server blocks so there are no repetitions
- [x] required directives: listen, root
- [ ] required directives: automate
- [ ] server\_name listening to localhost should match server listening to 0.0.0.0 ?
- [x] target: ending `/' does not imply file or directory; remove for simplification

## CLARA

- [ ] REDIRECTION HTTP - mirar q getroute li passi be els params
- [ ] mirar lu de www/ checkindex
- [ ] fer un cgi super simple amb getenv
- [ ] method o cgi -> compose\_response -> register\_send -> request\_send
- [ ] cgi\_param insert in the env
