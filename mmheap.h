#include <cstdint>
#include <vector>

namespace minmaxheap
{

class mmheap
{
public:
  mmheap (uint32_t capacity);
  virtual ~mmheap ();
  void buildHeap (const std::vector<int32_t> &vals);
  int32_t getMin () const;
  int32_t getMax () const;
  void insert (int32_t val);
  void deleteMin ();
  void deleteMax ();

private:
  bool isAtMinLevel (uint32_t index) const;
  void percolateDown (uint32_t index);
  void percolateDownMin (uint32_t index);
  void percolateDownMax (uint32_t index);
  void percolateUp (uint32_t index);
  void percolateUpMin (uint32_t index);
  void percolateUpMax (uint32_t index);
  // add your own private functions here, if any
  int parent(int i);
  int leftChild(int i);
  int rightChild(int i);
  int smallestChild(int i);
  int largestChild(int i);
  void swap(int a, int b);

private:
  int32_t *_arr;
  uint32_t _capacity;
  uint32_t _size;
};

} /* minmaxheap */ 
