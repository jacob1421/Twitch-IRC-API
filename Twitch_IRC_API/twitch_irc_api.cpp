#include <stdio.h>
#include <string.h>
#include <stdlib.h> 
#include "twitch_irc_api.h"
#include "bot_credentials.h"
	
//Const
constexpr const char* const TwitchWebsocketIRC::compatible_twitch_commands[];

/*
	-----------------------------------------------
						Callback Functions
	-----------------------------------------------
*/
/*
	Generic Callbacks
*/
void TwitchWebsocketIRC::onClearChat(void(*onClearChatPtr)(ClearChat arg)) {
	this->onClearChatPtr = onClearChatPtr;
}
void TwitchWebsocketIRC::onClearMsg(void(*onClearMsgPtr)(ClearMsg arg)) {
	this->onClearMsgPtr = onClearMsgPtr;
}
void TwitchWebsocketIRC::onNotice(void(*onNoticePtr)(Notice arg)) {
	this->onNoticePtr = onNoticePtr;
}
void TwitchWebsocketIRC::onGlobalUserState(void(*onGlobalUserStatePtr)(GlobalUserState arg)) {
	this->onGlobalUserStatePtr = onGlobalUserStatePtr;
}
void TwitchWebsocketIRC::onPrivMsg(void(*onPrivMsgPtr)(PrivMsg arg)) {
	this->onPrivMsgPtr = onPrivMsgPtr;
}
void TwitchWebsocketIRC::onRoomState(void(*onRoomStatePtr)(RoomState arg)) {
	this->onRoomStatePtr = onRoomStatePtr;
}
void TwitchWebsocketIRC::onUserNotice(void(*onUserNoticePtr)(UserNotice arg)) {
	this->onUserNoticePtr = onUserNoticePtr;
}
void TwitchWebsocketIRC::onUserState(void(*onUserStatePtr)(UserState arg)) {
	this->onUserStatePtr = onUserStatePtr;
}
void TwitchWebsocketIRC::onHostTarget(void(*onHostTargetPtr)(HostTarget arg)) {
	this->onHostTargetPtr = onHostTargetPtr;
}


/*
	Custom Callbacks
*/
void TwitchWebsocketIRC::onUserBan(void(*onUserBanPtr)(ClearChat arg)) {
	this->onUserBanPtr = onUserBanPtr;
}
void TwitchWebsocketIRC::onChannelHosted(void(*onChannelHostPtr)(Hostee arg)) {
	this->onChannelHostPtr = onChannelHostPtr;
}
void TwitchWebsocketIRC::onBits(void(*onBitsPtr)(PrivMsg arg)) {
	this->onBitsPtr = onBitsPtr;
}
void TwitchWebsocketIRC::onSub(void(*onSubPtr)(UserNotice arg)) {
	this->onSubPtr = onSubPtr;
}
void TwitchWebsocketIRC::onReSub(void(*onReSubPtr)(UserNotice arg)) {
	this->onReSubPtr = onReSubPtr;
}
void TwitchWebsocketIRC::onSubGift(void(*onSubGiftPtr)(UserNotice arg)) {
	this->onSubGiftPtr = onSubGiftPtr;
}
void TwitchWebsocketIRC::onAnonSubGift(void(*onAnonSubGiftPtr)(UserNotice arg)) {
	this->onAnonSubGiftPtr = onAnonSubGiftPtr;
}
void TwitchWebsocketIRC::onSubMysteryGift(void(*onSubMysteryGiftPtr)(UserNotice arg)) {
	this->onSubMysteryGiftPtr = onSubMysteryGiftPtr;
}
void TwitchWebsocketIRC::onGiftPaidUpgrade(void(*onGiftPaidUpgradePtr)(UserNotice arg)) {
	this->onGiftPaidUpgradePtr = onGiftPaidUpgradePtr;
}
void TwitchWebsocketIRC::onRewardGift(void(*onRewardGiftPtr)(UserNotice arg)) {
	this->onRewardGiftPtr = onRewardGiftPtr;
}
void TwitchWebsocketIRC::onAnonGiftPaidUpgrade(void(*onAnonGiftPaidUpgradePtr)(UserNotice arg)) {
	this->onAnonGiftPaidUpgradePtr = onAnonGiftPaidUpgradePtr;
}
void TwitchWebsocketIRC::onRaid(void(*onRaidPtr)(UserNotice arg)) {
	this->onRaidPtr = onRaidPtr;
}
void TwitchWebsocketIRC::onUnRaid(void(*onUnRaidPtr)(UserNotice arg)) {
	this->onUnRaidPtr = onUnRaidPtr;
}
void TwitchWebsocketIRC::onRitual(void(*onRitualPtr)(UserNotice arg)) {
	this->onRitualPtr = onRitualPtr;
}
void TwitchWebsocketIRC::onBitsBadgeTier(void(*onBitsBadgeTierPtr)(UserNotice arg)) {
	this->onBitsBadgeTierPtr = onBitsBadgeTierPtr;
}
void TwitchWebsocketIRC::onHostStart(void(*onHostStartPtr)(HostTarget arg)) {
	this->onHostStartPtr = onHostStartPtr;
}
void TwitchWebsocketIRC::onHostStop(void(*onHostStopPtr)(HostTarget arg)) {
	this->onHostStopPtr = onHostStopPtr;
}
void TwitchWebsocketIRC::onUserGivenMod(void(*onUserGivenModPtr)(User arg)) {
	this->onUserGivenModPtr = onUserGivenModPtr;
}
void TwitchWebsocketIRC::onCustomReward(void(*onCustomRewardPtr)(PrivMsg arg)) {
	this->onCustomRewardPtr = onCustomRewardPtr;
}
void TwitchWebsocketIRC::onHighlightedMsg(void(*onHighlightedMsgPtr)(PrivMsg arg)) {
	this->onHighlightedMsgPtr = onHighlightedMsgPtr;
}
void TwitchWebsocketIRC::onModChat(void(*onModChatPtr)(PrivMsg arg)) {
	this->onModChatPtr = onModChatPtr;
}

