typedef struct sortedArr {
    int len;
    int capacity;
    element* list;
} sortedArr;

// Functions defined in C-File

sortedArr *sortArr_new();

void sortArr_free(sortedArr *arr);

element sortArr_peek(sortedArr *arr);

element sortArr_get(sortedArr *arr, int index);

element *sortArr_getRef(sortedArr *arr, int index);

int sortArr_length(sortedArr *arr);

sortedArr *sortArr_insert(sortedArr **A, element el);

sortedArr *sortArr_removeAt(sortedArr **A, int index);

int sortArr_binSearchApprox(sortedArr *arr, element *el);

int sortArr_binSearch(sortedArr *arr, element *el);

element sortArr_pop(sortedArr **arr);

int sortArr_contains(sortedArr *arr, element el);

int sortArr_indexOf(sortedArr *arr, element el);

int sortArr_stringify(sortedArr *arr, char *s);