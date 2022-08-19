#include <cg.h>

int machine_logger(const char * fmt, ...);
#define printf machine_logger

struct cg_ctx_t * ctx = NULL;

void cg_test_rectangle(void)
{
	double x0 = 25.6,
		y0 = 25.6,
		rect_width = 204.8,
		rect_height = 204.8,
		radius = 102.4;
	double x1, y1;

	x1 = x0 + rect_width;
	y1 = y0 + rect_height;
	if(!rect_width || !rect_height)
		return;
	if(rect_width / 2 < radius)
	{
		if(rect_height / 2 < radius)
		{
			cg_move_to(ctx, x0, (y0 + y1) / 2);
			cg_curve_to(ctx, x0, y0, x0, y0, (x0 + x1) / 2, y0);
			cg_curve_to(ctx, x1, y0, x1, y0, x1, (y0 + y1) / 2);
			cg_curve_to(ctx, x1, y1, x1, y1, (x1 + x0) / 2, y1);
			cg_curve_to(ctx, x0, y1, x0, y1, x0, (y0 + y1) / 2);
		}
		else
		{
			cg_move_to(ctx, x0, y0 + radius);
			cg_curve_to(ctx, x0, y0, x0, y0, (x0 + x1) / 2, y0);
			cg_curve_to(ctx, x1, y0, x1, y0, x1, y0 + radius);
			cg_line_to(ctx, x1, y1 - radius);
			cg_curve_to(ctx, x1, y1, x1, y1, (x1 + x0) / 2, y1);
			cg_curve_to(ctx, x0, y1, x0, y1, x0, y1 - radius);
		}
	}
	else
	{
		if(rect_height / 2 < radius)
		{
			cg_move_to(ctx, x0, (y0 + y1) / 2);
			cg_curve_to(ctx, x0, y0, x0, y0, x0 + radius, y0);
			cg_line_to(ctx, x1 - radius, y0);
			cg_curve_to(ctx, x1, y0, x1, y0, x1, (y0 + y1) / 2);
			cg_curve_to(ctx, x1, y1, x1, y1, x1 - radius, y1);
			cg_line_to(ctx, x0 + radius, y1);
			cg_curve_to(ctx, x0, y1, x0, y1, x0, (y0 + y1) / 2);
		}
		else
		{
			cg_move_to(ctx, x0, y0 + radius);
			cg_curve_to(ctx, x0, y0, x0, y0, x0 + radius, y0);
			cg_line_to(ctx, x1 - radius, y0);
			cg_curve_to(ctx, x1, y0, x1, y0, x1, y0 + radius);
			cg_line_to(ctx, x1, y1 - radius);
			cg_curve_to(ctx, x1, y1, x1, y1, x1 - radius, y1);
			cg_line_to(ctx, x0 + radius, y1);
			cg_curve_to(ctx, x0, y1, x0, y1, x0, y1 - radius);
		}
	}
	cg_close_path(ctx);

	cg_set_source_rgb(ctx, 0.5, 0.5, 1);
	cg_fill_preserve(ctx);
	cg_set_source_rgba(ctx, 0.5, 0, 0, 0.5);
	cg_set_line_width(ctx, 10.0);
	cg_stroke(ctx);
}


void cg_test_arc(void)
{
	double xc = 128.0;
	double yc = 128.0;
	double radius = 100.0;
	static double angle1 = 45.0 * (M_PI / 180.0);
	angle1 += .1;
	if(angle1 > 2*M_PI) angle1 -= 2*M_PI;
	double angle2 = 180.0 * (M_PI / 180.0);

	cg_set_source_rgba(ctx, 1, 1, 0.5, 1);
	cg_set_line_width(ctx, 10.0);
	cg_arc(ctx, xc, yc, radius, angle1, angle2);
	cg_stroke(ctx);

	cg_set_source_rgba(ctx, 1, 0.2, 0.2, 0.6);
	cg_set_line_width(ctx, 6.0);

	cg_arc(ctx, xc, yc, 10.0, 0, 2 * M_PI);
	cg_fill(ctx);

	cg_arc(ctx, xc, yc, radius, angle1, angle1);
	cg_line_to(ctx, xc, yc);
	cg_arc(ctx, xc, yc, radius, angle2, angle2);
	cg_line_to(ctx, xc, yc);
	cg_stroke(ctx);
}


