#include <iostream>

#include <string>

#include <vector>
#include <deque>
#include <unordered_set>

#include "lib.hpp"

auto parse( const std::string& input, std::deque< std::unordered_set< int > >& energies )
{
    int w, h = 0;
    for( int i = 0; i != input.size(); ++i )
    {
        if( input[ i ] == '\n' )
        {
            if( h == 0 )
                w = i;
            h++;
            continue;
        }

        energies[ input[ i ] - '0' ].insert( i - h );
    }
    
    return std::make_pair( w, h );
}

void incrementAll( std::deque< std::unordered_set< int > >& energies )
{
    energies.push_front( std::move( energies.back() ) );
    energies.pop_back();
}

std::unordered_set< int > getNeighbors( int i, int w, int h )
{
    int x = i % w, y = i / w;
    std::unordered_set< int > indices;

    if( x != 0 && y != 0 ) indices.insert( i - 1 - w );
    if( x != w - 1 && y != 0 ) indices.insert( i + 1 - w );
    if( x != 0 && y != h - 1 ) indices.insert( i - 1 + w );
    if( x != w - 1 && y != h - 1 ) indices.insert( i + 1 + w );

    if( x != 0 ) indices.insert( i - 1 );
    if( x != w - 1 ) indices.insert( i + 1 );
    if( y != 0 ) indices.insert( i - w );
    if( y != h - 1 ) indices.insert( i + w );
    
    return indices;
}

void handleFlashing(
    std::deque< std::unordered_set< int > >& energies,
    std::unordered_set< int >& flashed,
    std::unordered_set< int > flashing,
    int w, int h )
{
    if( flashing.empty() )
        return;

    std::unordered_set< int > willFlash;
    
    for( auto i : flashing )
        flashed.insert( i );
    
    for( auto i : flashing )
    {
        std::unordered_set< int > neighbors = getNeighbors( i, w, h );

        for( int j = 0; j != energies.size(); ++j )
        {
            if( neighbors.empty() )
                break;

            for( auto it = neighbors.begin(); it != neighbors.end(); )
            {
                if( flashed.count( *it ) )
                {
                    it = neighbors.erase( it );
                    continue;
                }

                if( energies[ j ].count( *it ) )
                {
                    energies[ j ].erase( *it );
                    
                    int newEnergy = ( j + 1 ) % 10;
                    energies[ newEnergy ].insert( *it );

                    if( newEnergy == 0 )
                    {
                        willFlash.insert( *it );
                        flashed.insert( *it );
                    }
            
                    it = neighbors.erase( it );
                }
                else
                {
                    ++it;
                }
            }
        }
    }
    
    handleFlashing( energies, flashed, willFlash, w, h );
}

unsigned long long partOne( const std::string& input )
{
    std::deque< std::unordered_set< int > > energies( 10, std::unordered_set< int >() );
    std::pair< int, int > wh = parse( input, energies );
     
    int flashes = 0;
    for( int i = 0; i != 100; ++i )
    {
        incrementAll( energies );
        std::unordered_set< int > flashed;
        handleFlashing( energies, flashed, energies.front(), wh.first, wh.second );
        flashes += flashed.size();
    }

    return flashes;
}

unsigned long long partTwo( const std::string& input )
{
    std::deque< std::unordered_set< int > > energies( 10, std::unordered_set< int >() );
    std::pair< int, int > wh = parse( input, energies );
     
    for( int i = 0; true; ++i )
    {
        incrementAll( energies );
        std::unordered_set< int > flashed;
        handleFlashing( energies, flashed, energies.front(), wh.first, wh.second );

        if( energies.front().size() == ( wh.first * wh.second ) )
            return i + 1;
    }

    return 0;
}