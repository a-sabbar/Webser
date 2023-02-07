/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   run_server.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asabbar <asabbar@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/05 20:16:36 by asabbar           #+#    #+#             */
/*   Updated: 2023/02/07 16:07:24 by asabbar          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Webserv.hpp"
#include <fcntl.h>
#include <algorithm>

long long int	get_time(void)
{
	struct timeval	current_time;
	long long int	result;

	gettimeofday(&current_time, NULL);
	result = (current_time.tv_sec * 1000) + (current_time.tv_usec / 1000);
	return (result);
}

void myTrim(std::string &s, const std::string &toTrim = " \t\f\v\n\r")
{
	s = s.substr(s.find_first_not_of(toTrim), s.length());
	s = s.substr(0, s.find_last_not_of(toTrim) +1);
}

struct pollfd  Accept_read(std::vector<serv_d> &servers, int i, std::vector<client_d> &addNewFd, int fd)
{
	int addrlen = sizeof(servers.at(i).address);
	struct pollfd temp;
	client_d newFd;

	servers.at(i).is_accept = true;
	temp.fd = accept(fd, (sockaddr *)&servers.at(i).address, (socklen_t *)&addrlen);
	fcntl(temp.fd , F_SETFL, O_NONBLOCK);
	temp.events = POLLIN | POLLOUT | POLLHUP; 
	temp.revents = 0;

	
	newFd.acceptFd = temp.fd;
	newFd.socketFd = fd;
	newFd.lenRead = 0;
	newFd.isAccept = false;
	newFd.endRead = false;
	newFd.sendLen = 0;
	newFd.lastRead = 0;
	newFd.ResponsLength = 0;
	newFd.Con = false;
	newFd.endSend = false;

	addNewFd.push_back(newFd);
	return(temp);
}


int get_method(std::string request)
{
	std::string check = request.substr(0, 3);
	if(!check.compare("POS"))
		return(2);
	if(!check.compare("GET"))
		return(1);
	if(!check.compare("DEL"))
		return(3);
	return(-1);
}


int get_content_len(std::string request)
{
	int start = (int)request.find("Content-Length: ", 0) + (int)strlen("Content-Length: ");
	std::string check = request.substr(start, request.find("\r\n", start));
	return atoi(check.c_str());
}



void clearPollList(std::vector <struct pollfd> &fds, client_d client, std::vector <client_d> &addNewFd)
{
	if(fds.size())
	{
		std::vector<struct pollfd>::iterator it_poll = fds.begin();
		while(it_poll->fd != client.acceptFd && it_poll != fds.end())
			it_poll++;
		if(it_poll->fd == client.acceptFd)
			fds.erase(it_poll);
	}
	if(addNewFd.size())
	{
		std::vector <client_d>::iterator it_client = addNewFd.begin();
		while(it_client->acceptFd != client.acceptFd && it_client != addNewFd.end() )
			it_client++;
		if(it_client->acceptFd == client.acceptFd)
			addNewFd.erase(it_client);
	}
}


std::string get_path(std::string request)
{
	std::string firstline = request.substr(0, request.find_first_of("\r\n",0));
	
	int firstspace = firstline.find_first_of(" ",0) + 1;
	
	
	int secondspace = firstline.find_last_of(" ", firstline.length());

	return firstline.substr(firstspace, secondspace - firstspace);
};


void    run_server(std::vector<serv_d> &serv_data)
{
	int option = 1;
	std::vector<serv_d> servers;
    std::vector<serv_d> ::iterator it = serv_data.begin();

    for (; it < serv_data.end(); it++)
    {
    	it->address.sin_family = AF_INET;
		it->address.sin_port = htons(atoi(it->listen.c_str()));
		it->address.sin_addr.s_addr = htonl(0);
		it->sock = socket(AF_INET, SOCK_STREAM, 0);
		if(it->sock < 0)
		{
			perror("KO :(\nFailed to socket");
			continue ;
		}
	    setsockopt(it->sock, SOL_SOCKET, SO_REUSEADDR, &option, sizeof(1));
    }
    for (it = serv_data.begin(); it < serv_data.end(); it++)
	{
    	it->connection = bind(it->sock, (struct sockaddr *)&it->address, sizeof(it->address));
		if(it->connection < 0)
		{
			perror("KO :(\nFailed to bind");
			continue ;
		}
	}
    for (it = serv_data.begin(); it < serv_data.end(); it++)
	{
    	it->listening = listen(it->sock,it->backlog);
		if(it->listening < 0)
		{
			perror("KO :(\nFailed to listen");
			continue ;
		}
	}
	std::vector <int> serverSocket;
    for (it = serv_data.begin(); it < serv_data.end(); it++)
	{
		if(it->listening != -1 && it->connection != -1 && it->sock != -1 )
		{
			fcntl(it->sock, F_SETFL, O_NONBLOCK);
			servers.push_back(*it);
			serverSocket.push_back(it->sock);
		}
	}
	std::vector <struct pollfd> fds;
	std::vector <client_d> addNewFd;
	int nfds = 0;
    for (it = servers.begin(); it < servers.end(); it++)
	{
		struct pollfd temp;
		temp.fd = it->sock;
		temp.events = POLLIN | POLLOUT | POLLHUP; 
		temp.revents = 0;
		nfds++;
		fds.push_back(temp);
	}
	std::cout << "============== WAITING ==============\n";
	while(1)
	{
		int ret = poll(&fds[0], fds.size(), -1);
		if (ret == -1)
		{
			std::cout << "error :" << std::endl;
			break;
		}
		else if (ret > 0)
		{
			for (size_t i = 0; i < fds.size(); i++)
			{
				if (fds.at(i).revents & POLLIN)
				{
					if (find(serverSocket.begin(), serverSocket.end(), fds.at(i).fd) != serverSocket.end()) 
					{
						std::cout << "============== ACCEPT ==============\n";
						fds.push_back(Accept_read(servers, i, addNewFd, fds.at(i).fd));
					}
					std::vector<client_d> ::iterator it_c = addNewFd.begin();
					for ( ;it_c != addNewFd.end() && it_c->acceptFd != fds.at(i).fd; ){
						it_c++;
					}
					if (it_c == addNewFd.end()) {
						continue;
					}
					bzero(it_c->buffer, sizeof(it_c->buffer));
					it_c->lastRead =  get_time();
					int rec =  recv(fds.at(i).fd, it_c->buffer, sizeof(it_c->buffer), 0);
					if(rec < 1)
					{
						std::cout <<"hello bye"<<std::endl;
						close(fds.at(i).fd);
						clearPollList(fds, *it_c, addNewFd);
						break ;
					}
					it_c->request.append(it_c->buffer, rec);
					it_c->lenRead += rec;
					int len = get_content_len(it_c->request);
					if(it_c->lenRead >= (unsigned long)len )
					{
						it_c->endRead = true;
						for (it = servers.begin(); it->sock != it_c->socketFd; it++);
						// std::cout << it_c->request<< "\n";
						HandleRequest h(*it_c, *it);	
					}
				}
				else if (fds.at(i).revents & POLLOUT) {
					std::vector<client_d> ::iterator it_c = addNewFd.begin();
					for ( ;it_c != addNewFd.end() && it_c->acceptFd != fds.at(i).fd; ){
						it_c++;
					}
					if (it_c == addNewFd.end()) {
						continue;
					}
					if(it_c->endRead)
					{
						int sizeRead = 6000000;
						if((ssize_t)it_c->Respons.length() - it_c->sendLen < sizeRead)
							sizeRead = (ssize_t)it_c->Respons.length() - it_c->sendLen;
						it_c->sendLen += send(fds.at(i).fd, it_c->Respons.substr(it_c->sendLen).c_str(), sizeRead, 0);
						if(it_c->sendLen >= (ssize_t)it_c->Respons.length())
						{
							if(!it_c->Con)
							{
								close(fds.at(i).fd);
								clearPollList(fds, *it_c, addNewFd);
								std::cout << fds.size()<<" ============== CLOSE ==============\n";
							}
							else
							{
								it_c->lenRead = 0;
								it_c->isAccept = false;
								it_c->endRead = false;
								it_c->sendLen = 0;
								it_c->lastRead = 0;
								it_c->ResponsLength = 0;
								it_c->Con = false;
								it_c->endSend = false;
								it_c->Respons.clear();
								it_c->request.clear();
								std::cout << fds.size()<<" ============== KEEPALIVE ==============\n";
							}
							std::cout << fds.size()<<" ============== DONE ==============\n";
						}
					}
				}
				else if (fds.at(i).revents & POLLHUP)
				{
					std::cout << "TIMEOUUUUT\n" <<" ============== CLOSE ==============\n";
					std::vector<client_d> ::iterator it_c = addNewFd.begin();
					for ( ;it_c != addNewFd.end() && it_c->acceptFd != fds.at(i).fd; ){
						it_c++;
					}
					if (it_c == addNewFd.end()) {
						continue;
					}
					close(fds.at(i).fd);
					clearPollList(fds, *it_c, addNewFd);
				}
				std::vector<client_d> ::iterator it_c = addNewFd.begin();
				for ( ;it_c != addNewFd.end() && it_c->acceptFd != fds.at(i).fd; ){
					it_c++;
				}
				if (it_c == addNewFd.end()) {
					continue;
				}
				if(it_c->lastRead && get_time() - it_c->lastRead > 5000)
				{
					close(fds.at(i).fd);
					clearPollList(fds, *it_c, addNewFd);
					std::cout  << "  TIMEOUUUUT\n" <<" ============== CLOSE ==============\n";
				}
			}
		}
	}
	
}