/*
	-----------------------------------------------
						PARSING
	-----------------------------------------------
*/

//Parses the message and sends the tag data to be parsed
void TwitchWebsocketIRC::parse_twitch_command_message(char * twitch_command_message) {
	//PING AND RECONNECT
	if (strstr(twitch_command_message, "RECONNECT") != NULL) {
		//RECONNECT TO THE CHANNEL
		connect_to_twitch_websocket();
		return;
	}
	else if (strstr(twitch_command_message, "PING :tmi.twitch.tv") != NULL) {
		//PONG THE CHANNEL BACK.
		send("PONG :tmi.twitch.tv");
		return;
	}

	int flag = 0;
	//Check if the message has a command that we can parse
	for (unsigned int a = 0; (a < 9) && (flag == 0); a = a + 1){
		if (strstr(twitch_command_message, compatible_twitch_commands[a]) != NULL) {
			flag = 1;
		}
	}
	if (flag == 0) { 
		return; 
	} // Did not find the command message.

	char* twitch_component_splits[4] = { 0 };
	int assignCount = 0;
	int pFlag = 0;
	char* tokenLocation;
	char* currLocation = twitch_command_message;
	tokenLocation = strchr(currLocation, ' ');
	*tokenLocation = 0;
	while (tokenLocation != NULL && assignCount < 3) {
		if (strstr(currLocation, "tmi.twitch.tv") != NULL) {
			pFlag = 1;
			if (assignCount != 1) {
				assignCount++;
			}
		}
		else if (!pFlag) {
			//Tag Data
			*tokenLocation = 0;
			twitch_component_splits[assignCount++] = currLocation;
		}
		else if (pFlag) {
			//Data
			*tokenLocation = 0;
			twitch_component_splits[assignCount++] = currLocation;
		}
		currLocation = (tokenLocation + 1);
		tokenLocation = strchr(currLocation, ' ');
	}
	twitch_component_splits[assignCount++] = currLocation;
	if (twitch_component_splits[DATA]) {
		twitch_component_splits[DATA] = (twitch_component_splits[DATA] + 1);
		//Replace the ' :' with ';'
		char* replaceCol = strstr(twitch_component_splits[DATA], " :");
		if (replaceCol) {
			*replaceCol = ';';
			*(replaceCol + 1) = 0;
		}
	}

	if (twitch_component_splits[CHANNEL_NAME]) {
		twitch_component_splits[CHANNEL_NAME] = (twitch_component_splits[CHANNEL_NAME] + 1);
	}

	//NULL COMMAND_NAME
	if (twitch_component_splits[COMMAND_NAME] == NULL) {
		return;
	}

	//Check if valid tag before processing
	parse_tags(twitch_component_splits);
}

