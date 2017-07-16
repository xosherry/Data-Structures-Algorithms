#include <cstdint>
#include <vector>
#include <cmath> /* log2i */
#include <algorithm> /* max */
#include "mmheap.h"
#include <iostream>
using namespace std;
using namespace minmaxheap;

// constructor
mmheap::mmheap (uint32_t capacity) {
	// _arr (our heap) points to an array of size: capacity 
	_arr = new int[capacity];
	_size = 0;
}	

mmheap::~mmheap () {
	_size = 0;
	delete [] _arr;
}

void mmheap::buildHeap (const std::vector<int32_t> &vals) {
	_size = vals.size();
	for (int i=0; i < _size; i++) {
		_arr[i] = vals[i];
	}

	// begin at rightmost index of second to last level
	// go through every node from "bottom up"
	for (int j = (_size/2) - 1; j >=0; j--) {
		percolateDown(j);
	}
}

int32_t mmheap::getMin () const {
	if (_size == 0) 
		return 0;
	
	else
		return _arr[0];
}

int32_t mmheap::getMax () const {
	if (_size == 0) return 0;
	
	else if (_size == 1)
		return _arr[0];

	else if (_size == 2)
		return _arr[1];
	else
		return max(_arr[1], _arr[2]);
}

void mmheap::insert (int32_t val) {
	_size++;
	_arr[_size-1] = val;
        
	percolateUp(_size-1);
}

void mmheap::deleteMin () {
	if (_size == 1) {
                _arr[0] = 0;
		_size = 0;
	}

	else {
		swap(0, _size-1);
		_arr[_size-1] = 0;
		_size--;
		
		percolateDownMin(0);
	}
}

void mmheap::deleteMax () {
	int indexOfMax = 1;

	if (getMax() == _arr[2])
		indexOfMax = 2;
        
	swap(indexOfMax, _size-1);
	_arr[_size-1] = 0;
	_size--;

	if (isAtMinLevel(indexOfMax))
		percolateDownMin(indexOfMax);
	else
		percolateDownMax(indexOfMax);

}

bool mmheap::isAtMinLevel(uint32_t index) const {
	int level = floor(log2(index + 1));
	if (level % 2 == 0) 
		return true;
	else 
		return false;
}

void mmheap::percolateDown (uint32_t index) {
	if (isAtMinLevel(index)) 
		percolateDownMin(index);
	else 
		percolateDownMax(index);

}

void mmheap::percolateDownMin (uint32_t index) {
        // FIND INDEX OF GREATEST CHILDREN AND GRANDCHILDREN (IF ANY) OF _arr[index]
        int left = leftChild(index);
        int right = rightChild(index);
        int smallestGCIndex = 0;
        int indexOfSmallestLeftGC = 0;
        int indexOfSmallestRightGC = 0;
        int smallestIndex = 0;

	// if children
	if (left < _size) {
                // if there exists a grandchild/grandchildren
                if (leftChild(left) < _size){
                        // find largest grandchild of left side
                        
			indexOfSmallestLeftGC = smallestChild(left);

                        // find smallest grandchild of right side IF right side has children
                        if (leftChild(right) < _size) {
                                indexOfSmallestRightGC = smallestChild(right);
			}

                        // compare grandchild from left side to grandchild of right
                        if (_arr[indexOfSmallestLeftGC] < _arr[indexOfSmallestRightGC])
                                smallestGCIndex = indexOfSmallestLeftGC;
                        else
                                smallestGCIndex = indexOfSmallestRightGC;

                }

                	// so now we have index of smallest  grandchild & index of largest child

                	// if smallest index is a grandchild
                	if (smallestGCIndex != 0 && _arr[smallestGCIndex] < _arr[smallestChild(index)]) 
                        	smallestIndex = smallestGCIndex;
		
        	        else
                	        smallestIndex = smallestChild(index);
				
		
                        // if index of smallest value is a grandchild's index
                        if (smallestIndex == smallestGCIndex) {
                                // MAINTAIN MAX-LEVEL: node > parent & node < grandparent
		                if (_arr[smallestIndex] < _arr[index]) {
                                        swap(index, smallestIndex);
				
                                	if  (_arr[smallestIndex] > _arr[parent(smallestIndex)]) 
                                        	swap(parent(smallestIndex), smallestIndex);
		
                               		 percolateDownMin(smallestIndex);
				}
                  	}

                        // if index of largest value is a child
                        else if (smallestIndex == smallestChild(index)) {
                                if (_arr[smallestChild(index)] < _arr[index])
                                        swap(index, smallestChild(index));
                       	}
	}
}

