#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "../memory.h"
#include "../table.h"
#include "../value.h"
#include "../vm.h"

#define LARGE_COUNT 100000
#define CHURN_COUNT 10000
#define CHURN_ROUNDS 5
#define COLLISION_COUNT 512
#define COLLISION_CAPACITY 1024

static volatile double checksum;

static double seconds(void) {
  return (double)clock() / CLOCKS_PER_SEC;
}

static uint32_t randomNumber(uint32_t* state) {
  uint32_t x = *state;
  x ^= x << 13;
  x ^= x >> 17;
  x ^= x << 5;
  *state = x;
  return x;
}

static void require(bool condition, const char* message) {
  if (!condition) {
    fprintf(stderr, "Benchmark check failed: %s\n", message);
    exit(EXIT_FAILURE);
  }
}

static void benchmarkSequential(void) {
  Table table;
  initTable(&table);

  double start = seconds();
  for (int i = 0; i < LARGE_COUNT; i++) {
    tableSet(&table, NUMBER_VAL((double)i), NUMBER_VAL((double)(i + 1)));
  }
  double insertTime = seconds() - start;

  start = seconds();
  for (int i = 0; i < LARGE_COUNT; i++) {
    Value result;
    require(tableGet(&table, NUMBER_VAL((double)i), &result),
            "sequential key should be present");
    checksum += AS_NUMBER(result);
  }
  double lookupTime = seconds() - start;

  printf("sequential numeric keys (%d): insert %.4fs, hit lookup %.4fs\n",
         LARGE_COUNT, insertTime, lookupTime);
  freeTable(&table);
}

static void benchmarkRandom(void) {
  Table table;
  initTable(&table);
  Value* keys = malloc(sizeof(Value) * LARGE_COUNT);
  require(keys != NULL, "allocate random key list");

  uint32_t state = 0x13579bdfu;
  for (int i = 0; i < LARGE_COUNT; i++) {
    keys[i] = NUMBER_VAL((double)randomNumber(&state));
  }

  double start = seconds();
  for (int i = 0; i < LARGE_COUNT; i++) {
    tableSet(&table, keys[i], NUMBER_VAL((double)i));
  }
  double insertTime = seconds() - start;

  start = seconds();
  for (int i = 0; i < LARGE_COUNT; i++) {
    Value result;
    require(tableGet(&table, keys[i], &result),
            "random key should be present");
    checksum += AS_NUMBER(result);
  }
  double hitTime = seconds() - start;

  start = seconds();
  for (int i = 0; i < LARGE_COUNT; i++) {
    Value ignored;
    double missing = 4294967296.0 + (double)randomNumber(&state);
    require(!tableGet(&table, NUMBER_VAL(missing),
                      &ignored),
            "random miss should not be present");
  }
  double missTime = seconds() - start;

  printf("random numeric keys (%d): insert %.4fs, hit lookup %.4fs, "
         "miss lookup %.4fs\n",
         LARGE_COUNT, insertTime, hitTime, missTime);
  free(keys);
  freeTable(&table);
}

static void benchmarkCollisions(void) {
  Table table;
  initTable(&table);
  Value keys[COLLISION_COUNT];
  int found = 0;

  // Generate distinct numeric keys that share the same initial bucket at the
  // expected final capacity. This intentionally creates a difficult keyset.
  for (uint32_t candidate = 0; found < COLLISION_COUNT; candidate++) {
    Value key = NUMBER_VAL((double)candidate);
    if ((hashValue(key) & (COLLISION_CAPACITY - 1)) == 17) {
      keys[found++] = key;
    }
  }

  double start = seconds();
  for (int i = 0; i < COLLISION_COUNT; i++) {
    tableSet(&table, keys[i], NUMBER_VAL((double)i));
  }
  double insertTime = seconds() - start;

  start = seconds();
  for (int i = 0; i < COLLISION_COUNT; i++) {
    Value result;
    require(tableGet(&table, keys[i], &result),
            "colliding key should be present");
    checksum += AS_NUMBER(result);
  }
  double lookupTime = seconds() - start;

  printf("same-initial-bucket numeric keys (%d): insert %.4fs, "
         "hit lookup %.4fs\n",
         COLLISION_COUNT, insertTime, lookupTime);
  freeTable(&table);
}

static void benchmarkDeletionChurn(void) {
  Table table;
  initTable(&table);
  for (int i = 0; i < CHURN_COUNT; i++) {
    tableSet(&table, NUMBER_VAL((double)i), NUMBER_VAL((double)i));
  }

  double start = seconds();
  for (int round = 0; round < CHURN_ROUNDS; round++) {
    for (int i = 0; i < CHURN_COUNT; i++) {
      Value key = NUMBER_VAL((double)i);
      require(tableDelete(&table, key), "churn key should delete");
      tableSet(&table, key, NUMBER_VAL((double)(i + round + 1)));
    }
  }
  double churnTime = seconds() - start;

  for (int i = 0; i < CHURN_COUNT; i++) {
    Value result;
    require(tableGet(&table, NUMBER_VAL((double)i), &result),
            "reinserted churn key should be present");
  }

  printf("delete/reinsert churn (%d keys x %d rounds): %.4fs\n",
         CHURN_COUNT, CHURN_ROUNDS, churnTime);
  freeTable(&table);
}

static void checkPrimitiveTypes(void) {
  Table table;
  initTable(&table);

  tableSet(&table, NIL_VAL, NUMBER_VAL(10));
  tableSet(&table, BOOL_VAL(false), NUMBER_VAL(20));
  tableSet(&table, BOOL_VAL(true), NUMBER_VAL(30));
  tableSet(&table, NUMBER_VAL(0), NUMBER_VAL(40));

  Value result;
  require(tableGet(&table, NIL_VAL, &result) && AS_NUMBER(result) == 10,
          "nil key keeps its value");
  require(tableGet(&table, BOOL_VAL(false), &result) && AS_NUMBER(result) == 20,
          "false key keeps its value");
  require(tableGet(&table, BOOL_VAL(true), &result) && AS_NUMBER(result) == 30,
          "true key keeps its value");
  require(tableGet(&table, NUMBER_VAL(0), &result) && AS_NUMBER(result) == 40,
          "number zero remains distinct from nil and false");
  require(valuesEqual(NUMBER_VAL(0.0), NUMBER_VAL(-0.0)),
          "signed zero equality premise");
  freeTable(&table);
  puts("primitive-key check: nil, both Booleans, and numeric zero stay distinct");
}

int main(void) {
  initVM();
  checkPrimitiveTypes();
  benchmarkSequential();
  benchmarkRandom();
  benchmarkCollisions();
  benchmarkDeletionChurn();
  printf("checksum: %.0f\n", checksum);
  freeVM();
  return 0;
}
