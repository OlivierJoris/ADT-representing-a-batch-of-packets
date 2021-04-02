/*
 * MAGIC interface.
 * 
 * @author Maxime Goffart (180521) & Olivier Joris (182113)
 */

#ifndef __MAGIC__
#define __MAGIC__

/*
 * ADT that manages the matching between adresses and indexes
 * of the auxiliary array (opaque type)
 */
typedef struct MAGIC *MAGIC;

/*
 * Initialises the MAGIC structure to store a maximum of 
 * maxSize addresses each one with a size of addrSize bytes.
 * 
 * @param maxSize Maximum number of addresses.
 * @param addrSize Size in bytes of one address.
 * 
 * @return The newly created structure.
 */
MAGIC MAGICinit(int maxSize, int addrSize);

/*
 * Searches for the corresponding index of the address dest in 
 * the structure m. If no index is matching, we matches this 
 * address to the next available index in the auxiliary 
 * array.
 * 
 * @param m The MAGIC structure.
 * @param dest The address for which we are looking for the index.
 * 
 * @return Index in the auxiliary array.
 */
int MAGICindex(MAGIC m, char* dest);

/*
 * Resets all the matches between addresses and indexes in the structure
 * m.
 * 
 * @param m The MAGIC structure.
 */
void MAGICreset(MAGIC m);

#endif
