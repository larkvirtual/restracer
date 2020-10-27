#ifndef CORE_TYPES_H_
#define CORE_TYPES_H_

// Думаю понятно зачем. Чтобы можно было отслеживать типы на всех машинах (включая 64-х битные)
// где происходит отладка
typedef unsigned long long targetSizeT;

///////////////////////////////////////////

typedef enum {
    validHandle, badHandle
} handleState;

typedef enum {
    allocated, operated, deallocated
} evolutionState;

#endif /* CORE_TYPES_H_ */
