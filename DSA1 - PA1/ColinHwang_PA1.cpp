/*
 * Colin Hwang
 * DSA 1: Programming Assignment 1
 * Professor Sable
 *
 * Program Overview: This is a program that will manipulate stacks and queues. Given an input file, an output file will
 * be created that indicates what commands were preformed, errors caused by invalid commands, what values were pushed to
 * and popped from various stacks and queues, and what stacks or queues were created.
 */

#include <fstream>
#include <iostream>
#include <iterator>
#include <list>
#include <sstream>
#include <stdlib.h>
#include <string>
using namespace std;

//The program will read from input files and write to an output file
ifstream infile;
ofstream outfile;

//An abstract base class with aspects of singly linked list functionality with stack and queue subclasses derived from it
template <typename Object>
class SimpleList
{
public:
    //Public pure virtual member functions for push and pop
    virtual void push(Object value)=0;
    virtual Object pop()=0;
    string ListName();
    bool Empty() const;
    string listName;
    //This is a constructor for SimpleList
    SimpleList(string name)
    {
        listName = name;
        start = NULL;
        end = NULL;
    }
protected:
    void insertAtStart(Object value);
    void insertAtEnd(Object value);
    Object removeFromStart();
private:
    //This is the nested node class which allows the user to create nodes within the list
    struct Node
    {
        Object Data;
        Node* next;

        Node(Object value, Node* pointer)
        {
            Data = value;
            next = pointer;
        }
    };
    //Nodes pointing to the start and end of the list
    Node* start;
    Node* end;
};

//Declarations taken from Professor Sable's Programming Assignment instructions
list<SimpleList<double> *> listSLd;  // all double stacks and queues
list<SimpleList<int> *> listSLi;    // all integer stacks and queues
list<SimpleList<string> *> listSLs; // all string stacks and queues

//Function that indicates if a list is empty or not
template <typename Object>
bool SimpleList<Object>::Empty() const
{
    if (start == NULL)
    {
        return true;
    }
    else
    {
        return false;
    }
}

//Getter function that allows the program to retrieve the name of a list
template <typename Object>
string SimpleList<Object>::ListName()
{
    return listName;
}

//This will insert a node to the start of the list. This will be used when an element is pushed to a stack
template <typename Object>
void SimpleList<Object>::insertAtStart(Object data)
{
    Node *newNode = new Node(data, start);
    start = newNode;
}

//This will insert a node to the end of the list. This will be used when an element is pushed to a queue
template<typename Object>
void SimpleList<Object>::insertAtEnd(Object data)
{
    Node *newNode = new Node(data, NULL);
    if (end == NULL)
    {
        start = newNode;
    }
    else
    {
        (end->next) = newNode;
    }
    end = newNode;
}

//This will remove a node from the start of a list. This will be used when an element is popped from a stack and queue
template <typename Object>
Object SimpleList<Object>::removeFromStart()
{
    Node *temp = start;
    Object data = temp->Data;

    if (temp->next == NULL)
    {
        end = NULL;
    }

    start = temp->next;
    delete temp;
    return data;
}

// A stack class that is derived from the base SimpleList class
template <typename Object>
class Stack:public SimpleList<Object> {
public:
    void push(Object value);
    Object pop();
    //Constructor for Stack
    Stack(string n) : SimpleList<Object>(n) {}
};

//Push to stack
template <typename Object>
    void Stack<Object>::push(Object value) {Stack<Object>::insertAtStart(value);}

//Pop from stack
template <typename Object>
    Object Stack<Object>::pop() {return Stack<Object>::removeFromStart();}


//A queue class that is derived from the base SimpleList class
template <typename Object>
class Queue:public SimpleList<Object> {
public:
    void push(Object value);
    Object pop();
    //Constructor for Queue
    Queue(string n) : SimpleList<Object>(n) {}
};

//Push to queue
template <typename Object>
    void Queue<Object>::push(Object value) {Queue<Object>::insertAtEnd(value);}

//Pop from queue
template <typename Object>
    Object Queue<Object>::pop() {return Queue<Object>::removeFromStart();}

