#include <iostream>
#include <sstream>

#include <string>
#include <vector>
#include <bitset>

#include "lib.hpp"

std::string binaryFromHexChar( char c )
{
    switch( c )
    {
    case '0': return "0000";
    case '1': return "0001";
    case '2': return "0010";
    case '3': return "0011";
    case '4': return "0100";
    case '5': return "0101";
    case '6': return "0110";
    case '7': return "0111";
    case '8': return "1000";
    case '9': return "1001";
    case 'A': return "1010";
    case 'B': return "1011";
    case 'C': return "1100";
    case 'D': return "1101";
    case 'E': return "1110";
    case 'F': return "1111";
    }
    
    return "invalid";
}

std::string convertToBinary( const std::string& hexStr )
{
    std::string binStr;
    for( const char& c : hexStr )
        binStr += binaryFromHexChar( c );
    
    return binStr;
}

struct PacketHeader
{
    unsigned int version : 3, type : 3;
};

struct LiteralPacket
{
    PacketHeader header;
    unsigned long long num;
};

auto parse( const std::string& input )
{
    std::stringstream sstream{ convertToBinary( input ) };

    std::bitset< 3 > bits;
    while( sstream >> bits )
    {
        PacketHeader header;
        header.version = bits.to_ulong();
        sstream >> bits;
        header.type = bits.to_ulong();

        std::cout << header.version << ' ' << header.type << '\n';
        
        if( header.type == 4 )
        {
            LiteralPacket packet{ .header = header };

            bool read = true;
            while( read )
            {
                std::bitset< 5 > groupBits;
                unsigned int group;
                sstream >> groupBits;
                group = groupBits.to_ulong();
                
                packet.num = ( packet.num << 4 ) | ( group & 0xF ); 
                read = group & 0x10;
            }
            
            std::cout << packet.num << '\n';
        }
    }
}

unsigned long long partOne( const std::string& input )
{
    parse( input );

    return 0;
}

unsigned long long partTwo( const std::string& input )
{
    //parse( input );

    return 0;
}