//Parses tags and run the callback functions
void TwitchWebsocketIRC::parse_tags(char* twitch_component_splits[]) {
	//Wipe the memory for the union data.
	memset(&twitch_command, 0, sizeof(twitch_command));
	
	//Parse
	if (strcmp(twitch_component_splits[COMMAND_NAME], "CLEARCHAT") == 0) {
		//Default Assignments
		twitch_command.clear_chat_data.channel_name = twitch_component_splits[CHANNEL_NAME];
		twitch_command.clear_chat_data.username = twitch_component_splits[DATA];

		char* currTag = strtok(twitch_component_splits[TAGS_DATA], ";");
		while (currTag != NULL) {
			char* tagSplit = strchr(currTag, '=');
			*tagSplit = 0;
			//Assign tag data
			if (strcmp(currTag, "@ban-duration") == 0) {
				twitch_command.clear_chat_data.ban_duration = atoi((tagSplit + 1));
			}
			else if (twitch_command.clear_chat_data.ban_duration == 0) {
				//Perma ban
				twitch_command.clear_chat_data.ban_duration = 99999;
			}
			currTag = strtok(NULL, ";");
		}
		//Run Default Callback
		if (onClearChatPtr != NULL) {
			onClearChatPtr(twitch_command.clear_chat_data);
		}
		if (onUserBanPtr != NULL) {
			onUserBanPtr(twitch_command.clear_chat_data);
		}
	}
	else if (strcmp(twitch_component_splits[COMMAND_NAME], "CLEARMSG") == 0) {
		//Default Assignments
		twitch_command.clear_msg_data.channel_name = twitch_component_splits[CHANNEL_NAME];
		twitch_command.clear_msg_data.message = twitch_component_splits[DATA];

		char* currTag = strtok(twitch_component_splits[TAGS_DATA], ";");
		while (currTag != NULL) {
			char* tagSplit = strchr(currTag, '=');
			*tagSplit = 0;
			//Assign tag data
			if (strcmp(currTag, "@login") == 0) {
				twitch_command.clear_msg_data.login = (tagSplit + 1);
			}
			else if (strcmp(currTag, "target-msg-id") == 0) {
				twitch_command.clear_msg_data.target_msg_id = (tagSplit + 1);
			}
			currTag = strtok(NULL, ";");
		}
		if (onClearMsgPtr != NULL) {
			onClearMsgPtr(twitch_command.clear_msg_data);
		}
	}
	else if (strcmp(twitch_component_splits[COMMAND_NAME], "NOTICE") == 0) {
		//Default Assignments
		twitch_command.notice_data.channel_name = twitch_component_splits[CHANNEL_NAME];
		twitch_command.notice_data.message = twitch_component_splits[DATA];

		char* currTag = strtok(twitch_component_splits[TAGS_DATA], ";");
		while (currTag != NULL) {
			char* tagSplit = strchr(currTag, '=');
			*tagSplit = 0;
			//Assign tag data
			if (strcmp(currTag, "@msg-id") == 0) {
				twitch_command.notice_data.msg_id = (tagSplit + 1);
			}
			currTag = strtok(NULL, ";");
		}
		if (onNoticePtr != NULL) {
			onNoticePtr(twitch_command.notice_data);
		}
		if (onChannelHostPtr != NULL && (twitch_command.notice_data.msg_id != NULL)) {
			if (strcmp(twitch_command.notice_data.msg_id, "host_success") == 0) {
				Hostee hostee;
				char* splitData = strchr(twitch_command.notice_data.message, ' ');
				*splitData = 0;
				hostee.display_name = twitch_command.notice_data.message;
				onChannelHostPtr(hostee);
			}
			else if (strcmp(twitch_command.notice_data.msg_id, "host_success_viewers") == 0) {
				Hostee hostee;
				char* splitData = strchr(twitch_command.notice_data.message, ' ');
				*splitData = 0;
				hostee.display_name = twitch_command.notice_data.message;
				unsigned char c = 0;
				while (c < 7) {
					splitData = strchr((splitData + 1), ' ');
					c++;
				}
				char* s = strchr((splitData + 1), ' ');
				*s = 0;
				hostee.viewer_count = atoi(splitData + 1);
				onChannelHostPtr(hostee);
			}
		}
		if (onUserGivenModPtr != NULL && (twitch_command.notice_data.msg_id != NULL)) {
			if (strcmp(twitch_command.notice_data.msg_id, "mod_success") == 0) {
				User usr;
				char* splitData = strchr(twitch_command.notice_data.message, ' ');
				*splitData = 0;
				unsigned char c = 0;
				while (c < 2) {
					splitData = strchr((splitData + 1), ' ');
					c++;
				}
				char* s = strchr((splitData + 1), ' ');
				*s = 0;
				usr.display_name = (splitData + 1);
				onUserGivenModPtr(usr);
			}
		}
	}
	else if (strcmp(twitch_component_splits[COMMAND_NAME], "GLOBALUSERSTATE") == 0) {
		//Default Assignments
		//NA

		char* currTag = strtok(twitch_component_splits[TAGS_DATA], ";");
		while (currTag != NULL) {
			char* tagSplit = strchr(currTag, '=');
			*tagSplit = 0;
			//Assign tag data
			if (strcmp(currTag, "@badge-info") == 0) {
				parse_badge_tag(tagSplit + 1, twitch_command.global_user_state_data.badge_info);
			}
			else if (strcmp(currTag, "badges") == 0) {
				parse_badge_tag(tagSplit + 1, twitch_command.global_user_state_data.badges);
			}
			else if (strcmp(currTag, "color") == 0) {
				twitch_command.global_user_state_data.color = (tagSplit + 1);
			}
			else if (strcmp(currTag, "display-name") == 0) {
				twitch_command.global_user_state_data.display_name = (tagSplit + 1);
			}
			else if (strcmp(currTag, "emote-sets") == 0) {
				parse_emote_set(tagSplit + 1, twitch_command.global_user_state_data.emote_sets);
			}
			else if (strcmp(currTag, "user-id") == 0) {
				twitch_command.global_user_state_data.user_id = atoi((tagSplit + 1));
			}

			currTag = strtok(NULL, ";");
		}
		if (onGlobalUserStatePtr != NULL) {
			onGlobalUserStatePtr(twitch_command.global_user_state_data);
		}
	}
	else if (strcmp(twitch_component_splits[COMMAND_NAME], "PRIVMSG") == 0) {
		//Default Assignments
		twitch_command.priv_msg_data.channel_name = twitch_component_splits[CHANNEL_NAME];
		twitch_command.priv_msg_data.message = twitch_component_splits[DATA];

		char* currTag = strtok(twitch_component_splits[TAGS_DATA], ";");
		while (currTag != NULL) {
			char* tagSplit = strchr(currTag, '=');
			*tagSplit = 0;
			//Assign tag data
			if (strcmp(currTag, "@badge-info") == 0) {
				parse_badge_tag(tagSplit + 1, twitch_command.priv_msg_data.badge_info);
			}
			else if (strcmp(currTag, "badges") == 0) {
				parse_badge_tag(tagSplit + 1, twitch_command.priv_msg_data.badges);
			}
			else if (strcmp(currTag, "bits") == 0) {
				twitch_command.priv_msg_data.bits = atoi((tagSplit + 1));
			}
			else if (strcmp(currTag, "color") == 0) {
				twitch_command.priv_msg_data.color = (tagSplit + 1);
			}
			else if (strcmp(currTag, "display-name") == 0) {
				twitch_command.priv_msg_data.display_name = (tagSplit + 1);
			}
			else if (strcmp(currTag, "emotes") == 0) {
				parse_chat_emote((tagSplit + 1), twitch_command.priv_msg_data.emotes);
			}
			else if (strcmp(currTag, "id") == 0) {
				twitch_command.priv_msg_data.id = (tagSplit + 1);
			}
			else if (strcmp(currTag, "mod") == 0) {
				twitch_command.priv_msg_data.mod = atoi((tagSplit + 1));
			}
			else if (strcmp(currTag, "room-id") == 0) {
				twitch_command.priv_msg_data.room_id = atoi((tagSplit + 1));
			}
			else if (strcmp(currTag, "tmi-sent-ts") == 0) {
				twitch_command.priv_msg_data.tmi_sent_ts = (tagSplit + 1);
			}
			else if (strcmp(currTag, "user-id") == 0) {
				twitch_command.priv_msg_data.user_id = atoi((tagSplit + 1));
			}
			else if (strcmp(currTag, "msg-id") == 0) {
				twitch_command.priv_msg_data.msg_id = (tagSplit + 1);
			}
			else if (strcmp(currTag, "custom-reward-id") == 0) {
				twitch_command.priv_msg_data.custom_reward_id = (tagSplit + 1);
			}
			currTag = strtok(NULL, ";");
		}
		if (onPrivMsgPtr != NULL) {
			onPrivMsgPtr(twitch_command.priv_msg_data);
		}
		if (onBitsPtr != NULL && (twitch_command.priv_msg_data.bits > 0)) {
			onBitsPtr(twitch_command.priv_msg_data);
		}
		if (onCustomRewardPtr != NULL && (twitch_command.priv_msg_data.custom_reward_id != NULL)) {
			onCustomRewardPtr(twitch_command.priv_msg_data);
		}
		if (onHighlightedMsgPtr != NULL && (twitch_command.priv_msg_data.msg_id != NULL)) {
			if ((strcmp(twitch_command.priv_msg_data.msg_id, "highlighted-message") == 0)) {
				onHighlightedMsgPtr(twitch_command.priv_msg_data);
			}
		}
		if (onModChatPtr != NULL && (twitch_command.priv_msg_data.mod == 1)) {
			onModChatPtr(twitch_command.priv_msg_data);
		}
	}
	else if (strcmp(twitch_component_splits[COMMAND_NAME], "ROOMSTATE") == 0) {
		//Default Assignments
		twitch_command.room_state_data.channel_name = twitch_component_splits[CHANNEL_NAME];

		char* currTag = strtok(twitch_component_splits[TAGS_DATA], ";");
		while (currTag != NULL) {
			char* tagSplit = strchr(currTag, '=');
			*tagSplit = 0;
			if (strcmp(currTag, "@emote-only") == 0) {
				twitch_command.room_state_data.emote_only = atoi((tagSplit + 1));
			}
			else if (strcmp(currTag, "followers-only") == 0) {
				twitch_command.room_state_data.followers_only = atoi((tagSplit + 1));
			}
			else if (strcmp(currTag, "r9k") == 0) {
				twitch_command.room_state_data.r9k = atoi((tagSplit + 1));
			}
			else if (strcmp(currTag, "slow") == 0) {
				twitch_command.room_state_data.slow = atoi((tagSplit + 1));
			}
			else if (strcmp(currTag, "subs-only") == 0) {
				twitch_command.room_state_data.subs_only = atoi((tagSplit + 1));
			}
			currTag = strtok(NULL, ";");
		}
		if (onRoomStatePtr != NULL) {
			onRoomStatePtr(twitch_command.room_state_data);
		}
	}
	else if (strcmp(twitch_component_splits[COMMAND_NAME], "USERNOTICE") == 0) {
		//Default Assignments
		twitch_command.user_notice_data.channel_name = twitch_component_splits[CHANNEL_NAME];
		twitch_command.user_notice_data.message = twitch_component_splits[DATA];

		char* currTag = strtok(twitch_component_splits[TAGS_DATA], ";");
		while (currTag != NULL) {
			char* tagSplit = strchr(currTag, '=');
			*tagSplit = 0;

			//Assign tag data
			if (strcmp(currTag, "@badge-info") == 0) {
				parse_badge_tag(tagSplit + 1, twitch_command.user_notice_data.badge_info);
			}
			else if (strcmp(currTag, "badges") == 0) {
				parse_badge_tag(tagSplit + 1, twitch_command.user_notice_data.badges);
			}
			else if (strcmp(currTag, "color") == 0) {
				twitch_command.user_notice_data.color = (tagSplit + 1);
			}
			else if (strcmp(currTag, "display-name") == 0) {
				twitch_command.user_notice_data.display_name = (tagSplit + 1);
			}
			else if (strcmp(currTag, "emotes") == 0) {
				parse_chat_emote((tagSplit + 1), twitch_command.user_notice_data.emotes);
			}
			else if (strcmp(currTag, "id") == 0) {
				twitch_command.user_notice_data.id = (tagSplit + 1);
			}
			else if (strcmp(currTag, "login") == 0) {
				twitch_command.user_notice_data.login = (tagSplit + 1);
			}
			else if (strcmp(currTag, "mod") == 0) {
				twitch_command.user_notice_data.mod = atoi((tagSplit + 1));
			}
			else if (strcmp(currTag, "msg-id") == 0) {
				twitch_command.user_notice_data.msg_id = (tagSplit + 1);
			}
			else if (strcmp(currTag, "room-id") == 0) {
				twitch_command.user_notice_data.room_id = atoi((tagSplit + 1));
			}
			else if (strcmp(currTag, "system-msg") == 0) {
				twitch_command.user_notice_data.system_msg = (tagSplit + 1);
			}
			else if (strcmp(currTag, "tmi-sent-ts") == 0) {
				twitch_command.user_notice_data.tmi_sent_ts = (tagSplit + 1);
			}
			else if (strcmp(currTag, "user-id") == 0) {
				twitch_command.user_notice_data.user_id = atoi((tagSplit + 1));
			}
			else if (strcmp(currTag, "msg-param-cumulative-months") == 0) {
				twitch_command.user_notice_data.msg_param_cumulative_months = atoi((tagSplit + 1));
			}
			else if (strcmp(currTag, "msg-param-displayName") == 0) {
				twitch_command.user_notice_data.msg_param_display_name = (tagSplit + 1);
			}
			else if (strcmp(currTag, "msg-param-login") == 0) {
				twitch_command.user_notice_data.msg_param_login = (tagSplit + 1);
			}
			else if (strcmp(currTag, "msg-param-months") == 0) {
				twitch_command.user_notice_data.msg_param_months = atoi((tagSplit + 1));
			}
			else if (strcmp(currTag, "msg-param-promo-gift-total") == 0) {
				twitch_command.user_notice_data.msg_param_promo_gift_total = atoi((tagSplit + 1));
			}
			else if (strcmp(currTag, "msg-param-promo-name") == 0) {
				twitch_command.user_notice_data.msg_param_promo_name = (tagSplit + 1);
			}
			else if (strcmp(currTag, "msg-param-recipient-display-name") == 0) {
				twitch_command.user_notice_data.msg_param_recipient_display_name = (tagSplit + 1);
			}
			else if (strcmp(currTag, "msg-param-recipient-id") == 0) {
				twitch_command.user_notice_data.msg_param_recipient_id = atoi((tagSplit + 1));
			}
			else if (strcmp(currTag, "msg-param-recipient-user-name") == 0) {
				twitch_command.user_notice_data.msg_param_recipient_user_name = (tagSplit + 1);
			}
			else if (strcmp(currTag, "msg-param-sender-login") == 0) {
				twitch_command.user_notice_data.msg_param_sender_login = (tagSplit + 1);
			}
			else if (strcmp(currTag, "msg-param-sender-name") == 0) {
				twitch_command.user_notice_data.msg_param_sender_name = (tagSplit + 1);
			}
			else if (strcmp(currTag, "msg-param-should-share-streak") == 0) {
				twitch_command.user_notice_data.msg_param_should_share_streak = atoi((tagSplit + 1));
			}
			else if (strcmp(currTag, "msg-param-streak-months") == 0) {
				twitch_command.user_notice_data.msg_param_streak_months = atoi((tagSplit + 1));
			}
			else if (strcmp(currTag, "msg-param-sub-plan") == 0) {
				if (strcmp((tagSplit + 1), "Prime") == 0) {
					twitch_command.user_notice_data.msg_param_sub_plan = 0;
				}
				else {
					twitch_command.user_notice_data.msg_param_sub_plan = atoi((tagSplit + 1));
				}
			}
			else if (strcmp(currTag, "msg-param-sub-plan-name") == 0) {
				twitch_command.user_notice_data.msg_param_sub_plan_name = (tagSplit + 1);
			}
			else if (strcmp(currTag, "msg-param-viewerCount") == 0) {
				twitch_command.user_notice_data.msg_param_viewerCount = atoi((tagSplit + 1));
			}
			else if (strcmp(currTag, "msg-param-ritual-name") == 0) {
				twitch_command.user_notice_data.msg_param_ritual_name = (tagSplit + 1);
			}
			else if (strcmp(currTag, "msg-param-threshold") == 0) {
				twitch_command.user_notice_data.msg_param_threshold = atoi((tagSplit + 1));
			}
			currTag = strtok(NULL, ";");
		}
		
		if (onUserNoticePtr != NULL) {
			onUserNoticePtr(twitch_command.user_notice_data);
		}
		if (onSubPtr != NULL && (twitch_command.user_notice_data.msg_id != NULL)) {
			if (strcmp(twitch_command.user_notice_data.msg_id, "sub") == 0) {
				onSubPtr(twitch_command.user_notice_data);
			}
		}
		if (onReSubPtr != NULL && (twitch_command.user_notice_data.msg_id != NULL)) {
			if (strcmp(twitch_command.user_notice_data.msg_id, "resub") == 0) {
				onReSubPtr(twitch_command.user_notice_data);
			}
		}
		if (onSubGiftPtr != NULL && (twitch_command.user_notice_data.msg_id != NULL)) {
			if (strcmp(twitch_command.user_notice_data.msg_id, "subgift") == 0) {
				onSubGiftPtr(twitch_command.user_notice_data);
			}
		}
		if (onAnonSubGiftPtr != NULL && (twitch_command.user_notice_data.msg_id != NULL)) {
			if (strcmp(twitch_command.user_notice_data.msg_id, "anonsubgift") == 0) {
				onAnonSubGiftPtr(twitch_command.user_notice_data);
			}
		}
		if (onSubMysteryGiftPtr != NULL && (twitch_command.user_notice_data.msg_id != NULL)) {
			if (strcmp(twitch_command.user_notice_data.msg_id, "submysterygift") == 0) {
				onSubMysteryGiftPtr(twitch_command.user_notice_data);
			}
		}
		if (onGiftPaidUpgradePtr != NULL && (twitch_command.user_notice_data.msg_id != NULL)) {
			if (strcmp(twitch_command.user_notice_data.msg_id, "giftpaidupgrade") == 0) {
				onGiftPaidUpgradePtr(twitch_command.user_notice_data);
			}
		}
		if (onRewardGiftPtr != NULL && (twitch_command.user_notice_data.msg_id != NULL)) {
			if (strcmp(twitch_command.user_notice_data.msg_id, "rewardgift") == 0) {
				onRewardGiftPtr(twitch_command.user_notice_data);
			}
		}
		if (onAnonGiftPaidUpgradePtr != NULL && (twitch_command.user_notice_data.msg_id != NULL)) {
			if (strcmp(twitch_command.user_notice_data.msg_id, "anongiftpaidupgrad") == 0) {
				onAnonGiftPaidUpgradePtr(twitch_command.user_notice_data);
			}
		}
		if (onRaidPtr != NULL && (twitch_command.user_notice_data.msg_id != NULL)) {
			if (strcmp(twitch_command.user_notice_data.msg_id, "raid") == 0) { 
				onRaidPtr(twitch_command.user_notice_data); 
			}
		}
		if (onUnRaidPtr != NULL && (twitch_command.user_notice_data.msg_id != NULL)) {
			if (strcmp(twitch_command.user_notice_data.msg_id, "unraid") == 0) {
				onUnRaidPtr(twitch_command.user_notice_data);
			}
		}
		if (onRitualPtr != NULL && (twitch_command.user_notice_data.msg_id != NULL)) {
			if (strcmp(twitch_command.user_notice_data.msg_id, "ritual") == 0) {
				onRitualPtr(twitch_command.user_notice_data);
			}
		}
		if (onBitsBadgeTierPtr != NULL && (twitch_command.user_notice_data.msg_id != NULL)) {
			if (strcmp(twitch_command.user_notice_data.msg_id, "bitsbadgetier") == 0) {
				onBitsBadgeTierPtr(twitch_command.user_notice_data);
			}
		}

	}
	else if (strcmp(twitch_component_splits[COMMAND_NAME], "USERSTATE") == 0) {
		//Default Assignments
		twitch_command.user_state_data.channel_name = twitch_component_splits[CHANNEL_NAME];

		char* currTag = strtok(twitch_component_splits[TAGS_DATA], ";");
		while (currTag != NULL) {
			char* tagSplit = strchr(currTag, '=');
			*tagSplit = 0;
			//Assign tag data
			if (strcmp(currTag, "@badge-info") == 0) {
				parse_badge_tag(tagSplit + 1, twitch_command.user_state_data.badge_info);
			}
			else if (strcmp(currTag, "badges") == 0) {
				parse_badge_tag(tagSplit + 1, twitch_command.user_state_data.badges);
			}
			else if (strcmp(currTag, "color") == 0) {
				twitch_command.user_state_data.color = (tagSplit + 1);
			}
			else if (strcmp(currTag, "display-name") == 0) {
				twitch_command.user_state_data.display_name = (tagSplit + 1);
			}
			else if (strcmp(currTag, "emote-sets") == 0) {
				parse_emote_set(tagSplit + 1, twitch_command.user_state_data.emote_sets);
			}
			else if (strcmp(currTag, "mod") == 0) {
				twitch_command.user_state_data.mod = atoi((tagSplit + 1));
			}

			currTag = strtok(NULL, ";");
		}
		if (onUserStatePtr != NULL) {
			onUserStatePtr(twitch_command.user_state_data);
		}
	}
	else if (strcmp(twitch_component_splits[COMMAND_NAME], "HOSTTARGET") == 0) {
		//Default Assignments
		twitch_command.host_target_data.hosting_channel_name = twitch_component_splits[CHANNEL_NAME];

		//No tags to parse. Instead parsing the DATA
		char* split_data = strchr(twitch_component_splits[DATA], '-');
		if (split_data) {
			*split_data = 0;
			twitch_command.host_target_data.number_of_viewers = atoi((split_data + 1));
		}
		else {
			char* split_chan_view = strchr(twitch_component_splits[DATA], ' ');
			*split_chan_view = 0;
			twitch_command.host_target_data.channel_name = twitch_component_splits[DATA];
			twitch_command.host_target_data.number_of_viewers = atoi((split_chan_view + 1));
		}
		if (onHostTargetPtr != NULL) {
			onHostTargetPtr(twitch_command.host_target_data);
		}
		if (onHostStartPtr != NULL && (twitch_command.host_target_data.channel_name != NULL)) {
			onHostStartPtr(twitch_command.host_target_data);
		}
		if (onHostStopPtr != NULL && (twitch_command.host_target_data.channel_name == NULL)) {
			onHostStopPtr(twitch_command.host_target_data);
		}
	}
}

