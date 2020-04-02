#ifndef TWITCH_IRC_API_H
#define TWITCH_IRC_API_H
#include <ArduinoWebsockets.h>
#include "memory_constraints.h"

/* Structs */
struct Badge {
	char* name;
	short version;
};

struct Emote {
	unsigned int emote_id;
};

struct ChatEmote {
	Emote emote;
	unsigned short start_index_location;
	unsigned short end_index_location;
};

struct ClearChat {
	char* channel_name;
	char* username;
	unsigned int ban_duration; //(Optional)Duration of the timeout, in seconds.If omitted, the ban is permanent.
};

struct ClearMsg {
	char* channel_name; //Added from prototype
	char* login;
	char* message;
	char* target_msg_id;
};

struct Notice {
	char* channel_name;
	char* msg_id;
	char* message;
};

struct GlobalUserState {
	Badge badge_info[MAX_BADGES];
	Badge badges[MAX_BADGES];
	char* color;
	char* display_name;
	Emote emote_sets[MAX_EMOTES];
	unsigned int user_id;
};

struct PrivMsg {
	Badge badge_info[MAX_BADGES];
	Badge badges[MAX_BADGES];
	char* channel_name;
	unsigned int bits;
	char* color;
	char* display_name;
	ChatEmote emotes[MAX_CHAT_EMOTES];
	char* id;
	char* message;
	unsigned char mod;
	unsigned int room_id;
	char* tmi_sent_ts;
	unsigned int user_id;
	char* custom_reward_id;
	char* msg_id;
};

struct RoomState {
	unsigned char emote_only;
	short followers_only;
	unsigned char r9k;
	unsigned short slow;
	unsigned char subs_only;
	char* channel_name;
};

struct UserNotice {
	Badge badge_info[MAX_BADGES];
	Badge badges[MAX_BADGES];
	char* color;
	char* display_name;
	ChatEmote emotes[MAX_CHAT_EMOTES];
	char* id;
	char* login;
	char* message;
	unsigned char mod;
	char* msg_id;
	unsigned int room_id;
	char* system_msg;
	char* tmi_sent_ts;
	unsigned int user_id;
	char* channel_name;

	//Other msg-param parameters
	unsigned int msg_param_cumulative_months;
	char* msg_param_display_name;
	char* msg_param_login;
	unsigned int msg_param_months;
	unsigned int msg_param_promo_gift_total;
	char* msg_param_promo_name;
	char* msg_param_recipient_display_name;
	unsigned int msg_param_recipient_id;
	char* msg_param_recipient_user_name;
	char* msg_param_sender_login;
	char* msg_param_sender_name;
	unsigned int msg_param_should_share_streak;
	unsigned int msg_param_streak_months;
	unsigned int msg_param_sub_plan;
	char* msg_param_sub_plan_name;
	unsigned int msg_param_viewerCount;
	char* msg_param_ritual_name;
	unsigned int msg_param_threshold;
};

struct UserState {
	Badge badge_info[MAX_BADGES];
	Badge badges[MAX_BADGES];
	char* color;
	char* display_name;
	Emote emote_sets[MAX_EMOTES];
	char* channel_name;
	unsigned char mod;
};

struct HostTarget {
	char* hosting_channel_name;
	char* channel_name;
	unsigned int number_of_viewers;
};

struct Hostee {
	char* display_name;
	unsigned int viewer_count;
};

struct User {
	char* display_name;
};

/* Class */
class TwitchWebsocketIRC : websockets::WebsocketsClient {
	private:
		union TwitchCommandStruct {
			ClearChat clear_chat_data;
			ClearMsg clear_msg_data;
			Notice notice_data;
			GlobalUserState global_user_state_data;
			PrivMsg priv_msg_data;
			RoomState room_state_data;
			UserNotice user_notice_data;
			UserState user_state_data;
			HostTarget host_target_data;
		};
		enum TwitchTagName {
			TAGS_DATA,
			COMMAND_NAME,
			CHANNEL_NAME,
			DATA
		};

		//Private Vars
		TwitchCommandStruct twitch_command;
		char command_string[MAX_COMMAND_STRING_LEN];
		char img_emote_url[MAX_EMOTE_URL_STRING_LEN];
		
