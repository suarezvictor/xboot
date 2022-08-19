
#ifdef __SANDBOX__
#include <sys/types.h>
#include <sys/stat.h>
#include <alloca.h>
#include <signal.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#undef printf
#undef xboot_exit

#define USE_AF_INET 0
typedef unsigned uint32_t; //FIXME: why??
#else
#include "errno.h"
#include "malloc.h"
#include "string.h"
#include "alloca.h"
#include "stddef.h"
#define EOF -1

#define HAVE_SIGNAL 0
#define USE_AF_INET 0
#endif //__SANDBOX__

#define SCREEN_WIDTH		800
#define SCREEN_HEIGHT		480
//#define MWPIXEL_FORMAT		MWPF_TRUECOLORARGB

#if !USE_AF_INET
#define NONETWORK 1
#endif 

int machine_logger(const char * fmt, ...);
#define printf machine_logger


#if USE_AF_INET

ssize_t read(int fd, void *buf, size_t count);
ssize_t write(int fd, const void *buf, size_t count);
int close(int fd);

int protocol_read(int fd, void *buf, unsigned count)
{
    //printf("protocol_read %d, count %d\n", fd, count);
	return read(fd, buf, count);
}
int protocol_write(int fd, const void *buf, unsigned count)
{
    //printf("protocol_write %d, count %d\n", fd, count);
	return write(fd, buf, count);
}
int protocol_close(int fd)
{
  //printf("protocol_close %d\n", fd);
  return close(fd);
}
#else
int protocol_open(void);
int protocol_read(int fd, void *buf, unsigned count);
int protocol_write(int fd, const void *buf, unsigned count);
int protocol_close(int fd);
int protocol_available(int fd);
#endif

#define read protocol_read
#define write protocol_write
#define close protocol_close

//int GdError(const char *format, ...) { }

#define MWINCLUDECOLORS
#define MW_FEATURE_TIMERS 1
#define HAVE_MMAP 0
#define NANOWM 0
//#define POLYREGIONS 0

#include "mwconfig.h"

#include "../../../ucGPU/demos/third-party/microwindows/src/nanox/srvnet.c"
#include "../../../ucGPU/demos/third-party/microwindows/src/nanox/serv.h"
#include "../../../ucGPU/demos/third-party/microwindows/src/nanox/srvfunc.c"
#include "../../../ucGPU/demos/third-party/microwindows/src/nanox/srvevent.c"
#include "../../../ucGPU/demos/third-party/microwindows/src/nanox/srvclip2.c"
#include "nano-X.h"
#include "mwtypes.h"
#include "../../../ucGPU/demos/third-party/microwindows/src/nanox/nxdraw.c"
#include "../../../ucGPU/demos/third-party/microwindows/src/nanox/wmevents.c"
#include "../../../ucGPU/demos/third-party/microwindows/src/nanox/wmutil.c"
#undef X
#include "../../../ucGPU/demos/third-party/microwindows/src/nanox/wmaction.c"
#include "../../../ucGPU/demos/third-party/microwindows/src/engine/devmouse.c"
#include "../../../ucGPU/demos/third-party/microwindows/src/engine/devkbd.c"
#include "../../../ucGPU/demos/third-party/microwindows/src/engine/devopen.c"
#include "../../../ucGPU/demos/third-party/microwindows/src/engine/devclip2.c"
#include "../../../ucGPU/demos/third-party/microwindows/src/engine/devrgn2.c"
#include "../../../ucGPU/demos/third-party/microwindows/src/engine/devstipple.c"
#include "../../../ucGPU/demos/third-party/microwindows/src/engine/devblit.c"
#include "../../../ucGPU/demos/third-party/microwindows/src/engine/devrgn.c"
#include "../../../ucGPU/demos/third-party/microwindows/src/engine/devdraw.c"
#include "../../../ucGPU/demos/third-party/microwindows/src/engine/devfont.c"
#include "../../../ucGPU/demos/third-party/microwindows/src/engine/font_dbcs.c"
#include "../../../ucGPU/demos/third-party/microwindows/src/engine/devarc.c"
#include "../../../ucGPU/demos/third-party/microwindows/src/engine/devpoly.c"
#include "../../../ucGPU/demos/third-party/microwindows/src/engine/devtimer.c"
#include "../../../ucGPU/demos/third-party/microwindows/src/engine/devimage.c"
#include "../../../ucGPU/demos/third-party/microwindows/src/engine/convblit_mask.c"
#include "../../../ucGPU/demos/third-party/microwindows/src/engine/devpal8.c"
#include "../../../ucGPU/demos/third-party/microwindows/src/engine/devpal4.c"
#include "../../../ucGPU/demos/third-party/microwindows/src/engine/devpal2.c"
#include "../../../ucGPU/demos/third-party/microwindows/src/engine/devpal1.c"
#include "../../../ucGPU/demos/third-party/microwindows/src/engine/convblit_8888.c"
#include "../../../ucGPU/demos/third-party/microwindows/src/engine/convblit_frameb.c"
#include "../../../ucGPU/demos/third-party/microwindows/src/engine/devimage_stretch.c"
#include "../../../ucGPU/demos/third-party/microwindows/src/drivers/fb.c"
#include "../../../ucGPU/demos/third-party/microwindows/src/drivers/copyframebuffer.c"
#include "../../../ucGPU/demos/third-party/microwindows/src/drivers/fblin1.c"
#undef X
#undef notmask
#define notmask notmask2
#include "../../../ucGPU/demos/third-party/microwindows/src/drivers/fblin2.c"
#undef X
#undef notmask
#define notmask notmask4
#include "../../../ucGPU/demos/third-party/microwindows/src/drivers/fblin4.c"
#include "../../../ucGPU/demos/third-party/microwindows/src/drivers/fblin8.c"
#include "../../../ucGPU/demos/third-party/microwindows/src/drivers/fblin16.c"
#include "../../../ucGPU/demos/third-party/microwindows/src/drivers/fblin24.c"
#include "../../../ucGPU/demos/third-party/microwindows/src/drivers/fblin32.c"
#include "../../../ucGPU/demos/third-party/microwindows/src/drivers/fbportrait_down.c"
#include "../../../ucGPU/demos/third-party/microwindows/src/drivers/fbportrait_left.c"
#include "../../../ucGPU/demos/third-party/microwindows/src/drivers/fbportrait_right.c"
#include "../../../ucGPU/demos/third-party/microwindows/src/drivers/rasterops.c"
#include "../../../ucGPU/demos/third-party/microwindows/src/drivers/genmem.c"
#include "../../../ucGPU/demos/third-party/microwindows/src/drivers/genfont.c"
#include "../../../ucGPU/demos/third-party/microwindows/src/drivers/osdep.c"
#include "../../../ucGPU/demos/third-party/microwindows/src/drivers/mou_null.c"
#include "../../../ucGPU/demos/third-party/microwindows/src/drivers/kbd_null.c"
#undef OLDWAY
#include "../../../ucGPU/demos/third-party/microwindows/src/nanox/srvutil.c" //defines OLDWAY
#include "../../../ucGPU/demos/third-party/microwindows/src/fonts/compiled/winFreeSansSerif11x13.c"
#include "../../../ucGPU/demos/third-party/microwindows/src/fonts/compiled/X6x13.c"