//Helper parsing functions
void TwitchWebsocketIRC::parse_badge_tag(char* badgeTags, Badge* badges) {
	int counter = 0;
	char* token;
	char* tokLastLoc = badgeTags;
	token = strtok_s(badgeTags, ",", &tokLastLoc);
	while (token != NULL && counter < MAX_BADGES) {
		char* splitData = strchr(token, '/');
		*splitData = 0;

		Badge badge;
		badge.name = token;
		badge.version = atoi((splitData + 1));

		badges[counter++] = badge;
		token = strtok_s(NULL, ",", &tokLastLoc);
	}
}

void TwitchWebsocketIRC::parse_emote_set(char* badgeTags, Emote* emotes) {
	int counter = 0;
	char* token;
	char* tokLastLoc = badgeTags;
	token = strtok_s(badgeTags, ",", &tokLastLoc);
	while (token != NULL && counter < MAX_EMOTES) {
		Emote emote;
		emote.emote_id = atoi(token);
		emotes[counter++] = emote;
		token = strtok_s(NULL, ",", &tokLastLoc);
	}
}

void TwitchWebsocketIRC::parse_chat_emote(char* chatEmotes, ChatEmote* chat_emotes) {
	int counter = 0;
	char* token;
	char* tokLastLoc = chatEmotes;
	token = strtok_s(chatEmotes, "/", &tokLastLoc);
	while (token != NULL && counter < MAX_CHAT_EMOTES) {
		char* splitData = strchr(token, ':');
		*splitData = 0;
		int emote_id = atoi(token);
		char* emoteChatToken;
		char* emoteChatLastLoc = token;

		emoteChatToken = strtok_s((splitData + 1), ",", &emoteChatLastLoc);
		while (emoteChatToken != NULL) {
			ChatEmote chat_emote;
			chat_emote.emote.emote_id = emote_id;
			char* split_index_data = strchr(emoteChatToken, '-');
			*split_index_data = 0;
			chat_emote.start_index_location = atoi(emoteChatToken);
			chat_emote.end_index_location = atoi(split_index_data + 1);
			chat_emotes[counter++] = chat_emote;
			emoteChatToken = strtok_s(NULL, ",", &emoteChatLastLoc);
		}

		token = strtok_s(NULL, "/", &tokLastLoc);
	}
}

