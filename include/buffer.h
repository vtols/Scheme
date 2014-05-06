#ifndef BUFFER_H
#define BUFFER_H

#include <stddef.h>

#define BUFFER_BLOCK_SIZE 128

typedef struct buffer buffer;
typedef struct buffer_block buffer_block;

struct buffer
{
    /* It's first and last blocks */
    buffer_block *first_block, *last_block;
    
    /* Count of blocks in buffer */
    size_t block_count;
    
    /* Length of string stored in buffer */
    size_t string_length;
    
    /* How many characters used in last block */
    size_t last_fill;
};

struct buffer_block
{
    /* Pointer to next buffer block */
    buffer_block *next_block;
    
    /* Block data */
    char data[BUFFER_BLOCK_SIZE];
};

buffer *buffer_new(const char *s);
void buffer_append_char(buffer *b, char c);
void buffer_append_str(buffer *b, const char *c);

#endif /* BUFFER_H */
