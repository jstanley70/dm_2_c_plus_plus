//ects:D.M. v2.0:Sources.Jim:Event/Memory:Event Headers:Watch.h#ifndef	_STOPWATCH		#define		STOP_WATCH		0	#define		START_WATCH		1	#define		TRIG_WATCH		2		void	WatchInstall(void);	void	WatchRemove(void);	void	StartWatch(void);	void	StopWatch(void);	void	Watch(void);	void	CheckWatch(void);	void	KickWatch(void);	void	AllowBackground(void);	#endif