#include <graphic/text.h>
#include <graphic/surface.h>
extern struct surface_t *current_surface;
struct text_t txt;

void do_text(int x, int y, const char *utf8, unsigned count)
{
	struct matrix_t m;
	matrix_init(&m, ctx->state->matrix.a, ctx->state->matrix.b, ctx->state->matrix.c, ctx->state->matrix.d, x+ctx->state->matrix.tx, y+ctx->state->matrix.ty);
	text_set_text(&txt, utf8);
	surface_text(current_surface, NULL, &m, &txt);
}

void cg_test(uint32_t *pixels, unsigned w, unsigned h, unsigned stride)
{
	if(ctx == NULL)
	{
		struct cg_surface_t * surface = cg_surface_create_for_data(w, h, pixels);
		surface->stride = stride;
		ctx = cg_create(surface);
		cg_matrix_translate(&ctx->state->matrix, 800/2, 0);
		
		txt.fctx = font_context_alloc();
		static struct color_t text_color = { .r = 0xFF, .g = 0xFF, .b = 0xFF, .a = 0xFF}; //WARNING: not copied, pointer used
		text_init(&txt, "", &text_color, 0, txt.fctx, NULL, 12); //default family
	}
	
	//cg_test_rectangle();
	//do_text(100, 100, "Hello", 5);
	
	//cg_test_arc();
	
	//cg_destroy(ctx);
	//cg_surface_destroy(surface);
}

#include "nano-X.h"
#include "../nanox/nxproto.h"
#include "../nanox/serv.h"
extern int		current_fd;
static int GsWriteType(int fd, short type)
{
	return GsWrite(fd,&type,sizeof(type));
}

/*
#define GrNumSetGCBackground    34
typedef struct {
	BYTE8	reqType;
	BYTE8	hilength;
	UINT16	length;
	IDTYPE	gcid;
	UINT32	color;
} nxSetGCBackgroundReq;
*/