/*
 * The first command can consist of 3 options: create, push and pop. If the first command is "create", this function will
 * perform a list search to see if there is an existing stack or queue with the new stack or queue being created. If the
 * desired name already exists, an error message will be displayed. If not, a new stack or queue will be created. If the
 * first command is "push", a value will be pushed into a specified stack or a queue if it exists. If the first command
 * is "pop, a value will be popped from a specified stack or queue if it exists and is not empty. If the stack or queue
 * does not exist or is empty in the case of a pop, an error message will be displayed.
 */
template <typename Object>
void Commands(list<SimpleList<Object> *> &bigList, string command[], Object value)
{
    if (command[0] == "create")
    {
        if (ListSearch(bigList, command[1]) != NULL)
        {
            outfile << "ERROR: This name already exists!\n";
        }

        else
        {
            if (command[2] == "queue")
            {
                Queue<Object>* ptr = new Queue<Object>(command[1]);
                bigList.push_front(ptr);
            }

            else if (command[2] == "stack")
            {
                Stack<Object>* ptr = new Stack<Object>(command[1]);
                bigList.push_front(ptr);
            }
        }
    }

    else if (command[0] == "push")
    {
        SimpleList<Object> *ptr = ListSearch(bigList, command[1]);

        if (ptr == NULL)
        {
            outfile << "ERROR: This name does not exist!\n";
        }

        else
        {
            ptr->push(value);
        }
    }

    else if (command[0] == "pop")
    {
        SimpleList<Object> *ptr = ListSearch(bigList, command[1]);

        if (ptr == NULL)
        {
            outfile << "ERROR: This name does not exist!\n";
        }

        else if (ptr->Empty())
        {
            outfile << "ERROR: This list is empty!\n";
        }

        else
        {
            outfile << "Value popped: " << ptr->pop() << "\n";
        }
    }
}

/*
 * This function will process a line in the file and display it in a way that it is split into three parts: command, name
 * and value. In the case of "create", the third word will either be stack or queue. In the case of push, the third word
 * will represent a value that matches the appropriate datatype of the stack/queue. If the case of pop, there must be
 * no third word.
 */

void Display(string line)
{
    outfile << "PROCESSING COMMAND: " << line << "\n";
    stringstream linestream(line);
    string text;
    string* command = new string[3];

    // This will split the line into three parts: command, name, and value
    int i = 0;
    while (linestream >> text)
    {
        command[i++] = text;
    }
    if (command[1].substr(0,1) == "i")
    {
        int value = 0;
        if (!command[2].empty())
        {
            value = atoi(command[2].c_str()); //This will convert a string to an integer
        }
        Commands(listSLi, command, value);
    }
    else if (command[1].substr(0,1) == "d")
    {
        double value = 0.0;
        if (!command[2].empty())
        {

            value = atof(command[2].c_str()); //This will convert a string to a double
        }
        Commands(listSLd, command, value);
    }
    else if (command[1].substr(0,1) == "s")
    {
        string value = command[2];
        Commands(listSLs, command, value);
    }
}

/*
 * Preforms a search for a list in a list (called bigList) that contains pointers to all stacks/queues of a certain
 * datatype. This will be used to check if such a list exists and that values are popped/pushed to the correct list.
 */
template <typename Object>
SimpleList<Object>* ListSearch(list<SimpleList<Object> *> &bigList, string listname)
{
    for (typename list<SimpleList<Object> *>::const_iterator it = bigList.begin(); it != bigList.end(); ++it)
    {
        if ((*it)->ListName() == listname)
        {
            return *it;
        }
    }
    return NULL;
}

//This will prompt the user for an output file. The program will read from this file.
void OpenInput(ifstream &FileIn)
{
    string FileNameIn;
    cout << "Enter name of input file: ";
    cin >> FileNameIn;
    FileIn.open(FileNameIn.c_str());
}

//This will prompt the user for an input file. The program will write to this file.
void OpenOutput(ofstream &FileOut)
{
    string FileNameOut;
    cout << "Enter name of output file: ";
    cin >> FileNameOut;
    FileOut.open(FileNameOut.c_str());
}

//Main function
int main(void)
{
    OpenInput(infile);
    OpenOutput(outfile);

    string inLine;
    while (getline(infile, inLine))
    {
        Display(inLine);
    }
}
//END OF PROGRAM