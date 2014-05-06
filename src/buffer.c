#include <stdio.h>
#include <stdlib.h>

#include <buffer.h>

static void buffer_free_blocks(buffer *b);

buffer *buffer_new(const char *s)
{
    buffer *b = (buffer *) malloc(sizeof(buffer));
    
    b->first_block = NULL;
    buffer_reset(b);
    
    buffer_append_str(b, s);
    
    return b;
}

void buffer_reset(buffer *b)
{
    buffer_block *first;
    
    if (b->first_block &&
        b->first_block != b->last_block)
        buffer_free_blocks(b);
    
    first = (buffer_block *) malloc(sizeof(buffer_block));
    
    first->next_block = NULL;
    
    b->first_block = b->last_block = first;
    b->block_count = 1;
    b->last_fill = 0;
    b->string_length = 0;
}

void buffer_get_iterator(buffer *b, buffer_iterator *it)
{
    it->it_buf = b;
    it->cur_block = b->first_block;
    it->block_pos = 0;
}

int buffer_iterator_next(buffer_iterator *it)
{
    int c;
    
    if (!it->cur_block ||
        (it->cur_block == it->it_buf->last_block &&
         it->block_pos == it->it_buf->last_fill))
        return EOF;
    
    c = it->cur_block->data[it->block_pos++];
    
    if (it->block_pos == BUFFER_BLOCK_SIZE) {
        it->cur_block = it->cur_block->next_block;
        it->block_pos = 0;
    }
    
    return c;
}

static void buffer_free_blocks(buffer *b)
{
    buffer_block *current, *next;
    
    current = b->first_block;
    while (current) {
        next = current->next_block;
        free(current);
        current = next;
    }
}

void buffer_free(buffer *b)
{
    buffer_free_blocks(b);
    free(b);
}

void buffer_append_char(buffer *b, char c)
{
    buffer_block *new_block;
    
    if (b->last_fill == BUFFER_BLOCK_SIZE) {
        new_block = (buffer_block *) malloc(sizeof(buffer_block));
        new_block->next_block = NULL;
        
        b->last_block->next_block = new_block;
        b->last_block = new_block;
        b->last_fill = 0;
    }
    
    b->last_block->data[b->last_fill++] = c;
    b->string_length++;
}

void buffer_append_str(buffer *b, const char *s)
{
    if (!s)
        return;
    while (*s)
        buffer_append_char(b, *s++);
}

void buffer_append_buffer(buffer *b, buffer *x)
{
    int c;
    buffer_iterator it;
    
    buffer_get_iterator(x, &it);
    
    while((c = buffer_iterator_next(&it)) != EOF)
        buffer_append_char(b, c);
}

char *buffer_to_str(buffer *b)
{
    int c;
    size_t s_pos;
    buffer_iterator it;
    
    buffer_get_iterator(b, &it);
    
    char *s = calloc(b->string_length + 1, sizeof(char));
    
    s_pos = 0;
    while((c = buffer_iterator_next(&it)) != EOF)
        s[s_pos++] = c;
    
    return s;
}
