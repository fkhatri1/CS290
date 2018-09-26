/*
 * CS 261 Data Structures
 * Assignment 5
 * Name: Faysal Khatri
 * Date: June 3, 2018
 */

#include "hashMap.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <ctype.h>

int hashFunction1(const char* key)
{
    int r = 0;
    for (int i = 0; key[i] != '\0'; i++)
    {
        r += key[i];
    }
    return r;
}

int hashFunction2(const char* key)
{
    int r = 0;
    for (int i = 0; key[i] != '\0'; i++)
    {
        r += (i + 1) * key[i];
    }
    return r;
}

/**
 * Creates a new hash table link with a copy of the key string.
 * @param key Key string to copy in the link.
 * @param value Value to set in the link.
 * @param next Pointer to set as the link's next.
 * @return Hash table link allocated on the heap.
 */
HashLink* hashLinkNew(const char* key, int value, HashLink* next)
{
    HashLink* link = malloc(sizeof(HashLink));
    link->key = malloc(sizeof(char) * (strlen(key) + 1));
    strcpy(link->key, key);
    link->value = value;
    link->next = next;
    return link;
}

/**
 * Free the allocated memory for a hash table link created with hashLinkNew.
 * @param link
 */
static void hashLinkDelete(HashLink* link)
{
    free(link->key);
    free(link);
}

/**
 * Initializes a hash table map, allocating memory for a link pointer table with
 * the given number of buckets.
 * @param map
 * @param capacity The number of table buckets.
 */
void hashMapInit(HashMap* map, int capacity)
{
    map->capacity = capacity;
    map->size = 0;
    map->table = malloc(sizeof(HashLink*) * capacity);
    for (int i = 0; i < capacity; i++)
    {
        map->table[i] = NULL;
    }
}

/**
 * Removes all links in the map and frees all allocated memory. You can use
 * hashLinkDelete to free the links.
 * @param map
 */
void hashMapCleanUp(HashMap* map)
{
    // FIXED
	for (int idx = 0; idx<map->capacity; idx++) {
		HashLink* currLink = map->table[idx];
		while (currLink != NULL) {
			HashLink* temp = currLink;
			currLink = currLink->next;
			hashLinkDelete(temp);
		}
	}
}

/**
 * Creates a hash table map, allocating memory for a link pointer table with
 * the given number of buckets.
 * @param capacity The number of buckets.
 * @return The allocated map.
 */
HashMap* hashMapNew(int capacity)
{
    HashMap* map = malloc(sizeof(HashMap));
    hashMapInit(map, capacity);
    return map;
}

/**
 * Removes all links in the map and frees all allocated memory, including the
 * map itself.
 * @param map
 */
void hashMapDelete(HashMap* map)
{
    hashMapCleanUp(map);
    free(map);
}

/**
 * Returns a pointer to the value of the link with the given key  and skip traversing as well. Returns NULL
 * if no link with that key is in the table.
 * 
 * Use HASH_FUNCTION(key) and the map's capacity to find the index of the
 * correct linked list bucket. Also make sure to search the entire list.
 * 
 * @param map
 * @param key
 * @return Link value or NULL if no matching link.
 */
int* hashMapGet(HashMap* map, const char* key)
{
    // FIXED
	int idx = HASH_FUNCTION(key) % hashMapCapacity(map);
	HashLink* currLink = map->table[idx];
	
	while (currLink != NULL) {
		if (strcmp(currLink->key, key) == 0) {
			return (& (currLink->value));
		}
		currLink = currLink->next;
	}
	
	return NULL;
}

/**
 * Resizes the hash table to have a number of buckets equal to the given
 * capacity. After allocating the new table, all of the links need to be
 * rehashed into it because the capacity has changed.
 * 
 * Remember to free the old table and any old links if you use hashMapPut to
 * rehash them.
 * 
 * @param map
 * @param capacity The new number of buckets.
 */
void resizeTable(HashMap* map, int capacity)
{
    // FIXED
	HashMap* newMap = hashMapNew(capacity);
	
	for (int idx = 0; idx<map->capacity; idx++) {
		HashLink* currLink = map->table[idx];
		while (currLink != NULL) {
			hashMapPut(newMap, currLink->key, currLink->value);
			currLink = currLink->next;
		}
	}
	
	hashMapDelete(map);
	map = newMap;	
	
}

