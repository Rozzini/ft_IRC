/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parser.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mraspors <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/25 17:44:33 by mraspors          #+#    #+#             */
/*   Updated: 2023/06/25 19:07:11 by mraspors         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef IRC_PARSER_HPP
#define IRC_PARSER_HPP

#include <iostream>
#include <sstream>
#include <string>

#include <vector>
#include <map>


#include "Command.hpp"

class Server;
class Parser;
class Command;

class Parser
{
    private:

        Server*                             _srv;
        std::map<std::string, Command *>    _commands;

        std::string     trim(const std::string& str);

    public:

        Parser(Server* srv);
        ~Parser();

        void    invoke(Client* client, const std::string& message);
};

#endif