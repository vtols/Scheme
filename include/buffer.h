#ifndef BUFFER_H
#define BUFFER_H

#include <stddef.h>
#include <stdarg.h>

#define BUFFER_BLOCK_SIZE 128

typedef struct buffer buffer;
typedef struct buffer_block buffer_block;
typedef struct buffer_iterator buffer_iterator;

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

struct buffer_iterator
{
    /* Buffer for iteration */
    buffer *it_buf;
    
    /* Current block */
    buffer_block *cur_block;
    
    /* Position in block */
    size_t block_pos;
};

buffer *buffer_new(const char *s);
void buffer_reset(buffer *b);
void buffer_get_iterator(buffer *b, buffer_iterator *it);
int buffer_iterator_next(buffer_iterator *it);
void buffer_free(buffer *b);
void buffer_append_char(buffer *b, char c);
void buffer_append_str(buffer *b, const char *c);
void buffer_append_buffer(buffer *b, buffer *x);
char *buffer_to_str(buffer *b);

void buffer_printf(buffer *b, const char *format, ...);
buffer *buffer_nprintf(const char *format, ...);
void buffer_vprintf(buffer *b, const char *format, va_list ap);

#endif /* BUFFER_H */
