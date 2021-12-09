#include <sstream>

#include <string>
#include <vector>

#include "lib.hpp"

unsigned long long partOne( const std::string& input )
{
    std::stringstream inputStream{ input };

    int horiz = 0, depth = 0;

    std::string command;
    int num;
    while( inputStream >> command )
    {
        inputStream >> num;
        
        if( command == "forward" )
            horiz += num;
        else if( command == "up" )
            depth -= num;
        else if( command == "down" )
            depth += num;
    }
    
    return horiz * depth;
}

unsigned long long partTwo( const std::string& input )
{
    std::stringstream inputStream{ input };

    int horiz = 0, depth = 0, aim = 0;

    std::string command;
    int num;
    while( inputStream >> command )
    {
        inputStream >> num;
        
        if( command == "forward" )
        {
            horiz += num;
            depth += aim * num;
        }
        else if( command == "up" )
            aim -= num;
        else if( command == "down" )
            aim += num;
    }
    
    return horiz * depth;
}