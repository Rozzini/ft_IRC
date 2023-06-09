/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parser.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alalmazr <alalmazr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/25 17:44:41 by mraspors          #+#    #+#             */
/*   Updated: 2023/06/30 19:35:24 by alalmazr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Parser.hpp"

Parser::Parser(Server* srv) : _srv(srv)
{
    _commands["PASS"] = new PASS(_srv, false);
    _commands["NICK"] = new NICK(_srv, false);
    _commands["USER"] = new USER(_srv, false);
    _commands["QUIT"] = new QUIT(_srv, false);

    _commands["PRIVMSG"] = new PM(_srv);
    _commands["JOIN"] = new JOIN(_srv);
    _commands["KICK"] = new KICK(_srv);
    _commands["KILL"] = new KILL(_srv);
    _commands["TOPIC"] = new TOPIC(_srv);
    _commands["MODE"] = new MODE(_srv);
	_commands["INVITE"] = new INVITE(_srv);
}

Parser::~Parser ()
{
    std::map<std::string, Command *>::iterator it = _commands.begin();
    std::map<std::string, Command *>::iterator end = _commands.end();

    while (it != end)
    {
        delete it->second;
        it++;
    }
}


std::string     Parser::trim(const std::string& str)
{
    std::string WHITESPACE = " \n\r\t\f\v";
    std::string result = "";

    size_t  start = str.find_first_not_of(WHITESPACE);
    if (start != std::string::npos)
        result = str.substr(start);

    size_t  end = result.find_last_not_of(WHITESPACE);
    if (end != std::string::npos)
        result = result.substr(0, end + 1);

    return result;
}

void            Parser::invoke(Client* client, const std::string& message)
{
    std::stringstream   ss(message);
    std::string         syntax;
    
    while (std::getline(ss, syntax))
    {
        syntax = trim(syntax);

        std::string name = syntax.substr(0, syntax.find(' '));

        try
        {
            std::vector<std::string>    args;
            std::stringstream           line(syntax.substr(name.length(), syntax.length()));
            std::string                 buf;

            Command *cmd = _commands.at(name);

            while (line >> buf)
                args.push_back(buf);


            if (client->get_state() != 2 && cmd->need_auth())
            {
				client->reply(ERR_NO_REG(client->get_nick()));
                return;
            }

            cmd->execute(client, args);
        }
        catch (const std::exception& e)
        {
			
			client->reply(ERR_UNKNOWN_COMMAND(client->get_nick(), name));
        }
    }
} 