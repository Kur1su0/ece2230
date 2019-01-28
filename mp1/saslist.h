/* saslist.h 
 * Zice Wei
 * zicew
 * ECE 2230 Fall 2018 
 * MP1
 *
 * Purpose: The interface definition for MP1.  DO NOT CHANGE THIS FILE
 *
 * Assumptions: structure definitions and public functions as defined for
 * assignmnet.  There are no extensions or modifications permitted.
 *
 * Bugs: there are no bugs.
 */

struct saslist_t {
    int sas_list_size;  // size of array given by user
    int sec_usrs_count; // current number of secondary user records in SAS list
    struct secusr_info_t **sec_usrs_ptr;
};

struct secusr_info_t {
    int su_id;	        // secondary user's ID number 
    int ip_address;	// mobile's IP address 
    int access_point;	// IP address of access point connected to mobile 
    int authenticated;	// true or false 
    int privacy;	// mode 0 for none, 1 for encrypted
    float band;		// 3.5 for the CBRS frequency bands (in GHz) 
    int channel;	// 1-10 
    float data_rate;	// in bits per second 
    int time_received;	// time in seconds that information last updated
};

//  The sequential list ADT must have the following interface:

struct saslist_t *saslist_construct(int);
void saslist_destruct(struct saslist_t *);
int saslist_add(struct saslist_t *, struct secusr_info_t *);
int saslist_lookup(struct saslist_t *, int);
struct secusr_info_t *saslist_access(struct saslist_t *, int);
struct secusr_info_t *saslist_remove(struct saslist_t *, int);
int saslist_arraysize(struct saslist_t *);
int saslist_number_entries(struct saslist_t *);

