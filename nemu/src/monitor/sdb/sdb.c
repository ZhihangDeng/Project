#include <isa.h>
#include <cpu/cpu.h>
#include <readline/readline.h>
#include <readline/history.h>
#include "sdb.h"
#include<memory/paddr.h>

static int is_batch_mode = false;

void init_regex();
void init_wp_pool();
int htoi(char s[])
{
    int i = 0;

    if (s[i] == '0')
    {
        ++i;
    }

    if (s[i] == 'x' || s[i] == 'X')
    {
        ++i;    
    }

    int n = 0; 
    int hexdigit; 

    for(; i < strlen(s); ++i ){
        if (s[i] >= '0' && s[i] <= '9'){
            hexdigit = s[i] - '0';
            n = 16 * n + hexdigit;
        }
        if (s[i] >= 'A' && s[i] <= 'F'){
            hexdigit = s[i] - 'A' + 10;
            n = 16 * n + hexdigit;
        }
        if (s[i] >= 'a' && s[i] <= 'f'){
            hexdigit = s[i] - 'a' + 10;
            n = 16 * n + hexdigit;
        }
    }
    return n;
}
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


static int cmd_q(char *args) {
  return -1;
}

static int cmd_si(char *args){
  int n = args?atoi(args):1;
  cpu_exec(n);	
  return 0;
}

static int cmd_x(char *args){
  char *n_str, *base_str;
  paddr_t base;
  word_t word_read;

  if((n_str = strtok(NULL," ")) && (base_str = strtok(NULL," "))){
    int n = atoi(n_str);
    base = htoi(base_str);
    for(int i = 0;i < n; i++){
      word_read = paddr_read(base+4*i, 4);
      printf("0x%8x:\t0x%08x\n", base+4*i, word_read);
    }
  }
  return 0;
}

static int cmd_p(char *args){
	bool valid = NULL;
	uint32_t res=expr(args,&valid);
	if(valid==true){
		printf("%u\n",res);
	}
	else{
		printf("Invalid expression\n");
	}
	return 0;

}

static int cmd_info(char *args){
  if(!args){
    printf("Input args!");
  }else if(strcmp(args,"r") == 0){
    isa_reg_display();
  }else if(strcmp(args,"w") == 0){

  }
  return 0;
}

static int cmd_help(char *args);

static struct {
  const char *name;
  const char *description;
  int (*handler) (char *);
} cmd_table [] = {
  { "help", "Display informations about all supported commands", cmd_help },
  { "c", "Continue the execution of the program", cmd_c },
  { "q", "Exit NEMU", cmd_q },
  { "si", "",cmd_si},
  { "x", "",cmd_x},
  { "info", "",cmd_info},
	{"p", "p EXPR: evaluate the expression", cmd_p }
};

#define NR_CMD ARRLEN(cmd_table)

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

void sdb_set_batch_mode() {
  is_batch_mode = true;
}

void sdb_mainloop() {
  if (is_batch_mode) {
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

#ifdef CONFIG_DEVICE
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

void init_sdb() {
  /* Compile the regular expressions. */
  init_regex();

  /* Initialize the watchpoint pool. */
  init_wp_pool();
}