void mmheap::percolateDownMax (uint32_t index){
        // FIND INDEX OF GREATEST CHILDREN AND GRANDCHILDREN (IF ANY) OF _arr[index]
        int left = leftChild(index);
        int right = rightChild(index);
	int largestGCIndex = 0; 
        int indexOfLargestLeftGC = 0;
        int indexOfLargestRightGC = 0;
        int largestIndex = 0;

        if (left < _size) {
                // if there exists a grandchild/grandchildren
                if (leftChild(left) < _size){
			// find largest grandchild of left side
                        
			indexOfLargestLeftGC = largestChild(left);

			// find largest grandchild of right side IF right side has children
			if (leftChild(right) < _size) 
				indexOfLargestRightGC = largestChild(right);

			// compare grandchild from left side to grandchild of right
			if (_arr[indexOfLargestLeftGC] > _arr[indexOfLargestRightGC]) 
				largestGCIndex = indexOfLargestLeftGC;
			else
				largestGCIndex = indexOfLargestRightGC;
		}
		
		// so now we have index of largest grandchild & index of largest child
		
		// if largest index is a grandchild
			if (largestGCIndex != 0 && _arr[largestGCIndex] > _arr[largestChild(index)])
				largestIndex = largestGCIndex;
			else
				largestIndex = largestChild(index);	

			// if index of greatest value is a grandchild's index
			if (largestIndex == largestGCIndex) {
				// MAINTAIN MAX-LEVEL: node > parent & node < grandparent
				if (_arr[largestIndex] > _arr[index]) {
					swap(index, largestIndex);
				
					if  (_arr[largestIndex] < _arr[parent(largestIndex)]) 
						swap(parent(largestIndex), largestIndex);
				
					percolateDownMax(largestIndex);
				}	
			}
	        		
			// if index of largest value is a child
        		else {
                		if (_arr[largestChild(index)] > _arr[index]) 
			       			swap(index, largestChild(index));
        		}
	}
}

void mmheap::percolateUp (uint32_t index) {
	if (isAtMinLevel(index)) {
		// at min level, a node's parent should be greater (b/c parent is at max-level)
		if (index != 0 && _arr[index] > _arr[parent(index)]) { 
			swap(parent(index), index);
			percolateUpMax(parent(index));
		}
		else {
			percolateUpMin(index);
		}
		
	}
	
	// index is at max level
	else {
		// at max level, a node's parent should be smaller (b/c parent is at min-level)
		if (index != 0 && _arr[index] < _arr[parent(index)]) {
                        swap(parent(index), index);
			percolateUpMin(parent(index));
		}
		else {
			percolateUpMax(index);
		}
	}
}

void mmheap::percolateUpMin (uint32_t index){
        int gp = parent(parent(index));

        // if index has grandparent
        if (parent(index) >= 1){
                if (_arr[index] < _arr[gp]){
                        swap(gp, index);
                        percolateUpMin(gp);
                }
        }
}

void mmheap::percolateUpMax (uint32_t index){
	int gp = parent(parent(index));
	
	// if index has grandparent
        if (parent(index) >= 1){
		if (_arr[index] > _arr[gp]){
			swap(gp, index); 
			percolateUpMax(gp);
		}
	}  
}

// add your own private functions here, if any
int mmheap::parent(int i) {
	return (i - 1) / 2;
}  	

int mmheap::leftChild(int i) {
	return 2 * i + 1;
}

int mmheap::rightChild(int i) {
	return leftChild(i) + 1;
}

int mmheap::smallestChild(int i) {
	int left = leftChild(i);
	int right = rightChild(i);
	
	// smallest is left by default 
	int smallestIndex = left;
	int smallestValue = _arr[left];
	
	// if there exists a right child (the only case there wouldn't be
	// a right child is at the far right of leaf level bc HEAP rules) 
	if (right < _size)
		smallestValue = min(_arr[left], _arr[right]);
	
	if (smallestValue == _arr[right])
		smallestIndex = right;

	return smallestIndex; 			
}

int mmheap::largestChild(int i) {
	if (rightChild(i) > _size) return leftChild(i);

	else {
		if (smallestChild(i) == leftChild(i))
			return rightChild(i);
		else
			return leftChild(i);
	}
}


void mmheap::swap(int a, int b) {
	int temp = _arr[a];
	_arr[a] = _arr[b];
	_arr[b] = temp;	
}
