# WEBSERV

42cursus - (2021-2022) 💻

- Result : 125/100 ✅

![screen](https://i.imgur.com/v9L7au8.png)

## 📝 Description

This project is a web server like [nginx](https://nginx.org/en/). It is written in C++ and use [epoll](https://en.wikipedia.org/wiki/Epoll) for asynchronous I/O. This project tries to implement the most common HTTP methods and headers in [HTTP/1.1](https://www.rfc-editor.org/rfc/rfc2616). It also implements a CGI (Common Gateway Interface) to execute scripts in a child process.

## 📌 Features

- GET, POST, DELETE methods
- CGI
- Cookies
- File upload
- Listening on multiple ports
- Config file

### 📝 Config file options

| Field                | Description                                                       | in location block | default value |
| -------------------- | ----------------------------------------------------------------- | ----------------- | ------------- |
| server_name          | The name of the server block                                      | ❌                | needed        |
| listen               | The port to listen on                                             | ❌                | 4242          |
| location             | Define a location block for a particular URI                      | ❌                | none          |
| root                 | The root directory of the website                                 | ✅                | "./www"       |
| index                | The index file                                                    | ✅                | "index.html"  |
| error_page           | Define a custom error page for a status code                      | ✅                | default page  |
| client_max_body_size | The maximum size of a request body                                | ✅                | 1M            |
| cgi                  | When enabled, it forwards requests to a CGI script for processing | ✅                | none          |
| autoindex            | When enabled, it displays a directory listing                     | ✅                | false         |
| accepted_methods     | Define the accepted HTTP methods for a location block             | ✅                | none          |

If a field is not set in location block, it will be set for all the location blocks that don't have this field set.

### 📝 Config file example

```nginx
server {
	server_name server1;
	listen 4280;

	autoindex on;
	root /home/xubuntu/Desktop/webserv/www;

	accepted_methods	GET;
  error_page 400 403 404 405 /error4xx.html;


	location / {
		cgi .php /usr/bin/php-cgi;
		accepted_methods	POST;
		index index.html;
	}

	location /noindex {
		autoindex off;
		index not_exist.html;
	}

	location /multi_cgi {
		cgi .py /usr/bin/python3.7;
		cgi .php /usr/bin/php-cgi;
		root /home/xubuntu/Desktop/webserv/www/test;
		autoindex on;
	}

	location /delete_loc {
		root /home/xubuntu/Desktop/webserv/www/del;
		accepted_methods	DELETE;
	}
}
```

## 🚀 Usage

```bash
git clone https://github.com/philippebarradas/webserv && cd webserv
make

./webserv <path_to_config_file>
```

## 📚 Resources

https://www.youtube.com/watch?v=s3o5tixMFho\
https://devarea.com/linux-io-multiplexing-select-vs-poll-vs-epoll/ // select vs poll vs epoll\
https://www.csd.uoc.gr/~hy556/material/tutorials/cs556-3rd-tutorial.pdf // sockets\
\
https://webdevdesigner.com/q/what-s-the-difference-between-epoll-poll-treatpool-11439/ // epoll poll/select\
https://dev.to/frevib/a-tcp-server-with-kqueue-527 // epoll\
https://www.digitalocean.com/community/tutorials/understanding-nginx-server-and-location-block-selection-algorithms-fr // block nginx\
https://www.tecmint.com/limit-file-upload-size-in-nginx/ //client_max_body_size

## 👤 Author

- [@tsannie](https://github.com/tsannie)
- [@philippebarradas](https://github.com/philippebarradas)
- [@dovdjian](https://github.com/dovdjian)
