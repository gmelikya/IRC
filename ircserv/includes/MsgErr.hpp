#ifndef  ERR_MSG_HPP
# define ERR_MSG_HPP

# include <iostream>
# include <sys/socket.h>

class MsgErr
{
public:
// ############################ ERROR #################################
	void	sendCmdNotFound(int client_fd, std::string command);
	void	sendParamsMissing(int client_fd, std::string command);
	void	sendUserNotRegistered(int client_fd, std::string command);
	void	sendAlreadyRegistered(int client_fd, std::string userNick);
	void	sendInvalidNick(int client_fd, std::string userNick);
	void	sendNickMissing(int client_fd, std::string userNick);
	void	sendNickConflict(int client_fd, std::string userNick);
	void	sendNickTaken(int client_fd, std::string userNick);
	void	sendOriginMissing(int client_fd, std::string command);
	void	sendTextMissing(int client_fd, std::string command);
	void	sendRecipientMissing(int client_fd, std::string command);
	void	sendTooManyTargets(int client_fd, std::string tg);
	void	sendNickNotFound(int client_fd, std::string userNick);
	void	sendCannotMsgChannel(int client_fd, std::string chanName);
	void	sendChannelInviteOnly(int client_fd, std::string chanName);
	void	sendChannelNotFound(int client_fd, std::string chanName);
	void	sendOpPrivilegesNeeded(int client_fd, std::string chanName);
	void	sendNotOnChannel(int client_fd, std::string chanName);
	void	sendUserOnChannel(int client_fd, std::string chanName, std::string userNick);
	void	sendModeUnknown(int client_fd, std::string c);
	void	sendBannedFromChannel(int client_fd, std::string chanName);
	void	sendChannelFull(int client_fd, std::string chanName);
	void	sendWrongChannelKey(int client_fd, std::string chanName);

// ############################ REPLY #################################

	void	RPL_PONG(int client_fd, std::string userNick);
	void	RPL_KICK(int client_fd, std::string userNick, std::string uname, std::string tname, std::string chanName);
	void	RPL_QUIT(int client_fd, std::string userNick, std::string msg);
	void	RPL_INVITING(int client_fd, std::string chanName, std::string userNick, std::string uname, std::string tname);
	void	RPL_TOPIC(int client_fd, std::string chanName, bool flag, std::string channelTopic);
	
	void	RPL_REGISTER(int client_fd, std::string userNick, std::string uname);
	void	RPL_WHOREPLY(int client_fd, std::string userNick, std::string chanName, std::string memb, std::string username);
	void	RPL_NAMREPLY(int client_fd, std::string userNick, std::string chanName, std::string participants);
	void	RPL_ENDOFNAMES(int client_fd, std::string userNick, std::string chanName);
	void	RPL_ENDOFWHO(int client_fd, std::string userNick, std::string chanName);
	void	RPL_PRIVMSG(int client_fd, std::string userNick, std::string uname, std::string tname, std::string msg);
	void	RPL_NOTICE(int client_fd, std::string userNick, std::string tname, std::string msg);
	void	RPL_PART(int client_fd, std::string userNick, std::string uname, std::string chanName);

};
#endif 