int current_fd = -1, un_sock = -1;
//static int	persistent_mode = FALSE;
static int	portraitmode = MWPORTRAIT_NONE;


GR_WINDOW_ID	cachewindowid;		/* cached window id */
GR_WINDOW_ID    cachepixmapid;         /* cached pixmap id */
GR_GC_ID	cachegcid;		/* cached graphics context id */
GR_WINDOW	*cachewp;		/* cached window pointer */
GR_GC		*cachegcp;		/* cached graphics context */
GR_PIXMAP       *cachepp;               /* cached pixmap */
GR_PIXMAP       *listpp;                /* List of all pixmaps */
GR_WINDOW	*listwp;		/* list of all windows */
GR_WINDOW	*rootwp;		/* root window pointer */
GR_GC		*listgcp;		/* list of all gc */
GR_REGION	*listregionp;		/* list of all regions */
GR_FONT		*listfontp;		/* list of all fonts */
GR_CURSOR	*listcursorp;		/* list of all cursors */
GR_CURSOR	*stdcursor;		/* root window cursor */
GR_GC		*curgcp;		/* currently enabled gc */
GR_WINDOW	*clipwp;		/* window clipping is set for */
GR_WINDOW	*focuswp;		/* focus window for keyboard */
GR_WINDOW	*mousewp;		/* window mouse is currently in */
GR_WINDOW	*grabbuttonwp;		/* window grabbed by button */
GR_CURSOR	*curcursor;		/* currently enabled cursor */
GR_COORD	cursorx;		/* current x position of cursor */
GR_COORD	cursory;		/* current y position of cursor */
GR_BUTTON	curbuttons;		/* current state of buttons */
GR_CLIENT	*curclient;		/* client currently executing for */
GR_EVENT_LIST	*eventfree;		/* list of free events */
GR_BOOL		focusfixed;		/* TRUE if focus is fixed on a window */
PMWFONT		stdfont;		/* default font*/
char		*progname;		/* Name of this program.. */