/**
 * Updates the given key-value pair in the hash table. If a link with the given
 * key already exists, this will just update the value and skip traversing. Otherwise, it will
 * create a new link with the given key and value and add it to the table
 * bucket's linked list. You can use hashLinkNew to create the link.
 * 
 * Use HASH_FUNCTION(key) and the map's capacity to find the index of the
 * correct linked list bucket. Also make sure to search the entire list.
 * 
 * @param map
 * @param key
 * @param value
 */
void hashMapPut(HashMap* map, const char* key, int value)
{
    // FIXED
	int idx = HASH_FUNCTION(key) % hashMapCapacity(map);
	HashLink* currLink = map->table[idx];
	
	// CASE 1: Key already exists in map
	while (currLink != NULL) {
		if (strcmp(currLink->key, key) == 0) {
			currLink->value = value;
			return;
		}
		currLink = currLink->next;
	}
	
	// CASE 2: Key has not been added to map
	HashLink* newLink = hashLinkNew(key, value, map->table[idx]);
	map->table[idx] = newLink;
	(map->size)++;
	return;
}

/**
 * Removes and frees the link with the given key from the table. If no such link
 * exists, this does nothing. Remember to search the entire linked list at the
 * bucket. You can use hashLinkDelete to free the link.
 * @param map
 * @param key
 */
void hashMapRemove(HashMap* map, const char* key)
{
    // FIXED
	int idx = HASH_FUNCTION(key) % hashMapCapacity(map);
	HashLink* currLink = map->table[idx];
	
	//CASE 1:  First link to be removed
		if (strcmp(currLink->key, key) == 0) {
			map->table[idx] = currLink->next;
			(map->size)--;
			free(currLink);
		}
	
	//CASE 2:  Later link to be removed
	while (currLink->next != NULL) {
		if (strcmp(currLink->next->key, key) == 0) {
			currLink->next = currLink->next->next;
			(map->size)--;
			free(currLink->next);
			return;
		}
		currLink = currLink->next;
	}
	
    return;
}

/**
 * Returns 1 if a link with the given key is in the table and 0 otherwise.
 * 
 * Use HASH_FUNCTION(key) and the map's capacity to find the index of the
 * correct linked list bucket. Also make sure to search the entire list.
 * 
 * @param map
 * @param key
 * @return 1 if the key is found, 0 otherwise.
 */
int hashMapContainsKey(HashMap* map, const char* key)
{
    // FIXED
	int idx = HASH_FUNCTION(key) % hashMapCapacity(map);
	HashLink* currLink = map->table[idx];
	
	while (currLink != NULL) {
		if (strcmp(currLink->key, key) == 0) {
			return 1;
		}
		currLink = currLink->next;
	}
	
    return 0;
}

/**
 * Returns the number of links in the table.
 * @param map
 * @return Number of links in the table.
 */
int hashMapSize(HashMap* map)
{
    // FIXED
    return map->size;
}

/**
 * Returns the number of buckets in the table.
 * @param map
 * @return Number of buckets in the table.
 */
int hashMapCapacity(HashMap* map)
{
    // FIXED
	return map->capacity;
}

/**
 * Returns the number of table buckets without any links.
 * @param map
 * @return Number of empty buckets.
 */
int hashMapEmptyBuckets(HashMap* map)
{
    // FIXED
	int ret = 0;
	
	for (int i = 0; i < map->capacity; i++)
    {
        if (map->table[i] == NULL) {
			ret++;
		}
    }
	
	return ret;
}

/**
 * Returns the ratio of (number of links) / (number of buckets) in the table.
 * Remember that the buckets are linked lists, so this ratio tells you nothing
 * about the number of empty buckets. Remember also that the load is a floating
 * point number, so don't do integer division.
 * @param map
 * @return Table load.
 */
float hashMapTableLoad(HashMap* map)
{
    // FIXED
    return hashMapSize(map)/hashMapCapacity(map);
}

/**
 * Prints all the links in each of the buckets in the table.
 * @param map
 */
void hashMapPrint(HashMap* map)
{
    for (int i = 0; i < map->capacity; i++)
    {
        HashLink* link = map->table[i];
        if (link != NULL)
        {
            printf("\nBucket %i ->", i);
            while (link != NULL)
            {
                printf(" (%s, %d) ->", link->key, link->value);
                link = link->next;
            }
        }
    }
    printf("\n");
}