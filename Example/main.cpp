#include <ESP8266WiFi.h>
#include <twitch_irc_api.h>

//Twitch Api Instance
TwitchWebsocketIRC twitch_api;

const char* ssid = ""; //Enter SSID
const char* password = ""; //Enter Password

/* Generic Callback Functions */
void onClearChatCallback(ClearChat data) {
	printf("\n---------------------------------------------------\n");
	printf("\n=CLEARCHAT=\nChannel Name: %s\nUsername: %s\nBan Duration: %i\n", data.channel_name, data.username, data.ban_duration);
	printf("\n---------------------------------------------------\n");
}
void onClearMsgCallback(ClearMsg data) {
	printf("\n---------------------------------------------------\n");
	printf("\n=CLEARMSG=\nChannel Name: %s\nLogin: %s\nMessage: %s\nTargetMsgId: %s\n", data.channel_name, data.login, data.message, data.target_msg_id);
	printf("\n---------------------------------------------------\n");
}
void onNoticeCallback(Notice data) {
	printf("\n---------------------------------------------------\n");
	printf("\n=NOTICE=\nChannel Name: %s\nMsg-id: %s\nMessage: %s\n", data.channel_name, data.msg_id, data.message);
	printf("\n---------------------------------------------------\n");
}
void onGlobalUserStateCallback(GlobalUserState data) {
	printf("\n---------------------------------------------------\n");
	int count = 0;
	printf("\n=GLOBALUSERSTATE=\n");
	printf("\nBadges Info\n");
	while (data.badge_info[count].name != NULL) {
		printf("\t-Badge Name: %s Version: %i\n", data.badge_info[count].name, data.badge_info[count].version);
		count += 1;
	}
	count = 0;
	printf("\nBadges\n");
	while (data.badges[count].name != NULL) {
		printf("\t-Badge Name: %s Version: %i\n", data.badges[count].name, data.badges[count].version);
		count += 1;
	}
	count = 0;
	printf("\nEmote Sets\n");
	while (data.emote_sets[count].emote_id != 0 || count == 0) {
		printf("\t-Emote Id: %i\n", data.emote_sets[count].emote_id);
		count += 1;
	}
	printf("Color: %s\nDisplay Name: %s\nUser Id: %i\n",
		data.color, data.display_name, data.user_id);
	printf("\n---------------------------------------------------\n");
}
void onPrivMsgCallback(PrivMsg data) {
	printf("\n---------------------------------------------------\n");
	int count = 0;
	printf("\n=PRIVMSG=\n");
	printf("\nBadges Info\n");
	while (data.badge_info[count].name != NULL) {
		printf("\t-Badge Name: %s Version: %i\n", data.badge_info[count].name, data.badge_info[count].version);
		count += 1;
	}
	count = 0;
	printf("\nBadges\n");
	while (data.badges[count].name != NULL) {
		printf("\t-Badge Name: %s Version: %i\n", data.badges[count].name, data.badges[count].version);
		count += 1;
	}
	count = 0;
	printf("\nEmote Chat Set\n");
	while (data.emotes[count].start_index_location != 0 && data.emotes[count].end_index_location != 0) {
		printf("\t-Emote Id: %i\n", data.emotes[count].emote.emote_id);
		printf("\t-Emote First Index: %i\n", data.emotes[count].start_index_location);
		printf("\t-Emote Second Index: %i\n", data.emotes[count].end_index_location);
		count += 1;
	}
	printf("Bits: %i\nColor: %s\nDisplay Name: %s\nId: %s\nMessage: %s\nMod: %i\nRoom Id: %i\nTmi Sent Ts: %s\nUser Id: %i\nChannel Name: %s\n",
		data.bits, data.color, data.display_name, data.id, data.message, data.mod, data.room_id, data.tmi_sent_ts, data.user_id, data.channel_name);
	printf("\n---------------------------------------------------\n");
}
void onRoomStateCallback(RoomState data) {
	printf("\n---------------------------------------------------\n");
	printf("\n=ROOMSTATE=\n");
	printf("Emote Only: %i\nFollowers Only: %i\nR9K: %i\nSlow: %i\nSubs Only: %i\nChannel Name: %s\n",
		data.emote_only, data.followers_only, data.r9k, data.slow, data.subs_only, data.channel_name);
	printf("\n---------------------------------------------------\n");
}
void onUserNoticeCallback(UserNotice data) {
	printf("\n---------------------------------------------------\n");
	printf("\n=USERNOTICE=\n");
	printf("\nBadges Info\n");
	int count = 0;
	while (data.badge_info[count].name != NULL) {
		printf("\t-Badge Name: %s Version: %i\n", data.badge_info[count].name, data.badge_info[count].version);
		count += 1;
	}
	count = 0;
	printf("\nBadges\n");
	while (data.badges[count].name != NULL) {
		printf("\t-Badge Name: %s Version: %i\n", data.badges[count].name, data.badges[count].version);
		count += 1;
	}
	count = 0;
	printf("\nEmote Chat Set\n");
	while (data.emotes[count].start_index_location != 0 && data.emotes[count].end_index_location != 0){
		printf("\t-Emote Id: %i\n", data.emotes[count].emote.emote_id);
		printf("\t-Emote First Index: %i\n", data.emotes[count].start_index_location);
		printf("\t-Emote Second Index: %i\n", data.emotes[count].end_index_location);
		count += 1;
	}
	printf("Color: %s\nDisplay Name: %s\nId: %s\nLogin: %s\nMessage: %s\nMod: %i\nMsg_Id: %s\nRoom_Id: %i\nSystem_Msg: %s\nTmi Sent Ts: %s\nUser_Id: %i\nChannel_Name: %s\n",
		data.color, data.display_name, data.id, data.login, data.message, data.mod, data.msg_id, data.room_id, data.system_msg, data.tmi_sent_ts, data.user_id, data.channel_name);
	printf("\nmsg_param_cumulative_months: %i\nmsg_param_display_name: %s\nmsg_param_login: %s\nmsg_param_months: %i\nmsg_param_promo_gift_total: %i\n",
		data.msg_param_cumulative_months, data.msg_param_display_name, data.msg_param_login, data.msg_param_months, data.msg_param_promo_gift_total);
	printf("\nmsg_param_promo_name: %s\nmsg_param_recipient_display_name: %s\nmsg_param_recipient_id: %i\nmsg_param_recipient_user_name: %s\nmsg_param_sender_login: %s\nmsg_param_sender_name: %s\n",
		data.msg_param_promo_name, data.msg_param_recipient_display_name, data.msg_param_recipient_id, data.msg_param_recipient_user_name, data.msg_param_sender_login, data.msg_param_sender_name);
	printf("\nmsg_param_should_share_streak: %i\nmsg_param_streak_months: %i\nmsg_param_sub_plan: %i\nmsg_param_sub_plan_name: %s\nmsg_param_viewerCount: %i\nmsg_param_ritual_name: %s\nmsg_param_threshold: %i\n",
		data.msg_param_should_share_streak, data.msg_param_streak_months, data.msg_param_sub_plan, data.msg_param_sub_plan_name, data.msg_param_viewerCount, data.msg_param_ritual_name, data.msg_param_threshold);
	printf("\n---------------------------------------------------\n");
}
void onUserStateCallback(UserState data) {
	printf("\n---------------------------------------------------\n");
	printf("\n=USERSTATE=\n");
	printf("\nBadges Info\n");
	int count = 0;
	while (data.badge_info[count].name != NULL) {
		printf("\t-Badge Name: %s Version: %i\n", data.badge_info[count].name, data.badge_info[count].version);
		count += 1;
	}
	count = 0;
	printf("\nBadges\n");
	while (data.badges[count].name != NULL) {
		printf("\t-Badge Name: %s Version: %i\n", data.badges[count].name, data.badges[count].version);
		count += 1;
	}
	count = 0;
	printf("\nEmote Sets\n");
	while (data.emote_sets[count].emote_id != 0 || count == 0) {
		printf("\t-Emote Id: %i\n", data.emote_sets[count].emote_id);
		count += 1;
	}
	printf("Color: %s\nDisplay Name: %s\nMod: %i\nChannel Name: %s\n",
		data.color, data.display_name, data.mod, data.channel_name);
	printf("\n---------------------------------------------------\n");
}
void onHostTargetCallback(HostTarget data) {
	printf("\n---------------------------------------------------\n");
	printf("\n=HOSTTARGET=\n");
	printf("Hosting Channel Name: %s\nNumber Of Viewers: %i\nChannel Name: %s\n",
		data.hosting_channel_name, data.number_of_viewers, data.channel_name);
	printf("\n---------------------------------------------------\n");
}

