typedef int (*command_func_t)(int argc, char **argv);

typedef struct {
    const char* name;
    command_func_t func;
} command_entry_t;

extern command_entry_t commands[];
extern int command_count;

int run_command(int argc, char **argv);
int cmd_help(int argc, char **argv);
int cmd_exit(int argc, char **argv);
int cmd_clear(int argc, char **argv);