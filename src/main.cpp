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

    commandLib.debugCommand("INFO");
    commandLib.debugCommand("TESTS");
    commandLib.debugCommand("SENSORS");
}

void loop()
{
}