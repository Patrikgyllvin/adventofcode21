#include <iostream>
#include <sstream>

#include <string>
#include <vector>
#include <queue>

#include <functional>
#include <limits>

#include "lib.hpp"

auto parse( const std::string& input, int& w, int& h )
{
    std::vector< int > board;
    
    w = input.find( '\n' );
    for( const auto& c : input )
    {
        if( c == '\n' ) continue;

        board.push_back( c - '0' );
    }
    h = board.size() / w;
    
    return board;
}

auto tileParse( const std::string& input, int num, int& w, int& h )
{
    std::string tmp, full;
    
    for( int i = 0; i != num; ++i )
        tmp += ( input + '\n' );
    
    std::string line;
    std::stringstream sstream{ tmp };
    int inc = 0;
    while( std::getline( sstream, line ) )
    {
        for( int i = 0; i != num; ++i )
        {
            std::string incremented;
            for( std::size_t j = 0; j != line.size(); ++j )
            {
                int val = ( line[ j ] - '0' ) + i + inc / 100;
                while( val >= 10 ) val -= 9;
                incremented.push_back( val + '0' );
            }
            full += incremented;
        }
        full += '\n';
        inc++;
    }

    return parse( full, w, h );
}

std::vector< int > getDirections( int curr, int w, int h )
{
    std::vector< int > dirs;
    int x = curr % w, y = curr / w;

    if( x != w - 1 )
        dirs.push_back( curr + 1 );
    if( x != 0 )
        dirs.push_back( curr - 1 );
    if( y != h - 1 )
        dirs.push_back( curr + w );
    if( y != 0 )
        dirs.push_back( curr - w );
    
    return dirs;
}

std::vector< int > dijkstra( const std::vector< int >& board, int w, int h )
{
    std::vector< int > dist( board.size(), std::numeric_limits< int >::max() );
    std::priority_queue<
        std::pair< int, int >,
        std::vector< std::pair< int, int > >,
        std::greater< std::pair< int, int > > > q;

    dist[ 0 ] = 0;
    q.emplace( 0, 0 );

    while( !q.empty() )
    {
        int n = q.top().second;
        q.pop();

        std::vector< int > dirs = getDirections( n, w, h );
        for( const int& dir : dirs )
        {
            int distance = dist[ n ] + board[ dir ];
            if( distance < dist[ dir ] )
            {
                dist[ dir ] = distance;
                q.emplace( distance, dir );
            }
        }
    }
    
    return dist;
}

unsigned long long partOne( const std::string& input )
{
    int w, h;
    std::vector< int > board = parse( input, w, h );

    return dijkstra( board, w, h )[ board.size() - 1 ];
}

unsigned long long partTwo( const std::string& input )
{
    int w, h;
    std::vector< int > board = tileParse( input, 5, w, h );

    return dijkstra( board, w, h )[ board.size() - 1];
}