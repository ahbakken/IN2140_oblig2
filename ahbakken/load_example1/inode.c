#include "allocation.h"
#include "inode.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#define BLOCKSIZE 4096

struct inode* create_file( struct inode* parent, char* name, char readonly, int size_in_bytes )
{
    return NULL;
}

struct inode* create_dir( struct inode* parent, char* name )
{
    return NULL;
}

struct inode* find_inode_by_name( struct inode* parent, char* name )
{
    return NULL;
}



//hjelpefunksjon lager inoder
void lag_inode(int id, char* name, char is_directory, char is_readonly, int filesize, int num_entries, size_t* entries) {
  struct inode *i = malloc(sizeof(struct inode));
  if (i == NULL) {
    fprintf(stderr, "malloc failed\n");
    //rydde litt
    exit(EXIT_FAILURE);
  }
  i -> name = strdup(name);
  if (i->name == NULL) {
    perror("strdup");
    //rydde litt
    exit(EXIT_FAILURE);
  }

  i-> id = id;
  i -> is_directory = is_directory;
  i -> is_readonly = is_readonly;
  i -> filesize = filesize;
  i -> num_entries = num_entries;
  i -> entries = entries;

}

struct inode* load_inodes()
{
  FILE *fil = fopen("superblock", "rb");
  if (fil == NULL) {
    perror("fopen");
    exit (EXIT_FAILURE);
  }
  char navn[256];
  unsigned char navn_len, er_kat, er_les;
  int id, rc, filstr, ant_entr;
  size_t entr;

  //while-loop gaar gjennom superblock-filen og lager inoder
  while(fread(&id, sizeof(char), 1, fil)) {
    if (fread(&navn_len, sizeof(char), 1, fil) == 0) break;

    rc = fread(navn, sizeof(char), navn_len, fil);
    if(rc != navn_len) break;

    rc = fread(&er_kat, sizeof(char), 1, fil);
    rc = fread(&er_les, sizeof(char), 1, fil);
    rc = fread(&filstr, sizeof(int), 1, fil);
    rc = fread(&ant_entr, sizeof(char), 1, fil);
    navn[navn_len] = 0;

    lag_inode(id, navn, er_kat, er_les, filstr, ant_entr, entr);
  }
  if (ferror(fil)) {
    fprintf(stderr, "fread failed\n");
    //rydde litt
    exit(EXIT_FAILURE);
  }


  if(fclose(fil) == EOF) {
    perror("fclose");
    //rydde litt
    exit(EXIT_FAILURE);
  }
  return NULL;
}

void fs_shutdown( struct inode* inode )
{
}

/* This static variable is used to change the indentation while debug_fs
 * is walking through the tree of inodes and prints information.
 */
static int indent = 0;

void debug_fs( struct inode* node )
{
    if( node == NULL ) return;
    for( int i=0; i<indent; i++ )
        printf("  ");
    if( node->is_directory )
    {
        printf("%s (id %d)\n", node->name, node->id );
        indent++;
        for( int i=0; i<node->num_entries; i++ )
        {
            struct inode* child = (struct inode*)node->entries[i];
            debug_fs( child );
        }
        indent--;
    }
    else
    {
        printf("%s (id %d size %db blocks ", node->name, node->id, node->filesize );
        for( int i=0; i<node->num_entries; i++ )
        {
            printf("%d ", (int)node->entries[i]);
        }
        printf(")\n");
    }
}
