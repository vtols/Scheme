#include <stdlib.h>

#include <buffer.h>

buffer *buffer_new(const char *s)
{
    buffer *b = (buffer *) malloc(sizeof(buffer));
    buffer_block *first = (buffer_block *) malloc(sizeof(buffer_block));
    
    first->next_block = NULL;
    
    b->first_block = b->last_block = first;
    b->block_count = 1;
    b->last_fill = 0;
    b->string_length = 0;
    
    buffer_append_str(b, s);
    
    return b;
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
}

void buffer_append_str(buffer *b, const char *s)
{
    if (!s)
        return;
    while (*s)
        buffer_append_char(b, *s++);
}

char *buffer_to_str(buffer *b)
{
    size_t s_pos, b_pos, count;
    buffer_block *current_block = b->first_block;
    
    char *s = calloc(b->string_length + 1, sizeof(char));
    
    s_pos = 0;
    while(current_block) {
        count = (current_block == b->last_block ? 
                    b->last_fill :
                    BUFFER_BLOCK_SIZE);
        for (b_pos = 0; b_pos < count; b_pos++, s_pos++)
            s[s_pos] = current_block->data[b_pos];
        current_block = current_block->next_block;
    }
    
    return s;
}
