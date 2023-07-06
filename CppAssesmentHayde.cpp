// Honeywell.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <map>
#include <sstream>

#define SERVER 0u
#define CUSTOM 1u

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


struct Node {
    Message messageData;
    Node* next;
    Node* prev;
    unsigned short type;
    Node(unsigned short t, Node* p, Node* n, Message m) : type(t), prev(p), next(n), messageData(m) {};
    Node(unsigned short t, Message m) : type(t), prev(NULL), next(NULL), messageData(m) {};
};


class customQueue {
private:
    Node* head = NULL;
    Node* tail = NULL;

public:
    
    map<string, string> Map_client; //map to get the count of messages in queue
    map<string, string> Map_server;
    
    void enqeueue(ServerResponse e)// Returns nothing and enqueues the element e at the end of the queue.
    {
       
        if ((head == NULL) && (tail == NULL)) //First element to add
        {
            Node* newElement = new Node(SERVER,e); // prev = NULL next = NULL
            head = newElement;
            tail = newElement;
        }
        else //There are more elements, add at last
        {

            //Create new element with tail pointing to new element
            Node* newElement = new Node(SERVER, tail, NULL, e); // (prev = tail,next = NULL,Node)
        
            tail->next = newElement;
            //Update tail
            tail = newElement;
        }
        

        stringstream ss;
        ss << e.messageId << " " << e.serverId << " " << e.metadata;
     
        Map_server[e.messageId] = ss.str();
    }
    void enqeueue(ClientRequest e)// Returns nothing and enqueues the element e at the end of the queue.
    {
        if ((head == NULL) && (tail == NULL)) //First element to add
        {
            Node* newElement = new Node(CUSTOM,e);
            head = newElement;
            tail = newElement;
        }
        else //There are more elements, add at last
        {

            //Create new element with next pointing to head
            Node* newElement = new Node(CUSTOM, tail, NULL, e); // (prev = tail ,next = NULL,Node)
            
            tail->next = newElement;
            //Update tail
            tail = newElement;

        }
        stringstream ss;
        ss << e.messageId << " " << e.clientId << " " << e.requestTime;
        Map_client[e.messageId] = ss.str();
    }
    void deque()//Returns nothing and deletes the front element of the queue.
    {
        if (head->type == SERVER)
        {
            Map_server.erase(head->messageData.messageId);
        }
        else
        {
            Map_client.erase(head->messageData.messageId);
        }

        if (head == tail)//Just one element in queue
        {
            head = NULL;
            tail = NULL;
        }
        else
        {//Erase front
            if (head->next != NULL)
            {
                Node* temp = head->next;

                temp->prev = NULL;
                head = temp;
            }
            else
            {
                head = NULL;
                tail = NULL;
            }
        }
    }
    string getFront() //Returns the front element of the queue as a string
    {
        string returnString; 
        if (head != NULL)
        {
            if (head->type == SERVER)
            {
                for (auto i : Map_server)
                {
                    if (i.first == head->messageData.messageId)
                    {
                        returnString = i.second;
                    }

                }
                Map_server.erase(head->messageData.messageId);
            }
            else if (head->type == CUSTOM)
            {
                for (auto i : Map_client)
                {
                    if (i.first == head->messageData.messageId)
                    {
                        returnString = i.second;
                    }

                }
                Map_client.erase(head->messageData.messageId);

            }

            //Erase front
            if (head->next != NULL)
            {
                Node* temp = head->next;

                temp->prev = NULL;
                head = temp;
            }
            else
            {
                head = NULL;
                tail = NULL;
            }
        }
        else
        {
            returnString = "No elements in queue";
        }

        return returnString;


    }
    unsigned int getCountClients()// Returns the number of distinct clientIds present on queue.
    {
        return (unsigned int)Map_client.size();
    }

    unsigned int getCountServers() //Returns the number of distinct serversIds present on queue.
    {
        return (unsigned int)Map_server.size();
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
            cout<< MainQueue.getCountClients();
        }
        else if (op == 4)
        {
            cout<< MainQueue.getCountServers();
        }
        else if (op == 5) // Get front element
        {
            cout << MainQueue.getFront();
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
           
        }
        system("PAUSE");
    } while (flag == false);

    cout << "Bye";
}

