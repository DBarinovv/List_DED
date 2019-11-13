#include <stdio.h>
#include <stdlib.h>

#include "TXLib.h"

//=============================================================================

enum colors {D_blue = 1, D_green, D_cyan, D_red, D_purple, D_yellow, D_white, Grey,
               Blue,       Green,   Cyan,   Red,   Purple,   Yellow,   White};

//=============================================================================

#define PR_B(elem, color)     \
{                             \
    txSetConsoleAttr (color); \
    printf (#elem);           \
    txSetConsoleAttr (7);     \
}

//-----------------------------------------------------------------------------

// CH - change color
#define CH(color) \
    txSetConsoleAttr (color);

//-----------------------------------------------------------------------------

// CH_S - change color to standard
#define CH_S \
    txSetConsoleAttr (7);

//=============================================================================

typedef int elem_t;

//=============================================================================

struct list
{
    elem_t *data;
    elem_t *next;
    elem_t *prev;

    int head;
    int tail;
    int free_st;
    int free_end;
};

//=============================================================================

const elem_t C_size_of_array = 20;

const elem_t C_poison = -8;

//=============================================================================

list Constructor (const int size);

//-----------------------------------------------------------------------------

bool Insert_After (list *lst, const int pos, const elem_t argument);

bool Delete_After (list *lst, const int pos, list *lst);

bool Delete (list *lst, const int pos, list *lst);

//-----------------------------------------------------------------------------

bool Dump_List (list lst);

//=============================================================================

int main ()
{
    list lst_1 = Constructor (C_size_of_array);

//    Dump_List (lst_1);

    Insert_After (1, 17, &lst_1);

//    Dump_List (lst_1);

    Insert_After (2, 59, &lst_1);

//    Dump_List (lst_1);

    Insert_After (1, 89, &lst_1);

    Dump_List (lst_1);

    Delete (4, &lst_1);

    Dump_List (lst_1);

    Delete (3, &lst_1);

    Dump_List (lst_1);

    Delete (2, &lst_1);

    Dump_List (lst_1);

    return 0;
}

//=============================================================================

list Constructor (const int size)
{
    elem_t *data = (elem_t *) calloc (size, sizeof (elem_t));
    elem_t *next = (elem_t *) calloc (size, sizeof (elem_t));
    elem_t *prev = (elem_t *) calloc (size, sizeof (elem_t));

    for (int i = 1; i < size; i++)
    {
        data[i] = C_poison;
        next[i] = i + 1;
    }

    data[0] = C_poison;
    data[1] = 5;
    next[1] = 0;           // next of lst.tail always = 0

    list lst = {};
    lst.data = data;
    lst.next = next;
    lst.prev = prev;
    lst.head = 1;
    lst.tail = 1;
    lst.free_st = 2;
    lst.free_end = size - 1;

    return lst;
}

//=============================================================================

bool Insert_After (list *lst, const int pos, const elem_t argument)
{
    if ((*lst).free_st == 0)
    {
        ; // stack if full
    }
    else
    {
        if ((*lst).next[pos] == 0)             // in the end
        {
            (*lst).tail = (*lst).free_st;

            elem_t helper = (*lst).next[(*lst).free_st];

            (*lst).data[(*lst).free_st] = argument;
            (*lst).next[(*lst).free_st] = 0;
            (*lst).next[pos] = (*lst).free_st;
            (*lst).prev[(*lst).free_st] = pos;

            (*lst).free_st = helper;
        }
        else
        {
            (*lst).data[(*lst).free_st] = argument;                // in the middle

            (*lst).prev[(*lst).free_st] = pos;
            (*lst).prev[(*lst).next[pos]] = (*lst).free_st;

            elem_t helper = (*lst).next[(*lst).free_st];

            (*lst).next[(*lst).free_st] = (*lst).next[pos];
            (*lst).next[pos] = (*lst).free_st;

            (*lst).free_st = helper;
        }
    }

}

//=============================================================================

bool Delete_After (list *lst, const int pos)
{
    if ((*lst).next[pos] == 0)
    {
        return false;
    }
    else
    {
        if ((*lst).next[(*lst).next[pos]] == 0)
        {
            (*lst).tail = pos;

            (*lst).data[(*lst).next[pos]] = C_poison;
            (*lst).prev[(*lst).next[pos]] = 0;

            (*lst).next[(*lst).free_end] = (*lst).next[pos];
            (*lst).free_end = (*lst).next[pos];

            (*lst).next[pos] = 0;
        }
        else
        {
            (*lst).data[(*lst).next[pos]] = C_poison;

            (*lst).prev[(*lst).next[(*lst).next[pos]]] = (*lst).prev[(*lst).next[pos]];
            (*lst).prev[(*lst).next[pos]] = 0;

            (*lst).next[(*lst).free_end] = (*lst).next[pos];
            (*lst).free_end = (*lst).next[pos];

            (*lst).next[pos] = (*lst).next[(*lst).next[pos]];
            (*lst).next[(*lst).next[pos]] = 0;
        }
    }
}

//=============================================================================

// one function to delete them all

bool Delete (list *lst, const int pos)
{
    if ((*lst).prev[pos] == 0)                      // if first
    {
        if ((*lst).next[pos] == 0)   // if only 1 elem
        {
            return false;            // we can't delete all :)
        }
        else
        {
            (*lst).head = (*lst).next[pos];

            (*lst).prev[(*lst).next[pos]] = 0;

            (*lst).next[(*lst).free_end] = pos;
            (*lst).free_end = pos;

            (*lst).data[pos] = C_poison;

            (*lst).next[pos] = 0;
        }
    }
    else if ((*lst).next[pos] == 0)                 // if last
    {
        (*lst).data[pos] = C_poison;

        (*lst).next[(*lst).prev[pos]] = 0;
        (*lst).tail = (*lst).prev[pos];

        (*lst).next[(*lst).free_end] = pos;
        (*lst).free_end = pos;

        (*lst).prev[pos] = 0;
    }
    else                                            // if mid
    {
        (*lst).data[pos] = C_poison;

        (*lst).prev[(*lst).next[pos]] = (*lst).prev[pos];

        (*lst).next[(*lst).free_end] = pos;
        (*lst).free_end = pos;

        (*lst).next[(*lst).prev[pos]] = (*lst).next[pos];
        (*lst).next[pos] = 0;

        (*lst).prev[pos] = 0;
    }
}

//=============================================================================

bool Dump_List (list lst)
{
    PR_B(LIST:\n\n, Red)

//-----------------------------------------------------------------------------

    printf ("Indx = ");

    CH(Yellow)
    for (int i = 0; i < C_size_of_array; i++)
    {
        printf (" %.02d ", i);
    }
    CH_S

    printf ("\n\n");

//-----------------------------------------------------------------------------

    PR_B(Data, Purple)
    printf (" = ");
    CH(White)
    for (int i = 0; i < C_size_of_array; i++)
    {
        if (lst.data[i] == C_poison)
            CH(Red)

        if (lst.data[i] >= 0)
        {
            printf (" %.2d ", lst.data[i]);
        }
        else
        {
            printf ("%.2d ", lst.data[i]);
        }

        CH(White)
    }
    CH_S
    printf ("\n");

//-----------------------------------------------------------------------------

    PR_B(Next, Blue)
    printf (" = ");
    for (int i = 0; i < C_size_of_array; i++)
    {
        if (lst.data[i] != C_poison)
            CH(White)
        else
            CH(Green)

        printf (" %.2d ", lst.next[i]);
        CH_S
    }
    printf ("\n");

//-----------------------------------------------------------------------------

    PR_B(Prev, Cyan)
    printf (" = ");
    CH(White)
    for (int i = 0; i < C_size_of_array; i++)
    {
        if (lst.prev[i] == 0)
            CH(Grey)

        printf (" %.2d ", lst.prev[i]);
        CH(White)
    }
    printf ("\n\n");

//-----------------------------------------------------------------------------

    printf ("Head = %d,       Tail = %d\n", lst.head, lst.tail);
    printf ("Free start = %d, Free end = %d\n", lst.free_st, lst.free_end);

    printf ("\n\n\n");
}
