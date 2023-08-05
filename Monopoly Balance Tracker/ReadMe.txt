How to Run Gift1.c


gcc gift1.c -o gifts1  

./gifts1 new [playerName Playerbalance] [playerName Playerbalance]  ....

// To give other players money the command is
./gifts1 [sourcePlayer amount]  [receivingPlayer] [receivingPlayer] ....


Example:

gcc gift1.c -o gifts1  
./gifts1 new Mary 200 Tom 200 Amy 300 Sam 500

Expected Output:

    Mary: 200.00
    Tom: 200.00
    Amy: 300.00
    Sam: 500.00
    
    
// To give other players money the command is
./gifts1 Mary 20 Amy Sam

    Mary: 180.00
    Tom: 200.00
    Amy: 310.00
    Sam: 510.00