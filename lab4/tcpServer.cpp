//
// Created by allen on 10/29/21.
//
#include <sstream>
#include <fstream>
#include <iostream>
#include <cstdlib>
#include <SFML/Network.hpp>

using namespace std;
using namespace sf;
/*
 *
 * */
void runTcpServer(unsigned short port)
{
    // Create a server socket to accept new connections
    sf::TcpListener listener;

    // Listen to the given port for incoming connections
    if (listener.listen(port) != sf::Socket::Done)
        return;
    std::cout << "Server is listening to port " << port << ", waiting for connections... " << std::endl;

    // Wait for a connection
    sf::TcpSocket socket;
    if (listener.accept(socket) != sf::Socket::Done)
        return;
    std::cout << "Client connected: " << socket.getRemoteAddress() << std::endl;

    // Send a message to the connected client
    const char out[] = "Hi, I'm the server";
    if (socket.send(out, sizeof(out)) != sf::Socket::Done)
        return;
    std::cout << "Message sent to the client: \"" << out << "\"" << std::endl;

    // Receive a message back from the client
    char in[128];
    std::size_t received;
    if (socket.receive(in, sizeof(in), received) != sf::Socket::Done)
        return;
    std::cout << "Answer received from the client: \"" << in << "\"" << std::endl;
}

/*
 * This function is used to check whether an input is valid or not.
 *
 * @param strInput input string need to be checked
 * @param validOutput output the valid number
 * @return true for valid input and false for invalid input
 */
bool isValidInput(const string strInput, unsigned short & validOutput)
{
    bool validFlag = true;

    unsigned long tmpValid;
    //    check the input is a number or not
    for (auto chInput : strInput)
    {
        if(!isdigit(chInput))
        {
            validFlag = false;
            return validFlag;
        }

    }
    //    check the port number is valid or not
    stringstream(strInput) >> tmpValid;
    if (tmpValid < 61000 || tmpValid > 65535)
    {
        validFlag = false;
    }
    else
    {
        validOutput = tmpValid;
        validFlag = true;
    }
    return validFlag;
}

/*
 *
 * */
int main(int argc, char* argv[])
{
    // Choose an arbitrary port for opening sockets
    unsigned short port;
    // Create a file for output result
    fstream outputFile("server.log", ios::out | ios::app);
    // check input
    if (argc != 2)
    {
        cout<< "Invalid command line argument detected: ";
        for (int i=1;i<argc;i++)
        {
            cout<< argv[i] <<" ";
        }
        cout << "\nPlease check your values and press any key to end the program!\n";
    }
    else
    {
        if(!isValidInput(argv[1],port))
        {
            cout<< "Invalid command line argument detected: "<<argv[1]<<endl
                << "Please check your values and press any key to end the program!\n";
        }
    }



    // TCP, UDP or connected UDP ?
//    char protocol;
//    std::cout << "Do you want to use TCP (t) or UDP (u)? ";
//    std::cin  >> protocol;

    // Client or server ?
    char who;
    std::cout << "Do you want to be a server (s) or a client (c)? ";
    std::cin  >> who;

    runTcpServer(port);

//    if (protocol == 't')
//    {
//        // Test the TCP protocol
//        if (who == 's')
//            runTcpServer(port);
//        else
//            runTcpClient(port);
//    }
//    else
//    {
//        // Test the unconnected UDP protocol
//        if (who == 's')
//            runUdpServer(port);
//        else
//            runUdpClient(port);
//    }

    // Wait until the user presses 'enter' key
    std::cout << "Press enter to exit..." << std::endl;
    std::cin.ignore(10000, '\n');
    std::cin.ignore(10000, '\n');
    //    Output prime factors to file
    if (outputFile.good())
    {
//        outputFile << integralValue;
    }
    else
    {
        cout << "[ECE6122-Lab4] Can not open server.log\n";
    }
    return EXIT_SUCCESS;
}
