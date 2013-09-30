/* CSci4061 F2013 Assignment 1
 * login: poon0045
 * date: 9/25/2013
 * name: Vivian Poon, Daryl Xayavong
 * id: 4186433, 3948244 */

#include "main.h"


// =========================== Free Make Argv ===========================
// given from project1 files:

void freemakeargv(char **argv) {
	if (argv == NULL)
		return;
	if (*argv != NULL)
		free(*argv);
	free(argv);
}


// ============================== Make Argv ==============================
// given from project1 files:

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


// ========================== Parse Input Line ==========================
// takes a single line from the input file and a pointer to node_t (with memory allocated)
// and populates the prog, input, output, children and num_children fields of the node_t structure
// returns 1 on success

int parse_input_line(char *s, int id, node_t *node) {
    
	char **strings;
	char **child_list;
    char* line_location;
	int count = 0;
    int i;
    
    //parse lines at every : and check errors
    int tok_count = makeargv(s, ":", &strings);
	if (tok_count == -1) return 0;
    
	//parse child list (strings[1]) at every space for processes, check errors
	if(makeargv(strings[1], " ", &child_list) == -1) return 0;
    
    //read text line into line_location
    for (i = 0; i < tok_count; i++) {
		if((line_location = strpbrk(strings[i], "\n\r"))){
			*line_location = '\0';
        }
	}
    
	//set prog, input, output and id fields
	strcpy(node->prog,strings[0]);
	strcpy(node->input,strings[2]);
	strcpy(node->output,strings[3]);
	node->id = id;
    
	node->num_children = 0;
    
	//create child list and set number of children
	while (child_list[count] != NULL && strcmp(child_list[count],"none")!=0) {
		node->children[count] = atoi(child_list[count]);
		node->num_children++;
		count++;
	}
    
    freemakeargv(strings);
	return 1;
}


// ============================= Add Parent =============================
// assigns parent (via parent ID) to given node if parent isn't already linked

void add_parent(node_t * node, int parent_id) {
	int parent_count = node->num_parents;
    bool already_parent = false;
    int i;
    
    //check to see if parent ID already exists in node's parent array
    for (i = 0; i < parent_count; i++)
        if(node->parents[i] == parent_id){
            already_parent = true;
            break;
        }
    if (already_parent = false){
        node->parents[node->num_parents++] = parent_id;
    }
}


// ============================= Link Nodes =============================
// links children nodes to parent nodes
// by taking a list of nodes and fills in parent information in each node
// using the info from the node struct variables

void link_nodes(node_t * nodes[], int node_count) {
	int parent_id;
	int child_id;
	int child_index;
	int child_count;
	node_t * temp;
    
    //iterates through each parent in array
	for (parent_id = 0; parent_id < node_count; parent_id++) {
		temp = nodes[parent_id];
		child_count = temp->num_children;
        
        //iterate through parent's children list and add parent if applicable
		for (child_index = 0; child_index < child_count; child_index++) {
			child_id = temp->children[child_index];
			if (child_id < node_count) {
				add_parent(nodes[child_id], parent_id);
			}
		}
	}
}


// ============================= Read File =============================
// reads input file and parses lines into an array of nodes
// uses other functions to populate node variables and link them together
// returns number of nodes in new array

int read_file(FILE * file, node_t * array[], int max_nodes) {
	char line[max_nodes];
	int count = 0;
    
	while (fgets(line, MAX_LENGTH, file)) {
        node_t * node = (node_t*)malloc(sizeof(node_t));
        //parse node and check that it was properly created
        if(parse_input_line(line, count, node) == 1){
            array[count] = node;
			count++;
        } else {
            perror("Could not produce node from line");
            return -1;
        }
	}
	link_nodes(array, count);
	return count;
}


// ========================= Determine Eligible =========================
// checks each node in the entire graph to determine if each is eligible to run
// by marking status as READY if all the node's parents' statuses are FINISHED

void determine_eligible(node_t * nodes[], int count) {
	int i, j, parent_count;
    bool is_ready;
    node_t * node = (node_t*)malloc(sizeof(node_t));
    
    
	for (i = 0; i < count; i++) {
        node = nodes[i];
        parent_count = node->num_parents;
        is_ready = true;
        //only update node if originally INELIGIBLE
        if (node->status == INELIGIBLE) {
            //iterate through only the parents within the array of nodes
            while(j < parent_count && is_ready) {
                int parent_id = node->parents[j];
                if (nodes[parent_id]->status != FINISHED) {
                    is_ready = false;
                }
                j++;
            }
            if (is_ready) node->status = READY;
        }
	}
}


