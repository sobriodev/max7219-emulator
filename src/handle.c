#include "handle.h"

#include <stdlib.h>

/* -------------------------------------------------------------------------- */
/* ---------------------------- Private data types -------------------------- */
/* -------------------------------------------------------------------------- */

/* Struct to store handle details */
typedef struct
{
    HANDLE_Id handle;
    void* memory;
    bool occupied;
} HandleToMemoryMapping;

/* -------------------------------------------------------------------------- */
/* ----------------------------- Private variables -------------------------- */
/* -------------------------------------------------------------------------- */

/* Handle to memory look-up table */
HandleToMemoryMapping handleToMemoryLut[HANDLE_LUT_DEFAULT_SIZE] = {0};

/* -------------------------------------------------------------------------- */
/* ----------------------------- Private functions -------------------------- */
/* -------------------------------------------------------------------------- */

HandleToMemoryMapping* FindFirstEmptyLutEntry(void)
{
    HandleToMemoryMapping *lutEntry = NULL;
    for (size i = 0; i < HANDLE_LUT_DEFAULT_SIZE; ++i) {
        lutEntry = &handleToMemoryLut[i];
        if (!lutEntry->occupied) {
            break;
        }
    }
    return lutEntry;
}

/* -------------------------------------------------------------------------- */
/* ------------------------------- API functions ---------------------------- */
/* -------------------------------------------------------------------------- */

void HANDLE_Init(void)
{
    HandleToMemoryMapping *lutEntry;
    for (size i = 0; i < HANDLE_LUT_DEFAULT_SIZE; ++i) {
        lutEntry = &handleToMemoryLut[i];

        /* First time settings */
        lutEntry->handle = i;
        lutEntry->occupied = false;
    }
}

HANDLE_Status HANDLE_Alloc(HANDLE_Id* handle, size bytes)
{
    if (handle == NULL) {
        return HANDLE_StatusNullPtr;
    }

    HandleToMemoryMapping* entry = FindFirstEmptyLutEntry();
    entry->memory = malloc(bytes);
    entry->occupied = true;

    *handle = entry->handle;
    return HANDLE_StatusOk;
}