int alt_draw(int nreq, const void *req)
{
	switch(nreq)
	{
		case GrNumSetGCForeground:
		{
/*
#define GrNumSetGCForeground    33
typedef struct {
	BYTE8	reqType;
	BYTE8	hilength;
	UINT16	length;
	IDTYPE	gcid;
	UINT32	color;
} nxSetGCForegroundReq;
*/
			const nxSetGCForegroundReq *r = (const nxSetGCForegroundReq*) req;
			printf("nxSetGCForegroundReq color 0x%08X\r\n", r->color);
			uint8_t cr = (r->color & 0xFF), cg = ((r->color >> 8) & 0xFF), cb = ((r->color >> 16) & 0xFF), ca = (r->color >>24);
			cg_set_source_rgba(ctx, cr/255., cg/255., cb/255., ca/255.);
			if(txt.c)
			{
				//precalculates font color
				txt.c->r = cr;
				txt.c->g = cg;
				txt.c->b = cb;
				txt.c->a = ca;
			}

			return 0;
		}

		case GrNumLine:
		{
/*
#define GrNumLine               25
typedef struct {
	BYTE8	reqType;
	BYTE8	hilength;
	UINT16	length;
	IDTYPE	drawid;
	IDTYPE	gcid;
	INT16	x1;
	INT16	y1;
	INT16	x2;
	INT16	y2;
} nxLineReq;
*/
			const nxLineReq *r = (const nxLineReq*) req;
			printf("nxLineReq x1 %d, y1 %d, x2 %d, y2 %d\r\n", r->x1, r->y1, r->x2, r->y2);
			cg_set_line_width(ctx, 1);
			cg_move_to(ctx, r->x1, r->y1);
			cg_line_to(ctx, r->x2, r->y2);
			cg_stroke(ctx);
			return 0;
		}

		case GrNumRect:
		{
/*
#define GrNumRect               27
typedef struct {
	BYTE8	reqType;
	BYTE8	hilength;
	UINT16	length;
	IDTYPE	drawid;
	IDTYPE	gcid;
	INT16	x;
	INT16	y;
	INT16	width;
	INT16	height;
} nxRectReq;
*/		
			const nxRectReq *r = (const nxRectReq*) req;
			printf("nxRectReq x %d, y %d, w %d, h %d\r\n", r->x, r->y, r->width, r->height);
			cg_rectangle(ctx, r->x, r->y, r->width, r->height);
			cg_stroke(ctx);
			return 0;
		}

		case GrNumFillRect:
		{
/*
#define GrNumFillRect           28
typedef struct {
	BYTE8	reqType;
	BYTE8	hilength;
	UINT16	length;
	IDTYPE	drawid;
	IDTYPE	gcid;
	INT16	x;
	INT16	y;
	INT16	width;
	INT16	height;
} nxFillRectReq;
*/		
			const nxFillRectReq *r = (const nxFillRectReq*) req;
			printf("nxFillRectReq x %d, y %d, w %d, h %d\r\n", r->x, r->y, r->width, r->height);
			cg_rectangle(ctx, r->x, r->y, r->width, r->height);
			cg_fill(ctx);
			return 0;
		}

		case GrNumArcAngle:
		{
/*
#define GrNumArcAngle		77
typedef struct {
	BYTE8	reqType;
	BYTE8	hilength;
	UINT16	length;
	IDTYPE	drawid;
	IDTYPE	gcid;
	INT16	x;
	INT16	y;
	INT16	rx;
	INT16	ry;
	INT16	angle1;
	INT16	angle2;
	INT16	type;
} nxArcAngleReq;		*/
			const nxArcAngleReq *r = (const nxArcAngleReq*) req;
			printf("nxArcAngleReq x %d, y %d, rx %d, ry %d, angle1 %d, angle2 %d, type %s\r\n", r->x, r->y, r->rx, r->ry, r->angle1, r->angle2, r->type == GR_PIE ? "GR_PIE" : "GR_ARC");
			cg_set_line_width(ctx, 1);
			INT16 angle1 = r->angle1, angle2 = r->angle2;
			if(angle1!=angle2)
				cg_arc_negative(ctx, r->x, r->y, r->rx, angle1*(M_PI/(-180*64)), angle2*(M_PI/(-180*64)));
			else
				cg_ellipse(ctx, r->x, r->y, r->rx, r->ry);
			if(r->type == GR_ARC)
				cg_stroke(ctx);
			else
				cg_fill(ctx);
			
			return 0;
		}
		
		case GrNumArc:
		{
/*
#define GrNumArc		76
typedef struct {
	BYTE8	reqType;
	BYTE8	hilength;
	UINT16	length;
	IDTYPE	drawid;
	IDTYPE	gcid;
	INT16	x;
	INT16	y;
	INT16	rx;
	INT16	ry;
	INT16	ax;
	INT16	ay;
	INT16	bx;
	INT16	by;
	INT16	type;
} nxArcReq;
*/
			const nxArcReq *r = (const nxArcReq*) req;
			printf("nxArcReq x %d, y %d, rx %d, ry %d, ax %d, ay %d, bx %d, by %d, type 0x%08X\r\n", r->x, r->y, r->rx, r->ry, r->ax, r->ay, r->bx, r->by, r->type);
			cg_set_line_width(ctx, 1);
			if(r->type & MWOUTLINE)
			{
				cg_move_to(ctx, r->x, r->y);
				cg_line_to(ctx, r->x+r->ax, r->y+r->ay);
			}
			else
				cg_move_to(ctx, r->x+r->ax, r->y+r->ay);
			cg_line_to(ctx, r->x+r->bx, r->x+r->by);
			if(r->type & MWOUTLINE)
				cg_close_path(ctx);
			cg_stroke(ctx);
			
			return 0;
		}
		case GrNumText:
		{
/*
#define GrNumText               42
typedef struct {
	BYTE8	reqType;
	BYTE8	hilength;
	UINT16	length;
	IDTYPE	drawid;
	IDTYPE	gcid;
	INT16	x;
	INT16	y;
	INT16	count;
	INT16	pad;
	UINT32	flags;
	//BYTE8	text[];
} nxTextReq;
*/
			const nxTextReq *r = (const nxTextReq*) req;
			char text[1024];
			char *dst = text;
			const char *src = (const char *) GetReqData(r);
			printf("nxTextReq x %d, y %d, count %d, pad %d, flags 0x%08X, text: %.*s\r\n", r->x, r->y, r->count, r->pad, r->flags, r->count, src);
			do
				*dst++ = *src++;
			while(src - (char*) r < r->length && dst - text < sizeof(text));
			*dst = '\0'; //force null termination
			do_text(r->x, r->y, text, r->count);
			return 0;
		}
		case GrNumGetGCTextSize:
		{
/*
#define GrNumGetGCTextSize      38
typedef struct {
	BYTE8	reqType;
	BYTE8	hilength;
	UINT16	length;
	IDTYPE	gcid;
	UINT32	flags;
	UINT32	charcount;
	//BYTE8	text[];
} nxGetGCTextSizeReq;


	GrGetGCTextSize(req->gcid, GetReqData(req), req->charcount,
		req->flags, &retwidth, &retheight, &retbase);

*/	
			const nxGetGCTextSizeReq *r = (const nxGetGCTextSizeReq*) req;
			const char *text = (const char *) GetReqData(r);
			printf("nxGetGCTextSizeReq count %d, text: %.*s\r\n", r->charcount, text);
	GR_SIZE             retwidth = 20, retheight = 36, retbase = 15;
 	GsWriteType(current_fd,nreq);
	GsWrite(current_fd, &retwidth, sizeof(retwidth));
	GsWrite(current_fd, &retheight, sizeof(retheight));
	GsWrite(current_fd, &retbase, sizeof(retbase));	
			return 1;
		}
		case GrNumFillPoly:
		{
/*
#define GrNumFillPoly           30
typedef struct {
	BYTE8	reqType;
	BYTE8	hilength;
	UINT16	length;
	IDTYPE	drawid;
	IDTYPE	gcid;
	//INT16 pointtable[]; //x0,y0,x1,y1...
} nxFillPolyReq;
*/

			const nxFillPolyReq *r = (const nxFillPolyReq*) req;
			GR_POINT *p = (GR_POINT*) GetReqData(r);
			int len = (r->length - sizeof(*r))/sizeof(*p);
			printf("nxFillPolyReq length %d\r\n", len);
			cg_new_path(ctx);
			cg_move_to(ctx, p->x, p->y); ++p;
			for(int i = 1; i < len; ++i, ++p)
				cg_line_to(ctx, p->x, p->y);
			cg_close_path(ctx);
			cg_fill(ctx);
			return 0;
		}
		case GrNumPoly:
		{
/*
#define GrNumPoly               29
typedef struct {
	BYTE8	reqType;
	BYTE8	hilength;
	UINT16	length;
	IDTYPE	drawid;
	IDTYPE	gcid;
	//INT16 pointtable[];
} nxPolyReq;
*/

			const nxPolyReq *r = (const nxPolyReq*) req;
			GR_POINT *p = (GR_POINT*) GetReqData(r);
			int len = (r->length - sizeof(*r))/sizeof(*p);
			printf("nxFillPolyReq length %d\r\n", len);
			cg_new_path(ctx);
			cg_move_to(ctx, p->x, p->y); ++p;
			for(int i = 1; i < len; ++i, ++p)
				cg_line_to(ctx, p->x, p->y);
			cg_close_path(ctx);
			cg_stroke(ctx);
			return 0;
		}
		case GrNumFillEllipse:
		{
/*
#define GrNumFillEllipse        32
typedef struct {
	BYTE8	reqType;
	BYTE8	hilength;
	UINT16	length;
	IDTYPE	drawid;
	IDTYPE	gcid;
	INT16	x;
	INT16	y;
	INT16	rx;
	INT16	ry;
} nxFillEllipseReq;
*/
			const nxFillEllipseReq *r = (const nxFillEllipseReq*) req;
			printf("nxFillEllipseReq x %d, y %d, w %d, h %d\r\n", r->x, r->y, r->rx, r->ry);
			cg_ellipse(ctx, r->x, r->y, r->rx, r->ry); 
			cg_fill(ctx);
			return 0;
		}
		default:
			printf("\r\nunknown req %d\r\n\r\n", nreq);
	}
	return 0;
}

