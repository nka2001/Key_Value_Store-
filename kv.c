#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>


bool checks(int k);

struct node {//make a linked list :)

  int key;//store the key
  char * data;//store its value
  struct node *next;//pointer to next node (only one, so singly linked)


};

struct node *head = NULL;//pointer to head
struct node *current = NULL;//current node


int main(int argc, char * argv[]){

  //////////////////////////////////////////////////////////////////////////////////////////////////////

  //initial part, take the contents of the file and load it into a linked list

  
  FILE *fptr = fopen("database.txt", "r");//open the file

  if(!fptr){//if the file doesnt point to anything return an error 
    printf("KV: Error opening file");
    exit(1);
  }
 
  char c = fgetc(fptr);// gets the status, just checks if we are at the end of a file

  char line[500];//current line contents stored here
  char *string;//string for conversion
  
  while(c != EOF){//so, when the file is not at the end of the file, then...
    
   
    if(fgets(line, 500, fptr) != NULL){//if the current line contents is not null
    
      string = line;//set string to the line contents 

      char * token = strtok(string, " ");//split the string into two parts, the key and value

      int count = 0;//count, used for getting the current split string
 
      int k = 0;//k is key, just as an int
      char *value;//value, stores the key's value
      while(token != NULL){//while the token inst null, then

	if(count == 0){//if the count is zero, then 
	   k = atoi(token);//convert the key to an int and store
	} else if(count == 1){//also if the count is one, then
	  value = token;// store the value
	}
	token = strtok(NULL, " ");//split again
	count++;//increment
      }
   
      if(k >= 0){//if the key is < 0, then (im assuming that we do not store keys that are negative or 0)
	struct node *newNode = (struct node*) malloc(sizeof(struct node));//create a new node
   
	newNode->key = k;//store they key
	newNode->data = malloc(strlen(value) + 1);//store the value
	strcpy(newNode->data, value);//copy the string 
	newNode->next = head;//point the new node to the head
	head = newNode;//set the head to the newNode
      } 
  }
    
    c = fgetc(fptr);//get the next line from the file
  }
  
  fclose(fptr);//close the file

  ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  //now we parse the command line and do all the commands specified
  
  for(int i = 1; i < argc; i++){//for loop for going through multiple commands 

						       char* command;//store the command
    const char * key;//store the key
    char * value;//store the value

    char * token = strtok(argv[i], ",");//split the command, based on a , 

    int count = 0;

    while(token != NULL){//same idea as in the initial process
      if(count == 0){
	command = token;
      } else if(count == 1){
	key = token;
      } else if(count == 2){
	value = token;
      }
      token = strtok(NULL, ",");
      count++;
    }
   
  
    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //Put
    
    if(strcmp(command, "p") == 0){
      

      
      
      int k = atoi(key);//convert the key to an int
      if(!checks(k)){//check if the key exists (im assuming no duplicates)
	struct node *newNode = (struct node*) malloc(sizeof(struct node));//add a new node to the linked list
     newNode->key = k;
     newNode->data = value;
     newNode->next = head;
     head = newNode; 
      } else {//otherwise print an error
	printf("KV: Error attempt to enter a duplicate key/value pair, please try again.");
      }



   //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
   //Get

      
    } else if (strcmp(command, "g") == 0){


      int k = atoi(key);//convert the key to an int
      struct node* findMe = head;//create a node to find the value

      if(head == NULL){//if the head doesnt point to anything, then nothing is in the list
	printf("KV: Error Linked List is Empty, check the database");
	exit(1);
      }

      while(findMe->key != k){//while the key doesnt equal the searching key, then

	if(findMe->next == NULL){//if we reach the end of the list, then print an error
	  printf("End of List");

	} else {
	  findMe = findMe->next;//otherwise keeep going
	}
      }
	       
      printf(" %d, %s \n", findMe->key, findMe->data);//print out the asked for key 

    /////////////////////////////////////////////////////////////////////////////////////////////////////////
    // Clear
							  
    } else if (strcmp(command, "c") == 0){

      struct node* current = head;//create a node, 
      struct node* next;

      while(current != NULL){//while the current node isnt null

	next = current->next;//move through the list
	free(current);//free its memory
	current = next;//keep going
      }

      head = NULL;//remove the head

      ////////////////////////////////////////////////////////////////////////////////////////////////////////////
      //Delete
      
    } else if (strcmp(command, "d") == 0){

      int k = atoi(key);//convert the key to an int
      
      struct node* deleteMe = head;//create a node
      struct node* previous = NULL;//as well as another one

      if(head == NULL){
	printf("KV: Error Linked List is empty, check the database");
      }

      while(deleteMe->key != k){//if the key being deleted isnt equal

	if(deleteMe->next == NULL){//if the key is never founf then
	  printf("KV: Error end of list reached, no value by that key: %d was found",deleteMe->key);
	} else {

	  previous = deleteMe;//otherwise, point the previous node to the node that comes after the node being deleted
	  deleteMe = deleteMe->next;

	}
      }

      if(deleteMe == head){//if delete is the head, 
					 head = head->next;//then move the head, and 
      } else {
	previous->next = deleteMe->next;//move some pointers around
      }

      /////////////////////////////////////////////////////////////////////////////////////////////////////////////
      //All
      
    } else if (strcmp(command, "a") == 0) {

      struct node *ptr = head;//create a node

      printf("\n[");//print a [

      while(ptr != NULL){//while we are still moving through the list
	printf("(%d, %s ), \n ", ptr->key, ptr->data);//print the current node
	ptr = ptr->next;//move the pointer forward
      }

      printf(" ]");

      

      /////////////////////////////////////////////////////////////////////////////////////////////
      //Bad Command
      
    } else {

      printf("KV: Error Bad Command, Please try again, the commands are: \n p,<Key>,<Value> - Put \n a - All \n g,<Key> - Get \n c - Clear \n d,<Key> - Delete \n"); //print bad command 
     
																					 }
  
  }

  ////////////////////////////////////////////////////////////////////////////////////////////////////////
  //Write the LinkedList back to the file

  struct node *ptr = head;

  char * values;
  int k = 0;
  char * strings;
  char result[50];

  FILE *fileptr = fopen("database.txt", "w");
  fclose(fptr);
  char buffer[1024];
  while(ptr != NULL){
    k = ptr->key;
    values = ptr->data;
    
    sprintf(result, "%d", k);
    
    snprintf(buffer, sizeof(buffer), " %d, %s \n", k, values);
    
   
       

    FILE *fptr = fopen("database.txt", "a");

    if(fptr == NULL){
      printf("KV: Error opening file to write too");
      exit(1);
    }

    fprintf(fptr, "%s", buffer);
    fclose(fptr);

    
    ptr = ptr->next;
  }
    

  return 0;//all done!
}


bool checks(int k){//method to check the entire linked list for an existing key, if not found, then its false
      struct node* findMe = head;

      if(head == NULL){
	return false;
      }

      while(findMe->key != k){

	if(findMe->next == NULL){
	 
	  return false;
	} else {
	  findMe = findMe->next;
	}
      }   
      return true;
}


  