/* Helper Functions */
void TwitchWebsocketIRC::connect_to_twitch_websocket() {
	printf("Connecting to: %s\n", TWITCH_WEBSOCKET_URL);
	bool isConnected = WebsocketsClient::connect(TWITCH_WEBSOCKET_URL);
	while(!isConnected){
		printf("Failed connection to: %s\n", TWITCH_WEBSOCKET_URL); printf("Trying Again...\n");
		isConnected = WebsocketsClient::connect(TWITCH_WEBSOCKET_URL);
		delay(1000);
	}
	printf("Successfully connected to: %s\n", TWITCH_WEBSOCKET_URL);
	printf("Handshaking with Twitch..\n");
	cap_req(CAP_REQ_STR);
	pass(TWITCH_OAUTH_TOKEN);
	nick(BOT_ACCOUNT_NAME);
	join_channel(CONNECT_CHANNEL_NAME);
	printf("Read to go! Connected to channel: %s as User: %s\n", CONNECT_CHANNEL_NAME, BOT_ACCOUNT_NAME);
}

char * TwitchWebsocketIRC::get_emote_image_url(const int emote_id, const Emote_Image_Size size) {
	if (size >= Emote_Image_Size::IMG_SMALL && size <= Emote_Image_Size::IMG_LARGE) {
		sprintf(img_emote_url, "http://static-cdn.jtvnw.net/emoticons/v1/%i/%i.0", emote_id, size);
		return img_emote_url;
	}
	return NULL;
}

