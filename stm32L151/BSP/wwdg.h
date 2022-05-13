#ifndef __WWDG_H
#define __WWDG_H

// 不用WWDG，自动启用IWDG
#define DEBUG_NO_WWDG

#ifndef NDEBUG
#ifndef DEBUG_NO_WWDG
#define DEBUG_NO_WWDG
#endif
#endif

#ifdef DEBUG_NO_WWDG
#define DEBUG_IWDG
#endif

void wwdg_init(void);
void feed_wwdg(void);
void sys_restart(void);

void iwdg_init(void);
void feed_iwdg(void);

#endif /*__WWDG_H*/
