#ifndef WATCHDOG_H
#define WATCHDOG_H

extern void WatchdogInit(void);
extern void WatchdogEnable(void);
extern void WatchdogDisable(void);
extern void WatchdogKick(void);
extern void WatchdogTask(void);
extern int WatchdogCmdInit(void);
extern void WatchdogShow(void);
extern void WatchdogReset(void);

#endif
