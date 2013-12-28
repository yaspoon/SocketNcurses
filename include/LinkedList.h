/*LinkedList.h
Brock York 14840261
AMI 300 Assignment Greedy Search
LinkedList is a template class that implements a
generic LinkedList Data structure. It is core to everything
Because it is wrapped by the set and stack data structures
Which are core to the graph.
*/
#ifndef LINKEDLIST_H
#define LINKEDLIST_H

#include "LinkedListNode.h"
#include "Iterator.h"



template <class T>
class LinkedList
{
    public:

        LinkedList() //Constructor initilises a empty list
        {
            head = NULL;
            tail = NULL;
            count = 0;
        }

        //Copy constructor
        LinkedList( const LinkedList<T>& inList):
            head(NULL), tail(NULL), count(0)    //Need to initialse the members or else crazy shit happens when we try to copy elements across...
        {
            LinkedListNode<T> *currentNode = inList.head;

            while(currentNode != NULL)
            {
                this->pushBack(currentNode->getData());
                currentNode = currentNode->getNext();
            }
        }

        ~LinkedList() //Destructor
        {
            int i = 0;

            for(i = 0; i < count; i++)
            {
                    LinkedListNode<T>* temp = head;
                    head = head->getNext();
                    temp->setNext(NULL);

                    delete temp;
            }

            //DO NOT uncomment the following, it was what was causing the problem, it's just left here for the funny comment :3
            //delete head; this causes it to segfault when iterating over the list of vertices WTF WTF WTF WTF why has it suddenly broken
            //so it's just going to leak like a bitch till I get it working

            count = 0;
        }

        //Pushes the given element to the front of the list, useful for stacks
        void pushFront(T newData)
        {
            LinkedListNode<T> *newNode = new LinkedListNode<T>(newData); //Create a new LinkedListNode to hold the given data

            if(count == 0)  //if the list if empty
            {
                head = newNode; //Set start of the list to the new node
                tail = newNode;
            }
            else   //Oh man pushing to the head is so much faster
            {
                newNode->setNext(head); //Set the new node to point to the current head
                head = newNode; //Set the start of the list to the new node
            }

            count += 1;
        }


        //Searches for the end of the list then adds this element to the end
        void pushBack(T newEdge)
        {
            LinkedListNode<T> *newNode = new LinkedListNode<T>(newEdge); //Create a new LinkedListNode to hold the given data

            if(count == 0)  //if the list if empty
            {
                head = newNode; //Set node to the head
                tail = newNode;
            }
            else
            {
                tail->setNext(newNode);
                tail = newNode;
            }

            count += 1; //Increate the counter
        }

        //Get the front of the list and return its data to the caller
        T popFront()
        {
            LinkedListNode<T> *node = popNodeFront(); //use helper function to get the front node
            T data; //data object to return
            if(node != NULL)//Stop null dereferences
            {
                data = T(node->getData()); //Set the return object to the one held in the front node
            }

            delete node; //clean up the old node

            return data;
        }

        //Get the last node in the list and return it to the user
        //O(n)
        T popBack()
        {
            LinkedListNode<T> *node = popNodeBack(); //Call helper function to return last node in list
            T data;//Data object to return
            if(node != NULL) //Stop null dereferences
            {
                data = T(node->getData()); //set data object to return, to the one held within the returned node
            }


            delete node; //clean up stale node

            return data;
        }

        //Used to test if the list is empty
        //O(1)
        bool isEmpty()
        {
            bool retVal = true;

            if(count > 0) //The list isn't empty if the count > 0
            {
                retVal = false;
            }

            return retVal;
        }

        //Get the current size of the list
        //O(1)
        int noElements()
        {
            return count;
        }

        //Return a iterator to the start of the list
        //O(1)
        Iterator<T> begin()
        {
            Iterator<T> myIt(head); //Creates a iterator object which starts at the head of the list
            return myIt;
        }