/* Custom Callbacks */
void onChannelHostedCallback(Hostee data) {
	printf("\n---------------------------------------------------\n");
	printf("\n=CHANNEL HOSTED=\n");
	printf("Hostee Display Name: %s\nViewers: %i\n", data.display_name, data.viewer_count);
	printf("\n---------------------------------------------------\n");
}
void onBitsCallback(PrivMsg data) {
	printf("\n---------------------------------------------------\n");
	printf("\n=BITS=\n");
	printf("Bit Donater Name: %s\nBits: %i\n", data.display_name, data.bits);
	printf("\n---------------------------------------------------\n");
}
void onSubCallback(UserNotice data) {
	printf("\n---------------------------------------------------\n");
	printf("\n=SUB=\n");
	printf("Subscribers Name: %s\nTotal Months Subbed: %i\nSub Streak: %i\nSub Type: %i\nSub Plan Name: %s\nShare Sub Streak: %i\n",
		data.display_name, data.msg_param_cumulative_months, data.msg_param_streak_months, data.msg_param_sub_plan, data.msg_param_sub_plan_name, data.msg_param_should_share_streak);
	printf("\n---------------------------------------------------\n");
}
void onReSubCallback(UserNotice data) {
	printf("\n---------------------------------------------------\n");
	printf("\n=RESUB=\n");
	printf("Subscribers Name: %s\nTotal Months Subbed: %i\nSub Streak: %i\nSub Type: %i\nSub Plan Name: %s\nShare Sub Streak: %i\n",
		data.display_name, data.msg_param_cumulative_months, data.msg_param_streak_months, data.msg_param_sub_plan, data.msg_param_sub_plan_name, data.msg_param_should_share_streak);
	printf("\n---------------------------------------------------\n");
}
void onSubGiftCallback(UserNotice data) {
	printf("\n---------------------------------------------------\n");
	printf("\n=SUB GIFT=\n");
	printf("Gifters Name: %s\nTotal Months Subbed: %i\nRecipient Display Name: %s\nRecipient User Id: %i\nRecipient Username: %s\nSub Plan:%i\nSub Plan Name: %s\n",
		data.display_name, data.msg_param_months, data.msg_param_recipient_display_name, data.msg_param_recipient_id, data.msg_param_recipient_user_name, data.msg_param_sub_plan, data.msg_param_sub_plan_name);
	printf("\n---------------------------------------------------\n");
}
void onAnonSubGiftCallback(UserNotice data) {
	printf("\n---------------------------------------------------\n");
	printf("\n=ANON SUB GIFT=\n");
	printf("Total Months Subbed: %i\nRecipient Display Name: %s\nRecipient User Id: %i\nRecipient Username: %s\nSub Plan:%i\nSub Plan Name: %s\n",
		data.msg_param_months, data.msg_param_recipient_display_name, data.msg_param_recipient_id, data.msg_param_recipient_user_name, data.msg_param_sub_plan, data.msg_param_sub_plan_name);
	printf("\n---------------------------------------------------\n");
}
void onSubMysteryGiftCallback(UserNotice data) {
	printf("\n---------------------------------------------------\n");
	printf("\n=SUB MYSTERY GIFT=\n");
	printf("\n---------------------------------------------------\n");
}
void onGiftPaidUpgradeCallback(UserNotice data) {
	printf("\n---------------------------------------------------\n");
	printf("\n=GIFT PAID UPGRADE=\n");
	printf("Number of gifts subs given: %i\n Sub Promo Name: %s\n Gifters Login: %s\n Gifters Display Name: %s\n",
		data.msg_param_promo_gift_total, data.msg_param_promo_name, data.msg_param_sender_login, data.msg_param_sender_name);
	printf("\n---------------------------------------------------\n");
}
void onRewardGiftCallback(UserNotice data) {
	printf("\n---------------------------------------------------\n");
	printf("\n=REWARD GIFT=\n");
	printf("\n---------------------------------------------------\n");
}
void onAnonGiftPaidUpgradeCallback(UserNotice data) {
	printf("\n---------------------------------------------------\n");
	printf("\n=ANON GIFT PAID UPGRADE=\n");
	printf("Number of gifts subs given: %i\n Sub Promo Name: %s\n",
		data.msg_param_promo_gift_total, data.msg_param_promo_name);
	printf("\n---------------------------------------------------\n");
}
void onRaidCallback(UserNotice data) {
	printf("\n---------------------------------------------------\n");
	printf("\n=RAID=\n");
	printf("Raider Display Name: %s\nRaider Channel Name: %s\nViewer Count: %i\n",
		data.msg_param_display_name, data.msg_param_login, data.msg_param_viewerCount);
	printf("\n---------------------------------------------------\n");
}
void onUnRaidCallback(UserNotice data) {
	printf("\n---------------------------------------------------\n");
	printf("\n=UNRAID=\n");
	printf("\n---------------------------------------------------\n");
}
void onRitualCallback(UserNotice data) {
	printf("\n---------------------------------------------------\n");
	printf("\n=RITUAL=\n");
	printf("Name of ritual: %s\n", data.msg_param_ritual_name);
	printf("\n---------------------------------------------------\n");
}
void onBitsBadgeTierCallback(UserNotice data) {
	printf("\n---------------------------------------------------\n");
	printf("\n=BITS BADGE TIER=\n");
	printf("Bits Badge Tier: %i", data.msg_param_threshold);
	printf("\n---------------------------------------------------\n");
}
void onHostStartCallback(HostTarget data) {
	printf("\n---------------------------------------------------\n");
	printf("\n=HOST START=\n");
	printf("Hosting Channel Name: %s\nNumber Of Viewers: %i\nChannel Name: %s\n",
		data.hosting_channel_name, data.number_of_viewers, data.channel_name);
	printf("\n---------------------------------------------------\n");
}
void onHostStopCallback(HostTarget data) {
	printf("\n---------------------------------------------------\n");
	printf("\n=HOST STOP=\n");
	printf("Hosting Channel Name: %s\nNumber Of Viewers: %i\n",
		data.hosting_channel_name, data.number_of_viewers);
	printf("\n---------------------------------------------------\n");
}
void onUserBanCallback(ClearChat data) {
	printf("\n---------------------------------------------------\n");
	printf("\n=USER BAN=\n");
	printf("Banned User: %s\nDuration of Ban: %i\n", data.username, data.ban_duration);
	printf("\n---------------------------------------------------\n");
}
void onUserGivenModCallback(User data) {
	printf("\n---------------------------------------------------\n");
	printf("\n=USER MOD=\n");
	printf("Modded User: %s\n", data.display_name);
	printf("\n---------------------------------------------------\n");
}
void onCustomRewardCallback(PrivMsg data) {
	printf("\n---------------------------------------------------\n");
	printf("\n=CUSTOM REWARD ID=\n");
	printf("Custom Reward Id: %s\n", data.custom_reward_id);
	printf("\n---------------------------------------------------\n");
}
void onHighlightedMsgCallback(PrivMsg data) {
	printf("\n---------------------------------------------------\n");
	printf("\n=HIGHLIGHTED MESSAGE=\n");
	printf("Highlighted Message: %s\n", data.message);
	printf("\n---------------------------------------------------\n");
}
void onModChatCallback(PrivMsg data) {
	printf("\n---------------------------------------------------\n");
	printf("\n=MODCHAT=\n");
	printf("Moderator's Display Name: %s\nModerator's Message: %s\n", data.display_name, data.message);
	printf("\n---------------------------------------------------\n");
}

