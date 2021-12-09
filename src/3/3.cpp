#include <sstream>

#include <unordered_set>

unsigned long long partOne( const std::string& input )
{
    unsigned int gamma = 0, epsilon;

    std::stringstream inputStream{ input };
    std::string line;
    std::getline( inputStream, line );
    
    int* numBits = new int[ line.size() ]{};
    int numNums = 1;

    for( int i = 0; i != line.size(); ++i )
    {
        if( line[ i ] == '1' )
            numBits[ i ]++;
    }

    while( std::getline( inputStream, line ) )
    {
        numNums++;
        for( int i = 0; i != line.size(); ++i )
        {
            if( line[ i ] == '1' )
                numBits[ i ]++;
        }
    }
    
    for( int i = 0; i != line.size(); ++i )
    {
        gamma = ( gamma << 1 ) + ( numBits[ i ] > numNums / 2 ? 1 : 0 );
    }
    
    epsilon = ( ~gamma ) & ( ( 1 << line.size() ) - 1 );
    
    delete[] numBits;
    
    return epsilon * gamma;
}

unsigned long long partTwo( const std::string& input )
{
    std::unordered_set< int > nums;

    std::stringstream inputStream{ input };
    std::string line;

    while( std::getline( inputStream, line ) )
    {
        nums.emplace( std::stoi( line, nullptr, 2 ) );
    }
    
    int numBits = line.size();
    
    std::unordered_set< int > O2nums{ nums }, CO2nums{ nums };
    for( int i = 0; ( O2nums.size() != 1 || CO2nums.size() != 1 ) && i != numBits; ++i )
    {
        int bitMask = ( 1 << ( numBits - i - 1 ) );
        
        int setO2Bits = 0;
        for( auto it = O2nums.begin(); it != O2nums.end(); ++it )
        {
            if( O2nums.size() == 1 ) break;
            
            if( *it & bitMask )
                setO2Bits++;
        }
        
        int setCO2Bits = 0;
        for( auto it = CO2nums.begin(); it != CO2nums.end(); ++it )
        {
            if( CO2nums.size() == 1 ) break;

            if( *it & bitMask )
                setCO2Bits++;
        }
        
        int nonSetO2 = O2nums.size() - setO2Bits;
        for( auto it = O2nums.begin(); it != O2nums.end(); )
        {
            if( O2nums.size() == 1 ) break;

            if( (bool)( *it & bitMask ) != setO2Bits >= nonSetO2 )
                it = O2nums.erase( it );
            else
                ++it;
        }

        int nonSetCO2 = CO2nums.size() - setCO2Bits;
        for( auto it = CO2nums.begin(); it != CO2nums.end(); )
        {
            if( CO2nums.size() == 1 ) break;

            if( (bool)( *it & bitMask ) == setCO2Bits >= nonSetCO2 )
                it = CO2nums.erase( it );
            else
                ++it;
        }
    }
    
    return *O2nums.begin() * *CO2nums.begin();
}