

#include "types.h"
# include <stdio.h>
#include <string.h>

extern struct NODE* root;
extern struct NODE* cwd;

//make directory
void mkdir(char pathName[]){

    // TO BE IMPLEMENTED
    //
    // YOUR CODE TO REPLACE THE PRINTF FUNCTION BELOW

    if (strcmp(pathName, "/") == 0 || strlen(pathName) == 0) {
	    printf("MKDIR ERROR: no path provided\n");
	    return;
	 }

    char dirName[128] = "";
    char baseName[64] = "";
    struct NODE* parent = splitPath(pathName, baseName, dirName); // returns parent directory, where the child will be created
    if (parent == NULL) {
	    return; // error is managed in splitpath function
	}
    struct NODE* temp = parent->childPtr; // iterating over all of the parent's children to see if one with the same name already exists
    while(temp != NULL) {
    if (strcmp(temp->name, baseName) == 0 ) {
	    printf("MKDIR ERROR: directory %s already exists\n", pathName);
	    return;
	}
    temp = temp->siblingPtr;
    }

    struct NODE* newDir = (struct NODE*)malloc(sizeof(struct NODE)); // allocate memory for the new directory node
    if (!newDir) {
	    perror("Failed to allocate memory for new directory");
	    return;
	}
    strncpy(newDir->name, baseName, sizeof(newDir->name) - 1);
    newDir->name[sizeof(newDir->name) -1] = '\0';
    strcpy(newDir->name, baseName); // set the name and child, sibling, and parent pointers for the new directory
    newDir->fileType = 'D';
    newDir->childPtr = NULL;
    newDir->siblingPtr = NULL;
    newDir->parentPtr = parent;

	// adding new directory to the file tree    
    if (parent->childPtr == NULL) { // if it is the first child of the directory, then no siblings need to be added
	    parent->childPtr = newDir;
	}
    else { // else iterate through siblings to add to tree
	    temp = parent->childPtr;
	    while (temp->siblingPtr != NULL) {
		    temp = temp->siblingPtr;
		}
	    temp->siblingPtr = newDir;
	 }

    // print message to show directory has been created
    printf("MKDIR SUCCESS: node %s successfully created\n", baseName);

    return;
}

//handles tokenizing and absolute/relative pathing options
struct NODE* splitPath(char* pathName, char* baseName, char* dirName){

    // TO BE IMPLEMENTED./main < test_01.in> myoutput.txt 2>/dev/null
    // NOTE THAT WITHOUT COMPLETING THIS FUNCTION CORRECTLY
    // rm, rmdir, ls, cd, touch COMMANDS WILL NOT EXECUTE CORRECTLY
    // SEE THE PROVIDED EXECUTABLE TO SEE THEIR EXPECTED BEHAVIOR

    // YOUR CODE HERE
    struct NODE* curr;
    // seperate into dirName and baseName: first check if empty
    if (strcmp(pathName, "/") == 0) {
	   strcpy(dirName, "/");
	  strcpy(baseName, "");
	 return root;
	}

   // copy pathname into temporary variable
   char tempPath[128];
  strncpy(tempPath, pathName, sizeof(tempPath));
 	tempPath[127] = '\0'; // null terminator so no overflow 
	
	// seperation of pathName by the last /
	char* last = strrchr(tempPath, '/');	
	if (last != NULL) { // check if last exists
		*last = '\0'; // will terminate at last ptr
		strcpy(baseName, last + 1);
		if (strlen(tempPath) == 0) {
			strcpy(dirName, "/");
		} else {
			strcpy(dirName, tempPath);
		}
	}
	else { // if there is no / in the path, then we are in cwd
		strcpy(dirName, "");
		strcpy(baseName, pathName);
	}
	
	// navigation
	if (dirName[0] == '/') {
		curr = root; // starting from root directory 
	} else {
		curr = cwd; // relative paths
	}

	// tokenize then directory traversal
	char* token = strtok(dirName, "/");
	while (token != NULL) { // loop through directories to traverse 
		struct NODE* found = NULL;
		struct NODE* child = curr->childPtr;

		while (child != NULL) {
			if (strcmp(child->name, token) == 0 && child->fileType == 'D') {
				found = child;
				break;
			}
			child = child->siblingPtr;
		}

		if (found == NULL) { // if directory is not found
			printf("ERROR: directory %s does not exist \n", token);
			return NULL;
		}

		// move to next directory
		curr = found;
		token = strtok(NULL, "/");
	}

	return curr;

    return NULL;
} 
