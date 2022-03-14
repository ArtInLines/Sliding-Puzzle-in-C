typedef struct dynamicArr {
    int len;
    int capacity;
    element* list;
} dynamicArr;

// Functions defined in C-File

dynamicArr *dynArr_new(int length);

element dynArr_get(dynamicArr *arr, int index);

element *dynArr_getRef(dynamicArr *arr, int index);

void dynArr_set(dynamicArr *arr, int index, element newValue);

int dynArr_length(dynamicArr *arr);

dynamicArr *dynArr_removeAt(dynamicArr **A, int index);

dynamicArr *dynArr_insert(dynamicArr **A, int index, element el);

dynamicArr *dynArr_push(dynamicArr **arr, element el);

element dynArr_pop(dynamicArr **arr);

element dynArr_peek(dynamicArr *arr);

int dynArr_contains(dynamicArr *arr, element el);

int dynArr_indexOf(dynamicArr *arr, element el);

int dynArr_findIndexOf(dynamicArr *arr, int (*fn) (element, int, dynamicArr*));

element *dynArr_find(dynamicArr *arr, int (*fn) (element, int, dynamicArr*));

dynamicArr *dynArr_map(dynamicArr **arr, element (*fn) (element, int, dynamicArr*));

dynamicArr *dynArr_immutableMap(dynamicArr **arr, element (*fn) (element, int, dynamicArr*));

dynamicArr *dynArr_filter(dynamicArr **arr, int (*fn) (element, int, dynamicArr*));

element dynArr_reduce(dynamicArr *arr, element (*fn) (element, element, int, dynamicArr*));

void *dynArr_fullReduce(dynamicArr *arr, void* (*firstEl) (element, int, dynamicArr*), void* (*fn) (element, element, int, dynamicArr*));

// Stringify

int dynArr_stringifyCapacity(dynamicArr *arr);

int dynArr_stringify(dynamicArr *arr, char *s);