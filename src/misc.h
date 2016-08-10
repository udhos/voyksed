
#ifndef MISC_H
#define MISC_H


#ifndef MAX
#define MAX(a, b) (((a) > (b)) ? (a) : (b))
#endif

#ifndef MIN
#define MIN(a, b) (((a) < (b)) ? (a) : (b))
#endif

#define DATA_DIR "../data"


typedef struct sprite_t     sprite_t;
typedef struct ship_t       ship_t;
typedef struct projectile_t projectile_t;
typedef struct frame_t      frame_t;


#endif /* MISC_H */

