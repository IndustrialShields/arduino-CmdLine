#ifndef __CmdLine_H__
#define __CmdLine_H__

#include <Arduino.h>

#define CMDLINE_BUFFER_SIZE 100

typedef void (*cmd_action_t)(const char *);

typedef struct {
	const char *command;
	cmd_action_t action;
} cmd_t;

class CmdLine {
	public:
		explicit CmdLine(Stream &stream);

	public:
		void begin(const cmd_t *commands, size_t num, bool enabled = true);
		void update();

		void enable();
		inline void disable() {
			_enabled = false;
		}
		inline bool isEnabled() const {
			return _enabled;
		}

	private:
		void reset();
		void processCommand();
		void prompt();

	private:
		Stream &_stream;
		char _buffer[CMDLINE_BUFFER_SIZE];
		size_t _bufferLen;

		const cmd_t *_commands;
		size_t _num;
		bool _enabled;
};

#endif // __CmdLine_H__