int		current_fd;		/* the fd of the client talking to */
int		connectcount = 0;	/* number of connections to server */
GR_CLIENT	*root_client;		/* root entry of the client table */
GR_CLIENT	*current_client;	/* the client we are currently talking*/
char		*current_shm_cmds;
int		current_shm_cmds_size;
static int	keyb_fd;		/* the keyboard file descriptor */
static int	mouse_fd;		/* the mouse file descriptor */
char		*curfunc;		/* the name of the current server func*/
GR_BOOL		screensaver_active;	/* time before screensaver activates */
GR_SELECTIONOWNER selection_owner;	/* the selection owner and typelist */
int		autoportrait = FALSE;	/* auto portrait mode switching*/
MWCOORD		nxres;			/* requested server x resolution*/
MWCOORD		nyres;			/* requested server y resolution*/
GR_GRABBED_KEY  *list_grabbed_keys = NULL;     /* list of all grabbed keys */

#if MW_FEATURE_TIMERS
GR_TIMEOUT	screensaver_delay;	/* time before screensaver activates */
GR_TIMER_ID     cache_timer_id;         /* cached timer ID */
GR_TIMER        *cache_timer;           /* cached timer */
GR_TIMER        *list_timer;            /* list of all timers */
#endif
#if MW_FEATURE_TWO_KEYBOARDS
static int	keyb2_fd;		/* the keyboard file descriptor */
#endif
/*
 * Open a connection from a new client to the server.
 * Returns -1 on failure.
 */
int
GrOpen(void)
{
	printf("In GrOpen()\n");
	GdPlatformInit();			/* platform-specific initialization*/
	SERVER_LOCK();

	/* Client calls this routine once.  We init everything here*/
	if (connectcount <= 0) {
		if(GsInitialize() < 0) {
			SERVER_UNLOCK();
			return -1;
		}
		curclient = root_client;
	}
	SERVER_UNLOCK();
	wm_init();	/* init built-in window manager*/
    return 1;
}

/*
 * Close the current connection to the server.
 */
void
GrClose(void)
{
	SERVER_LOCK();
	GsClose(current_fd);
	SERVER_UNLOCK();
}

/*
 * Drop a specific server connection.
 */
void
GsClose(int fd)
{
/*
	GsDropClient(fd);
	if(!persistent_mode && connectcount == 0) */
		GsTerminate();
}

  
/*
 * Initialize the graphics and mouse devices at startup.
 * Returns nonzero with a message printed if the initialization failed.
 */
