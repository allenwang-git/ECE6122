/*
Author: Yinuo Wang
Class: ECE 6122
Last Date Modified: 10/30/2021

Description:
This program use sfml socket to create a tcp client which can connect with tcp server.
If input is not a valid ip address and port number, there will be error message displayed in command line.
To run this program, using "./client localhost 6xxxx"
*/
#include <sstream>
#include <fstream>
#include <iostream>
#include <cstdlib>
#include <SFML/Network.hpp>

using namespace std;

/*
 * This is the tcp client loop function
 * @param server is the ip address from user input
 * @param port is the port number form user input
 * */
void runTcpClient(sf::IpAddress server, unsigned short port)
{
    // Create a socket for communicating with the server
    sf::TcpSocket socket;
    // Set timeout for connect attempts
    sf::Time tOut = sf::milliseconds(1000);

    // Connect to the server
    if (socket.connect(server, port, tOut) != sf::Socket::Done)
    {
        cout << "Failed to connect to the server at " << server.toString() << " on " << to_string(port)
             << ".\nPlease check your values and press Enter key to end program!" << endl;
        return;
    }
    std::cout << "Connected to server " << server << std::endl;

    // Send a message to the server
    string clientMessage;
    while (true)
    {
        cout << "Please enter a message: ";
        getline(cin, clientMessage);
        socket.send(clientMessage.c_str(), clientMessage.size());
    }
}

/*
 * This function is used to check whether an input is valid or not.
 *
 * @param strInput input string need to be checked
 * @param validOutput output the valid port number
 * @return true for valid input and false for invalid input
 */
bool isValidPort(const string strInput, unsigned short &validOutput)
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
 * This function is used to check whether a ip address is valid or not
 * @param strInput input address need to be checked
 * @param address output the valid ip address
 * @return true for valid input and false for invalid input
 * */
bool isValidIP(const string strInput, sf::IpAddress &address)
{
    stringstream(strInput) >> address;
    if (address == sf::IpAddress::None)
        return false;
    else
        return true;
}

/*
 * This is the main function to parse the command
 * line arguments and call tcp client function
 * */
int main(int argc, char *argv[])
{
    // Choose an arbitrary port for opening sockets
    unsigned short port;
    sf::IpAddress address;
    // Create a file for output result
    fstream outputFile("server.log", ios::out | ios::app);
    // check input arguments
    if (argc != 3)
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
                cout << argv[i] << " ";
            cout << "\nPlease check your values and press Enter key to end the program!\n";
        }
    }
    else
    {
        //        check ip address
        if (!isValidIP(argv[1], address))
        {
            cout << "Invalid command line argument detected: " << argv[1] << " " << argv[2] << endl
                 << "Please check your values and press Enter key to end the program!\n";
        }
        //        check port number
        if (!isValidPort(argv[2], port))
        {
            cout << "Invalid command line argument detected: " << argv[1] << " " << argv[2] << endl
                 << "Please check your values and press Enter key to end the program!\n";
        }
        //        run tcp client
        if (isValidIP(argv[1], address) && isValidPort(argv[2], port))
        {
            runTcpClient(address, port);
        }
    }
    // Wait until the user presses 'enter' key
    std::cin.ignore(10000, '\n');
    std::cin.ignore(10000, '\n');

    return EXIT_SUCCESS;
}
