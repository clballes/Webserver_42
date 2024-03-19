- [ ] excludes ?options from URI
- [ ] proper log system
- [ ] finish getoptions()
- [ ] if not REUSE_SOCKET, loop timer to retry bind
- [ ] send connection: close header
- [ ] encode_url %20 whitespaces, etc
- [ ] allow_methods -> flags (Server)
- [ ] Tests: request headers
- [ ] Clear unused / properly reorder included headers!
- [ ] Implement keepalive_disable, keepalive_request and (maybe) keepalive_timeout
- [x] load file function
- [x] decode_url
- [x] http Server & from Client
- [x] HTTP::http_get: translate target += root
- [x] parse headers
- [x] std::map for headers
- [x] Deal with `keep-alive' clients
- [x] Move Server::kq to IEvent::kq
- [x] Each client has it's own HTTP instance
- [x] Find out why binded addresses keep binded for some seconds after cntrl+D


## CLARA COMENTARIS MARTI
- en el arxiu de HTTP/file.cpp, veruas que he afegit; linea 21: 		http._message_body.append( line + " " ); "el espai, perque quan sortia lerror 404 sortia tot junt"
- en el arxiu HTTP/events.cpp linea 52: 	http._response_headers["content-type"] = "text/html"; //SUPER IMPORTANT PER FER LA RESPONSE del error que es vegi en el format correcte, pero crec que ha petat en algun moment que volia fer lautoindex, despres funcionava b'e, REVISA
- he tret les classes de CGIhandler, he canviat el nom de la carpeta de cgi i h fet un mini script de python
