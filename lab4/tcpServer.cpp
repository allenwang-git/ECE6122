/*
Author: Yinuo Wang
Class: ECE 6122
Last Date Modified: 10/30/2021

Description:
This program use sfml socket to create a tcp server listening specific port and connect to 5 clients.
If input is not a valid port number, corresponding error message will be displayed on command line.
The program log will be output into file "server.log".
To run this program, using "./server 6xxxx" on command line
*/
#include <sstream>
#include <fstream>
#include <iostream>
#include <cstdlib>
#include <SFML/Network.hpp>
#include <vector>

using namespace std;
using namespace sf;
/*
 * This is the TCP server loop
 * @param port is the port number server will listen
 * */
void runTcpServer(unsigned short port)
{
    // Create a file for output result
    fstream outputFile("server.log", ios::out | ios::app);
    // Create a server socket to accept new connections
    sf::TcpListener listener;

    // Listen to the given port for incoming connections
    if (listener.listen(port) != sf::Socket::Done)
        return;
    cout << "Server is listening to port " << port << ", waiting for connections... " << std::endl;
    //  Create a selecter to handle multi clients
    sf::SocketSelector selector;
    selector.add(listener);
    short clientNum = 0;
    vector<sf::TcpSocket *> sockets;

    while (true)
    {
        // Make the selector wait for data on any socket
        if (selector.wait())
        {
            // Test the listener
            if (clientNum < 5 && selector.isReady(listener))
            {
//                if ()
//                {
                    // The listener is ready: there is a pending connection
                    sf::TcpSocket *socket = new sf::TcpSocket;
                    if (listener.accept(*socket) == sf::Socket::Done)
                    {
                        clientNum++;
                        // Add the new client to the clients list
                        sockets.push_back(socket);
                        // Add the new client to the selector so that we will be notified when he sends something
                        selector.add(*socket);
                        cout << "Client connected: " << (*socket).getRemoteAddress() << " current clients: "<<clientNum<<std::endl;
                            outputFile << "Client connected" << endl;
                    }
                    else
                    {
                        // Error, we won't get a new connection, delete the socket
                        delete socket;
                    }
//                }
            }
            else
            {
                // The listener socket is not ready, test all other sockets (the clients)
                for (vector<sf::TcpSocket *>::iterator it = sockets.begin(); it != sockets.end(); ++it)
                {
                    sf::TcpSocket &socket = **it;
                    if (selector.isReady(socket))
                    {
                        // The client has sent some data, we can receive it
                        char clientMessage[1000] = {0};
                        size_t received;
                        if (socket.receive(clientMessage, sizeof(clientMessage), received) == sf::Socket::Done)
                        {
                            cout << "Received Message from the client: \"" << clientMessage << "\"" << std::endl;
                            //    Output prime factors to file
                            if (outputFile.good())
                                outputFile << clientMessage << endl;
                        }
                        Packet packet;
                        if (socket.send(packet) == sf::Socket::Disconnected)
                        {
                            outputFile << "Client disconnected" << endl;
                            selector.remove(socket);
                            clientNum--;
                            cout << "Client Disconnected, current clients: "  << clientNum<< endl;

                        }
                     }
                }
            }
        }
    }
}

/*
 * This function is used to check whether a port is valid or not.
 *
 * @param strInput input port need to be checked
 * @param validOutput output the valid port number
 * @return true for valid input and false for invalid input
 */
bool isValidInput(const string strInput, unsigned short &validOutput)
{
    bool validFlag = true;
    unsigned long tmpValid;
    //    check the input is a number or not
    for (auto chInput: strInput)
    {
        if (!isdigit(chInput))
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
 * This is the main function to parse command line arguments
 * and call TCP server function.
 * */
int main(int argc, char *argv[])
{
    // Create a port for opening sockets
    unsigned short port;
    // check input
    if (argc != 2)
    {
        if (argc == 1)
        {
            cout << "Invalid command line argument detected: (no argument input)";
            cout << "\nPlease check your values and press Enter key to end the program!\n";
        }
        else
        {
            cout << "Invalid command line argument detected: ";
            for (int i = 1; i < argc; i++)
            {
                cout << argv[i] << " ";
            }
            cout << "\nPlease check your values and press Enter key to end the program!\n";
        }
    }
    else
    {
        if (!isValidInput(argv[1], port))
        {
            cout << "Invalid command line argument detected: " << argv[1] << endl
                 << "Please check your values and press Enter key to end the program!\n";
        }
        else
        {
            runTcpServer(port);
        }
    }
    // Wait until the user presses 'enter' key
    std::cout << "Press Enter to exit..." << std::endl;
    std::cin.ignore(10000, '\n');
    std::cin.ignore(10000, '\n');
    return EXIT_SUCCESS;
}
