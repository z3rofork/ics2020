#include <isa.h>
#include "expr.h"
#include "watchpoint.h"
#include "cpu/exec.h"
#include <stdlib.h>
#include <readline/readline.h>
#include <readline/history.h>

void cpu_exec(uint64_t);
int is_batch_mode();
static int cmd_q(char *args);
/* We use the `readline' library to provide more flexibility to read from stdin. */
static char* rl_gets() {
  static char *line_read = NULL;

  if (line_read) {
    free(line_read);
    line_read = NULL;
  }

  line_read = readline("(nemu) ");

  if (line_read && *line_read) {
    add_history(line_read);
  }

  return line_read;
}

static int cmd_c(char *args) {
  cpu_exec(-1);
  return 0;
}

static int cmd_si(char *args){
  int step = 0;
  char *arg = strtok(NULL," ");
  // judge the args is int 
  if (arg == NULL)  { cpu_exec(1); return 0;}
  sscanf(arg,"%d",&step);
  //if (step == -1)  { cpu_exec(-1); return 0;}
  cpu_exec(step);
  return 0;
}

static int cmd_x(char *args){
  //bool success = true;
  if(args == NULL) {printf("expecting more arguements.");return 1;} 
  char *arg1 = strtok(NULL," ");
  int steps = atoi(arg1);
  if(steps == 0){printf("steps must be setted.\n");return 1;}
  char *arg2 = strtok(NULL," ");
  uint32_t address;
  sscanf(arg2,"%x",&address);
  if(address == '0') {printf("memory address must be setted.\n");return 1;}
    /* code */
  //expr(address,&success);
  //if (success==0)   {return 0;}
  printf("0x%08x:\t ",address);
  int i = 1;
  while(steps){
    if ((i) % 4 == 0){
      printf("\n");
      printf("0x%08x:\t",address);
    }
    printf("0x%08x\t",vaddr_read(address,4));
    address+=4;
    steps--;
    i++;
  }
  printf("\n");
  return 0;  
}

static int cmd_q(char *args) {
  return -1;
}

static int cmd_info_regs(char *args){
  char *arg = strtok(NULL," ");
  if(strcmp(arg,"r") == 0){
    isa_reg_display();
  }
  return 0;
}

static int cmd_help(char *args);

static struct {
  char *name;
  char *description;
  int (*handler) (char *);
} cmd_table [] = {
  { "help", "Display informations about all supported commands", cmd_help },
  { "c", "Continue the execution of the program", cmd_c },
  { "q", "Exit NEMU", cmd_q },

  /* TODO: Add more commands */
  {"si","step N steps in the program ,N =1 by default.",cmd_si},
  {"info","print the status of registers",cmd_info_regs},
  {"x","print the hex value of memoery" , cmd_x},

};

#define NR_CMD (sizeof(cmd_table) / sizeof(cmd_table[0]))

static int cmd_help(char *args) {
  /* extract the first argument */
  char *arg = strtok(NULL, " ");
  int i;

  if (arg == NULL) {
    /* no argument given */
    for (i = 0; i < NR_CMD; i ++) {
      printf("%s - %s\n", cmd_table[i].name, cmd_table[i].description);
    }
  }
  else {
    for (i = 0; i < NR_CMD; i ++) {
      if (strcmp(arg, cmd_table[i].name) == 0) {
        printf("%s - %s\n", cmd_table[i].name, cmd_table[i].description);
        return 0;
      }
    }
    printf("Unknown command '%s'\n", arg);
  }
  return 0;
}

void ui_mainloop() {
  if (is_batch_mode()) {
    cmd_c(NULL);
    return;
  }

  for (char *str; (str = rl_gets()) != NULL; ) {
    char *str_end = str + strlen(str);

    /* extract the first token as the command */
    char *cmd = strtok(str, " ");
    if (cmd == NULL) { continue; }

    /* treat the remaining string as the arguments,
     * which may need further parsing
     */
    char *args = cmd + strlen(cmd) + 1;
    if (args >= str_end) {
      args = NULL;
    }

#ifdef HAS_IOE
    extern void sdl_clear_event_queue();
    sdl_clear_event_queue();
#endif

    int i;
    for (i = 0; i < NR_CMD; i ++) {
      if (strcmp(cmd, cmd_table[i].name) == 0) {
        if (cmd_table[i].handler(args) < 0) { return; }
        break;
      }
    }

    if (i == NR_CMD) { printf("Unknown command '%s'\n", cmd); }
  }
}