		static const constexpr char* compatible_twitch_commands[] = {
			"HOSTTARGET",	  //Channel starts or stops host mode.
			"NOTICE",	      //General notices from the server.
			"RECONNECT",	  //Rejoin channels after a restart.
			"CLEARCHAT",	  //Purges all chat messages in a channel, or purges chat messages from a specific user(typically after a timeout or ban).
			"CLEARMSG",		  //Removes a single message from a channel.This is triggered via / delete <target - msg - id> on IRC.
			"GLOBALUSERSTATE",//On successful login, provides data about the current logged - in user through IRC tags.It is sent after successfully authenticating(sending a PASS / NICK command).
			"PRIVMSG",		  //Sends a message to a channel.
			"ROOMSTATE",      //Sends room - state data when a user joins a channel or a room setting is changed.For a join, the message contains all chat - room settings.For changes, only the relevant tag is sent.
			"USERNOTICE",     //Sends a notice to the user when any of several events occurs.
			"USERSTATE"      //Sends user - state data when a user joins a channel or sends a PRIVMSG to a channel.
		};

		/* Parsing function */
		void parse_badge_tag(char * badgeTags, Badge* badges);
		void parse_emote_set(char * badgeTags, Emote* emotes);
		void parse_chat_emote(char * chatEmotes, ChatEmote* chat_emotes);
		void parse_tags(char* twitch_component_splits[]);
		void parse_twitch_command_message(char * twitch_command_message);

		/* Web socket functions */
		void cap_req(const char* cap_request);
		void pass(const char* oauth_token);
		void nick(const char* bot_account_name);
		void send_in_twitch_format(const char * channel_name);

		/* Generic callback function pointers */
		void(*onClearChatPtr)(ClearChat arg) = nullptr;
		void(*onClearMsgPtr)(ClearMsg arg) = nullptr;
		void(*onNoticePtr)(Notice arg) = nullptr;
		void(*onGlobalUserStatePtr)(GlobalUserState arg) = nullptr;
		void(*onPrivMsgPtr)(PrivMsg arg) = nullptr;
		void(*onRoomStatePtr)(RoomState arg) = nullptr;
		void(*onUserNoticePtr)(UserNotice arg) = nullptr;
		void(*onUserStatePtr)(UserState arg) = nullptr;
		void(*onHostTargetPtr)(HostTarget arg) = nullptr;
		/* Custom callback function pointers */
		void(*onUserBanPtr)(ClearChat arg) = nullptr;
		void(*onChannelHostPtr)(Hostee arg) = nullptr;
		void(*onBitsPtr)(PrivMsg arg) = nullptr;
		void(*onSubPtr)(UserNotice arg) = nullptr;
		void(*onReSubPtr)(UserNotice arg) = nullptr;
		void(*onSubGiftPtr)(UserNotice arg) = nullptr;
		void(*onAnonSubGiftPtr)(UserNotice arg) = nullptr;
		void(*onSubMysteryGiftPtr)(UserNotice arg) = nullptr;
		void(*onGiftPaidUpgradePtr)(UserNotice arg) = nullptr;
		void(*onRewardGiftPtr)(UserNotice arg) = nullptr;
		void(*onAnonGiftPaidUpgradePtr)(UserNotice arg) = nullptr;
		void(*onRaidPtr)(UserNotice arg) = nullptr;
		void(*onUnRaidPtr)(UserNotice arg) = nullptr;
		void(*onRitualPtr)(UserNotice arg) = nullptr;
		void(*onBitsBadgeTierPtr)(UserNotice arg) = nullptr;
		void(*onHostStartPtr)(HostTarget arg) = nullptr;
		void(*onHostStopPtr)(HostTarget arg) = nullptr;
		void(*onUserGivenModPtr)(User arg) = nullptr;
		void(*onCustomRewardPtr)(PrivMsg arg) = nullptr;
		void(*onHighlightedMsgPtr)(PrivMsg arg) = nullptr;
		void(*onModChatPtr)(PrivMsg arg) = nullptr;

	public:

		TwitchWebsocketIRC(){
			WebsocketsClient::onMessage([&](websockets::WebsocketsMessage message) {
				parse_twitch_command_message((char *)message.data().c_str());
			});
		}

		using websockets::WebsocketsClient::WebsocketsClient::poll;
		using websockets::WebsocketsClient::available;

