#include <iostream>

#include <fstream>
#include <sstream>

#include "lib.hpp"

int main( int argc, char** argv )
{
    std::ios::sync_with_stdio( false );
    
    std::ifstream file{ argv[ 1 ] };
    std::stringstream inputStream;
    
    if( file )
    {
        inputStream << file.rdbuf();
        file.close();
    }

    std::string input = inputStream.str();
    std::cout << partOne( input ) << '\n';
    std::cout << partTwo( input ) << '\n';

    return 0;
}