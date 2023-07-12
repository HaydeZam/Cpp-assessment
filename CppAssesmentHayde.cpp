// Honeywell.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <map>
#include <sstream>
#include <list>

#define SERVER 0u
#define CLIENT 1u

using namespace std;

class Message {
public:
    string messageId;
    Message() {}
};

class ServerResponse : public Message {
public:
    string serverId;
    string metadata;
    ServerResponse(string messageId, string serverId, string metadata) {
        this->messageId = messageId;
        this->serverId = serverId;
        this->metadata = metadata;
    }
};

class ClientRequest : public Message {
public:
    string clientId;
    int requestTime;
    ClientRequest(string messageId, string clientId, int requestTime) {
        this->messageId = messageId;
        this->clientId = clientId;
        this->requestTime = requestTime;
    }
};

class customQueue {
private:
    map<string, string> mp; //map to get the count of messages in queue
    list <pair<unsigned int, Message> > queue; // type, message
    unsigned int server;
    unsigned int client;

public: 
    
    void enqeueue(ServerResponse e)// Returns nothing and enqueues the element e at the end of the queue.
    {
        stringstream ss;
        ss << e.messageId << " " << e.serverId << " " << e.metadata;
        mp[e.messageId] = ss.str();

        queue.push_back({ SERVER,e });
        server++;
        
    }
    void enqeueue(ClientRequest e)// Returns nothing and enqueues the element e at the end of the queue.
    {
        stringstream ss;
        ss << e.messageId << " " << e.clientId << " " << e.requestTime;
        mp[e.messageId] = ss.str();
        queue.push_back({ CLIENT,e });
        client++;
    }
    void deque()//Returns nothing and deletes the front element of the queue.
    {
        if (queue.front().first == CLIENT) client--;
        else if (queue.front().first == SERVER) server--;
        mp.erase(queue.front().second.messageId);
        queue.pop_front();
    }
    string getFront() //Returns the front element of the queue as a string
    {
        string returnString; 
        returnString = mp[queue.front().second.messageId];
        mp.erase(queue.front().second.messageId);
        queue.pop_front();
        
        return returnString;

    }
    unsigned int getCountClients()// Returns the number of distinct clientIds present on queue.
    {
        return client;
    }

    unsigned int getCountServers() //Returns the number of distinct serversIds present on queue.
    {
        return server;
    }

    customQueue()
    {
        server = 0 ;
        client = 0;
    }
};

int main()
{
    int op=0;
    customQueue MainQueue;
    string messageid;
    string server_client_id;
    string metadata;
    int requesttime;
    bool flag = false;
    do 
    {
        op = 0;
        system("CLS");
        cout << " Please select an option: " << endl;
        cout << " 1) Enter Client Request " << endl;
        cout << " 2) Enter Server Response " << endl;
        cout << " 3) Get Count of Clients " << endl;
        cout << " 4) Get count of servers " << endl;
        cout << " 5) Get Front Element " << endl;
        cout << " 6) Delete front element from queue  " << endl;
        cout << " 7) Quit " << endl;

        cin >> op;

        if (op == 1)
        {
            cout << "Enter Client Request: " ;
            cin >> messageid >> server_client_id >> requesttime;
            ClientRequest req(messageid, server_client_id, requesttime);
            MainQueue.enqeueue(req);
        }
        else if (op == 2)
        {
            cout << "Enter Server Response: ";
            cin >> messageid >> server_client_id >> metadata;
            ServerResponse res(messageid, server_client_id, metadata);
            MainQueue.enqeueue(res);
        }
        else if (op == 3)
        {
           cout<< MainQueue.getCountClients()<<endl;
        }
        else if (op == 4)
        {
            cout<< MainQueue.getCountServers()<<endl;
        }
        else if (op == 5) // Get front element
        {
            cout << MainQueue.getFront()<<endl;
        }
        else if (op == 6) // Delete Element
        {
            MainQueue.deque();
        }
        else if (op == 7)
        {
            flag = true;
            break;
        }
        else
        {
            cout << "Not valid option" << endl;
            cin.clear();
            cin.ignore(INT_MAX, '\n');
           
        }
        system("PAUSE");
    } while (flag == false);

    cout << "Bye";
}

