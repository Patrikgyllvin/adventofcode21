#include <iostream>
#include <sstream>

#include <string>
#include <vector>

#include <unordered_set>

#include "lib.hpp"

auto parse( const std::string& input,
            std::vector< std::vector< std::string > >& digitsVec,
            std::vector< std::vector< std::string > >& outputsVec )
{
    std::stringstream sstream{ input };
    std::string line;

    while( std::getline( sstream, line ) )
    {
        std::size_t pos = line.find( '|' );
        std::string part1 = line.substr( 0, pos - 1 ), part2 = line.substr( pos + 2 );

        std::vector< std::string > tmp;
        while( ( pos = part1.find( ' ' ) ) != std::string::npos )
        {
            tmp.emplace_back( part1.substr( 0, pos ) );
            part1.erase( 0, pos + 1 );
        }
        digitsVec.emplace_back( std::move( tmp ) );

        std::vector< std::string > tmp1;
        while( ( pos = part2.find_first_of( " \n" ) ) != std::string::npos )
        {
            tmp1.emplace_back( part2.substr( 0, pos ) );
            part2.erase( 0, pos + 1 );
        }
        tmp1.emplace_back( part2.substr( 0 ) );
        outputsVec.emplace_back( std::move( tmp1 ) );
    }
}

unsigned long long partOne( const std::string& input )
{
    std::vector< std::vector< std::string > > digitsVec, outputsVec;
    parse( input, digitsVec, outputsVec );
    
    std::unordered_set< int > numSegments { 2, 4, 3, 7 };
    
    int sum = 0;
    for( int i = 0; i != outputsVec.size(); ++i )
    {
        for( int j = 0; j != outputsVec[ i ].size(); ++j )
        {
            if( numSegments.count( outputsVec[ i ][ j ].size() ) )
                sum++;
        }
    }

    return sum;
}

unsigned long long partTwo( const std::string& input )
{
    std::vector< std::vector< std::string > > digitsVec, outputsVec;
    parse( input, digitsVec, outputsVec );

    return 0;
}