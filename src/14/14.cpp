#include <iostream>
#include <sstream>

#include <string>
#include <unordered_map>

#include <algorithm>

#include "lib.hpp"

auto parse( const std::string& input, std::unordered_map< std::string, char >& rules )
{
    std::size_t pos = input.find( '\n' );
    std::stringstream sstream{ input.substr( pos + 2 ) };
    
    std::string line;
    const std::string arrow = " -> ";
    while( std::getline( sstream, line ) )
    {
        std::size_t arrowPos = line.find( arrow );
        rules.emplace( line.substr( 0, arrowPos ),
                            line.substr( arrowPos + arrow.length() )[ 0 ] );
    }

    return input.substr( 0, pos );
}

std::unordered_map< char, long long > performSteps(
    const std::string& polymer,
    const std::unordered_map< std::string, char >& rules,
    int steps )
{
    std::unordered_map< char, long long > res;
    std::unordered_map< std::string, long long > toExpand;

    res[ polymer.back() ]++;
    // Find pairs
    for( std::size_t i = 0; i != polymer.length() - 1; ++i )
    {
        std::string pair{ polymer[ i ], polymer[ i + 1 ] };
        if( rules.count( pair ) )
            toExpand[ pair ]++;
        else
            res[ pair[ 0 ] ]++;
    }
    
    // Do expansions
    for( int i = 0; i != steps; ++i )
    {
        std::unordered_map< std::string, long long > tmp;
        for( auto& [ key, value ] : toExpand )
        {
            const char& c = rules.at( key );
            std::string pair1{ key[ 0 ], c }, pair2{ c, key[ 1 ] };
            
            if( rules.count( pair1 ) )
                tmp[ pair1 ] += value;
            else
                res[ pair1[ 0 ] ] += value;

            if( rules.count( pair2 ) )
                tmp[ pair2 ] += value;
            else
                res[ pair2[ 0 ] ] += value;
        }
        
        toExpand = std::move( tmp );
    }
    
    for( auto& [ key, value ] : toExpand )
    {
        res[ key[ 0 ] ] += value;
    }
    
    return res;
}

unsigned long long partOne( const std::string& input )
{
    std::unordered_map< std::string, char > rules;
    std::string polymer = parse( input, rules );
    
    std::unordered_map< char, long long > freqs = performSteps( polymer, rules, 10 );
    
    std::vector< std::pair< char, long long > > freqVec( freqs.begin(), freqs.end() );
    std::sort( freqVec.begin(), freqVec.end(),
        []( auto& a, auto& b ){ return a.second > b.second; } );
    
    return freqVec.front().second - freqVec.back().second;
}

unsigned long long partTwo( const std::string& input )
{
    std::unordered_map< std::string, char > rules;
    std::string polymer = parse( input, rules );
    
    std::unordered_map< char, long long > freqs = performSteps( polymer, rules, 40 );
    
    std::vector< std::pair< char, long long > > freqVec( freqs.begin(), freqs.end() );
    std::sort( freqVec.begin(), freqVec.end(),
        []( auto& a, auto& b ){ return a.second > b.second; } );
    
    return freqVec.front().second - freqVec.back().second;
}