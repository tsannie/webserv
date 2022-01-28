/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   moteur.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dodjian <dovdjianpro@gmail.com>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/09 16:27:13 by dodjian           #+#    #+#             */
/*   Updated: 2021/12/17 18:58:04dodjian          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "moteur.hpp"
#include "../Treat_request/treat_request.hpp"
#include "../Parse_request/parse_request.hpp"

#include <stdlib.h>

// Creating socket file descriptor
int	Moteur::create_socket()
{
	int listen_fd = 0;
	listen_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (listen_fd == 0)
		throw std::runtime_error("[Error] create_socket() failed");
	return (listen_fd);
}

// Set socket file descriptor to be reusable
void	Moteur::set_socket(int listen_fd)
{
	int opt = 1;
	fcntl(listen_fd, F_SETFL, O_NONBLOCK);
	if (setsockopt(listen_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)))
		throw std::runtime_error("[Error] set_socket() failed");
}

// Put a name to a socket
void	Moteur::bind_socket(int listen_fd, const std::vector<Server> & src)
{
	struct sockaddr_in address;
	int port_config = 0;

	std::istringstream(src[this->i_server].getListen()) >> port_config;
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = INADDR_ANY;
	address.sin_port = htons(port_config);
	std::cout << GREEN << "Port: " << port_config << std::endl << END;
	if (bind(listen_fd, (struct sockaddr *)&address, sizeof(address)) < 0)
		throw std::runtime_error("[Error] Port already attribute");
}

// Make the socket passive, waiting to accept
void	Moteur::listen_socket(int listen_fd)
{
	if (listen(listen_fd, MAX_EVENTS) < 0)
		throw std::runtime_error("[Error] listen_socket() failed");
}

// Accept connexion and return socket accepted
int	Moteur::accept_connexions(int listen_fd)
{
	int new_socket = 0;

	new_socket = accept(listen_fd, NULL, NULL);
	if (new_socket < 0)
		throw std::runtime_error("[Error] accept_connexions() failed");
	return (new_socket);
}

// savoir si le fd dans le epoll est un listener (socket d'un port) ou non
bool	Moteur::is_listener(int fd, int *tab_fd, int nbr_servers, const std::vector<Server> & src)
{
	for (int i = 0; i < nbr_servers; i++)
	{
		if (fd == tab_fd[i])
		{
			std::istringstream(src[i].getListen()) >> this->port;
			return (TRUE);
		}
	}
	return (FALSE);
}

/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/

Moteur::Moteur()
{
}

Moteur::Moteur(const std::vector<Server> & src)
{
	std::cout << BLUE << "----------------- Starting server -----------------" << std::endl << std::endl;
	setup_socket_server(src);
	loop_server(src);
}

/*
** -------------------------------- DESTRUCTOR --------------------------------
*/

Moteur::~Moteur()
{
	std::cout << GREEN << "----------------- End of server -----------------" << END << std::endl << std::endl;
}

/*
** --------------------------------- OVERLOAD ---------------------------------
*/

Moteur&				Moteur::operator=( Moteur const & rhs )
{
	(void)rhs;
	return *this;
}

/*
** --------------------------------- METHODS ----------------------------------
*/

/* cree la socket -> set la socket -> donne un nom a la socket ->
	mets la socket comme passive -> set le premier events fd avec la socket passive */
void	Moteur::setup_socket_server(const std::vector<Server> & src)
{
	this->port = 0;
	this->nbr_servers = src.size();
	this->timeout = 3 * 60 * 1000; // 3 min de timeout (= keepalive nginx ?)
	this->epfd = epoll_create(MAX_EVENTS);
	if (this->epfd < 0)
		throw std::runtime_error("[Error] epoll_create() failed");
	for (this->i_server = 0; this->i_server < this->nbr_servers; this->i_server++)
	{
		this->listen_fd[this->i_server] = create_socket();
		this->fds_events[this->i_server].data.fd = this->listen_fd[this->i_server];
		this->fds_events[this->i_server].events = EPOLLIN;
		if (epoll_ctl(this->epfd, EPOLL_CTL_ADD, this->listen_fd[this->i_server], &fds_events[this->i_server]) == -1)
			throw std::runtime_error("[Error] epoll_ctl_add() failed");
		set_socket(this->listen_fd[this->i_server]);
		bind_socket(this->listen_fd[this->i_server], src);
		listen_socket(this->listen_fd[this->i_server]);
	}
}


void	Moteur::read_send_data(int fd, const std::vector<Server> & src)
{
	Treat_request	treat;
	Parse_request	parse_head;
	std::string		file_body;
	std::string 	buff_send;

	size_t	buff_size = 33000;
	char	buff[buff_size];
	int		valread = -1;
	int		nbr_bytes_send = 0;
	bool	is_valid = true;
	size_t	recv_len = 0;

	bzero(&buff, sizeof(buff));
    while (valread != KEEP && is_valid == true)
	{
		valread = recv(fd, &buff[recv_len], buff_size - recv_len, 0);
		if (valread == STOP)
			throw std::runtime_error("[Error] recv() failed");
		else
			recv_len += valread;
		if (parse_head.buff_is_valid(buff) == 0)	
			epoll_wait(this->epfd, this->fds_events, MAX_EVENTS, this->timeout);
		else 
			is_valid = false;
	}

	std::cout << std::endl << std::endl << std::endl;

	std::cout << std::endl << std::endl << std::endl;

 	if (valread != KEEP)
	{
		//file_body = "<html>"; //treat.is_Treat_request(buff, src, this->port, parse_head);

		buff_send = treat.is_Treat_request(buff, src, this->port, parse_head);

		nbr_bytes_send = send(fd, buff_send.c_str(), buff_send.size(), 0);
		if (nbr_bytes_send == -1)
			throw std::runtime_error("[Error] sent() failed");
		else if (nbr_bytes_send == 0)
			std::cout << "nbr bytes send = 0" << std::endl;
		std::cout << RED << "End of connexion" << END << std::endl << std::endl;
	}
	//if (parse_head.get_request("Status").compare("200") != 0 ||
	//parse_head.get_request("Connection:").find("close") != std::string::npos)
		close(fd);
}

void	Moteur::loop_server(const std::vector<Server> & src)
{
	int nbr_connexions = 0;
	int new_socket = 0;
	int i = 0;

	while (TRUE)
	{
		if ((nbr_connexions = epoll_wait(this->epfd, this->fds_events, MAX_EVENTS, this->timeout)) < 0)
			throw std::runtime_error("[Error] epoll_wait() failed");
		for (i = 0; i < nbr_connexions; i++)
		{
			if (is_listener(this->fds_events[i].data.fd, this->listen_fd, this->nbr_servers, src))
			{
				new_socket = accept_connexions(this->fds_events[i].data.fd);
				fcntl(new_socket, F_SETFL, O_NONBLOCK);
				this->fds_events[i].events = EPOLLIN;
				this->fds_events[i].data.fd = new_socket;
				if (epoll_ctl(this->epfd, EPOLL_CTL_ADD, new_socket, &this->fds_events[i]) == -1)
					throw std::runtime_error("[Error] epoll_ctl_add() failed");
			}
			else
			{
				read_send_data(this->fds_events[i].data.fd, src);
				//break ;
			}
		}
		//send_and_close(this->fds_events[i].data.fd, src);
	}
}

/*
** --------------------------------- ACCESSOR ---------------------------------
*/


/* ************************************************************************** */