const char * TwitchWebsocketIRC::sub_type_to_string(int subType) {
	switch (subType) {
		case 0:
			return "Prime";
		break;
		case 1000:
			return "Tier 1";
		break;
		case 2000:
			return "Tier 2";
		break;
		case 3000:
			return "Tier 3";
		break;
		default:
			return "Invalid Plan Type";
		break;
	}
}

/*
	-----------------------------------------------
					IRC CHAT FUNCTIONS
	-----------------------------------------------
*/

/* Generic IRC Commands */
void TwitchWebsocketIRC::part_channel(const char* channel_name) {
	snprintf(command_string, MAX_COMMAND_STRING_LEN, "PART #%s", channel_name);
	send(command_string);
}

void TwitchWebsocketIRC::join_channel(const char* channel_name) {
	snprintf(command_string, MAX_COMMAND_STRING_LEN, "JOIN #%s", channel_name);
	send(command_string);
}

void TwitchWebsocketIRC::cap_req(const char* cap_request) {
	snprintf(command_string, MAX_COMMAND_STRING_LEN, "CAP REQ :%s", cap_request);
	send(command_string);
}

void TwitchWebsocketIRC::pass(const char* oauth_token) {
	snprintf(command_string, MAX_COMMAND_STRING_LEN, "PASS %s", oauth_token);
	send(command_string);
}