        /*Search for the end node in the list and return a iterator object
        that points to that element, used for iterating over the list so
        you know if your at the end of the list
        It's extremely inefficient but it handles the case where the end object changes
        between calls to ::end() so it's worth it for safety
        O(1)
        */

        Iterator<T> end()
        {

            Iterator<T> retIt(tail); //Create a iterator to the found node, could be NULL if the list if empty

            return retIt;
        }

        //O(N)
        void free()
        {
            int i = 0;

            for(i = 0; i < count; i++)
            {
                    LinkedListNode<T>* temp = head;
                    head = head->getNext();
                    temp->setNext(NULL);

                    delete temp;
            }

            count = 0;
            head = NULL;
            tail = NULL;
        }

        /*O(n)
        Still O(N) even though there is a function call of O(N)
        and a loop of O(N) which ends up being 2N which is just N
        shifted by a constant so it's N
        */
        void operator=(LinkedList<T>& inList)
        {
            //Test that the list we are copying isn't our selves, or this will fail miserably when we free this list
            //If the lists are the same this function is a noop
            if(this != &inList)
            {
                //Since we are "copying" the other object we better get rid of all the stuff this list currently holds
                this->free(); //O(n)

                //Copy all the objects from the other list into this one
                for(Iterator<T> iT = inList.begin(); iT != inList.end(); ++iT) //O(n)
                {
                    T tmp = iT.getData();
                    this->pushBack(tmp);
                }
            }
            else
            {
                std::cout << "Attempted to assign a list to itself, this is a no-op" << std::endl;
            }

        }

        //O(n) same as above
        LinkedList<T>& operator=( const LinkedList<T>& inList)
        {
            if(this != &inList)
            {
                this->free();

                LinkedListNode<T> *currentNode = inList.head;

                while(currentNode != NULL)
                {
                    this->pushBack(currentNode->getData());
                    currentNode = currentNode->getNext();
                }
            }

            return *this;
        }
    protected:
    private:
        LinkedListNode<T> *head; //Points to the start of the list
        LinkedListNode<T> *tail;
        int count; //Number of nodes in the list

        //Helper function to find, remove and return the node at the front of the list
        //O(1)
        LinkedListNode<T> *popNodeFront()
        {
            LinkedListNode<T> *node = NULL; //Temporary node to hold return node, call be NULL if list is empty

            if( count > 0) // if the list isn't empty
            {
                if(count == 1)
                {
                    node = head;
                    head = NULL;
                    tail = NULL;
                }
                else
                {

                    node = head; //Node to return is the start of the list
                    head = head->getNext(); //Set the head the to next node in the list
                }
                count -= 1; //Reduce the counter
            }

            return node;
        }

        //Helper function to return the last node in the list
        //O(n)
        LinkedListNode<T> *popNodeBack()
        {

            LinkedListNode<T> *node = NULL; //Node to return which has the last node in the list, can be null if list is empty

            if(count == 1) //if there is only one element in the list don't bother searching
            {
                node = head; //The head in this case is the start and end of the list
                head = NULL;
                tail = NULL;
                count -= 1; //Decrement the count
            }
            else if(count > 1)      //if this gets to below zero we are in serious trouble and should probably just abort anyway :/
            {
                //If we have more then one element we have to traverse the list and find the last node
                LinkedListNode<T> *lastNode = NULL; //Last node which is the node before the last node in the list we have to set this one's next to null
                node = head; //start at the head of the list

                while(node->getNext() != NULL) //While we arn't at the end of the list
                {
                    lastNode = node; //Set the second last node to this current one
                    node = node->getNext(); //Set the current node to the next on in the list
                }
                //ASSERTION node now points to the last node in the list, so last node now needs to remove it's pointer to node
                //so node can the be returned to the caller
                lastNode->setNext(NULL);
                tail = lastNode;

                count -= 1;

            }

            return node;
        }
};

#endif // LINKEDLIST_H