int
GsInitialize(void)
{
	GR_WINDOW	*wp;		/* root window */
	PSD		psd;
	GR_CURSOR_ID	cid;
	static const MWIMAGEBITS cursorbits[16] = {
	      0xe000, 0x9800, 0x8600, 0x4180,
	      0x4060, 0x2018, 0x2004, 0x107c,
	      0x1020, 0x0910, 0x0988, 0x0544,
	      0x0522, 0x0211, 0x000a, 0x0004
	};
	static const MWIMAGEBITS cursormask[16] = {
	      0xe000, 0xf800, 0xfe00, 0x7f80,
	      0x7fe0, 0x3ff8, 0x3ffc, 0x1ffc,
	      0x1fe0, 0x0ff0, 0x0ff8, 0x077c,
	      0x073e, 0x021f, 0x000e, 0x0004
	};

	/* If needed, initialize the server mutex. */
	SERVER_LOCK_INIT();

	wp = (GR_WINDOW *) malloc(sizeof(GR_WINDOW));
	if (wp == NULL) {
		EPRINTF("Cannot allocate root window\n");
		return -1;
	}

#if HAVE_SIGNAL
	/* catch terminate signal to restore tty state*/
	signal(SIGTERM, (void *)GsTerminate);
#endif

#if MW_FEATURE_TIMERS
	screensaver_delay = 0;
#endif
	screensaver_active = GR_FALSE;

	selection_owner.wid = 0;
	selection_owner.typelist = NULL;

#if !NONETWORK
#if HAVE_SIGNAL
	/* ignore pipe signal, sent when clients exit*/
	signal(SIGPIPE, SIG_IGN);
	signal(SIGHUP, SIG_IGN);
#endif

	if (GsOpenSocket() < 0) {
		EPRINTF("Cannot bind to named socket\n");
		free(wp);
		return -1;
	}
#endif

	if ((keyb_fd = GdOpenKeyboard()) == -1) {
		EPRINTF("Cannot initialise keyboard\n");
		/*GsCloseSocket();*/
		free(wp);
		return -1;
	}

#if MW_FEATURE_TWO_KEYBOARDS
	if ((keyb2_fd = GdOpenKeyboard2()) == -1) {
		EPRINTF("Cannot initialise second keyboard\n");
		/*GsCloseSocket();*/
		free(wp);
		return -1;
	}
#endif

	if ((psd = GdOpenScreen()) == NULL) {
		EPRINTF("Cannot initialise screen\n");
		/*GsCloseSocket();*/
		GdCloseKeyboard();
		free(wp);
		return -1;
	}
	GdSetPortraitMode(psd, portraitmode);

	if ((mouse_fd = GdOpenMouse()) == -1) {
		EPRINTF("Cannot initialise mouse\n");
		/*GsCloseSocket();*/
		GdCloseScreen(psd);
		GdCloseKeyboard();
		free(wp);
		return -1;
	}

#if HAVE_VNCSERVER
        if (!GdOpenVNC(psd, Argc, Argv)) {
                EPRINTF("Cannot open VNC Socket\n");
                GdCloseMouse();
                GdCloseScreen(psd);
                GdCloseKeyboard();
                free(wp);
                return -1;
        }
#endif        
	/*
	 * Create std font.
	 */
#if (HAVE_BIG5_SUPPORT | HAVE_GB2312_SUPPORT | HAVE_JISX0213_SUPPORT | HAVE_KSC5601_SUPPORT)
	/* system fixed font looks better when mixed with builtin fixed fonts*/
	stdfont = GdCreateFont(psd, MWFONT_SYSTEM_FIXED, 0, 0, NULL);
#else
	stdfont = GdCreateFont(psd, MWFONT_SYSTEM_VAR, 0, 0, NULL);
#endif

	/*
	 * Initialize the root window.
	 */
	wp->x = 0;
	wp->y = 0;
	wp->width = psd->xvirtres;
	wp->height = psd->yvirtres;
	wp->psd = psd;
	wp->id = GR_ROOT_WINDOW_ID;
	wp->next = NULL;
	wp->owner = NULL;
	wp->parent = NULL;
	wp->children = NULL;
	wp->siblings = NULL;
	wp->bordersize = 0;
	wp->background = BLACK;
	wp->bordercolor = wp->background;
	wp->bgpixmap = NULL;
	wp->bgpixmapflags = GR_BACKGROUND_TILE;
	wp->nopropmask = 0;
	wp->eventclients = NULL;
	wp->cursorid = 0;
	wp->mapped = GR_TRUE;
	wp->realized = GR_TRUE;
	wp->output = GR_TRUE;
	wp->props = 0;
	wp->title = NULL;
	wp->clipregion = NULL;
	wp->buffer = NULL;

	listpp = NULL;
	listwp = wp;
	rootwp = wp;
	focuswp = wp;
	mousewp = wp;
	focusfixed = GR_FALSE;

	/*
	 * Initialize and position the default cursor.
	 */
	curcursor = NULL;
	cursorx = -1;
	cursory = -1;
	GdShowCursor(psd);
	GrMoveCursor(psd->xvirtres / 2, psd->yvirtres / 2);
	cid = GrNewCursor(16, 16, 0, 0, WHITE, BLACK, (MWIMAGEBITS *)cursorbits,
				(MWIMAGEBITS *)cursormask);
	GrSetWindowCursor(GR_ROOT_WINDOW_ID, cid);
	stdcursor = GsFindCursor(cid);

#if VTSWITCH
	MwInitVt();
	/* Check for VT change every 50 ms: */
	GdAddTimer(50, CheckVtChange, NULL);
#endif
	psd->FillRect(psd, 0, 0, psd->xvirtres-1, psd->yvirtres-1,
		GdFindColor(psd, wp->background));

	/*
	 * Tell the mouse driver some things.
	 */
	curbuttons = 0;
	GdRestrictMouse(0, 0, psd->xvirtres - 1, psd->yvirtres - 1);
	GdMoveMouse(psd->xvirtres / 2, psd->yvirtres / 2);

	/* Force root window screen paint*/
	GsRedrawScreen();

	/* 
	 * Force the cursor to appear on the screen at startup.
	 * (not required with above GsRedrawScreen)
	GdHideCursor(psd);
	GdShowCursor(psd);
	 */

	/*
	 * All done.
	 */
	connectcount = 0;
	return 0;
}

void
GsTerminate(void)
{
	GdCloseScreen(rootwp->psd);
	GdCloseMouse();
	GdCloseKeyboard();
	exit(0);
}


void
GsAcceptClientFd(int i)
{
	GR_CLIENT *client, *cl;

	if(!(client = malloc(sizeof(GR_CLIENT)))) {
#if !NONETWORK
		close(i);
#endif
		return;
	}

	client->id = i;
	client->eventhead = NULL;
	client->eventtail = NULL;
	/*client->errorevent.type = GR_EVENT_TYPE_NONE;*/
	client->next = NULL;
	client->prev = NULL;
	client->waiting_for_event = FALSE;
	client->shm_cmds = 0;

	if(connectcount++ == 0)
		root_client = client;
	else {
		cl = root_client;
			while(cl->next)
				cl = cl->next;
		client->prev = cl;
		cl->next = client;
	}
	printf("Accepted connections %d\n", connectcount);
}

