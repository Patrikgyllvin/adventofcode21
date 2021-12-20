#include <iostream>
#include <sstream>

#include <memory>

#include <string>
#include <vector>
#include <bitset>

#include <limits>

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
    
    return "";
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

struct Packet
{
    virtual ~Packet() = default;
    PacketHeader header;
};

struct LiteralPacket : Packet
{
    unsigned long long num;
};

struct OperatorPacket : Packet
{
    std::vector< int > subPackets;
};

class PacketReader
{
public:
    PacketReader( const std::string& input )
    {
        parse( input );
    }

    ~PacketReader()
    {}
    
    int versionSum()
    {
        int sum = 0;
        for( const auto& packet : packets )
        {
            sum += packet->header.version;
        }
        
        return sum;
    }
    
    unsigned long long evaluate()
    {
        return value( 0 );
    }

private:
    void parse( const std::string& input )
    {
        std::string bitStr = convertToBinary( input );
        std::stringstream sstream{ bitStr };
        
        std::size_t trailingZeroes = bitStr.size() - ( bitStr.rfind( '1' ) + 1 ),
                    bitsToParse = bitStr.size();

        while( bitsToParse > trailingZeroes )
            bitsToParse -= parsePacket( sstream ).second;
    }
    
    std::pair< int, int > parsePacket( std::stringstream& sstream )
    {
        int parsedBits = 0, parsedPackets = 0;

        std::bitset< 3 > bits;

        PacketHeader header;
        sstream >> bits;
        parsedBits += 3;
        header.version = bits.to_ulong();

        sstream >> bits;
        parsedBits += 3;
        header.type = bits.to_ulong();
        
        if( header.type == 4 )
        {
            LiteralPacket& packet = *static_cast< LiteralPacket* >(
                packets.emplace_back( std::make_unique< LiteralPacket >() ).get() );
            packet.header = header;

            bool read = true;
            while( read )
            {
                std::bitset< 5 > groupBits;
                unsigned int group;
                sstream >> groupBits;
                parsedBits += 5;
                group = groupBits.to_ulong();

                packet.num = ( packet.num << 4 ) | ( group & 0xF ); 
                read = group & 0x10;
            }
        }
        else
        {
            OperatorPacket& packet = *static_cast< OperatorPacket* >(
                packets.emplace_back( std::make_unique< OperatorPacket >() ).get()
            );
            packet.header = header;

            std::bitset< 1 > lengthID;
            sstream >> lengthID;
            parsedBits++;
            
            if( !lengthID.to_ulong() )
            {
                std::bitset< 15 > lengthBits;
                sstream >> lengthBits;
                parsedBits += 15;
                unsigned long len = lengthBits.to_ulong();

                for( unsigned long i = 0; i != len; )
                {
                    packet.subPackets.push_back( packets.size() );
                    auto [ pkts, bts ] = parsePacket( sstream );
                    i += bts;
                    parsedBits += bts;
                    parsedPackets += pkts;
                }
            }
            else
            {
                std::bitset< 11 > numBits;
                sstream >> numBits;
                parsedBits += 11;
                unsigned long numPackets = numBits.to_ulong();
                
                for( unsigned long i = 0; i != numPackets; ++i )
                {
                    packet.subPackets.push_back( packets.size() );
                    auto [ pkts, bts ] = parsePacket( sstream );
                    parsedBits += bts;
                    parsedPackets += pkts;
                }
            }
        }
        
        parsedPackets++;
        
        return std::make_pair( parsedPackets, parsedBits );
    }
    
    unsigned long long value( int index )
    {
        Packet* packet = packets[ index ].get();
        if( packet->header.type == 4 )
        {
            LiteralPacket& litPacket = *static_cast< LiteralPacket* >( packet );
            return litPacket.num;
        }
        else
        {
            OperatorPacket& opPacket = *static_cast< OperatorPacket* >( packet );
            
            switch( opPacket.header.type )
            {
            case 0:
            {
                unsigned long long sum = 0;
                for( const int& i : opPacket.subPackets )
                    sum += value( i );
                return sum;
            }
            case 1:
            {
                unsigned long long prod = 1;
                for( const int& i : opPacket.subPackets )
                    prod *= value( i );
                return prod;
            }
            case 2:
            {
                unsigned long long min = std::numeric_limits< unsigned long long >::max();
                for( const int& i : opPacket.subPackets )
                {
                    unsigned long long val = value( i );
                    if( val < min )
                        min = val;
                }
                return min;
            }
            case 3:
            {
                unsigned long long max = std::numeric_limits< unsigned long long >::min();
                for( const int& i : opPacket.subPackets )
                {
                    unsigned long long val = value( i );
                    if( val > max )
                        max = val;
                }
                return max;
            }
            case 5:
                return value( opPacket.subPackets[ 0 ] ) > value( opPacket.subPackets[ 1 ] );
            case 6:
                return value( opPacket.subPackets[ 0 ] ) < value( opPacket.subPackets[ 1 ] );
            case 7:
                return value( opPacket.subPackets[ 0 ] ) == value( opPacket.subPackets[ 1 ] );
            }
        }
        
        return 0;
    }

    std::vector< std::unique_ptr< Packet > > packets;
};


unsigned long long partOne( const std::string& input )
{
    PacketReader reader{ input };

    return reader.versionSum();
}

unsigned long long partTwo( const std::string& input )
{
    PacketReader reader{ input };

    return reader.evaluate();
}
