typedef struct dynamicArr binaryHeap;

binaryHeap *binHeap_new(int initialCapacity);

element binHeap_peek(binaryHeap *A);

element binHeap_poll(binaryHeap **A);

element binHeap_get(binaryHeap *A, int index);

int binHeap_size(binaryHeap *A);

int binHeap_isEmpty(binaryHeap *A);

int binHeap_lastLeafIndex(binaryHeap *A);

element binHeap_remove(binaryHeap **A, int index);

int binHeap_insert(binaryHeap **A, element node);

int binHeap_replace(binaryHeap **A, int index, element node);

int binHeap_fullSiftup(binaryHeap **A, int index);

int binHeap_siftup(binaryHeap **A, int index);

int binHeap_fullSiftdown(binaryHeap **A, int index);

int binHeap_siftdown(binaryHeap **A, int index);

int binHeap_getLeftChildIndex(int index);

int binHeap_getRightChildIndex(int index);

int binHeap_getParentIndex(int index);

void binHeap_switchPositions(binaryHeap *A, int i, int j);

int binHeap_contains(binaryHeap *A, element el);

int binHeap_indexOf(binaryHeap *A, element el);

int binHeap_findIndexOf(binaryHeap *A, int (*fn) (element, int, binaryHeap*));

element *binHeap_find(binaryHeap *A, int (*fn) (element, int, binaryHeap*));

// Stringify

int binHeap_stringify(binaryHeap *A, char *s);