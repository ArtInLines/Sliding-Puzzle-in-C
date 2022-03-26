typedef struct element {
    int id;
    int weight;
    int prevID;
    int moves_amount;
    int direction;
    int *empty_field;
    int *board;
    struct element *next[4];
    struct element *prev;
} element;


void El_free(element *el);

element El_getNull();

element *El_new(int weight, int moves_amount, int direction, int *empty_field, int *board, element *prev);

int El_isInvalid(element *el);

int El_same(element *el1, element *el2);

int El_eq(element *el1, element *el2);

int El_lt(element *el1, element *el2);

int El_gt(element *el1, element *el2);

int El_lte(element *el1, element *el2);

int El_gte(element *el1, element *el2);

// Stringify

int El_stringify(element *el, char *s);