void TwitchWebsocketIRC::nick(const char* bot_account_name) {
	snprintf(command_string, MAX_COMMAND_STRING_LEN, "NICK #%s", bot_account_name);
	send(command_string);
}

void TwitchWebsocketIRC::send_in_twitch_format(const char * channel_name) {
	memmove((command_string + strlen(channel_name) + 11), command_string, MAX_COMMAND_STRING_LEN);
	memcpy(command_string, "PRIVMSG #", 9);
	memcpy((command_string + 9), channel_name, strlen(channel_name));
	memcpy((command_string + strlen(channel_name) + 9), " :", 2);
	send(command_string);
}

/*Basic Command For Everyone*/
void TwitchWebsocketIRC::send_chat_message(char * chat_message){
	strcpy(command_string, chat_message);
	send_in_twitch_format(CONNECT_CHANNEL_NAME);
}

void TwitchWebsocketIRC::color(const char * color) {
	snprintf(command_string, MAX_COMMAND_STRING_LEN, "/color %s", color);
	send_in_twitch_format(CONNECT_CHANNEL_NAME);
}
void TwitchWebsocketIRC::block(const char* username) {
	snprintf(command_string, MAX_COMMAND_STRING_LEN, "/block %s", username);
	send_in_twitch_format(CONNECT_CHANNEL_NAME);
}
void TwitchWebsocketIRC::unblock(const char* username) {
	snprintf(command_string, MAX_COMMAND_STRING_LEN, "/unblock %s", username);
	send_in_twitch_format(CONNECT_CHANNEL_NAME);
}
void TwitchWebsocketIRC::me(const char* message) {
	snprintf(command_string, MAX_COMMAND_STRING_LEN, "/me %s", message);
	send_in_twitch_format(CONNECT_CHANNEL_NAME);
}
void TwitchWebsocketIRC::disconnect() {
	strcpy(command_string, "/disconnect");
	send_in_twitch_format(CONNECT_CHANNEL_NAME);
}
void TwitchWebsocketIRC::whisper(const char* username, const char* message) {
	snprintf(command_string, MAX_COMMAND_STRING_LEN, "/w %s %s", username, message);
	send_in_twitch_format(CONNECT_CHANNEL_NAME);
}

