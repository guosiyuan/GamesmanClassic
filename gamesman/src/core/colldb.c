/************************************************************************
**
** NAME:	memdb.c
**
** DESCRIPTION:	Accessor functions for the in-memory database.
**
** AUTHOR:	GamesCrafters Research Group, UC Berkeley
**		Supervised by Dan Garcia <ddgarcia@cs.berkeley.edu>
**
** DATE:	2005-01-11
**
** LICENSE:	This file is part of GAMESMAN,
**		The Finite, Two-person Perfect-Information Game Generator
**		Released under the GPL:
**
** This program is free software; you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation; either version 2 of the License, or
** (at your option) any later version.
**
** This program is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU General Public License for more details.
**
** You should have received a copy of the GNU General Public License
** along with this program, in COPYING; if not, write to the Free Software
** Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
**
**************************************************************************/

#include "gamesman.h"
#include "colldb.h"
#include "db.h"



colldb_value_node** colldb_hash_table;
POSITION colldb_num_allocated;
#define colldb_HASHSIZE 1019 //currently hardcoded. Next revision calculate
/*
** Code
*/


DB_Table* colldb_init(){
  //Make db_table
  DB_Table *new_db = (DB_Table *) SafeMalloc(sizeof(DB_Table));
  //setup internal memory table
  colldb_hash_table = (colldb_value_node **) SafeMalloc (colldb_HASHSIZE * sizeof(colldb_value_node*));
  
  
  //set function pointers
  new_db->get_value = colldb_get_value;
  new_db->put_value = colldb_set_value;
  new_db->get_remoteness = colldb_get_remoteness;
  new_db->put_remoteness = colldb_set_remoteness;
  new_db->check_visited = colldb_check_visited;
  new_db->mark_visited = colldb_mark_visited;
  new_db->unmark_visited = colldb_unmark_visited;
  
  new_db->free_db = colldb_free;
  
  return new_db;
}

void colldb_free(){
  int i;
  colldb_value_node *next, *cur;
  if(colldb_hash_table){
    for(i=0;i<colldb_HASHSIZE;i++){
      cur = colldb_hash_table[i];
      while(cur != NULL){
	next = cur->next;
	SafeFree(cur);
	cur = next;
      }
    }
    SafeFree(colldb_hash_table);
  }
}

colldb_value_node *colldb_find_pos(POSITION position){
  POSITION i;
  colldb_value_node *cur;

  i = position % colldb_HASHSIZE;
  cur = colldb_hash_table[i];

  while(cur != NULL){
    if(cur->myPos == position)
      return cur;
    cur = cur->next;
  }
  
  return NULL;
  
  
}

colldb_value_node* colldb_make_node(POSITION pos, VALUE val, colldb_value_node *next){
  colldb_value_node *ptr = (colldb_value_node *) SafeMalloc (sizeof(colldb_value_node));
  ptr->myPos = pos;
  ptr->myValue = val;
  ptr->next = next;
  colldb_num_allocated++;
  return ptr;
}
colldb_value_node *colldb_add_node(POSITION position){
  POSITION i;
  i = position % colldb_HASHSIZE;
  colldb_hash_table[i] = colldb_make_node(position, undecided, colldb_hash_table[i]);
  return colldb_hash_table[i];
}

VALUE colldb_set_value(POSITION position, VALUE value)
{
  colldb_value_node *ptr;
  
  ptr = colldb_find_pos(position);
  if(ptr == NULL)
    ptr = colldb_add_node(position);
  
  ptr->myValue = (ptr->myValue & ~VALUE_MASK) | (value & VALUE_MASK);    

  return value & VALUE_MASK;
}

// This is it
VALUE colldb_get_value(POSITION position)
{
  colldb_value_node *ptr;
  
  ptr = colldb_find_pos(position);
  if(ptr == NULL)
    return undecided;
  return ptr->myValue & VALUE_MASK;

}

REMOTENESS colldb_get_remoteness(POSITION position)
{
  colldb_value_node *ptr;
  
  ptr = colldb_find_pos(position);
  if(ptr == NULL)
    return 0;
  return (ptr->myValue & REMOTENESS_MASK) >> REMOTENESS_SHIFT;
  
}

void colldb_set_remoteness (POSITION position, REMOTENESS remoteness)
{
  colldb_value_node *ptr;
  
  if(remoteness > REMOTENESS_MAX) {
    printf("Remoteness request (%d) for " POSITION_FORMAT  " larger than Max Remoteness (%d)\n",remoteness,position,REMOTENESS_MAX);
    ExitStageRight();
    exit(0);
  }
  ptr = colldb_find_pos(position);
  if(ptr == NULL)
    ptr = colldb_add_node(position);
  
  ptr->myValue = (ptr->myValue & ~REMOTENESS_MASK) | (remoteness << REMOTENESS_SHIFT);
}

BOOLEAN colldb_check_visited(POSITION position)
{
  colldb_value_node *ptr;
  
  ptr = colldb_find_pos(position);
  if(ptr == NULL)
    return FALSE;

  return ((ptr->myValue & VISITED_MASK) == VISITED_MASK);
}

void colldb_mark_visited (POSITION position)
{
  colldb_value_node *ptr;
  
  ptr = colldb_find_pos(position);
  if(ptr == NULL)
    return;
      
  ptr->myValue = ptr->myValue | VISITED_MASK;
}

void colldb_unmark_visited (POSITION position)
{
  colldb_value_node *ptr;
  
  ptr = colldb_find_pos(position);
  if(ptr == NULL)
    return;

  ptr->myValue = ptr->myValue & ~VISITED_MASK;
}

/* Not implemented yet. Will implement once actually using real db classes
void MexStore(POSITION position, MEX theMex)
{
    if (!gTwoBits)
        gDatabase[position] |= ((theMex % 32) * 8) ;
}

MEX MexLoad(POSITION position)
{
#ifdef SYMMETRY_REVISITED
    position = GetCanonicalPosition(position);
#endif
    //Gameline code removed
    return (gTwoBits ? -1 : (gDatabase[position]/8) % 32);
}
*/

