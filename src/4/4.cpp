#include <iostream>
#include <sstream>

#include <string>
#include <vector>

#include "lib.hpp"

struct Cell
{
    int num;
    bool marked;
};

int sumNonMarked( const std::vector< Cell >& board )
{
    int sum = 0;
    for( auto it = board.begin(); it != board.end(); ++it )
    {
        if( !it->marked )
            sum += it->num;
    }

    return sum;
}

auto parse( const std::string& input )
{
    std::stringstream inputStream{ input };

    std::string line;
    std::getline( inputStream, line );
    
    std::vector< int > nums = getNumbers< int >( line, ',' );
    
    std::vector< std::vector< Cell > > boards;
    while( inputStream >> std::ws )
    {
        std::vector< Cell > board;
        for( int i = 0; i != 5; ++i )
        {
            for( int j = 0; j != 5; ++j )
            {
                int num;
                inputStream >> num;
                board.emplace_back( num, false );
            }
        }
        
        boards.push_back( std::move( board ) );
    }
    
    return std::make_pair( nums, boards );
}

unsigned long long partOne( const std::string& input )
{
    std::pair< std::vector< int >, std::vector< std::vector< Cell > > > parsedInput = parse( input );
    std::vector< int > nums = parsedInput.first;
    std::vector< std::vector< Cell > > boards = parsedInput.second;

    std::vector< int > winnerScores;

    for( int i = 0; i != nums.size(); ++i )
    {
        int num = nums[ i ];
        
        for( auto boardsIt = boards.begin(); boardsIt != boards.end(); )
        {
            std::vector< Cell >& board = *boardsIt;
            
            for( auto it = board.begin(); it != board.end(); ++it )
            {
                if( it->num == num )
                    it->marked = true;
            }

            bool winner;
            for( int k = 0; k != 25; k += 5 )
            {
                winner = true;
                for( int l = 0; l != 5; ++l )
                {
                    if( !board[ k + l ].marked )
                    {
                        winner = false;
                        break;
                    }
                }
                
                if( winner )
                {
                    return sumNonMarked( board ) * num;
                }
            }
            
            if( winner ) continue;
            
            for( int k = 0; k != 5; ++k )
            {
                winner = true;
                for( int l = 0; l != 25; l += 5 )
                {
                    if( !board[ k + l ].marked )
                    {
                        winner = false;
                        break;
                    }
                }

                if( winner )
                {
                    return sumNonMarked( board ) * num;
                }
            }
            
            if( !winner ) ++boardsIt;
        }
    }
    
    return 0;
}

unsigned long long partTwo( const std::string& input )
{
    std::pair< std::vector< int >, std::vector< std::vector< Cell > > > parsedInput = parse( input );
    std::vector< int > nums = parsedInput.first;
    std::vector< std::vector< Cell > > boards = parsedInput.second;

    std::vector< int > winnerScores;

    for( int i = 0; i != nums.size(); ++i )
    {
        int num = nums[ i ];
        
        for( auto boardsIt = boards.begin(); boardsIt != boards.end(); )
        {
            std::vector< Cell >& board = *boardsIt;
            
            for( auto it = board.begin(); it != board.end(); ++it )
            {
                if( it->num == num )
                    it->marked = true;
            }

            bool winner;
            for( int k = 0; k != 25; k += 5 )
            {
                winner = true;
                for( int l = 0; l != 5; ++l )
                {
                    if( !board[ k + l ].marked )
                    {
                        winner = false;
                        break;
                    }
                }
                
                if( winner )
                {
                    winnerScores.push_back( sumNonMarked( board ) * num );
                    boardsIt = boards.erase( boardsIt );

                    break;
                }
            }
            
            if( winner ) continue;
            
            for( int k = 0; k != 5; ++k )
            {
                winner = true;
                for( int l = 0; l != 25; l += 5 )
                {
                    if( !board[ k + l ].marked )
                    {
                        winner = false;
                        break;
                    }
                }

                if( winner )
                {
                    winnerScores.push_back( sumNonMarked( board ) * num );
                    boardsIt = boards.erase( boardsIt );

                    break;
                }
            }
            
            if( !winner ) ++boardsIt;
        }
    }
    
    return winnerScores.back();;
}