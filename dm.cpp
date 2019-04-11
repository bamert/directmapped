#include <iostream>
#include <vector>
using namespace std;

/**
 * @brief Measures access statistics on a simulated direct mapped cache
 */
class DirectMapped{
  private:
    int sizeInBytes;
    int blocksize;
    int numblocks;
    int compulsoryMisses;
    int conflictMisses;
    int hits;
    int accesses;
    std::vector<int> cache;
  public:

    /**
     * @brief Builds a direct mapped cache with a total size of sizeInBytes and blocksize.
     *
     * @param sizeInBytes total cache size in bytes
     * @param blocksize   total size of a block in bytes.
     */
    DirectMapped(int sizeInBytes, int blocksize) : sizeInBytes(sizeInBytes), blocksize(blocksize) {
      numblocks = sizeInBytes / blocksize; 
      cache.resize(numblocks);
      std::fill(cache.begin(), cache.end(), -1);
      compulsoryMisses=0;
      conflictMisses=0;
      hits=0;
      accesses=0;
    }
    /**
     * @brief Issues a given floating point word address access
     *        (float is assumed to occupy 4 bytes)
     *
     * @param addr float-word address(32bit wide words)
     */
    void accessFloatAddr(int addr){
      accessByteAddr(4*addr);
    }
    /**
     * @brief Issues a given byte address access
     *
     * @param addr byte address
     */
    void accessByteAddr(int addr){
      int byteAddr = addr % sizeInBytes; 
      int blockId = byteAddr / blocksize;
      int blockOffset = byteAddr % blocksize;
      accesses++;
      //Check if cache block occupied:
      if( cache[blockId] != -1){
        // Check if block contains the address we want (hit)
        if(addr >= cache[blockId] && addr < cache[blockId] + blocksize){ 
          hits++;
        }else{ //Something else is there
          conflictMisses++;
        }
      }else{
        compulsoryMisses++;
      }
      cache[blockId] = addr; //byteAddr;
    }

    /**
     * @brief Resets the cache state to empty and resets the statistics
     */
    void reset(void){
      std::fill(cache.begin(), cache.end(), -1);
      compulsoryMisses=0;
      conflictMisses=0;
      hits=0;
      accesses=0;
    }

    /**
     * @brief Prints a summary of the recorded hit and miss rates
     */
    void stats(){
      cout << compulsoryMisses << " compulsory Misses" << endl;
      cout << conflictMisses << " conflict Misses" << endl;
      cout << (conflictMisses+compulsoryMisses) << " total misses" << endl;
      cout << accesses << " total accesses" << endl;
      cout << (conflictMisses+compulsoryMisses)/float(accesses) << " miss rate" << endl;
      cout << hits/float(accesses) << " hit rate" << endl<<endl;
    }
};
/**
 * @brief Simulates cache accesses for the method:
 *        void add_rows(float* A, int n){
 *          for(int i=1; i<n;i++){
 *            for(int j=0;j<n;j++){
 *              float a = A[i*n+j];
 *              float b = A[(i-1)*n+j];
 *              A[i*n+j] = a+b;
 *            }
 *          }
 *        }
 * @param dm  Direct mapped cache object 
 * @param n   size of the matrix A
 */
void simulateAccess(DirectMapped& dm, int n){
  cout << "n=" << n<< endl;
  for (int i = 1; i < n; i ++) {
    for (int j = 0; j < n; j ++) {
      //float a = A[i*n + j ];
      dm.accessFloatAddr(i*n+j);
     //float b = A [( i -1)* n + j ];
      dm.accessFloatAddr((i-1)*n+j);
     //A[i *n + j ] = a + b ;
      dm.accessFloatAddr(i*n+j);
     }
   }
}
int main(){
  cout << endl <<"Task 2019" << endl ;
  // Direct mapped cache
  DirectMapped c1(4096,16);
  simulateAccess(c1,256);
  c1.stats();
  c1.reset();
  simulateAccess(c1,2048);
  c1.stats();
  c1.reset();
  simulateAccess(c1,2052);
  c1.stats();

}
