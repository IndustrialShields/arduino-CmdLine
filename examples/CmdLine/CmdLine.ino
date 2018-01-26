/*
   Copyright (c) 2018 Boot&Work Corp., S.L. All rights reserved

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU Lesser General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU Lesser General Public License for more details.

   You should have received a copy of the GNU Lesser General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <CmdLine.h>

// Create the command line and use the Serial port to introduce commands
CmdLine cmdline(Serial);

// Define the commands and associate them a function. The function is called when the
// command is typed.
// i.e. The "help" command calls the printHelpCmd function
// IMPORTANT: functions MUST be declared before the commands definition, MUST accept a
// 'const char *' argument and MUST return nothing (void)
// The function argument is an optional argument typed after the command
// i.e. > setOutput HIGH // 'arg' is "HIGH"
void printHelpCmd(const char *arg);
void setOutputCmd(const char *arg);
void getInputCmd(const char *arg);

const cmd_t commands[] = {
  {"help", printHelpCmd},
  {"setOutput", setOutputCmd},
  {"getInput", getInputCmd},
};

////////////////////////////////////////////////////////////////////////////////////////////////////
void setup() {
  // Start the Serial port to be used as a commandline
  Serial.begin(9600L);
  while (!Serial);

  // Start the cmdline to show the prompt and begin processing commands
  cmdline.begin(commands, sizeof(commands));
}

////////////////////////////////////////////////////////////////////////////////////////////////////
void loop() {
  // Update cmdline often to process commands written in the Serial port
  cmdline.update();
}

////////////////////////////////////////////////////////////////////////////////////////////////////
void printHelpCmd(const char *arg) {
  Serial.println("List of commands:");
  Serial.println("help                    print this help");
  Serial.println("setOutput HIGH|LOW      set Q0.0 to HIGH or LOW");
  Serial.println("getInput                get I0.0 digital value");
}

////////////////////////////////////////////////////////////////////////////////////////////////////
void setOutputCmd(const char *arg) {
  if (strcmp(arg, "HIGH") == 0) {
    digitalWrite(Q0_0, HIGH);
    Serial.println("Q0.0 set to HIGH");
  } else if (strcmp(arg, "LOW") == 0) {
    digitalWrite(Q0_0, LOW);
    Serial.println("Q0.0 set to LOW");
  }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
void getInputCmd(const char *arg) {
  int value = digitalRead(I0_0);
  if (value == HIGH) {
    Serial.println("I0.0 is HIGH");
  } else {
    Serial.println("I0.0 is LOW");
  }
}
