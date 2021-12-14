#include <iostream>
#include <sstream>

#include <string>
#include <vector>
#include <unordered_set>

#include <algorithm>

#include "lib.hpp"

/***
 * Terrible algorithm ahead
 */

const std::unordered_map< std::string, int > segmentsToNum{
    { "012456", 0 },
    { "25", 1 },
    { "02346", 2 },
    { "02356", 3 },
    { "1235", 4 },
    { "01356", 5 },
    { "013456", 6 },
    { "025", 7 },
    { "0123456", 8 },
    { "012356", 9 }
};

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

std::vector< char > findMapping( std::vector< std::string >& digits )
{
    std::sort(
        digits.begin(),
        digits.end(),
        []( const std::string& a, const std::string& b ){ return a.size() < b.size(); } );

    std::vector< std::vector< char > > mappings;

    for( const std::string& pattern : digits )
    {
        std::vector< std::vector< char > > nextMappings;

        std::vector< int > digitIndices;
        if( pattern.size() == 2 )
            digitIndices = { 2, 5 };
        else if( pattern.size() == 3 )
            digitIndices = { 0, 2, 5 };
        else if( pattern.size() == 4 )
            digitIndices = { 1, 2, 3, 5 };
        else if( pattern.size() == 7 )
            digitIndices = { 0, 1, 2, 3, 4, 5, 6 };

        do
        {
            if( mappings.empty() )
            {
                std::vector< char > mapping( 7 );
                for( int i = 0; i != digitIndices.size(); ++i )
                {
                    mapping[ digitIndices[ i ] ] = pattern[ i ];
                }
                
                nextMappings.emplace_back( std::move( mapping ) );
            }
            else
            {
                for( std::vector< char > mapping : mappings )
                {
                    bool ok = true;
                    for( int i = 0; i != digitIndices.size(); ++i )
                    {
                        char& c = mapping[ digitIndices[ i ] ];
                        if( c == '\0' )
                            c = pattern[ i ];
                        else if( c != pattern[ i ] )
                        {
                            ok = false;
                            break;
                        }
                    }

                    if( ok )
                        nextMappings.emplace_back( std::move( mapping ) );
                }
            }
        } while( std::next_permutation( digitIndices.begin(), digitIndices.end() ) );
        
        mappings = std::vector( std::move( nextMappings ) );
    }
    
    for( const std::vector< char >& mapping : mappings )
    {
        for( int i = 0; i != mapping.size(); ++i )
            std::cout << i << ' ' << mapping[ i ] << '\n';
        std::cout << '\n';
    }
    
    for( const std::vector< char >& mapping : mappings )
    {
        bool ok = true;
        for( const std::string& pattern : digits )
        {
            if( pattern.size() != 5 && pattern.size() != 6 ) continue;

            std::string segments;
            // Bad search, needs reworking algorithm, data structures
            for( int i = 0; i != mapping.size(); ++i )
                if( pattern.find( mapping[ i ] ) != std::string::npos )
                    segments += std::to_string( i );

            // Even worse. This could probably be the entire algorithm, somehow
            std::sort( segments.begin(), segments.end() );
            if( !segmentsToNum.count( segments ) )
            {
                ok = false;
                break;
            }
        }
        
        if( ok ) return mapping;
    }

    return std::vector< char >();
}

int calcValue( const std::vector< char >& mapping, const std::vector< std::string >& outputs )
{
    int sum = 0;
    for( const std::string& output : outputs )
    {
        // This again, really terrible
        std::string segments;
        for( int i = 0; i != mapping.size(); ++i )
            if( output.find( mapping[ i ] ) != std::string::npos )
                segments += std::to_string( i );

        std::cout << segments << '\n';
        std::sort( segments.begin(), segments.end() );
        
        // Throws std::out_of_range
        sum = sum * 10 + segmentsToNum.at( segments );
    }

    return sum;
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
    
    int sum = 0;
    for( int i = 0; i != digitsVec.size(); ++i )
    {
        std::vector< char > mapping = findMapping( digitsVec[ i ] );
        sum += calcValue( mapping, outputsVec[ i ] );
    }

    return sum;
}