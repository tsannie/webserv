/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_header.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dodjian <dovdjianpro@gmail.com>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/11 18:24:00 by user42            #+#    #+#             */
/*   Updated: 2022/01/17 17:43:24 by dodjian          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSE_HEADER_HPP
# define PARSE_HEADER_HPP

#include "../Config/Server.hpp"

class Server;

class Parse_header
{
	public:
	    Parse_header();
		Parse_header( Parse_header const & src );
		~Parse_header();


		Parse_header &		operator=( Parse_header const & rhs );

		void	display_content_header();
		void	fill_variables();

		int		buff_is_valid(char *buff, char *line);
		int		parse_first_line();

		std::vector<std::string> all_header;


		std::string get_method() const {return _method;};
		std::string get_path() const {return _path;};
		std::string get_protocol() const {return _protocol;};

		size_t	get_nbr_line() const {return this->_nbr_line;};
		size_t	get_request_status() const {return this->_request_status;};

		void	incr_nbr_line(){this->_nbr_line++;};

	private:
		void	fill_elements(int pos, std::string str);
		std::string _buffer;

		size_t	_request_status;

		size_t _nbr_line;

		std::string _method;
		std::string _path;
		std::string	_protocol;

		// request header

		std::string _host;
		std::string _user_agent;

		std::string _accept;
		std::string _accept_language;
		std::string _accept_encoding;
		std::string _method_charset;

		//general headers

		std::string _keep_alive;
		std::string _connection;
		std::string _upgrade_insecure_requests;

		// representation header

		std::string _content_type;
		std::string	_content_length;

		std::string _pragma;
		std::string _cache_control;

	public :
		std::string get_method(){return _method;};
		std::string get_path(){return _path;};
		std::string	get_protocol(){return _protocol;};

		// request header

		std::string get_host(){return _host;};
		std::string get_user_agent(){return _user_agent;};

		std::string get_accept(){return _accept;};
		std::string get_accept_language(){return _accept_language;};
		std::string get_accept_encoding(){return _accept_encoding;};
		std::string get_method_charset(){return _method_charset;};

		//general headers

		std::string get_keep_alive(){return _keep_alive;};
		std::string get_connection(){return _connection;};
		std::string get_upgrade_insecure_requests(){return _upgrade_insecure_requests;};

		// representation header

		std::string get_content_type(){return _content_type;};
		std::string	get_content_length(){return _content_length;};

		std::string get_pragma(){return _pragma;};
		std::string get_cache_control(){return _cache_control;};

};


/////////////////////////////////////////////////////////

#endif


