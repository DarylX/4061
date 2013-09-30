#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>
#include <stdbool.h>

// exit calls
#define	EXIT_FAIL_TO_OPEN_FILES -2
#define	EXIT_FAIL_TO_DUP_FILES -2
#define	EXIT_BAD_INPUT -3


// taken from project writeup:

// for 'status' variable:
#define INELIGIBLE 0
#define READY 1
#define RUNNING 2
#define FINISHED 3

#define MAX_LENGTH 1024
#define MAX_CHILDREN 10
#define MAX_NODES 50

typedef struct node {
    int id; // corresponds to line number in graph text file
    char prog[MAX_LENGTH]; // prog + arguments
    char input[MAX_LENGTH]; // filename
    char output[MAX_LENGTH]; // filename
    int children[MAX_CHILDREN]; // children IDs
    int parents[MAX_NODES]; // parent IDs
    
    int num_children; // how many children this node has
    int num_parents; // how many parents this node has
    int status; // ineligible/ready/running/finished
    pid_t pid; // track it when it’s running
} node_t;