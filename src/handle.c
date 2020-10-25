#include "handle.h"

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
            return lutEntry;
        }
    }
    return NULL;
}

static HandleToMemoryMapping* FindLutEntry(HANDLE_Id handle)
{
    HandleToMemoryMapping *lutEntry = NULL;
    for (size i = 0; i < HANDLE_LUT_DEFAULT_SIZE; ++i) {
        lutEntry = &handleToMemoryLut[i];
        if (handle == lutEntry->handle) {
            return lutEntry;
        }
    }
    return NULL;
}

static inline void InvalidateHandle(HANDLE_Id* handle)
{
    *handle = HANDLE_INVALID;
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

HANDLE_Status HANDLE_AllocWithAllocator(
        HANDLE_Id* handle,
        size bytes,
        HANDLE_MemAllocator allocator)
{
    if (handle == NULL) {
        return HANDLE_StatusNullPtr;
    }

    HandleToMemoryMapping* entry = FindFirstEmptyLutEntry();
    entry->memory = allocator(bytes);

    if (entry->memory == NULL) {
        return HANDLE_StatusMemError;
    }

    entry->occupied = true;

    *handle = entry->handle;
    return HANDLE_StatusOk;
}

HANDLE_Status HANDLE_Dealloc(HANDLE_Id* handle)
{
    COMMON_NULLPTR_GUARD(handle, HANDLE_StatusNullPtr);

    HandleToMemoryMapping* entry = FindLutEntry(*handle);
    COMMON_NULLPTR_GUARD(entry, HANDLE_StatusWrongHandle);

    /* Free memory and clean up fields */
    free(entry->memory);
    entry->memory = NULL;
    entry->occupied = false;

    /* Invalidate handle */
    InvalidateHandle(handle);

    return HANDLE_StatusOk;
}