/* Broadcaster and Moderators */
void TwitchWebsocketIRC::timeout(const char* username, const int seconds) {
	snprintf(command_string, MAX_COMMAND_STRING_LEN, "/timeout %s %i", username, seconds);
	send_in_twitch_format(CONNECT_CHANNEL_NAME);
}
void TwitchWebsocketIRC::untimeout(const char* username) {
	snprintf(command_string, MAX_COMMAND_STRING_LEN, "/untimeout %s", username);
	send_in_twitch_format(CONNECT_CHANNEL_NAME);
}
void TwitchWebsocketIRC::ban(const char* username) {
	snprintf(command_string, MAX_COMMAND_STRING_LEN, "/ban %s", username);
	send_in_twitch_format(CONNECT_CHANNEL_NAME);
}
void TwitchWebsocketIRC::unban(const char* username) {
	snprintf(command_string, MAX_COMMAND_STRING_LEN, "/unban %s", username);
	send_in_twitch_format(CONNECT_CHANNEL_NAME);
}
void TwitchWebsocketIRC::clear() {
	strcpy(command_string, "/clear");
	send_in_twitch_format(CONNECT_CHANNEL_NAME);
}

/* Restrict Chat */
void TwitchWebsocketIRC::slow(const int seconds) {
	snprintf(command_string, MAX_COMMAND_STRING_LEN, "/slow %i", seconds);
	send_in_twitch_format(CONNECT_CHANNEL_NAME);
}
void TwitchWebsocketIRC::slow_off() {
	strcpy(command_string, "/slowoff");
	send_in_twitch_format(CONNECT_CHANNEL_NAME);
}
void TwitchWebsocketIRC::followers(const int time, const Time_Type time_t) {
	switch (time_t) {
		case Time_Type::MINUTES:
			snprintf(command_string, MAX_COMMAND_STRING_LEN, "/followers %im", time);
		break;
		case Time_Type::HOURS:
			snprintf(command_string, MAX_COMMAND_STRING_LEN, "/followers %ih", time);
		break;
		case Time_Type::DAYS:
			snprintf(command_string, MAX_COMMAND_STRING_LEN, "/followers %id", time);
		break;
		case Time_Type::WEEKS:
			snprintf(command_string, MAX_COMMAND_STRING_LEN, "/followers %iw", time);
			break;
		case Time_Type::MONTHS:
			snprintf(command_string, MAX_COMMAND_STRING_LEN, "/followers %imo", time);
		break;
		default:
			//Failed
			return;
		break;
	}
	send_in_twitch_format(CONNECT_CHANNEL_NAME);
}
void TwitchWebsocketIRC::followers_off() {
	strcpy(command_string, "/followersoff");
	send_in_twitch_format(CONNECT_CHANNEL_NAME);
}
void TwitchWebsocketIRC::subscribers() {
	strcpy(command_string, "/subscribers");
	send_in_twitch_format(CONNECT_CHANNEL_NAME);
}
void TwitchWebsocketIRC::subscribers_off() {
	strcpy(command_string, "/subscribersoff");
	send_in_twitch_format(CONNECT_CHANNEL_NAME);
}
void TwitchWebsocketIRC::r9kbeta() {
	strcpy(command_string, "/r9kbeta");
	send_in_twitch_format(CONNECT_CHANNEL_NAME);
}
void TwitchWebsocketIRC::r9kbeta_off() {
	strcpy(command_string, "/r9kbetaoff");
	send_in_twitch_format(CONNECT_CHANNEL_NAME);
}
void TwitchWebsocketIRC::emote_only() {
	strcpy(command_string, "/emoteonly");
	send_in_twitch_format(CONNECT_CHANNEL_NAME);
}
void TwitchWebsocketIRC::emote_only_off() {
	strcpy(command_string, "/emoteonlyoff");
	send_in_twitch_format(CONNECT_CHANNEL_NAME);
}

/* Channel Editor and Broadcaster */

/* Partner Only */
void TwitchWebsocketIRC::commercial(const Commercial_Times seconds) {
	snprintf(command_string, MAX_COMMAND_STRING_LEN, "/commercial %i", seconds);
	send_in_twitch_format(CONNECT_CHANNEL_NAME);
}

void TwitchWebsocketIRC::host(const char * channel_name) {
	snprintf(command_string, MAX_COMMAND_STRING_LEN, "/host %s", channel_name);
	send_in_twitch_format(CONNECT_CHANNEL_NAME);
}
void TwitchWebsocketIRC::unhost() {
	strcpy(command_string, "/unhost");
	send_in_twitch_format(CONNECT_CHANNEL_NAME);
}
void TwitchWebsocketIRC::raid(const char * channel_name) {
	snprintf(command_string, MAX_COMMAND_STRING_LEN, "/raid %s", channel_name);
	send_in_twitch_format(CONNECT_CHANNEL_NAME);
}
void TwitchWebsocketIRC::unraid() {
	strcpy(command_string, "/unraid");
	send_in_twitch_format(CONNECT_CHANNEL_NAME);
}
void TwitchWebsocketIRC::marker(const char * description) {
	snprintf(command_string, MAX_COMMAND_STRING_LEN, "/marker %s", description);
	send_in_twitch_format(CONNECT_CHANNEL_NAME);
}

/* Broadcaster */
void TwitchWebsocketIRC::mod(const char * username) {
	snprintf(command_string, MAX_COMMAND_STRING_LEN, "/mod %s", username);
	send_in_twitch_format(CONNECT_CHANNEL_NAME);
}
void TwitchWebsocketIRC::unmod(const char * username) {
	snprintf(command_string, MAX_COMMAND_STRING_LEN, "/unmod %s", username);
	send_in_twitch_format(CONNECT_CHANNEL_NAME);
}
void TwitchWebsocketIRC::vip(const char * username) {
	snprintf(command_string, MAX_COMMAND_STRING_LEN, "/vip %s", username);
	send_in_twitch_format(CONNECT_CHANNEL_NAME);
}
void TwitchWebsocketIRC::unvip(const char * username) {
	snprintf(command_string, MAX_COMMAND_STRING_LEN, "/unvip %s", username);
	send_in_twitch_format(CONNECT_CHANNEL_NAME);
}