void
GsSelect(GR_TIMEOUT timeout)
{
#if 0
	fd_set	rfds;
	int 	e;
	int	setsize = 0;
	struct timeval tout;
	struct timeval *to;
#if NONETWORK
	int	fd;
#endif
#if HAVE_VNCSERVER 
#if VNCSERVER_PTHREADED
        int dummy;
#else
        rfbClientIteratorPtr i;
        rfbClientPtr cl;
#endif 
#endif

	/* X11/SDL perform single update of aggregate screen update region*/
	if (scrdev.PreSelect)
	{
		/* returns # pending events*/
		if (scrdev.PreSelect(&scrdev))
		{
			/* poll for mouse data and service if found*/
			while (GsCheckMouseEvent())
				continue;

			/* poll for keyboard data and service if found*/
			while (GsCheckKeyboardEvent())
				continue;

			/* if events found, don't return unless polling, events handled below*/
			if (timeout != GR_TIMEOUT_BLOCK)
				return;
		}
	}

	/* Set up the FDs for use in the main select(): */
	FD_ZERO(&rfds);
	if(mouse_fd >= 0)
	{
		FD_SET(mouse_fd, &rfds);
		if (mouse_fd > setsize)
			setsize = mouse_fd;
	}
	if(keyb_fd >= 0)
	{
		FD_SET(keyb_fd, &rfds);
		if (keyb_fd > setsize)
			setsize = keyb_fd;
	}
#if MW_FEATURE_TWO_KEYBOARDS
	if(keyb2_fd >= 0)
	{
		FD_SET(keyb2_fd, &rfds);
		if (keyb2_fd > setsize)
			setsize = keyb2_fd;
	}
#endif
#if NONETWORK
	/* handle registered input file descriptors*/
	for (fd = 0; fd < regfdmax; fd++)
	{
		if (!FD_ISSET(fd, &regfdset))
			continue;

		FD_SET(fd, &rfds);
		if (fd > setsize) setsize = fd;
	}
#else /* !NONETWORK */
	/* handle client socket connections*/
	FD_SET(un_sock, &rfds);
	if (un_sock > setsize) setsize = un_sock;
	curclient = root_client;
	while(curclient)
	{
		if(curclient->waiting_for_event && curclient->eventhead)
		{
			curclient->waiting_for_event = FALSE;
			GrGetNextEventWrapperFinish(curclient->id);
			return;
		}
		FD_SET(curclient->id, &rfds);
		if(curclient->id > setsize) setsize = curclient->id;
		curclient = curclient->next;
	}
#endif /* NONETWORK */

#if HAVE_VNCSERVER 
#if VNCSERVER_PTHREADED
	/* Add file vnc thread fd. This is useful to force handling of events generated by the VNC thread*/
	FD_SET( vnc_thread_fd, &(rfds) );
	if ( vnc_thread_fd > setsize )
		setsize = vnc_thread_fd;
#else
        /* Add all VNC open sockets to nano-X select set */
        FD_SET( rfbScreen->listenSock, &(rfds) );
        if ( rfbScreen->listenSock > setsize )
                setsize = rfbScreen->listenSock;

        FD_SET( rfbScreen->httpListenSock, &(rfds) );
        if ( rfbScreen->httpListenSock > setsize )
                setsize = rfbScreen->httpListenSock;

        i = rfbGetClientIterator(rfbScreen);
        cl = rfbClientIteratorNext(i);

        while ( cl ) {
                if ( cl->sock >= 0 ) {
                        FD_SET( cl->sock, &(rfds) );
                        if ( cl->sock > setsize )
                                setsize = cl->sock;

                }
                cl = rfbClientIteratorNext(i);
        }
        rfbReleaseClientIterator(i);
#endif
#endif /* HAVE_VNCSERVER*/


	/* setup timeval struct for block or poll in select()*/
	tout.tv_sec = tout.tv_usec = 0;					/* setup for assumed poll*/
	to = &tout;
	int poll = (timeout == GR_TIMEOUT_POLL);
	if (!poll)
	{
#if MW_FEATURE_TIMERS
		/* get next timer or use passed timeout and convert to timeval struct*/
		if (!GdGetNextTimeout(&tout, timeout))		/* no app timers or VTSWITCH?*/
#else
		if (timeout)								/* setup mwin poll timer*/
		{
			/* convert wait timeout to timeval struct*/
			tout.tv_sec = timeout / 1000;
			tout.tv_usec = (timeout % 1000) * 1000;
		}
		else
#endif
		{
			to = NULL;								/* no timers, block*/
		}
	}

	/* some drivers can't block in select as backend is poll based (SDL)*/
	if (scrdev.flags & PSF_CANTBLOCK)
	{
#define WAITTIME	5000
		/* check if would block permanently or timeout > WAITTIME*/
		if (to == NULL || tout.tv_sec != 0 || tout.tv_usec > WAITTIME)
		{
			/* override timeouts and wait for max WAITTIME ms*/
			to = &tout;
			tout.tv_sec = 0;
			tout.tv_usec = WAITTIME;
		}
	}

	/* Wait for some input on any of the fds in the set or a timeout*/
#if NONETWORK
again:
	SERVER_UNLOCK();	/* allow other threads to run*/
#endif
	e = select(setsize+1, &rfds, NULL, NULL, to);
#if NONETWORK
	SERVER_LOCK();
#endif
	if(e > 0)			/* input ready*/
	{
		/* service mouse file descriptor*/
		if(mouse_fd >= 0 && FD_ISSET(mouse_fd, &rfds))
			while(GsCheckMouseEvent())
				continue;

		/* service keyboard file descriptor*/
		if( (keyb_fd >= 0 && FD_ISSET(keyb_fd, &rfds))
#if MW_FEATURE_TWO_KEYBOARDS
		    || (keyb2_fd >= 0 && FD_ISSET(keyb2_fd, &rfds))
#endif
		  )
			while(GsCheckKeyboardEvent())
				continue;

#if HAVE_VNCSERVER && VNCSERVER_PTHREADED
        if(vnc_thread_fd >= 0 && FD_ISSET(vnc_thread_fd, &rfds))
            /* Read from vnc pipe */
            read( vnc_thread_fd, &dummy, sizeof(int));

#endif
#if NONETWORK
		/* check for input on registered file descriptors */
		for (fd = 0; fd < regfdmax; fd++)
		{
			GR_EVENT_FDINPUT *	gp;

			if (!FD_ISSET(fd, &regfdset)  ||  !FD_ISSET(fd, &rfds))
				continue;

			gp = (GR_EVENT_FDINPUT *)GsAllocEvent(curclient);
			if(gp) {
				gp->type = GR_EVENT_TYPE_FDINPUT;
				gp->fd = fd;
			}
		}
#else /* !NONETWORK */

		/* If a client is trying to connect, accept it: */
		if(FD_ISSET(un_sock, &rfds))
			GsAcceptClient();

		/* If a client is sending us a command, handle it: */
		curclient = root_client;
		while (curclient)
		{
			GR_CLIENT *curclient_next;

			/* curclient may be freed in GsDropClient*/
			curclient_next = curclient->next;
			if(FD_ISSET(curclient->id, &rfds))
				GsHandleClient(curclient->id);
			curclient = curclient_next;
		}

#if HAVE_VNCSERVER && !VNCSERVER_PTHREADED
		rfbProcessEvents(rfbScreen, 0);
#endif
		
#endif /* NONETWORK */
	} 
	else if (e == 0)		/* timeout*/
	{
#if NONETWORK
		/* 
		 * Timeout has occured. Currently return a timeout event
		 * regardless of whether client has selected for it.
		 * Note: this will be changed back to GR_EVENT_TYPE_NONE
		 * for the GrCheckNextEvent/LINK_APP_TO_SERVER case
		 */
#if MW_FEATURE_TIMERS
		if(GdTimeout())
#endif
		{
			GR_EVENT_GENERAL *	gp;
			if ((gp = (GR_EVENT_GENERAL *)GsAllocEvent(curclient)) != NULL)
				gp->type = GR_EVENT_TYPE_TIMEOUT;
		}
		else if(!poll && timeout && (scrdev.flags & PSF_CANTBLOCK)) {
			if (!GsPumpEvents())    /* process mouse/kbd events */
				goto again;		/* retry until passed timeout */
		}
#else /* !NONETWORK */
#if MW_FEATURE_TIMERS
		/* check for timer timeouts and service if found*/
		GdTimeout();
#endif
#endif /* NONETWORK */
	} else if(errno != EINTR)
		EPRINTF("Select() call in main failed\n");
#endif
}

