/*Iterator.h
Brock York 14840261
AMI 300 Assignment Greedy Search
Defines a template class that is used to iterate
over the linked list objects. Because it is generic
it can the be used on the set and stack objects that
use the linkedlist.
This abstracts away how the linkedlist works - Abstraction principle
This means the program doesn't need to mess with pointers to iterate over
the list. Helps solves screwing up the iteration and causing NULL dereferences
It's all done here inside this class.
Lists are iterated over at just about every step so it's super helpful
*/
#ifndef ITERATOR_H
#define ITERATOR_H

#include "LinkedListNode.h"
#include <iostream>

template <class T>
class Iterator
{
    public:
        Iterator() //Setup a blank iterator to not point to anything
        {
            count = 0;
            currentNode = NULL;
        }

        Iterator(LinkedListNode<T> *inHead) //Given the head of a list to point to
        {
            currentNode = inHead;
            count = 0;
        }

        virtual ~Iterator() //Probably should be destructing something
        {

        }

        T getData() //Returns the data object of the current node the iterator points to
        {
            return currentNode->getData();
        }

        void operator++() //Loads the next node in the list
        {
            if(currentNode != NULL) //If we arn't at the end or a null list continue
            {
                count++; //Set the count
                currentNode = currentNode->getNext(); //Get the next node by loading it from the currentNode we have
            }

        }

        bool operator==(Iterator<T> inIt) //Compare this iterator to another iterator object, used in the for loop to test if we are at the end
        {
            bool retVal = false; //By default they arn't equal

            if( currentNode != NULL) //Make sure we don't dereference a null pointer
            {
                if( (currentNode->getData()) == (inIt.getData()) ) //Compare the Nodes the two iterators point to
                {
                    retVal = true; //If they nodes they point to are the same they are equivalient iterator objects
                }
            }

            return retVal;
        }

        //BROKEN ATM DON"T USE
        bool operator!=(Iterator<T> inIt) //Compare this iterator with the given one and check if they are not equivalent
        {
            bool retVal = false; //By default they are equal

            if( currentNode != NULL) //stops null pointer derefences
            {
                //if( (currentNode->getData()) != (inIt.getData()) )
                {
                    retVal = true;
                }
            }

            return retVal;
        }

    protected:
    private:
        LinkedListNode<T> *currentNode; //The current node this iterator is attached to
        int count; //How many nodes we have iterated over atm

};

#endif // ITERATOR_H
