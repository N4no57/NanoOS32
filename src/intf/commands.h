typedef int (*command_func_t)(int argc, char **argv);

typedef struct {
    const char* name;
    command_func_t func;
} command_entry_t;

command_entry_t commands[] = {
    { "help", cmd_help }
};

const int command_count = sizeof(commands) / sizeof(commands[0]);