/////////////framebuffer device


static PSD  fbe_open(PSD psd);
static void fbe_close(PSD psd);
static void fbe_setpalette(PSD psd,int first,int count,MWPALENTRY *pal);
static int fbe_preselect(PSD psd);
static void fbe_update(PSD psd, MWCOORD x, MWCOORD y, MWCOORD width, MWCOORD height);

SCREENDEVICE scrdev = {
	0, 0, 0, 0, 0, 0, 0, NULL, 0, NULL, 0, 0, 0, 0, 0, 0,
	gen_fonts,
	fbe_open,
	fbe_close,
	fbe_setpalette,
	gen_getscreeninfo,
	gen_allocatememgc,
	gen_mapmemgc,
	gen_freememgc,
	gen_setportrait,
	fbe_update,
	fbe_preselect
};

/* open framebuffer mmap'd by FBE*/
static PSD
fbe_open(PSD psd)
{
	//char *env;

	int flags = PSF_SCREEN;		/* init psd, don't allocate framebuffer*/

	if (!gen_initpsd(psd, MWPIXEL_FORMAT, SCREEN_WIDTH, SCREEN_HEIGHT, flags))
		return NULL;

	psd->flags |= PSF_DELAYUPDATE;

	/* set if screen driver subsystem requires polling and select()*/
	psd->flags |= PSF_CANTBLOCK;

	/*
	 * Allocate framebuffer
	 * psd->size is calculated by subdriver init
	 */
	if ((psd->addr = malloc(psd->size)) == NULL)
		return NULL;
	psd->flags |= PSF_ADDRMALLOC;


	return psd;	/* success*/
}

