# Twitch-IRC-API
<br>
Tested on Arduino ESP8266 NodeMCU 1.0<br>
See the example<br>
<br>
<b>Library Stats</b><br>
Ram Memory Footprint: 33.4KB<br>
Flash Memory Footprint: 415KB<br>
<br>
Uses a library dependency for the websocket transfers<br>
https://github.com/gilmaimon/ArduinoWebsockets<br>
<br>
<b>Twitch Docs.</b> The functions are a mirror of the twitch docs.<br>
https://dev.twitch.tv/docs/irc/guide<br>

    /* When requesting a EmoteURL use the enum types to determine the size.*/
    enum class Emote_Image_Size {
    IMG_SMALL = 1,
    IMG_MEDIUM = 2,
    IMG_LARGE = 3
    };

    /* Used for the /followers. Blocks users from chating that havent been following for x time.*/
    enum class Time_Type {
    MINUTES,
    HOURS,
    DAYS,
    WEEKS,
    MONTHS
    };

    /* Enum for commerical times */
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
    void onUserBan(void(*onUserBanPtr)(ClearChat arg)); // Happens when a user is banned from the channel.
    void onChannelHosted(void(*onChannelHostPtr)(Hostee arg)); //When the your channel is hosted(yours).
    void onBits(void(*onBitsPtr)(PrivMsg arg)); // Happens when someone donates bits
    void onSub(void(*onSubPtr)(UserNotice arg)); // Happens when someone subscribes
    void onReSub(void(*onReSubPtr)(UserNotice arg)); //Happens when someone resubscribes
    void onSubGift(void(*onSubGiftPtr)(UserNotice arg)); //Happens when someone gifts subs.
    void onAnonSubGift(void(*onAnonSubGiftPtr)(UserNotice arg)); //Happens when someone anonymously gifts subs
    void onSubMysteryGift(void(*onSubMysteryGiftPtr)(UserNotice arg)); // See docs
    void onGiftPaidUpgrade(void(*onGiftPaidUpgradePtr)(UserNotice arg)); // See docs
    void onRewardGift(void(*onRewardGiftPtr)(UserNotice arg)); // See docs
    void onAnonGiftPaidUpgrade(void(*onAnonGiftPaidUpgradePtr)(UserNotice arg)); // See docs
    void onRaid(void(*onRaidPtr)(UserNotice arg)); //When your channel is raided
    void onUnRaid(void(*onUnRaidPtr)(UserNotice arg)); //When your channel is unraided
    void onRitual(void(*onRitualPtr)(UserNotice arg)); 
    void onBitsBadgeTier(void(*onBitsBadgeTierPtr)(UserNotice arg)); //When someone gets their bits badge tier
    void onHostStart(void(*onHostStartPtr)(HostTarget arg)); //Happens when you host a channel
    void onHostStop(void(*onHostStopPtr)(HostTarget arg)); //Happens when you stop hosting a channel
    void onUserGivenMod(void(*onUserGivenModPtr)(User arg)); //When a user is promoted to mod
    void onCustomReward(void(*onCustomRewardPtr)(PrivMsg arg)); //When someone uses a custom channel point reward
    void onHighlightedMsg(void(*onHighlightedMsgPtr)(PrivMsg arg)); //When someone uses a highlighted message
    void onModChat(void(*onModChatPtr)(PrivMsg arg)); //When a moderator message is detected in chat.

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
