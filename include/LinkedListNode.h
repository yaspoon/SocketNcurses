/*LinkedListNode.h
Brock York 14840261
AMI 300 Assignment Greedy Search
A generic class that is used to implement a linkedList
It holds the data given to it from the List and also
points to the next node
*/
#ifndef LINKEDLISTNODE_H
#define LINKEDLISTNODE_H

#include <algorithm>

template <class T>
class LinkedListNode
{
    public:
        /*Sets up the defaul values
        Using null so you can I can iterate
        over the list and test if NEXT=NULL
        without problems*/
        LinkedListNode()
        {
            next = NULL;
        }

        //When created if given some data make a copy and hold onto it.
        LinkedListNode(T nodeData)
        {
            data = nodeData;
            next = NULL;
        }

        //It does nothing but needs to be defined
        ~LinkedListNode()
        {

        }

        //Returns the data object that is held by this node
        //if it's not null that is
        T getData()
        {

            return data;
        }

        //Return a pointer to the next node in the list
        LinkedListNode *getNext()
        {
            return next;
        }

        //Set the next node in the list that we point to
        void setNext(LinkedListNode *node)
        {
            next = node;
        }
    protected:
    private:
        LinkedListNode *next; //The next node in the list
        T data; //The data we hold onto

};

#endif // LINKEDLISTNODE_H
