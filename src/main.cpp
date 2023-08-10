/**
 * @file main.cpp
 * @author Riccardo Iacob
 * @brief Main (test) file
 * @version 0.1
 * @date 2023-08-10
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#include <Arduino.h>
#include "ArduinoWrapper.h"
#include "CommandLib.h"

CommandLib commandLib(new ArduinoWrapper());

void cmdCallback(CommandLib::CommandParameter *params, size_t paramsCount) {
    Serial.println("callback yo");
    for (size_t i = 0; i < paramsCount; i++) {
        Serial.println(params[0].name);
    }
}

void setup()
{
    Serial.begin(9600);

    commandLib.registerCommand(CommandLib::CommandType::READ, "INFO");
    commandLib.registerCommand(CommandLib::CommandType::WRITE, "TESTS");
    commandLib.registerCommand(CommandLib::CommandType::RUN, "SENSORS");

    commandLib.addParameter("TESTS","P1",CommandLib::CommandParameterType::STRING);
    commandLib.addParameter("TESTS","P2",CommandLib::CommandParameterType::INTEGER);
    commandLib.addParameter("TESTS","P367",CommandLib::CommandParameterType::FLOAT);
    commandLib.addParameter("TESTS","P4432",CommandLib::CommandParameterType::STRING);

    commandLib.setWriteCallback("TESTS",cmdCallback);

    commandLib.debugCommand("INFO");
    commandLib.debugCommand("TESTS");
    commandLib.debugCommand("SENSORS");
}

void loop()
{
    commandLib.handleCommands();
}