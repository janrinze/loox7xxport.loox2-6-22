/*
 *  Functions private to power supply class
 *
 *  Copyright (c) 2007  Anton Vorontsov <cbou@mail.ru>
 *  Copyright (c) 2004  Szabolcs Gyurko
 *  Copyright (c) 2003  Ian Molton <spyro@f2s.com>
 *
 *  Modified: 2004, Oct     Szabolcs Gyurko
 *
 *  You may use this code as per GPL version 2
 */

#ifdef CONFIG_SYSFS

extern int power_supply_create_attrs(struct power_supply *psy);
extern void power_supply_remove_attrs(struct power_supply *psy);
extern int power_supply_uevent(struct device *dev, char **envp, int num_envp,
                               char *buffer, int buffer_size);

#else

static inline int power_supply_create_attrs(struct power_supply *psy)
{ return 0; }
static inline void power_supply_remove_attrs(struct power_supply *psy) {}
#define power_supply_uevent NULL

#endif /* CONFIG_SYSFS */

#ifdef CONFIG_LEDS_TRIGGERS

extern void power_supply_update_leds(struct power_supply *psy);
extern int power_supply_create_triggers(struct power_supply *psy);
extern void power_supply_remove_triggers(struct power_supply *psy);

#else

static inline void power_supply_update_leds(struct power_supply *psy) {}
static inline int power_supply_create_triggers(struct power_supply *psy)
{ return 0; }
static inline void power_supply_remove_triggers(struct power_supply *psy) {}

#endif /* CONFIG_LEDS_TRIGGERS */
