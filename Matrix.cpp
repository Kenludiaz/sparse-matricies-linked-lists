#include "Matrix.h"

Matrix::Matrix(int rows, int cols) {
    this->rows = rows;
    this->cols = cols;
    this->first = nullptr;
    this->iterator = nullptr;
}

void Matrix::read(std::string fileName) {
    std::ifstream file;
    file.open(fileName);

    int value = 0;
    Node *temp = nullptr;

    for(int i =0; i < this->rows; i++){
        for(int j = 0; j < this->cols; j++){
            file >> value;
            if(value != 0){
                Node *node = new Node(value, i, j);
                if(this->isEmpty()){
                    this->first = node;
                    this->iterator = node;
                    temp = node;
                }
                else{
                    temp->next = node;
                    temp->previous = temp;
                    temp = temp->next;
                }
            }
        }
    }
    file.close();
}

bool Matrix::isEmpty() {
    if (!this->first) {
        return true;
    }
    return false;
}

void Matrix::print() {
    if (this->isEmpty()) {
        std::cout << "[INVALID] This matrix is empty." << std::endl;
        return;
    }
    Node *temp = this->first;
    for(int i = 0; i < this->rows; i++){
        for(int j = 0; j < this->cols; j++){
            if(temp->row == i && temp->col == j){
                std::cout << temp->value << " ";
                if(temp->next != nullptr){
                    temp = temp->next;
                }
            }
            else{
                std::cout << "0" << " ";
            }
        }
        std::cout << std::endl;
    }
}

Matrix Matrix::add(Matrix& other) {
    if(!this->canBeAdded(other)) {
        std::cout << "[INVALID] These matrixes can not be added." << std::endl;
        std::exit(1);
    }
    //A copy is taken from the first matrix because every value
    //in the resulting matrix will be equivalent until the second
    //matrix has been iterated upon.
    Matrix* result = new Matrix(this->rows, this->cols);
    this->deepCopy(*(result));

    while(other.iterator != nullptr) {
        if (result->end()) {
            Node* newNode = new Node(other.iterator->value, 
                                    other.iterator->row,
                                    other.iterator->col);
            // When the first Matrix has no more nodes then you will always need to
            // add the remaining Nodes from the second Matrix
            result->pushBack(newNode);
            other.next();

        } else if (other.iterator->isBehind(result->iterator)) {
            Node* newNode = new Node(other.iterator->value, 
                                other.iterator->row,
                                other.iterator->col);
            if (result->itrAtHead()) {
                result->pushFront(newNode);
            } else {
                result->insertBehind(newNode);
                
            }
            other.next();

        } else if (other.iterator->isTied(result->iterator)) {
            result->iterator->value += other.iterator->value;
            result->next();
            other.next();

        } else if (result->iterator->isBehind(other.iterator)) {
            result->next();
        }
    }
    return (*result);
}

Matrix Matrix::multiply(Matrix& multiply){
    if(!this->canBeMultiplied(multiply)){
        std::cout << "[INVALID] These matrixes can not be multiply." << std::endl;
        std::exit(1);
    }
    Matrix* result = new Matrix(this->rows, multiply.cols);
    Node *resultIterator = result->first;

    // 2 pointer fucntion to trace the linked list
    Node *temp1 = this->first;
    Node *temp2 = multiply.first;
    
    // the size of the matrix should be row of the first matrix by the col of the secound matrix
    for(int i = 0; i < this->rows; i++){
        for(int j = 0; j < multiply.cols; j++){
            //Loop till row == i
            while(temp1->row != i){
                temp1 = temp1->next;
            }
            int value = 0;
            //runs if temp1 and temp 2 is not null and temp1 row == i
            while(temp1 != nullptr && temp2 != nullptr && temp1->row == i){
                //if temp2 col is not equal to j move to the next node
                if(temp2->col != j){
                    temp2 = temp2->next;
                }
                //if temp1 col is not equal to temp2 row shift temp1 or temp2 to next node
                else if(temp1->col != temp2->row){
                    if(temp1->next != nullptr) {
                        temp1 = temp1->next;
                    }
                    else{
                         temp2 = temp2->next;
                    }

                }
                //once all the conditon is meet add value of temp1*temp2 to value holder, then shift both node to the next
                else{
                    value += temp1->value * temp2->value;
                    temp1 = temp1->next;
                    temp2 = temp2->next;
                }
            }
            //if value is not 0 create a new node
            if(value != 0){
                Node* newNode = new Node(value, i, j);
                if(resultIterator == nullptr){
                    result->first = newNode;
                    resultIterator = result->first;
                }
                else{
                    Node* temp = resultIterator;
                    resultIterator->next = newNode;
                    resultIterator->previous = temp;
                    resultIterator = resultIterator->next;
                }
            }
            // reset the pointers
            temp1 = this->first;
            temp2 = multiply.first;
        }
    }
    //print the matrix
    return(*result);
}


bool Matrix::canBeAdded(Matrix& other){
    if (this->isEmpty() || other.isEmpty()) {
        std::cout << "[INVALID] One of these matrixes is empty." << std::endl;
        return false;
    } else if(this->rows == other.rows && this->cols == other.cols) {
        return true;
    } else {
        return false;
    }
}
void Matrix::deepCopy(Matrix& result) {
    Node* resultItr = nullptr;
    Node* currentItr = this->first;
    bool firstIteration = true;

    while (currentItr) {
        Node* newNode = new Node(currentItr->value, 
                                 currentItr->row, 
                                 currentItr->col);

        if (firstIteration) {
            result.iterator = newNode;
            result.first = newNode;
            resultItr = newNode;
            firstIteration = false;
        } else {
            Node* temp = resultItr;
            resultItr->next = newNode;
            resultItr = resultItr->next;
            resultItr->previous = temp;
        }

        currentItr = currentItr->next;
    }
}


bool Matrix::canBeMultiplied(Matrix& other){
    if (this->isEmpty() || other.isEmpty()) {
        std::cout << "[INVALID] One of these matrixes is empty." << std::endl;
        return false;
    } else if (this->cols == other.rows){
        return true;
    } else {
        return false;
    }
}
void Matrix::next() {
    this->iterator = iterator->next;
}
bool Matrix::itrAtHead() {
    if (this->iterator == this->first) {
        return true;
    } 
    return false;
}
void Matrix::insertBehind(Node* newNode) {
    // Assertion thisIterator != thisFirstNode
    Node* temp = this->iterator->previous;
    temp->next = newNode;
    newNode->previous = temp;
    this->iterator->previous = newNode;
    newNode->next = this->iterator;
}
void Matrix::pushBack(Node* newNode) {
    // Assertion thisIterator = NULL
    Node* previous = this->lastNode();
    previous->insertAhead(newNode);
}
void Matrix::pushFront(Node* newNode) {
    Node* temp = this->iterator;
    this->first = newNode;
    newNode->next = temp;
    temp->previous = newNode;
}
Node* Matrix::lastNode() {
    Node* temp = this->first;
    while(temp->next != nullptr) {
        temp = temp->next;
    }
    return temp;
}


bool Matrix::end() {
    return (this->iterator == nullptr);
}

Matrix::~Matrix() {
    this->destroyNodes();
}

void Matrix::destroyNodes() {
    Node* iterator = this->first;
    int i =1;
    while (iterator) {
        Node* next = iterator->next;
        delete iterator;
        iterator = next;
        if (iterator == nullptr) {
            break;
        }
    }
}