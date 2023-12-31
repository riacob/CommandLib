/**
 * @file CommandLib.h
 * @author Riccardo Iacob
 * @brief
 * @version 0.1
 * @date 2023-08-09
 *
 * @copyright Copyright (c) 2023
 *
 */
#ifndef COMMANDLIB_H
#define COMMANDLIB_H

#include "PlatformWrapper.h"

class CommandLib
{
private:
    // Default: "AT", example: AT
    const char *commandPrefix = "AT";
    // Default: 3, size includes null-terminator
    size_t commandPrefixSize = 3;
    // Default: '+', example: AT+VERSION?
    char separatorPrefix = '+';
    // Default: ',', example AT+PARAMETERS=custom,custom2
    char separatorParameters = ',';
    // Default: '+', example: AT+PARAMETERS=custom,custom2
    char subcommandWrite = '=';
    // Default: '?', example: AT+VERSION?
    char subcommandRead = '?';
    // Default: '\0', example: AT+OFF
    char subcommandRun = '\0';
    void malloc_assert(void *ptr)
    {
        if (ptr == NULL)
        {
            w->debugln("[FATAL] Memory allocation failed");
        }
    }

public:
    /**
     * @brief Possible command types
     *
     */
    enum class CommandType
    {
        WRITE,
        READ,
        RUN
    };
    /**
     * @brief Possible parameter types
     *
     */
    enum class CommandParameterType
    {
        INTEGER,
        STRING,
        FLOAT
    };
    /**
     * @brief Single command parameter
     *
     */
    class CommandParameter
    {
    public:
        CommandParameterType type;
        char *name;
        size_t nameSize = 0;
        void *parameter;
        // only used if parametertype is string
        size_t stringSize = 0;
    };
    /**
     * @brief Single command
     *
     */
    class Command
    {
    public:
        // Write callback
        void (*wcallback)(CommandParameter *params, size_t paramsCount);
        bool wcallbackSet = false;
        // Read callback
        void (*rdcallback)();
        bool rdcallbackSet = false;
        // Run callback
        void (*runcallback)();
        bool runcallbackSet = false;
        // Type of the command
        CommandType type;
        // Command text
        char *command;
        // Command text size
        size_t commandSize = 0;
        // Command parameters
        CommandParameter *parameters;
        size_t parametersCount = 0;
    };

private:
    // Platform wrapper
    PlatformWrapper *w;
    // Array with all commands
    Command *commands;
    // Number of total commands
    size_t commandsCount;

public:
    /**
     * @brief Constructs a new CommandLib object
     *
     * @param wrapper: The desired platform wrapper. Example: CommandLib commandLib(new ArduinoWrapper())
     */
    CommandLib(PlatformWrapper *wrapper)
    {
        w = wrapper;
    }
    /**
     * @brief Registers a new command
     *
     * @param type: Type of command: WRITE, READ, RUN
     * @param command: Command text
     */
    void registerCommand(CommandType type, const char *command)
    {
        // Iterate through all commands to find possible duplicates
        // Note that a command can have two types with the same command name
        for (size_t i = 0; i < commandsCount; i++)
        {
            // strcmp returns 0 if strings match
            if (((strcmp(commands[i].command, command)) == 0) && (commands[i].type == type))
            {
                w->debug("[ERROR] Command ");
                w->debug(command);
                w->debugln(" already exists");
                return;
            }
        }

        // Register command
        // Expand the size of the command array
        commandsCount++;
        // Create a new Command object
        Command newCommand;
        // Assign type to Command
        newCommand.type = type;
        // Assign command size 0 to Command (useless?)
        newCommand.commandSize = 0;
        // Find size of command
        while (command[newCommand.commandSize] != '\0')
        {
            newCommand.commandSize++;
        }
        // Currently commandSize is an index, we need to convert it to size if the index (aka size) is greater than zero (also considers null-terminator)
        if (newCommand.commandSize > 0)
        {
            newCommand.commandSize++;
        }
        // Allocate enough memory to store the command then store it in Command :facepalm:
        newCommand.command = new char[newCommand.commandSize];
        malloc_assert(newCommand.command);
        memmove(newCommand.command, command, newCommand.commandSize * sizeof(char));
        // Move the old command array into the new command array
        // Also remembering that Command is not a single byte in size :secondfacepalm:
        Command *newCommands = new Command[commandsCount];
        malloc_assert(newCommands);
        memmove(newCommands, commands, (commandsCount - 1) * sizeof(Command));
        // Add the new command to the array
        newCommands[commandsCount - 1] = newCommand;
        // Delete the old commands array and reassign its pointer to the new array
        delete[] commands;
        commands = newCommands;
        w->debug("[INFO] Command ");
        w->debug(commands[commandsCount - 1].command);
        w->debug(" registered successfully at commands[");
        w->debug((char)commandsCount - 1 + 48);
        w->debugln("]");
    }
    void addParameter(const char *command, const char *parameterName, CommandParameterType parameterType)
    {
        // Iterate through all commands to find the desired one
        for (size_t i = 0; i < commandsCount; i++)
        {
            // strcmp returns 0 if strings match
            if ((strcmp(commands[i].command, command)) == 0)
            {
                // If command type does not accept parameters return
                if (commands[i].type != CommandType::WRITE)
                {
                    w->debug("[ERROR] Command ");
                    w->debug(command);
                    w->debugln(" is not of CommandType::WRITE and cannot accept parameters");
                    return;
                }
                else
                {
                    w->debug("[INFO] Command parameter ");
                    w->debug(parameterName);
                    w->debug(":");
                    switch (parameterType)
                    {
                    case CommandParameterType::INTEGER:
                    {
                        w->debug("INTEGER");
                        break;
                    }
                    case CommandParameterType::STRING:
                    {
                        w->debug("STRING");
                        break;
                    }
                    case CommandParameterType::FLOAT:
                    {
                        w->debug("FLOAT");
                        break;
                    }
                    }
                    w->debug(" added successfully to command ");
                    w->debugln(command);
                }
                // TODO check if parameter already exists
                // add command parameters
                commands[i].parametersCount++;
                // Insert new parameter to the right of the array
                CommandParameter *newParameters = new CommandParameter[commands[i].parametersCount];
                malloc_assert(newParameters);
                memmove(newParameters, commands[i].parameters, (commands[i].parametersCount - 1) * sizeof(CommandParameter));
                // if serial print sizeof commands[i].command bug sometimes disappears
                // Find the size of parameterName
                while (parameterName[newParameters[commands[i].parametersCount - 1].nameSize] != '\0')
                {
                    newParameters[commands[i].parametersCount - 1].nameSize++;
                }
                // Currently commandSize is an index, we need to convert it to size if the index (aka size) is greater than zero (also considers null-terminator)
                if (newParameters[commands[i].parametersCount - 1].nameSize > 0)
                {
                    newParameters[commands[i].parametersCount - 1].nameSize++;
                    // newParameters[commands[i].parametersCount - 1].nameSize++;
                }
                // This garbled mess to move parameterName to newParameters[currentparameter].name
                newParameters[commands[i].parametersCount - 1].name = new char[newParameters[commands[i].parametersCount - 1].nameSize];
                malloc_assert(newParameters[commands[i].parametersCount - 1].name);
                memmove(newParameters[commands[i].parametersCount - 1].name, parameterName, (newParameters[commands[i].parametersCount - 1].nameSize) * sizeof(char));
                // Save the parameter type; we will need it for casting during the parsing
                switch (parameterType)
                {
                case CommandParameterType::INTEGER:
                {
                    newParameters[commands[i].parametersCount - 1].type = CommandParameterType::INTEGER;
                    ;
                    break;
                }
                case CommandParameterType::STRING:
                {
                    newParameters[commands[i].parametersCount - 1].type = CommandParameterType::STRING;
                    break;
                }
                case CommandParameterType::FLOAT:
                {
                    newParameters[commands[i].parametersCount - 1].type = CommandParameterType::FLOAT;
                    break;
                }
                }
                // compiler should automatically delete?? very weird bug
                // delete[] commands[i].parameters;
                commands[i].parameters = newParameters;
                return;
            }
        }
        // No valid command was found
        w->debug("[ERROR] No command exists with the name");
        w->debugln(command);
        return;
    }
    void debugCommand(const char *command)
    {
        // Iterate through all commands to find possible duplicates
        for (size_t i = 0; i < commandsCount; i++)
        {
            // strcmp returns 0 if strings match
            if ((strcmp(commands[i].command, command)) == 0)
            {
                w->debug("[INFO] Debugging of command ");
                w->debug(command);
                w->debug(" -> ");
                w->debug(commandPrefix);
                w->debug(separatorPrefix);
                w->debug(command);
                switch (commands[i].type)
                {
                case CommandType::READ:
                {
                    w->debug(subcommandRead);
                    break;
                }
                case CommandType::WRITE:
                {
                    w->debug(subcommandWrite);
                    if (commands[i].parametersCount == 0)
                    {
                        w->debug("<no parameters>");
                        break;
                    }
                    for (size_t j = 0; j < commands[i].parametersCount; j++)
                    {
                        // print "<parameter_name:parameter_type>"
                        w->debug("<");
                        w->debug(commands[i].parameters[j].name);
                        w->debug(":");
                        switch (commands[i].parameters[j].type)
                        {
                        case CommandParameterType::INTEGER:
                        {
                            w->debug("INTEGER");
                            break;
                        }
                        case CommandParameterType::STRING:
                        {
                            w->debug("STRING");
                            break;
                        }
                        case CommandParameterType::FLOAT:
                        {
                            w->debug("FLOAT");
                            break;
                        }
                        }
                        w->debug(">");
                        // Print comma if the current parameter is not the last one
                        if (j < commands[i].parametersCount - 1)
                        {
                            w->debug(separatorParameters);
                        }
                    }
                    break;
                }
                case CommandType::RUN:
                {
                    w->debug(subcommandRun);
                    break;
                }
                }
                w->debug('\n');
                return;
            }
        }
    }
    void setWriteCallback(const char *command, void (*callback)(CommandParameter *params, size_t paramsCount))
    {
        // Iterate through all commands to find the desired one
        for (size_t i = 0; i < commandsCount; i++)
        {
            // TODO make sure type is WRITE
            // strcmp returns 0 if strings match
            if ((strcmp(commands[i].command, command)) == 0)
            {
                commands[i].wcallback = callback;
                commands[i].wcallbackSet = true;
            }
        }
    }
    void setReadCallback(const char *command, void(callback)())
    {
        // Iterate through all commands to find the desired one
        for (size_t i = 0; i < commandsCount; i++)
        {
            // TODO make sure type is READ
            // strcmp returns 0 if strings match
            if ((strcmp(commands[i].command, command)) == 0)
            {
                commands[i].rdcallback = callback;
                commands[i].rdcallbackSet = true;
            }
        }
    }
    void setRunCallback(const char *command, void(callback)())
    {
        // Iterate through all commands to find the desired one
        for (size_t i = 0; i < commandsCount; i++)
        {
            // TODO make sure type is RUN
            // strcmp returns 0 if strings match
            if ((strcmp(commands[i].command, command)) == 0)
            {
                commands[i].runcallback = callback;
                commands[i].runcallbackSet = true;
            }
        }
    }
    /**
     * @brief Handles and parses commands. Should be run in main loop.
     *
     */
    void handleCommands()
    {
        // Read the incoming serial data
        static char cmdbuf[128] = {0};
        static size_t cmdsize = 0;
        cmdsize = w->readBytesUntil(cmdbuf, '\n');
        // Respond with OK
        if (cmdsize)
        {
            w->writeBytes("OK", 3, "\n");
        }
    }
};

#endif