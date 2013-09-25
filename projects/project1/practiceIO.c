#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int main(int argc, char* argv[]){
    
  printf("%s", argv[1]);

    int x = open(argv[1], O_WRONLY);
	
    pid_t pid;    

    int status = 0;
	
    int count = 0;
    int parentCount = 0;
    
    printf("Reading file now:\n");

    
    if(x == -1){
      printf("no\n");
    }else{
      printf("file is: %d\n", x);
    }

    // pid = fork();
    
    


    close(x);
    
    return 0;   
}

/* The parse_input_line function takes a single line from the input file and a pointer to node_t (with memory allocated)
 * and populates the prog, input, output, children and num_children fields of the node_t structure. It returns 1 on success */
/*
int parse_input_line(char *s, int id, node_t *n) {


	char **strings;
	char **child_list;
	int c = 0;
	if (makeargv(s,":",&strings) == 0) { // chop it at every : char
		return 0; // if makeargv didnt do anything
	}

	//strings[1] has the child list. Chop at every space to get list of child processes
	if(makeargv(strings[1]," ",&child_list) == 0) {
		return 0;
	}

	//set prog, input, output and id fields
	strcpy(n->prog,strings[0]);
	strcpy(n->input,strings[2]);
	strcpy(n->output,strings[3]);
	n->id = id;

	n->num_children = 0;
	//create child list and set number of children
	while (child_list[c] != NULL && strcmp(child_list[c],"none")!=0) {
		n->children[c] = atoi(child_list[c]);
		n->num_children++;
		c++;
	}
	return 1;
}

int makeargv(const char *s, const char *delimiters, char ***argvp) {
	int error;
	int i;
	int numtokens;
	const char *snew;
	char *t;

	if ((s == NULL) || (delimiters == NULL) || (argvp == NULL)) {
		errno = EINVAL;
		return -1;
	}
	*argvp = NULL;
	snew = s + strspn(s, delimiters);
	if ((t = malloc(strlen(snew) + 1)) == NULL)
		return -1;
	strcpy(t,snew);
	numtokens = 0;
	if (strtok(t, delimiters) != NULL)
		for (numtokens = 1; strtok(NULL, delimiters) != NULL; numtokens++) ;

	if ((*argvp = malloc((numtokens + 1)*sizeof(char *))) == NULL) {
		error = errno;
		free(t);
		errno = error;
		return -1;
	}

	if (numtokens == 0)
		free(t);
	else {
		strcpy(t,snew);
		**argvp = strtok(t,delimiters);
		for (i=1; i<numtokens; i++)
			*((*argvp) +i) = strtok(NULL,delimiters);
	}
	*((*argvp) + numtokens) = NULL;
	return numtokens;
}

void freemakeargv(char **argv) {
	if (argv == NULL)
		return;
	if (*argv != NULL)
		free(*argv);
	free(argv);
}
*/