void setup() {
  	// Connect to wifi
  	WiFi.begin(ssid, password);

	// Check if connected to wifi
	while(WiFi.status() != WL_CONNECTED) {
		delay(2000);
	}

  	Serial.begin(9600);
  	Serial.println("Connected to Wifi.");

	//Connect to twitch via websockets
	twitch_api.connect_to_twitch_websocket();

	//Register generic callbacks
	twitch_api.onClearChat(onClearChatCallback);
	twitch_api.onClearMsg(onClearMsgCallback);
	twitch_api.onNotice(onNoticeCallback);
	twitch_api.onGlobalUserState(onGlobalUserStateCallback);
	twitch_api.onPrivMsg(onPrivMsgCallback);
	twitch_api.onRoomState(onRoomStateCallback);
	twitch_api.onUserNotice(onUserNoticeCallback);
	twitch_api.onUserState(onUserStateCallback);
	twitch_api.onHostTarget(onHostTargetCallback);

	//Register custom callbacks
	twitch_api.onChannelHosted(onChannelHostedCallback);
	twitch_api.onBits(onBitsCallback);
	twitch_api.onSub(onSubCallback);
	twitch_api.onReSub(onReSubCallback);
	twitch_api.onSubGift(onSubGiftCallback);
	twitch_api.onAnonSubGift(onAnonSubGiftCallback);
	twitch_api.onSubMysteryGift(onSubMysteryGiftCallback);
	twitch_api.onGiftPaidUpgrade(onGiftPaidUpgradeCallback);
	twitch_api.onRewardGift(onRewardGiftCallback);
	twitch_api.onAnonGiftPaidUpgrade(onAnonGiftPaidUpgradeCallback);
	twitch_api.onRaid(onRaidCallback);
	twitch_api.onUnRaid(onUnRaidCallback);
	twitch_api.onRitual(onRitualCallback);
	twitch_api.onBitsBadgeTier(onBitsBadgeTierCallback);
	twitch_api.onHostStart(onHostStartCallback);
	twitch_api.onHostStop(onHostStopCallback);
	twitch_api.onUserBan(onUserBanCallback);
	twitch_api.onUserGivenMod(onUserGivenModCallback);
	twitch_api.onCustomReward(onCustomRewardCallback);
	twitch_api.onHighlightedMsg(onHighlightedMsgCallback);
	twitch_api.onModChat(onModChatCallback);

	//Helper Functions
	/*
		A sub plan is returned on subs, resub, gift, etc. You can use the helper function to convert int value to string.
	*/
	printf("%s\n", twitch_api.sub_type_to_string(0));
	printf("%s\n", twitch_api.sub_type_to_string(1000));
	printf("%s\n", twitch_api.sub_type_to_string(2000));
	printf("%s\n", twitch_api.sub_type_to_string(3000));
	/*
		Emotes are returned when people say them in chat or in messages when donating. You can use the function to get the image url of the icon.
		Sizes: IMG_SMALL, IMG_MEDIUM, IMG_LARGE
	*/
	printf("Emote Image URL: %s\n", twitch_api.get_emote_image_url(301448586, TwitchWebsocketIRC::Emote_Image_Size::IMG_MEDIUM));
}

void loop() {
	if(twitch_api.available()) {
        twitch_api.poll();
    }
    delay(500);
}