		enum class Emote_Image_Size {
			IMG_SMALL = 1,
			IMG_MEDIUM = 2,
			IMG_LARGE = 3
		};
		enum class Time_Type {
			MINUTES,
			HOURS,
			DAYS,
			WEEKS,
			MONTHS
		};
		enum class Commercial_Times {
			COMMERCIAL_30_SECONDS = 30,
			COMMERCIAL_60_SECONDS = 60,
			COMMERCIAL_90_SECONDS = 90,
			COMMERCIAL_120_SECONDS = 120,
			COMMERCIAL_150_SECONDS = 150,
			COMMERCIAL_180_SECONDS = 180,
		};
		
		//Allow user to connect to twitch easily
		void connect_to_twitch_websocket();

		/*
			Generic Callbacks
		*/
		void onClearChat(void(*onClearChatPtr)(ClearChat arg));
		void onClearMsg(void(*onClearMsgPtr)(ClearMsg arg));
		void onNotice(void(*onNoticePtr)(Notice arg));
		void onGlobalUserState(void(*onGlobalUserStatePtr)(GlobalUserState arg));
		void onPrivMsg(void(*onPrivMsgPtr)(PrivMsg arg));
		void onRoomState(void(*onRoomStatePtr)(RoomState arg));
		void onUserNotice(void(*onUserNoticePtr)(UserNotice arg));
		void onUserState(void(*onUserStatePtr)(UserState arg));
		void onHostTarget(void(*onHostTargetPtr)(HostTarget arg));


		/*
			Custom Callbacks
		*/
		void onUserBan(void(*onUserBanPtr)(ClearChat arg));
		void onChannelHosted(void(*onChannelHostPtr)(Hostee arg));
		void onBits(void(*onBitsPtr)(PrivMsg arg));
		void onSub(void(*onSubPtr)(UserNotice arg));
		void onReSub(void(*onReSubPtr)(UserNotice arg));
		void onSubGift(void(*onSubGiftPtr)(UserNotice arg));
		void onAnonSubGift(void(*onAnonSubGiftPtr)(UserNotice arg));
		void onSubMysteryGift(void(*onSubMysteryGiftPtr)(UserNotice arg));
		void onGiftPaidUpgrade(void(*onGiftPaidUpgradePtr)(UserNotice arg));
		void onRewardGift(void(*onRewardGiftPtr)(UserNotice arg));
		void onAnonGiftPaidUpgrade(void(*onAnonGiftPaidUpgradePtr)(UserNotice arg));
		void onRaid(void(*onRaidPtr)(UserNotice arg));
		void onUnRaid(void(*onUnRaidPtr)(UserNotice arg));
		void onRitual(void(*onRitualPtr)(UserNotice arg));
		void onBitsBadgeTier(void(*onBitsBadgeTierPtr)(UserNotice arg));
		void onHostStart(void(*onHostStartPtr)(HostTarget arg));
		void onHostStop(void(*onHostStopPtr)(HostTarget arg));
		void onUserGivenMod(void(*onUserGivenModPtr)(User arg));
		void onCustomReward(void(*onCustomRewardPtr)(PrivMsg arg));
		void onHighlightedMsg(void(*onHighlightedMsgPtr)(PrivMsg arg));
		void onModChat(void(*onModChatPtr)(PrivMsg arg));

		/* 
			API FUNCTIONS 
		*/

		//Helper functions
		char* get_emote_image_url(const int emote_id, const Emote_Image_Size size);
		const char* sub_type_to_string(int subType);
		
		/* Generic IRC Commands */
		void part_channel(const char* channel_name);
		void join_channel(const char* channel_name);
		void send_chat_message(char * chat_message);
		
		/*Basic Command For Everyone*/
		void color(const char* color);
		void block(const char* username);
		void unblock(const char* username);
		void me(const char* message);
		void disconnect();
		void whisper(const char* username, const char* message);

		/* Broadcaster and Moderators */
		void timeout(const char* username, const int seconds);
		void untimeout(const char* username);
		void ban(const char* username);
		void unban(const char* username);
		void clear();

		/* Restrict Chat */
		void slow(const int seconds);
		void slow_off();
		void followers(const int time, const Time_Type time_t);
		void followers_off();
		void subscribers();
		void subscribers_off();
		void r9kbeta();
		void r9kbeta_off();
		void emote_only();
		void emote_only_off();

		/* Channel Editor and Broadcaster */
		void commercial(const Commercial_Times seconds); /* Partner Only */

		void host(const char* channel_name);
		void unhost();
		void raid(const char* channel_name);
		void unraid();
		void marker(const char* description);

		/* Broadcaster */
		void mod(const char* username);
		void unmod(const char* username);
		void vip(const char* username);
		void unvip(const char* username);
};

#endif