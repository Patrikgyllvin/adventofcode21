#include <sstream>

#include <string>
#include <vector>
#include <stack>

#include <algorithm>

#include "lib.hpp"

auto parse( const std::string& input )
{
    std::vector< std::string > lines;
    std::stringstream stream{ input };
    
    std::string line;
    while( std::getline( stream, line ) )
        lines.push_back( line );

    return lines;
}

unsigned long long checkScore( const std::vector< std::string >& lines, bool considerIncomplete )
{
    int score = 0;
    std::vector< unsigned long long > scores;

    for( int i = 0; i != lines.size(); ++i )
    {
        std::stack< char > stack;
        for( int j = 0; j != lines[ i ].size(); ++j )
        {
            const char& c = lines[ i ][ j ];

            if( c == '(' || c == '[' || c == '{' || c == '<' )
                stack.push( lines[ i ][ j ] );
            else
            {
                const char& top = stack.top();
                if( top == c - 2 || top == c - 1 )
                    stack.pop();
                else
                {
                    if( c == ')' )
                        score += 3;
                    else if( c == ']' )
                        score += 57;
                    else if( c == '}' )
                        score += 1197;
                    else if( c == '>' )
                        score += 25137;
                    
                    stack.push( 'X' );
                    
                    break;
                }
            }
        }
        
        if( considerIncomplete )
        {
            if( !stack.empty() && stack.top() != 'X' )
            {
                unsigned long long incompleteScore = 0;
                while( !stack.empty() )
                {
                    char top = stack.top();
                    stack.pop();
                    
                    if( top == '(' )
                        incompleteScore = incompleteScore * 5 + 1;
                    else if( top == '[' )
                        incompleteScore = incompleteScore * 5 + 2;
                    else if( top == '{' )
                        incompleteScore = incompleteScore * 5 + 3;
                    else if( top == '<' )
                        incompleteScore = incompleteScore * 5 + 4;
                }
                
                scores.push_back( incompleteScore );
            }
        }
    }

    if( considerIncomplete )
    {
        std::sort( scores.begin(), scores.end() );
        return scores[ scores.size() / 2 ];
    }

    return score;
}

unsigned long long partOne( const std::string& input )
{
    std::vector< std::string > lines = parse( input );

    return checkScore( lines, false );
}

unsigned long long partTwo( const std::string& input )
{
    std::vector< std::string > lines = parse( input );

    return checkScore( lines, true );
}