/* close framebuffer*/
static void
fbe_close(PSD psd)
{
}

/* setup palette*/
static void
fbe_setpalette(PSD psd,int first,int count,MWPALENTRY *pal)
{
}

/*
 * The following routines are not required for FBE framebuffer, but
 * may be useful when developing a new screen driver.
 * SAMPLE UNWORKING CODE, requires dstpixels and dstpitch initialization below.
 */

/* bounding rectangle for aggregrate screen update*/
static MWCOORD upminX, upminY, upmaxX, upmaxY;


static uint32_t *dstpixels = NULL; /* set to destination pixels in graphics lib*/
static unsigned int dstpitch = 0;	 /* set to width in bytes of destination pixel row*/

/* update graphics lib from framebuffer*/
static void
fbe_draw(PSD psd, MWCOORD x, MWCOORD y, MWCOORD width, MWCOORD height)
{

	/* assumes destination pixels in same format * as MWPIXEL_FORMAT set in config!*/
	if (dstpixels)
	{
		printf("###### updating pixels %d,%d,%d,%d\n", upminX, upminY, upmaxX-upminX+1, upmaxY-upminY+1);
		copy_framebuffer(psd, upminX, upminY, upmaxX-upminX+1, upmaxY-upminY+1, dstpixels, dstpitch);
	}
}

/* called before select(), returns # pending events*/
static int
fbe_preselect(PSD psd)
{
	/* perform single blit update of aggregate update region*/
	if ((psd->flags & PSF_DELAYUPDATE) && (upmaxX >= 0 || upmaxY >= 0)) {
		fbe_draw(psd, upminX, upminY, upmaxX-upminX+1, upmaxY-upminY+1);

		/* reset update region*/
		upminX = upminY = MAX_MWCOORD;
		upmaxX = upmaxY = MIN_MWCOORD;
	}

	/* return nonzero if subsystem events available and driver uses PSF_CANTBLOCK*/
	return 0;
}

/* called from framebuffer drivers with bounding rect of updated framebuffer region*/
static void
fbe_update(PSD psd, MWCOORD x, MWCOORD y, MWCOORD width, MWCOORD height)
{
	/* window moves require delaying updates until preselect for speed*/
	if ((psd->flags & PSF_DELAYUPDATE)) {
			/* calc aggregate update rectangle*/
			upminX = MWMIN(x, upminX);
			upminY = MWMIN(y, upminY);
			upmaxX = MWMAX(upmaxX, x+width-1);
			upmaxY = MWMAX(upmaxY, y+height-1);
	} else
		fbe_draw(psd, x, y, width, height);
}



