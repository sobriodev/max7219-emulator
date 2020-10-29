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
static HandleToMemoryMapping handleToMemoryLut[HANDLE_LUT_DEFAULT_SIZE] = {0};

/* -------------------------------------------------------------------------- */
/* ----------------------------- Private functions -------------------------- */
/* -------------------------------------------------------------------------- */

static void LutInit(HandleToMemoryMapping* lut, size sizeOfLut)
{
    HandleToMemoryMapping *lutEntry;
    for (size i = 0; i < sizeOfLut; ++i) {
        lutEntry = &lut[i];

        /* First time settings */
        lutEntry->handle = i;
        lutEntry->occupied = false;
        lutEntry->memory = NULL;
    }
}

static HandleToMemoryMapping* FindFirstEmptyLutEntry(void)
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
    LutInit(handleToMemoryLut, HANDLE_LUT_DEFAULT_SIZE);
}

HANDLE_Status HANDLE_AllocWithAllocator(
        HANDLE_Id* handle,
        size bytes,
        HANDLE_MemAllocator allocator)
{
    COMMON_NULLPTR_GUARD(handle, HANDLE_StatusNullPtr);

    HandleToMemoryMapping* entry = FindFirstEmptyLutEntry();
    entry->memory = allocator(bytes);

    /* TODO check for NULL and eventually extend lut memory */

    COMMON_NULLPTR_GUARD(entry->memory, HANDLE_StatusMemError);

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

size HANDLE_CountFree(void)
{
    size freeHandles = 0;
    for (size i = 0; i < HANDLE_LUT_DEFAULT_SIZE; ++i) {
        if (!handleToMemoryLut[i].occupied) {
            ++freeHandles;
        }
    }

    return freeHandles;
}

size HANDLE_CountAll(void)
{
    return HANDLE_LUT_DEFAULT_SIZE;
}

void HANDLE_DeallocAll(void)
{
    for (size i = 0; i < HANDLE_LUT_DEFAULT_SIZE; i++) {
        HandleToMemoryMapping* handle = &handleToMemoryLut[i];

        /* Free memory and clean up fields */
        free(handle->memory);
        handle->memory = NULL;
        handle->occupied = false;

        /* TODO Combine this and HANDLE_Dealloc actions into one common fn */
    }
}