// ============================= Run Node =============================
// runs node, assuming it is eligible, and does the nessecary forks, execs, etc
// returns status of the node after completion, which should be 3 (FINISHED)
// or returns -1 if failed to run

int run_node(node_t * node) {
	char ** child_argv;
	int child_argc;
	int oldstdin, oldstdout;
	int input_fd, output_fd;
    
	int status;
	pid_t child_pid;
    
	//store old input and output file descriptors
	fflush(stdout);
	if ((oldstdin = dup(0)) == -1) {
		perror("Failed to back-up stdin");
        return -1;
    }
	if ((oldstdout = dup(1)) == -1) {
		perror("Failed to back-up stdout");
		return -1;
	}
    
	//open input and output file descriptors
	if ((input_fd = open(node->input, O_RDONLY | O_CREAT, 0644)) == -1) {
		perror("Failed to open input file");
        return -1;
    }
	if ((output_fd = open(node->output, O_WRONLY | O_CREAT, 0644)) == -1) {
		perror("Failed to open output file");
        return -1;
    }
    
	//redirect input and output file descriptors
	fflush(stdout);
	if (dup2(input_fd, STDIN_FILENO) == -1) {
		perror("Failed to redirect stdin");
        return -1;
    }
	if (dup2(output_fd, STDOUT_FILENO) == -1) {
		perror("Failed to redirect stdout");
        return -1;
    }
    
    //parse arg at every space
	child_argc = makeargv(node->prog, " ", &child_argv);
    
    node->status = RUNNING;
    
	//fork, exec, and wait
	node->pid = child_pid = fork();
	if (child_pid) wait(&status);
	else execvp(child_argv[0], &child_argv[0]);

	freemakeargv(child_argv);
    
	//node->return_value = status;
	node->status = FINISHED;
    
    
	//replace old input and output file descriptors
	fflush(stdout);
	if (dup2(oldstdout, STDOUT_FILENO) == -1) {
		perror("Failed to redirect stdout to original stdout");
        return -1;
    }
	if (dup2(oldstdin, STDIN_FILENO) == -1) {
		perror("Failed to redirect stdin to original stdin");
        return -1;
    }
    
	if (close(input_fd)) perror("Failed to close input_fd");
	if (close(output_fd)) perror("Failed to close output_fd");
	if (close(oldstdin)) perror("Failed to close oldstdin");
	if (close(oldstdout)) perror("Failed to close oldstdout");
    
	return node->status;
}


// ============================= Free Array =============================
// takes array of nodes and frees allocated memory

void free_array(node_t * nodes[], int count) {
	int i;
	for(i = 0; i < count; i++) {
		if (nodes[i]) free(nodes[i]);
	}
}

// ================================ Main ================================

int main(int argc, const char * argv[]) {
	char filename[MAX_LENGTH];
	node_t * nodes[MAX_NODES];
	FILE * file;
	int count;
	int i;

    //check that there is correct number of arguments
	if (argc != 2) {
		printf("graphexec requires 2 arguments.\n");
        return -1;
    }
    
    //open file and check that it was sucessful
	file = fopen(argv[1], "r");
	if (!file) {
		printf("The file specified does not exist or could not be opened.\n");
	}
    
    //if things check out, proceed to read and process file
    else {
		count = read_file(file, nodes, MAX_NODES);
        bool finished;
        
		while (!finished) {
			finished = true;
			determine_eligible(nodes, count);
            
            //iterate through and run nodes that are marked READY
			for (i = 0; i < count; i++) {
				if (nodes[i]->status != FINISHED) {
					finished = false;
					if (nodes[i]->status == READY) {
						printf("Node %i Status: Running", i);
						if(run_node(nodes[i]) == FINISHED &&
                            run_node(nodes[i]) != -1) {
                            printf(" >> Finished!\n");
                        } else
                            perror("Error running node");
					}
				}
			}
		}
        
		//cleanup
		free_array(nodes, count);
        if (fclose(file) == -1)
            perror("Failed to close file");
	}
    
	return 0;
}