/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asabbar <asabbar@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/19 10:42:44 by asabbar           #+#    #+#             */
/*   Updated: 2023/02/18 22:39:28 by asabbar          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Webserv.hpp"

                    
void    print_error(std::string  print)
{
    std::cerr << print << std::endl;
}

int main(int ac, char **av)
{
    if (ac != 2)
    {
        std::cout << "Ghayarha" << std::endl;
        return 1;
    }
    std::string arg = std::string(av[1]);
    try
    {
        parse Parsing(arg);

        run_server(Parsing.getServer());
        // map<string,vector<string> > lMap;
        // map<string,vector<string> > upload;
        // map<string,vector<string> > Mtest;
        // vector<string> tVec;

        // tVec.push_back("off");
        // lMap["autoindex"] = tVec;        
        // // lMap["upload_enable"] = tVec;
        // tVec.clear();
        // tVec.push_back("/upload"); 
        // lMap["upload_store"] = tVec;       
        // tVec.clear();
        // tVec.push_back("index.html"); 
        // lMap["index"] = tVec;       
        // tVec.clear();
        // tVec.push_back("GET");
        // // tVec.push_back("POST");
        // // tVec.push_back("DELETE");
        // lMap["allow_methods"] = tVec;
        // tVec.clear();

        
        // tVec.push_back("on");
        // upload["autoindex"] = tVec;        
        // upload["upload_enable"] = tVec;
        // tVec.clear();
        // tVec.push_back("/"); 
        // upload["upload_store"] = tVec;       
        // tVec.clear();
        // tVec.push_back("ss.png"); 
        // upload["index"] = tVec;       
        // tVec.clear();
        // tVec.push_back("GET");
        // tVec.push_back("POST");
        // // tVec.push_back("DELETE");
        // upload["allow_methods"] = tVec;
        // tVec.clear();
  
        // tVec.push_back("on");
        // Mtest["autoindex"] = tVec;        
        // Mtest["upload_enable"] = tVec;
        // tVec.clear();
        // // tVec.push_back("/upload"); 
        // // Mtest["upload_store"] = tVec;       
        // // tVec.clear();
        // tVec.push_back("GET");
        // // tVec.push_back("POST");
        // Mtest["allow_methods"] = tVec;
        
        // std::vector<serv_d> test;
        // serv_d s1;

        // s1.locations["/"] = lMap;
        // s1.locations["/upload"] = upload;
        // // s1.locations["/upload/test"] = Mtest;

        // s1.set_host("127.0.0.1");
        // s1.listens.push_back("6060");
        // s1.listens.push_back("3030");
        // s1.listens.push_back("5533");
        // s1.set_server_name("achraf.com");
        // s1.set_max_body_size(999999999);
        // s1.set_root("/Users/asabbar/Desktop/pushGit/Run_serv/html");

        // serv_d s2;
    
        // s2.set_host("127.0.0.1");
        // s2.listens.push_back("1234");
        // s2.set_server_name("achraf.com");
        // s2.set_max_body_size(999999999);
        // s2.set_root("/Users/asabbar/Desktop/pushGit/Run_serv/html");
        // serv_d s3;
    
        // s3.set_host("127.0.0.1");
        // s3.listens.push_back("9090");
        // s3.set_server_name("achraf.com");
        // s3.set_max_body_size(999999999);
        // s3.set_root("/Users/asabbar/Desktop/pushGit/Run_serv/html");
        // serv_d s4;
    
        // s4.set_host("127.0.0.1");
        // s4.listens.push_back("5050");
        // s4.set_server_name("achraf.com");
        // s4.set_max_body_size(999999999);
        // s4.set_root("/Users/asabbar/Desktop/pushGit/Run_serv/html");
        // serv_d s5;
    
        // s5.set_host("127.0.0.1");
        // s5.listens.push_back("7070");
        // s5.set_server_name("achraf.com");
        // s5.set_max_body_size(999999999);
        // s5.set_root("/Users/asabbar/Desktop/pushGit/Run_serv/html");
    
        // test.push_back(s1);
        // test.push_back(s2);
        // test.push_back(s3);
        // test.push_back(s4);
        // test.push_back(s5);
        // run_server(test);
    }
    catch(std::exception &e)
    {
        std::cerr << e.what() << std::endl;
    }
    return (0);
}
