#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TAPE_CHUNK_SIZE 1024
#define UCHAR unsigned char

static UCHAR* _S_Tape = NULL;
static size_t _S_Tape_Size = 0;
static size_t _S_Pointer = 0;

void resize_tape()
{
    if (_S_Tape_Size <= _S_Pointer)
    {
        size_t new_size = ((_S_Pointer / TAPE_CHUNK_SIZE) + 1) * TAPE_CHUNK_SIZE;
        void *new_tape = realloc(_S_Tape, new_size);
        if (new_tape)
        {
            _S_Tape = new_tape;
            memset(_S_Tape + _S_Tape_Size, 0, new_size - _S_Tape_Size);
            _S_Tape_Size = new_size;
        }
        // todo: check bad alloc
    }
}

struct Command
{
    long cmd;
    size_t arg;
};

size_t parse(const char* code, struct Command *commands)
{
    size_t *open_braces;
    size_t code_len;
    size_t i = 0, j = 0;
    size_t ci = 0, bi = 0, obp;
    char c;

    code_len = strlen(code);
    open_braces = calloc(code_len, sizeof(size_t));

    while (i < code_len)
    {
        c = code[i];
        switch (c)
        {
        case '-':
        case '+':
        case '>':
        case '<':
            j = i;
            while (code[i] == c && i < code_len) i++;
            if (ci && commands[ci - 1].cmd == c)
            {
                commands[ci - 1].arg += i - j;
            }
            else
            {
                commands[ci].cmd = c;
                commands[ci++].arg = i - j;
            }
            continue;
        case '[':
            commands[ci].cmd = c;
            open_braces[bi++] = ci++;
            break;
        case ']':
            obp = open_braces[--bi];
            commands[ci].cmd = c;
            commands[ci].arg = obp;
            commands[obp].arg = ci++;
            break;
        case '.':
        case ',':
            commands[ci++].cmd = c;
        }
        i++;
    }

    free(open_braces);

    return ci;
}

void run_commands(struct Command *commands, size_t ncommands)
{
    size_t pc = 0;

    while(pc < ncommands)
    {
        switch (commands[pc].cmd)
        {
        case '+': _S_Tape[_S_Pointer] += commands[pc].arg;          break;
        case '-': _S_Tape[_S_Pointer] -= commands[pc].arg;          break;
        case '<': _S_Pointer -= commands[pc].arg; resize_tape();    break;
        case '>': _S_Pointer += commands[pc].arg;                   break;
        case '[': if (!_S_Tape[_S_Pointer]) pc = commands[pc].arg;  break;
        case ']': if ( _S_Tape[_S_Pointer]) pc = commands[pc].arg;  break;
        case '.': putchar(_S_Tape[_S_Pointer]);                     break;
        case ',': _S_Tape[_S_Pointer] = (UCHAR)getchar();           break;
        }
        pc++;
    }
}

char* read_file(const char* file_path)
{
    FILE *f = fopen(file_path, "r");
    fseek(f, 0, SEEK_END);
    long fsize = ftell(f);
    fseek(f, 0, SEEK_SET);

    char *result = malloc(fsize + 1);
    fread(result, 1, fsize, f);
    fclose(f);

    result[fsize] = 0;
    return result;
}

int main(int argc, const char **argv)
{
    struct Command *commands;
    char *code;
    size_t ncommands;

    if (argc != 2)
    {
        printf("Usage: %s path_to_src_code\n", argv[0]);
        return 1;
    }

    // Init tape
    resize_tape();

    code = read_file(argv[1]);
    commands = calloc(strlen(code), sizeof(struct Command));
    ncommands = parse(code, commands);
    run_commands(commands, ncommands);

    free(commands);
    free(code);
    free(_S_Tape);

    return 0;
}
