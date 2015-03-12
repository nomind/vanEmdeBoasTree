
#include <iostream>
#include <cassert>
#include <cstdlib>

using namespace std;

int getHeight(int nodeCount) {
  int height = 0;
  int maxNodes = 1;
  while (maxNodes <= nodeCount) {
    maxNodes <<= 1;
    height++;
  }
  return height;
}

int* binarySearch(int key, int* veb, int height, int &rank) {
  if (height == 1) {
    if (veb[0] == key) {
      return veb;
    }
    if (veb[0] < key) {
      rank++;
    }
    return NULL;
  }

  int leftSubtreeSz = (1 << (height - 1)) - 1;
  int currentRank = 0;
  int parentHeight = height/2;
  int childHeight = height - parentHeight;
  if (key == veb[0]) {
    rank += leftSubtreeSz;
    return veb;
  }

  int* found = binarySearch(key, veb, parentHeight, currentRank);
  int childSubtreeSz = (1 << childHeight) - 1;
  int parentSubtreeSz = (1 << parentHeight) - 1;
  rank += currentRank + currentRank * childSubtreeSz;
  if (found != NULL) {
    rank += childSubtreeSz;
    return found;
  }
  return binarySearch(key, veb + currentRank * childSubtreeSz + parentSubtreeSz, childHeight, rank);
}

int* inorderTovEB(int* sortedArray, int sz) {
  assert(sz > 0);
  if(sz == 1) {
    int* veb = new int[1];
    veb[0] = sortedArray[0];
    return veb;
  }

  int height = getHeight(sz);
  int parentHeight = height/2;
  int childHeight = height - parentHeight;
  int parentSubtreeSz = (1 << parentHeight) - 1;
  int childSubtreeSz = (1 << childHeight) - 1;

  int* parentSubtree = new int[parentSubtreeSz];
  for (int i = 0, j = childSubtreeSz; j < sz; i++, j += childSubtreeSz + 1) {
    parentSubtree[i] = sortedArray[j];
  }

  int* vEB = new int[sz];
  int* parentSubtreevEB = inorderTovEB(parentSubtree, parentSubtreeSz);
  memcpy(vEB, parentSubtreevEB, parentSubtreeSz * sizeof(parentSubtreevEB));
  int* vEBSubtreeStart = vEB + parentSubtreeSz;

  int childSubtreeCount = parentSubtreeSz + 1;
  for (int i = 0; i < childSubtreeCount; i++) {
    int* childSubtreeStart = sortedArray + i * childSubtreeSz + i;
    int* childSubtreevEB = inorderTovEB(childSubtreeStart, childSubtreeSz);
    memcpy(vEBSubtreeStart, childSubtreevEB, childSubtreeSz * sizeof(childSubtreevEB));
    vEBSubtreeStart += childSubtreeSz;
  }

  return vEB;
}

int* binarySearch(int key, int* sortedArray, int sz) {
  int a = 0, b = sz - 1;
  while(a < b) {
    int m = (a+b)/2;
    if(sortedArray[m] < key) {
      a = m+1;
    } else {
      b = m;
    }
  }
  return sortedArray + a;
}

int main() {
  int height = 25;
  int sz = (1 << height) - 1;
  int* sortedArray = new int[sz];
  for (int i = 0; i < sz; i++) {
    sortedArray[i] = i;
  }

  int* vEB = inorderTovEB(sortedArray, sz);
  /*for (int i = 0; i < sz; i++) {
    cout<<vEB[i]<<" ";
  }
  cout<<endl;

  int rank = 0;
  int* found = binarySearch(14, vEB, 4, rank);
  cout<<rank<<endl;
  if (found != NULL) {
    cout<<*found<<endl;
  }*/

  int testCases = 1000000;
  for(int i=0; i<testCases; i++) {
    int k = rand() % sz;
    int rank;
    //int* found = binarySearch(k, vEB, height, rank);
    int* found = binarySearch(k, sortedArray, sz);
    assert(*found == k);
  }
  return 0;
}