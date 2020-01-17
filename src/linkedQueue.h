/// @author Janiel Venice C. Abad
/// @brief Implementation file for template class linkedQueue
/// @file linkedQueue.h
/// @date 2018-09-24

#ifndef H_LINKEDQUEUE
#define H_LINKEDQUEUE

#include <iostream>
#define SIZE 32000    /// size of array in node 

/// ----------------------------------------------------------------------------
/// @brief queueNode for nodes in linked list modelled as a queue
/// ----------------------------------------------------------------------------
template <class myType>
struct queueNode {
    myType dataSet[SIZE];       /// stores elements in node (also a queue)
    int front;                  /// index for front of queue in current node
    int back;                   /// index for back of queue in current node
    queueNode<myType> *link;    /// link to next node
};

/// ----------------------------------------------------------------------------
/// @class linkedQueue
///
/// @brief The template class linkedQueue implements a queue as a linked list
/// and another queue within its nodes as an array.  
/// ----------------------------------------------------------------------------
template <class myType>
class linkedQueue {
private:
    queueNode<myType> *queueFront;    /// pointer to first node
    queueNode<myType> *queueRear;     /// pointer to last node
    int count;                        /// number of elements in linked list
    
public:
    /// ------------------------------------------------------------------------
    /// @brief (Constructor) Initializes the queue to an empty state (sets 
    /// queueFront and queueRear to nullptr and count to 0).
    /// ------------------------------------------------------------------------
    linkedQueue();
    
    /// ------------------------------------------------------------------------
    /// @brief (Destructor) Destroys the queue (linked list).
    /// ------------------------------------------------------------------------
    ~linkedQueue();
    
    /// ------------------------------------------------------------------------
    /// @brief Checks if queue is empty.
    ///
    /// @return True if queue is empty, false if not
    /// ------------------------------------------------------------------------
    bool isEmptyQueue() const;
    
    /// ------------------------------------------------------------------------
    /// @brief Resets queue to a new, empty state.
    /// ------------------------------------------------------------------------
    void initializeQueue();
    
    /// ------------------------------------------------------------------------
    /// @brief Adds item to the back of the queue.
    ///
    /// @param newItem Item to be added to the queue
    /// ------------------------------------------------------------------------
    void addItem(const myType& newItem);
    
    /// ------------------------------------------------------------------------
    /// @return Value at the front of the queue
    /// ------------------------------------------------------------------------
    myType front() const;
    
    /// ------------------------------------------------------------------------
    /// @return Value at the back of the queue
    /// ------------------------------------------------------------------------
    myType back() const;
    
    /// ------------------------------------------------------------------------
    /// @brief Removes item from the front of the queue.
    /// ------------------------------------------------------------------------
    void deleteItem();
    
    /// ------------------------------------------------------------------------
    /// @return Total number of elements in the queue
    /// ------------------------------------------------------------------------
    int queueCount() const;
    
    /// ------------------------------------------------------------------------
    /// @brief Prints the queue.
    /// ------------------------------------------------------------------------
    void printQueue();
};

/// ----------------------------------------------------------------------------
/// FUNCTION DEFINITIONS
/// ----------------------------------------------------------------------------

template <class myType>
linkedQueue<myType>::linkedQueue() {
    queueFront = nullptr;
    queueRear = nullptr;
    count = 0;
}

/// ----------------------------------------------------------------------------

template <class myType>
linkedQueue<myType>::~linkedQueue() {
    queueNode<myType> *temp = nullptr;
    
    // delete nodes
    while(queueFront != nullptr) {
        temp = queueFront;
        queueFront = queueFront->link;
        delete temp;
    }
    
    queueRear = nullptr;
    temp = nullptr;
    count = 0;
    
}

/// ----------------------------------------------------------------------------

template <class myType>
bool linkedQueue<myType>::isEmptyQueue() const {
    return queueFront == nullptr;
}

/// ----------------------------------------------------------------------------

template <class myType>
void linkedQueue<myType>::initializeQueue() {
    queueNode<myType> *temp = nullptr;
    
    // delete nodes
    while(queueFront != nullptr) {
        temp = queueFront;
        queueFront = queueFront->link;
        delete temp;
    }
    
    queueRear = nullptr;
    temp = nullptr;
    count = 0;
    
}

/// ----------------------------------------------------------------------------

template <class myType>
void linkedQueue<myType>::addItem(const myType& newItem) {
    int arraySize = count % SIZE;
    
    // if queue is empty, create node
    if(isEmptyQueue()) {
        queueNode<myType> *newNode = new queueNode<myType>;
        newNode->dataSet[0] = newItem;
        newNode->front = 0;
        newNode->back = 0;
        newNode->link = nullptr;
        queueFront = newNode;
        queueRear = newNode;
        newNode = nullptr;
    // if current node is full
    } else if(!arraySize) {
        queueNode<myType> *newNode = new queueNode<myType>;
        newNode->dataSet[0] = newItem;
        newNode->front = 0;
        newNode->back = 0;
        newNode->link = nullptr;
        queueRear->link = newNode;
        queueRear = newNode;        
    // if current node isn't full yet
    } else {
        // if current back is at the end of the array
        if(queueRear->back == SIZE - 1) {
            queueRear->dataSet[0] = newItem;
            queueRear->back = 0;
        } else {
            queueRear->dataSet[queueRear->back + 1] = newItem;
            queueRear->back++;
        }
    }
    
    count++;
    
}

/// ----------------------------------------------------------------------------

template <class myType>
myType linkedQueue<myType>::front() const {
    return queueFront->dataSet[queueFront->front];
}

/// ----------------------------------------------------------------------------

template <class myType>
myType linkedQueue<myType>::back() const {
    return queueRear->dataSet[queueRear->back];
}

/// ----------------------------------------------------------------------------

template <class myType>
void linkedQueue<myType>::deleteItem() {
	
	if(!isEmptyQueue()) {
        // if there's only one node
		if(queueFront->link == nullptr) {
            // if there's only one element in it
            if(count == 1) {
                delete queueFront;
                queueFront = nullptr;
                queueRear = nullptr;
            } else {
                // if current front is at the end of the array
                if(queueFront->front == SIZE - 1) {
                    queueFront->front = 0;
                } else {
                    queueFront->front++;
                }
            }
        } else {
            // if there's only one element in the current node
            if(queueFront->front == queueFront->back) {
                queueNode<myType> *temp = queueFront;
                queueFront = queueFront->link;
                delete temp;
                temp = nullptr;
            } else {
                // if current front is at the end of the array
                if(queueFront->front == SIZE - 1) {
                    queueFront->front = 0;
                } else {
                    queueFront->front++;
                }
            }
        }
        
        count--;
	}
    
}

/// ----------------------------------------------------------------------------

template <class myType>
int linkedQueue<myType>::queueCount() const {
    return count;
}

/// ----------------------------------------------------------------------------

template <class myType>
void linkedQueue<myType>::printQueue() {
    using namespace std;
    
    queueNode<myType> *temp = queueFront;
    int f = 0;
    int b = 0;
    
    while(temp != nullptr) {
       f = temp->front;
       b = temp->back;
       
       for(int i = f; i <= b && i < SIZE; i++) {
           cout << temp->dataSet[i] << ' ';
       }
       
       temp = temp->link;
   }
}

/// ----------------------------------------------------------------------------

#endif /* H_LINKEDQUEUE */
