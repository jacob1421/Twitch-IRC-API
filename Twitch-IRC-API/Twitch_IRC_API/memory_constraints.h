#ifndef BOT_MEMORY_CONSTRAINTS_H
#define BOT_MEMORY_CONSTRAINTS_H

/* Change these to satisfy smaller memory requirements. Defaults should be fine for most devices. */
#if !defined(MAX_EMOTES)
	#define MAX_EMOTES 30
#endif

#if !defined(MAX_BADGES)
	#define MAX_BADGES 15
#endif

#if !defined(MAX_CHAT_EMOTES)
	#define MAX_CHAT_EMOTES 30 
#endif

#if !defined(MAX_EMOTE_URL_STRING_LEN)
	#define MAX_EMOTE_URL_STRING_LEN 71
#endif

#if !defined(MAX_COMMAND_STRING_LEN)
	#define MAX_COMMAND_STRING_LEN 512
#endif

//Cross compatibility Windows and Linux
#ifdef Arduino_h
	#define printf Serial.printf
	#define strtok_s strtok_r
#else
	#define printf printf
#endif

#endif