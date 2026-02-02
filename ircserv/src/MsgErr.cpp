#include "../includes/MsgErr.hpp"

void	MsgErr::sendCmdNotFound(int client_fd, std::string b)
{
	send(client_fd, (":" + b + "@localhost 421 Unknown command\n").c_str(), (b.size() + 33),0);
}

void	MsgErr::sendParamsMissing(int client_fd, std::string command)
{
	send(client_fd, (":" + command + "@localhost 461 Not enough parameters\n").c_str(), (command.size() + 39), 0);
}

void	MsgErr::sendUserNotRegistered(int client_fd, std::string userNick)
{
	send(client_fd, (":" + userNick + "@localhost 451 You have not registered\n").c_str(), (userNick.size() + 41), 0);
}

void	MsgErr::sendAlreadyRegistered(int client_fd, std::string userNick)
{
	send(client_fd, (":" + userNick + "@localhost 462 You may not reregister\n").c_str(), (userNick.size() + 40), 0);
}

void	MsgErr::sendInvalidNick(int client_fd, std::string userNick)
{
	send(client_fd, (":" + userNick + "@localhost 432 Erroneus userNickname\n").c_str(), (userNick.size() + 35), 0);
}

void	MsgErr::sendNickMissing(int client_fd, std::string userNick)
{
	send(client_fd, (":" + userNick + "@localhost 431 No userNickname given\n").c_str(), (userNick.size() + 35), 0);
}

void	MsgErr::sendNickConflict(int client_fd, std::string userNick)
{
	send(client_fd, (":" + userNick + "@localhost 436 Nickname collision KILL\n").c_str(), (userNick.size() + 42), 0);
}

void	MsgErr::sendNickTaken(int client_fd, std::string userNick)
{
	send(client_fd, (":" + userNick + "@localhost 433 Nickname is already in use\n").c_str(), (userNick.size() + 44), 0);
}

void	MsgErr::sendOriginMissing(int client_fd, std::string command)
{
	send(client_fd, (":" + command + "@localhost 409 No origin specified\n").c_str(), (command.size() + 37), 0);
}

void	MsgErr::sendTextMissing(int client_fd, std::string command)
{
	send(client_fd, (":" + command + "@localhost 412 No text to send\n").c_str(), (command.size() + 33), 0);
}

void	MsgErr::sendRecipientMissing(int client_fd, std::string command)
{
	send(client_fd, (":@localhost 411 No recipient given " + command + "\n").c_str(), (command.size() + 37), 0);
}

void	MsgErr::sendTooManyTargets(int client_fd, std::string tg)
{
	send(client_fd, (":" + tg + "@localhost 407 Duplicate recipients. No message \n").c_str(), (tg.size() + 51), 0);
}

void	MsgErr::sendNickNotFound(int client_fd, std::string userNick)
{
	send(client_fd, (":" + userNick + "@localhost 401 No such userNick/channel\n").c_str(), (userNick.size() + 38), 0);
}

void	MsgErr::sendCannotMsgChannel(int client_fd, std::string chanName)
{
	send(client_fd, (":" + chanName + "@localhost 404 Cannot send to channel\n").c_str(), (chanName.size() + 40), 0);
}

void	MsgErr::sendChannelInviteOnly(int client_fd, std::string chanName)
{
	send(client_fd, (":" + chanName + "@localhost 473 Cannot join channel (+i)\n").c_str(), (chanName.size() + 42), 0);
}

void	MsgErr::sendChannelNotFound(int client_fd, std::string chanName)
{
	send(client_fd, (":" + chanName + "@localhost 403 No such channel\n").c_str(), (chanName.size() + 33), 0);
}

void	MsgErr::sendOpPrivilegesNeeded(int client_fd, std::string chanName)
{
	send(client_fd, (":" + chanName + "@localhost 482 You're not channel operatorsator\n").c_str(), (chanName.size() + 45), 0);
}

void	MsgErr::sendNotOnChannel(int client_fd, std::string chanName)
{
	send(client_fd, (":" + chanName + "@localhost 442 You're not on that channel\n").c_str(), (chanName.size() + 44), 0);
}

void	MsgErr::sendUserOnChannel(int client_fd, std::string chanName, std::string userNick)
{
	send(client_fd, (":" + userNick + chanName + "@localhost 443 is already on channel\n").c_str(), (chanName.size() + userNick.size() + 39), 0);
}

void	MsgErr::sendModeUnknown(int client_fd, std::string c)
{
	send(client_fd, (":" + c + "@localhost 472 is unknown mode char to me\n").c_str(), (c.size() + 44), 0);
}

void	MsgErr::sendBannedFromChannel(int client_fd, std::string chanName)
{
	send(client_fd, (":" + chanName + "@localhost 474 Cannot join channel (+b)\n").c_str(), (chanName.size() + 42), 0);
}

