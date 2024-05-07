- [ ] leaks
- [ ] leaks: graceful\_stop SIGQUIT
- [ ] leaks: SIGTERM
- [ ] test: required directives ( Server check() )
- [ ] test: wrong number of arguments ( conf )
- [ ] test: wrong input(s)
- [ ] test: default values
- [ ] test: HTTP methods with wrong directory control ( . .. )

## mpuig-ma

- [ ] compare server blocks so there are no repetitions
- [ ] proper POST
- [ ] large request, expect: 100 + content-length--
- [ ] chunked requests (es el EOF tambe si)

## CLARA

- [ ] REDIRECTION HTTP - mirar q getroute li passi be els paramss
- [ ] REDIRECTION HTTP amb post ont wrking
	//nose si em faltaria parsejar si la ruta existeix de la direccio o ja petara despres
    // Extract the route
    	// PREGUNTA, si no tenim location i fem un get, no hem dafegir res en el root, la ruta es la q es ya oi?

- [ ] si faig un get al root quinns alowed methods tenim x defetcte ?? em posa q no te allow methods perq no en te nose quins hi ha x defautl - 
- [ ] parsing tirar error quan tira en la redireccio
- [ ] cgi_param insert in the env
- [] per handle uload files he de parsejar el body de la request del post, 20:12:45] request_recv [fd=8 bytes=193]
--------------------------f9c4845872258caa\rContent-Disposition: form-data; name="file"; filename="hola.txt"\rContent-Type: text/plain\r\raaddass\r--------------------------f9c4845872258caa--\r
- [ ] upload\_files directive to store the upload file , check upload file