void render_test(uint32_t *pixels, unsigned w, unsigned h, unsigned stride)
{
	dstpixels = pixels;
	dstpitch = stride;
#if 0
	{
		static uint8_t c = 0;
		++c;
		memset(pixels, c, stride*h/2);
		printf("render_test %d\r\n", c);
	}
#else
	void cg_test(uint32_t *pixels, unsigned w, unsigned h, unsigned stride);
	cg_test(pixels, w, h, stride);

#if !NONETWORK
	if(un_sock == -1)
	{
		if(GsInitialize() < 0)
		{
		    printf("GsInitialize failed\n");
			exit(1);
		}
	    printf("GsInitialize called, listen port %d\n", GR_NUM_SOCKET);
	}	

	//GsSelect(GR_TIMEOUT_BLOCK); //this replaces the following

	if (scrdev.PreSelect)
	{
		/* returns # pending events*/
		if (scrdev.PreSelect(&scrdev))
		{
			/* poll for mouse data and service if found*/
			while (GsCheckMouseEvent())
				continue;

			/* poll for keyboard data and service if found*/
			while (GsCheckKeyboardEvent())
				continue;
		}
	}
	
	{
		fd_set	rfds;
		struct timeval tout = { .tv_sec = 0, .tv_usec = 1000000/1000 };
		FD_ZERO(&rfds);
		FD_SET(un_sock, &rfds);
		select(FD_SETSIZE, &rfds, NULL, NULL, &tout); //returns 1 once connected
		/* If a client is trying to connect, accept it: */
		if(FD_ISSET(un_sock, &rfds))
		{
		    printf("incoming connection\n");
			GsAcceptClient();
		}
		
		FD_ZERO(&rfds);
		/* If a client is sending us a command, handle it: */
		curclient = root_client;
		while (curclient)
		{
			if(curclient->waiting_for_event && curclient->eventhead)
			{
				curclient->waiting_for_event = FALSE;
				GrGetNextEventWrapperFinish(curclient->id);
				return;
			}

			GR_CLIENT *curclient_next;

			/* curclient may be freed in GsDropClient*/
			curclient_next = curclient->next;
	    	//printf("trying to handle client %d\n", curclient->id);
		FD_SET(curclient->id, &rfds);
		select(FD_SETSIZE, &rfds, NULL, NULL, &tout);
			if(FD_ISSET(curclient->id, &rfds))
			{
	    		//printf("handling client %d\n", curclient->id);
				GsHandleClient(curclient->id);
			}
			curclient = curclient_next;
		}
	}
#else
	if(un_sock == -1)
	{
		un_sock = protocol_open();
		if(GsInitialize() < 0)
		{
		    printf("GsInitialize failed\n");
			exit(1);
		}
		GsAcceptClientFd(un_sock);
	}


	if (scrdev.PreSelect)
	{
		/* returns # pending events*/
		if (scrdev.PreSelect(&scrdev))
		{
			/* poll for mouse data and service if found*/
			while (GsCheckMouseEvent())
				continue;

			/* poll for keyboard data and service if found*/
			while (GsCheckKeyboardEvent())
				continue;
		}
	}
	
		curclient = root_client;
		while (curclient)
		{
			GR_CLIENT *curclient_next;
			int avail;
			if(curclient->waiting_for_event && curclient->eventhead)
			{
				curclient->waiting_for_event = FALSE;
				GrGetNextEventWrapperFinish(curclient->id);
				return;
			}
			avail = protocol_available(curclient->id);

			/* curclient may be freed in GsDropClient*/
			curclient_next = curclient->next;
			if(avail>0)
			{
				//printf("available bytes %d\n", avail);
				GsHandleClient(curclient->id);
				//printf("request handled\n");
			}
			curclient = curclient_next;
		}
#endif

    dstpixels = NULL;
#endif
}


#if !USE_AF_INET

#include <xboot/device.h>
//#include <stdio.h>

struct uart_t * uart_dev = NULL;
int protocol_open(void)
{
	struct device_t * pos, * n;
	list_for_each_entry_safe(pos, n, &__device_head[DEVICE_TYPE_UART], head)
	{
		struct uart_t *uart = (struct uart_t *)(pos->priv);
		if(!uart)
			continue;
		printf("UART found: %s\r\n", pos->name);
		uart_dev = uart;
		
	}
	if(!uart_dev)
	  return -1;
	return 12345;
}

//uart.h
ssize_t uart_read(struct uart_t * uart, u8_t * buf, size_t count);
ssize_t uart_write(struct uart_t * uart, const u8_t * buf, size_t count);

int protocol_read(int fd, void *buf, unsigned count)
{
	int n;
	//printf("protocol_read: fd=%d, count=%d\n", fd, count);
	do
	n = uart_read(uart_dev, (u8_t*) buf, count);
#ifndef __SANDBOX__
	while(n==0);
#else
	while(n<0);
#endif
	//printf("protocol_read (actual): %d bytes\n", n);
	/*
	for(const u8_t *p = buf; p < (u8_t*)buf+n; ++p)
	  printf("0x%02x ", *p);
    printf("\r\n");*/
	return n;
}

int protocol_write(int fd, const void *buf, unsigned count)
{
	//printf("protocol_write: fd=%d, count = %d\r\n", fd, count);
	return uart_write(uart_dev, (u8_t*) buf, count);
}

int protocol_close(int fd)
{
  return 0/*uart_close(uart_dev)*/;
}

int protocol_available(int fd)
{
  int uart_available(struct uart_t * uart);
  return uart_dev != NULL && uart_available(uart_dev);
}
#endif