void	MsgErr::sendChannelFull(int client_fd, std::string chanName)
{
	send(client_fd, (":" + chanName + "@localhost 471 Cannot join channel (+l)\n").c_str(), (chanName.size() + 42), 0);
}

void	MsgErr::sendWrongChannelKey(int client_fd, std::string chanName)
{
	send(client_fd, (":" + chanName + "@localhost 475 Cannot join channel (+k)\n").c_str(), (chanName.size() + 42), 0);
}

// ##################################### REPLY ##########################################

void	MsgErr::RPL_PONG(int client_fd, std::string userNick)
{
	send(client_fd, (":PONG@localhost PONG message from " + userNick + "\n").c_str(), 36 + userNick.size(), 0);
}

void	MsgErr::RPL_QUIT(int client_fd, std::string userNick, std::string msg)
{
	send(client_fd, (":QUIT@localhost " + userNick + " " + msg + "\n").c_str(), 19 + userNick.size() + msg.size(), 0);
}

void	MsgErr::RPL_INVITING(int client_fd, std::string chanName, std::string userNick, std::string uname, std::string tname)
{
	std::string m = ":" + userNick + "!" + uname + "@localhost 341 " + chanName + " " + tname + "\n";
	send(client_fd, m.c_str(), m.size(), 0);
}

void	MsgErr::RPL_TOPIC(int client_fd, std::string chanName, bool flag, std::string channelTopic)
{
	std::string m;
	if (flag)
		m = ":" + chanName + "@localhost 331 No channelTopic is set\n";
	else
		m = ":" + chanName + "@localhost 332 " + channelTopic + "\n";
	send(client_fd, m.c_str(), m.size(), 0);
}

void	MsgErr::RPL_WHOREPLY(int client_fd, std::string userNick, std::string chanName, std::string memb, std::string username)
{
	std::string m = "352 " + userNick + " " + chanName + " " + username + " 127.0.0.1 irc " + memb + " H :1 " + username + "\n";
	send(client_fd, m.c_str(), m.size(), 0);
}

void	MsgErr::RPL_ENDOFNAMES(int client_fd, std::string userNick, std::string chanName)
{
	send(client_fd, ("366 " + userNick + " " + chanName + " :End of /NAMES list\n").c_str(), chanName.size() + userNick.size() + 27, 0);
}

void	MsgErr::RPL_NAMREPLY(int client_fd, std::string userNick, std::string chanName, std::string participants)
{
	send(client_fd, ("353 " + userNick + " = " + chanName + " :" + participants + "\n").c_str(), 10 + userNick.size() + chanName.size() + participants.size(), 0);
}

void	MsgErr::RPL_ENDOFWHO(int client_fd, std::string userNick, std::string chanName)
{
	std::string a = &(chanName.c_str())[1];
	std::string m = "315 " + userNick + " " + a + " :End of WHO list\n";
	send(client_fd, m.c_str(), m.size(), 0);
}

void	MsgErr::RPL_REGISTER(int client_fd, std::string userNick, std::string uname)
{
	std::string msg = ":" + userNick + "!" + uname + "@localhost 001 " + userNick + " :Welcome to the IRC Network\n";
	send(client_fd, msg.c_str(), msg.length(), 0);
}

void	MsgErr::RPL_PRIVMSG(int client_fd, std::string userNick, std::string uname, std::string tname, std::string msg)
{
	std::string a = msg.substr(msg.find(":") + 1, msg.size() - msg.find(":"));
	std::string m = (":" + userNick + "!" + uname + "@localhost PRIVMSG " + tname + " "+ ":" + " " + a + "\n");
	send(client_fd, m.c_str(), m.size(), 0);
}

void	MsgErr::RPL_NOTICE(int client_fd, std::string userNick, std::string tname, std::string msg)
{
	send(client_fd, (":" + userNick + "@localhost NOTICE " + tname + " " + msg).c_str(), 21 + userNick.size() + tname.size() + msg.size(), 0);
}

void	MsgErr::RPL_PART(int client_fd, std::string userNick, std::string uname, std::string chanName)
{
	send(client_fd, (":" + uname + "!" + userNick + "@localhost 324 PART " + chanName + " :Left the channel\n").c_str(), 41 + uname.size() + userNick.size() + chanName.size(), 0);
}

void	MsgErr::RPL_KICK(int client_fd, std::string userNick, std::string uname, std::string tname, std::string chanName)
{
	std::string msg = (":" + userNick + "!" + uname + "@localhost KICK " + chanName + " " + tname + " :The \n");
	send(client_fd, msg.c_str(), msg.size(), 0);
}