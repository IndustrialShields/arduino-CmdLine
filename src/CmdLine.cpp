#include "CmdLine.h"

////////////////////////////////////////////////////////////////////////////////////////////////////
CmdLine::CmdLine(Stream &stream) : _stream(stream) {
	_bufferLen = 0;
	_commands = nullptr;
	_num = 0;
	_enabled = false;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
void CmdLine::begin(const cmd_t *commands, size_t num, bool enabled) {
	_bufferLen = 0;
	_commands = commands;
	_num = num;
	_enabled = enabled;

	if (_enabled) {
		prompt();
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////
void CmdLine::update() {
	while (_stream.available()) {
		char in = _stream.read();

		if (_enabled) {
			_stream.write(in);
			if (_bufferLen == 0 && in == ' ') {
				// trim
			} else if (in == '\r' || in == '\n') {
				if (in == '\r') {
					_stream.write('\n');
				} else if (_bufferLen == 0) {
					// received "\n" alone: ignore it
					break;
				}

				// finish command
				_buffer[_bufferLen] = '\0';

				// process it
				if (_bufferLen > 0) {
					processCommand();
				}

				// start again
				prompt();
				break;
			} else if ((in == 0x7F) || (in == '\b')) {
				// backspace
				if (_bufferLen > 0) {
					--_bufferLen;
					_stream.write("\b \b", 3);
				}
			} else if (_bufferLen < CMDLINE_BUFFER_SIZE - 1) {
				_buffer[_bufferLen++] = in;
			}
		}
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////
void CmdLine::processCommand() {
	char *ptr = _buffer;
	char *arg = nullptr;
	while (*ptr != '\0') {
		if (*ptr == ' ') {
			*ptr++ = '\0';
			break;
		}
		++ptr;
	}
	while (*ptr == ' ') {
		++ptr;
	}
	arg = ptr;
	if (_commands && (_num > 0)) {
		const cmd_t *command = _commands;
		for (size_t i = 0; i < _num; ++i) {
			if (strcmp(command->command, _buffer) == 0) {
				command->action(arg);
				return;
			}
			++command;
		}
		_stream.println("Invalid command");
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////
void CmdLine::prompt() {
	_bufferLen = 0;
	_stream.print("> ");
}

////////////////////////////////////////////////////////////////////////////////////////////////////
void CmdLine::enable() {
	if (!_enabled) {
		_enabled = true;
		prompt();
	}
}
