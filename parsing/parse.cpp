/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asabbar <asabbar@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/30 04:40:23 by zait-sli          #+#    #+#             */
/*   Updated: 2023/02/22 10:22:14 by asabbar          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parse.hpp"

std::vector<std::string> split(std::string str, char delimiter)
{
    std::vector<std::string> tokens;
    std::stringstream ss(str);
    std::string token;
    while (std::getline(ss, token, delimiter))
	{
		if(token.compare(""))
        	tokens.push_back(token);
		
	}
    return tokens;
}

int findBracket(const char *str)
{
	for (size_t i = 0; i < strlen(str); i++)
	{
		if(str[i] == '{')
			return(i);
	}
	return(-1);
}

int check_brackets(const char *str)
{

	int i = 0;
	while (str[i])
	{
		if (str[i] == '{')
		{
			if(check_brackets(&str[i + 1]) == -1)
				exit(1);
		}
		if (str[i] == '}')
			return (1);
		i++;
	}
	exit(1);
}

int check_Semicolon(std::string line)
{
	int i = line.find(";");
	if(i == - 1)
		return 0;
	if((int)line.find(";", i + 1) != -1)
		return 0;
	if(line[line.length() - 1] != ';')
		return 0;
	return 1;
}

void parse::checkSantax()
{
	std::vector<std::string> configFile;

	configFile = split(rawFile, '\n');
	for (std::vector<std::string>::iterator i = configFile.begin(); i != configFile.end(); i++)
	{
		myTrim(*i," \t\f\v\n\r");
		if(!(*i).compare("")  || !(*i).compare("\0"))
		{
			configFile.erase(i);
			continue;
		}
	}
	int count = 0;
	for (std::vector<std::string>::iterator i = configFile.begin(); i != configFile.end(); i++)
	{
		myTrim(*i," \t\f\v\n\r");
		if(count == 0)
		{
			int findd = (*i).find("server{");
			if(findd == -1 || (*i).compare("server{"))
				throw ConfigNotValid();
		}
		if(!(*i).compare("server{"))
		{
			if (count != 0)
			{
				std::vector<std::string>::iterator  temp1 = i;
				temp1--;
				std::vector<std::string>::iterator  temp2 = temp1;
				temp2--;
				if((*temp1).compare("}") || (*temp2).compare("}"))
				{
					std::cout<< " -- server -- \n";
					throw ConfigNotValid();
				}
			}
		}
		else if((int)(*i).find("location ") != - 1)
		{
			if(!(split((*i), ' ').at(1)).compare("/"))
			{
				std::vector<std::string>::iterator  temp1 = i;
				temp1++;
				if((*temp1).compare("{"))
					throw ConfigNotValid();
			}
			else
			{
				std::vector<std::string>::iterator  temp1 = i;
				std::vector<std::string>::iterator  temp2 = i;
				temp1++;
				temp2--;
				if((*temp1).compare("{") || (*temp2).compare("}"))
					throw ConfigNotValid();
			}
		}
		else if((int)(*i).find("listen ") != -1 || (int)(*i).find("host ") != -1 || (int)(*i).find("server_name ") != -1
		|| (int)(*i).find("methods ") != -1 || (int)(*i).find("client_max_body_size ") != -1 || (int)(*i).find("root ") != -1
		|| (int)(*i).find("return ") != -1 || (int)(*i).find("root ") != -1 || (int)(*i).find("errorPage ") != -1
		|| (int)(*i).find("cgi_bin ") != -1 || (int)(*i).find("upload_store ") != -1 || (int)(*i).find("indexPath ") != -1 
		|| (int)(*i).find("autoindex ") != -1 )
		{
			if(!check_Semicolon(*i))
			{
				std::cout << "from check_Semicolon" <<"\n";
				throw ConfigNotValid();
				
			}
		}
		else  {
			if((int)(*i).find("}") == - 1 && (int)(*i).find("{") == - 1 )
			{
				std::cout << "undefine attribute\n";
				throw ConfigNotValid();
			}
		}
		std::vector<std::string>::iterator  temp = i + 1;
		if(temp == configFile.end())
		{
			if(i->compare("}"))
			{
				std::cout << "close bracket not found\n";
				throw ConfigNotValid();
			}
			std::vector<std::string>::iterator  temp1 = i  - 1;
			if(temp1->compare("}"))
			{
				std::cout << "close bracket not found\n";
				throw ConfigNotValid();
			}
		}
		count++;
	}
}

void	parse::mergeParser()
{
	std::vector<serv_d>::iterator it = serv.begin();
	for(;it != serv.end(); it++)
	{
		for(std::vector<class Location>::iterator it_location = it->Loc.begin(); it_location != it->Loc.end(); it_location++)
		{
			std::map<std::string,vector<std::string> > tempLocation;
			if(it_location->autoindex.compare("NAN"))
				tempLocation["autoindex"] = split(it_location->autoindex, ' ');
			else
				tempLocation["autoindex"] = split("off", ' ');
			if(it_location->root.compare("NAN"))
				tempLocation["root"] = split(it_location->root, ' ');
			else
				tempLocation["root"] = split("/Users/asabbar/Desktop/pushGit/Run_serv/html", ' ');
			if(it_location->index.compare("NAN"))
				tempLocation["index"] = split(it_location->index, ' ');
			if(it_location->methods[0].compare("NAN"))
				tempLocation["allow_methods"] = it_location->methods;
			if(it_location->cgi_bin.compare("NAN"))
				tempLocation["cgi_bin"] = split(it_location->cgi_bin, ' ');
			if(it_location->returnn.compare("NAN"))
			{
				tempLocation["return"] = split(it_location->returnn, ' ');
				std::cout <<tempLocation["return"].size() << "\n";
				if(tempLocation["return"].size() != 2)
				{
					std::cout << "bad return value\n";
					throw ConfigNotValid();
				}
			}
			if(it_location->upload_store.compare("NAN"))
				tempLocation["upload_store"] = split(it_location->upload_store, ' ');
			it->locations[it_location->path] = tempLocation;
		}
	}

}


void	parse::checkErrorPage()
{
	std::vector<serv_d>::iterator it = serv.begin();
	for(;it != serv.end(); it++)
	{
		for(std::vector<std::string>::iterator pageError = it->errorPage.begin(); pageError != it->errorPage.end(); pageError++)
		{
			std::vector<std::string> spliit = split(*pageError, ' ');
			if(spliit.size() != 2)
			{
				std::cout << "from checkErrorPage : pageError not valid :(\n";
				throw ConfigNotValid();
			}
			else
			{
				std::string key =spliit.at(0);
				std::string value = spliit.at(1);
				mytrim(key);
				mytrim(value);
				it->pageErrorpageError[key] = value;
			}
		}
		
	}
}

void	parse::checkDuplicatePort()
{
	std::vector<serv_d>::iterator it = serv.begin();
	for(;it != serv.end(); it++)
	{
		for(std::vector<serv_d>::iterator i = serv.begin();i != serv.end(); i++)
		{
			if(i != it)
			{
				for(std::vector<std::string>::iterator port = it->listens.begin(); port != it->listens.end(); port++)
				{
					if(atoi(port->c_str()) == 0)
					{
						std::cout << "invalid Port : " << port->c_str() <<"\n";
						throw ConfigNotValid();
					}
					if(std::find(i->listens.begin(),i->listens.end(), *port) != i->listens.end())
					{
						std::cout << "from checkDuplicatPort : Duplicate Port : " << *port <<"\n";
						throw ConfigNotValid();
					}
				}
			}
			else
			{
				for(std::vector<std::string>::iterator port = i->listens.begin(); port != i->listens.end(); port++)
				{
					if(find(port + 1 ,i->listens.end(), *port) != i->listens.end())
					{
						std::cout << "from checkDuplicatPort : Duplicate Port : " << *port <<"\n";
						throw ConfigNotValid();
					}
					
				}
			}
		}
		
	}
	
}

void parse::printData()
{
	std::vector<class serv_d>::iterator it = serv.begin();
	for (; it != serv.end(); it++)
	{
		std::cout << "=============== SERVER ===============\n";
		std::cout<<it->host<<"\n";
		for (std::vector<std::string>::iterator i = it->listens.begin(); i != it->listens.end(); i++)
			std::cout<< *i << " ";
		std::cout<< "\n";
		std::cout<<it->max_body_size<<"\n";
		std::cout<<it->root<<"\n";
		std::cout<<it->server_name<<"\n";
		for (std::vector<std::string> ::iterator i = it->errorPage.begin(); i != it->errorPage.end(); i++)
			std::cout<< *i << "\n";
		for (std::vector<class Location> ::iterator i = it->Loc.begin(); i != it->Loc.end(); i++)
		{
			std::cout << "=============== LOCATION ===============\n";
			std::cout<<	i->path		 <<"\n";
			std::cout<<	i->autoindex <<"\n";
			std::cout<<	i->index	 <<"\n";
			std::cout<<	i->root		 <<"\n";
			// std::cout<<	i->methods	 <<"\n";
			std::cout<<	i->returnn	 <<"\n";
			std::cout<<	i->cgi_bin	 <<"\n";
			std::cout << "=============== END LOCATION ===============\n";
		}
		std::cout << "=============== END ===============\n\n";
	}
	 
}
parse::parse()
{
}

parse::parse(std::string file)
{
	readConfig(file);
	checkSantax();
	setsers();
	collectData();
	checkData();
	mergeParser();
	// printData();
}

parse::parse(const parse &copy)
{
	(void) copy;
}

int getCloseBrackets(std::string &str, size_t i)
{
	int j = 0;
	while (str[i])
	{
		if (str[i] == '{')
			j++;
		if (str[i] == '}')
			j--;
		if (j == 0)
			return (i);
		i++;
	}
	return (-1);
}

void parse::setsers()
{
	size_t first = rawFile.find_first_of("{",0);
	size_t second = getCloseBrackets(rawFile, first);
	for (;;)
	{
		servs.push_back(rawFile.substr(first, second - first + 1));
		first = rawFile.find_first_of("{", second);
		if (first == std::string::npos)
			break;
		second = getCloseBrackets(rawFile, first);	
	}
}

void parse::remove_comments()
{
	int i = 0;
	while (this->rawFile[i])
	{
		if (this->rawFile[i] == '#')
		{
			int j = i;
			while (this->rawFile[j] != '\n' && this->rawFile[j] != '\0')
				j++;
			if(this->rawFile[j] == '\0')
				return ;
			else
				this->rawFile.erase (i , j - i );
			i = 0;
		}
		i++;
	}
}


void parse::readConfig(std::string fileName)
{
    std::ifstream   myfile;
    myfile.open(fileName);
	if(!myfile.is_open())
		throw ConfigNotValid();
	getline(myfile,rawFile, '\0');
	if(rawFile.empty())
		throw ConfigNotValid();
	remove_comments();
}

std::vector<serv_d> &parse::getServer()
{
	return serv;
}

void parse::checkData()
{
	checkDuplicatePort();
	checkErrorPage();
}
void parse::collectData()
{
	for (std::vector<std::string> ::iterator i = servs.begin(); i != servs.end(); i++)
	{
		int l = (*i).find("location ");
		class serv_d temp;
		temp.host 			=  split(getParam(*i,"host ",l),' ')[0];
		temp.listens 		=  split(getParam(*i,"listen ",l), ' ');
		temp.root 			=  getParam(*i, "root ", l);
		// if (access(temp.root.c_str(), X_OK) != 0)
		// {

		// 	std::cout << "getParam -> invalid path : "<< temp.root <<"\n";
		// 	throw ConfigNotValid(); 
		// }
		temp.max_body_size	= (size_t)atoi(getParam(*i, "client_max_body_size ",l).c_str());
		temp.server_name 	= getParam(*i, "server_name ",l);
		temp.errorPage 		= getErrorPage(*i, "errorPage ");
		temp.Loc 			= getLocations(*i);
		serv.push_back(temp);
	}
}


void parse::myTrim(std::string &s, const std::string &toTrim = " \t\f\v\n\r")
{
	if(s.find_first_not_of(toTrim)> s.length())
	{
		s.clear();
		return;
	}
	s = s.substr(s.find_first_not_of(toTrim), s.length());
	s = s.substr(0, s.find_last_not_of(toTrim) +1);
}


bool	checkIdDigit(const char *str)
{
	int i = -1;
	while (str[++i])
	{
		if(!isdigit(str[i]) && str[i] == 'B' && str[i + 1] =='\0')
			return true;
		if(!isdigit(str[i]))
			return false;
	}
	return false;
}


std::string parse::getParam(std::string &Fpart, std::string gg, size_t max)
{
	size_t i = 0,j = 0;
	i = Fpart.find(gg,0);
	if((int)i == -1)
		throw ConfigNotValid(); 
	i += gg.length();
	size_t temp = Fpart.find(gg, i);
	if((int)temp != -1 && temp < max)
	{ 
		std::cout<< "duplicata : " << gg << " !!\n";
		throw ConfigNotValid(); 
	}
	j = Fpart.find_first_of(";\n",i);
	std::string t =  Fpart.substr(i,j - i);
	myTrim(t);
	if (!t.compare(""))
		throw ConfigNotValid(); 
	if(!gg.compare("client_max_body_size "))
	{
		if(!checkIdDigit(t.c_str()))
		{
			std::cout << "Error : client_max_body_size" << "\n";
			throw ConfigNotValid(); 
		}
	}
	return t;
}


std::vector<std::string> parse::getErrorPage(std::string Fpart, std::string gg)
{
	size_t i = 0,j, l;
	std::vector<std::string> ret;
	l = Fpart.find("location ");
	for(;;)
	{
		i = Fpart.find(gg, i);
		if(i ==std::string::npos)
			break;
		i += 9;// 9 lenght of errorPage
		j = i;
		for (; j < l; j++)
		{
			if(Fpart[j] == ';' && Fpart[j + 1] == '\n')
				break;
		}
		std::string t =  Fpart.substr(i,j - i);
		myTrim(t);
		ret.push_back(t);
		i += j - i;
	} 
	return ret;
}

std::string parse::getPaPath(std::string &Fpart, std::string gg,size_t i)
{
	size_t j = 0;
	i = Fpart.find(gg,i) + gg.length();
	j = Fpart.find_first_of("\n",i);
	std::string t =  Fpart.substr(i,j - i);
	myTrim(t);
	if (t.empty())
		throw ConfigNotValid(); 
	return t;
}
std::string parse::getParamLocation(std::string &Fpart, std::string gg,int min, size_t max, int obli)
{
	size_t j = 0;
	int i = 0;
	i = Fpart.find(gg,min);
	if ((i == -1 || (size_t)i > max || i < min)&& obli == 1)
		throw ConfigNotValid(); 
	if((size_t)i > max || i == -1)
		return "NAN";
	i +=  gg.length();
	if((int)Fpart.find(gg,i) != -1&& Fpart.find(gg,i) < max)
	{
		std::cout << "Duplicate : "<< gg <<"\n";
		throw ConfigNotValid(); 
	}
	j = Fpart.find_first_of(";\n",i);
	std::string t =  Fpart.substr(i,j - i);
	myTrim(t);
	if (!t.compare("") && obli == 1)
		throw ConfigNotValid(); 
	return t;
}

std::vector<Location> parse::getLocations(std::string Fpart)
{
	std::vector<Location> ret;
	size_t i = 0;
	for (;;)
	{
		Location temp;
		int	obli = 0;
		
		int fisrtLoc = Fpart.find("location ", i);
		if(fisrtLoc == -1)
			break;

		temp.path			=	getPaPath(Fpart,"location ", fisrtLoc);
		// if (access((temp.path).c_str(), X_OK) != 0)
		// {
			
		// 	std::cout << "getPaPath -> invalid path : '"<< temp.path <<"'\n";
		// 	throw ConfigNotValid(); 
		// }
		mytrim(temp.path);
		if(!(temp.path).compare("*.php") || !(temp.path).compare("*.py"))
			obli = 1;
		int fisrtBracket 	=	Fpart.find("{", fisrtLoc);
		int secondBracket 	=	Fpart.find("}", fisrtBracket);
		temp.autoindex 		=	getParamLocation(Fpart,"autoindex ", fisrtBracket, secondBracket,0);
		temp.root	 		=	getParamLocation(Fpart,"root ", fisrtBracket, secondBracket, 0);
		// if (access(temp.root.c_str(), X_OK) != 0)
		// {

		// 	std::cout << "getParamLocation -> invalid path : "<< temp.root <<"\n";
		// 	throw ConfigNotValid(); 
		// }
		temp.index 			=	getParamLocation(Fpart,"indexPath", fisrtBracket, secondBracket, 0);
		temp.methods 		=	split(getParamLocation(Fpart,"methods", fisrtBracket, secondBracket, 1), ' ');
		temp.returnn 		=	getParamLocation(Fpart, "return", fisrtBracket, secondBracket, 0);
		temp.cgi_bin 		=	getParamLocation(Fpart,"cgi_bin", fisrtBracket, secondBracket, 0);
		temp.upload_store 	=	getParamLocation(Fpart,"upload_store", fisrtBracket, secondBracket, 0);
		
		ret.push_back(temp);
		i = secondBracket;
	}
	return ret;
}

// Destructor
parse::~parse()
{
}


// Operators
parse & parse::operator=(const parse &assign)
{
	(void) assign;
	return *this;
}
