/* nuklear - v1.05 - public domain */
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdarg.h>
#include <string.h>
#include <math.h>
#include <assert.h>
#include <time.h>
#include <limits.h>

int machine_logger(const char * fmt, ...);
#define printf machine_logger
//#define printf(...)

#define NK_INCLUDE_FIXED_TYPES
#define NK_INCLUDE_STANDARD_IO
#define NK_INCLUDE_DEFAULT_ALLOCATOR
#define NK_INCLUDE_VERTEX_BUFFER_OUTPUT
#define NK_INCLUDE_FONT_BAKING
#define NK_INCLUDE_DEFAULT_FONT
#define NK_IMPLEMENTATION
#include "nuklear.h"


/* macros */
#define WINDOW_WIDTH 1200
#define WINDOW_HEIGHT 800


#define UNUSED(a) (void)a
#define MIN(a,b) ((a) < (b) ? (a) : (b))
#define MAX(a,b) ((a) < (b) ? (b) : (a))
#define LEN(a) (sizeof(a)/sizeof(a)[0])


struct media {
    struct nk_font *font_14;
    struct nk_font *font_18;
    struct nk_font *font_20;
    struct nk_font *font_22;

    struct nk_image unchecked;
    struct nk_image checked;
    struct nk_image rocket;
    struct nk_image cloud;
    struct nk_image pen;
    struct nk_image play;
    struct nk_image pause;
    struct nk_image stop;
    struct nk_image prev;
    struct nk_image next;
    struct nk_image tools;
    struct nk_image dir;
    struct nk_image copy;
    struct nk_image convert;
    struct nk_image del;
    struct nk_image edit;
    struct nk_image images[9];
    struct nk_image menu[6];
};

/* ===============================================================
 *
 *                          CUSTOM WIDGET
 *
 * ===============================================================*/
static int
ui_piemenu(struct nk_context *ctx, struct nk_vec2 pos, float radius,
            struct nk_image *icons, int item_count)
{
    int ret = -1;
    struct nk_rect total_space;
    struct nk_rect bounds;
    int active_item = 0;

    /* pie menu popup */
    struct nk_color border = ctx->style.window.border_color;
    struct nk_style_item background = ctx->style.window.fixed_background;
    ctx->style.window.fixed_background = nk_style_item_hide();
    ctx->style.window.border_color = nk_rgba(0,0,0,0);

    total_space  = nk_window_get_content_region(ctx);
    ctx->style.window.spacing = nk_vec2(0,0);
    ctx->style.window.padding = nk_vec2(0,0);

    if (nk_popup_begin(ctx, NK_POPUP_STATIC, "piemenu", NK_WINDOW_NO_SCROLLBAR,
        nk_rect(pos.x - total_space.x - radius, pos.y - radius - total_space.y,
        2*radius,2*radius)))
    {
        int i = 0;
        struct nk_command_buffer* out = nk_window_get_canvas(ctx);
        const struct nk_input *in = &ctx->input;

        total_space = nk_window_get_content_region(ctx);
        ctx->style.window.spacing = nk_vec2(4,4);
        ctx->style.window.padding = nk_vec2(8,8);
        nk_layout_row_dynamic(ctx, total_space.h, 1);
        nk_widget(&bounds, ctx);

        /* outer circle */
        nk_fill_circle(out, bounds, nk_rgb(50,50,50));
        {
            /* circle buttons */
            float step = (2 * 3.141592654f) / (float)(MAX(1,item_count));
            float a_min = 0; float a_max = step;

            struct nk_vec2 center = nk_vec2(bounds.x + bounds.w / 2.0f, bounds.y + bounds.h / 2.0f);
            struct nk_vec2 drag = nk_vec2(in->mouse.pos.x - center.x, in->mouse.pos.y - center.y);
            float angle = (float)atan2(drag.y, drag.x);
            if (angle < -0.0f) angle += 2.0f * 3.141592654f;
            active_item = (int)(angle/step);

            for (i = 0; i < item_count; ++i) {
                struct nk_rect content;
                float rx, ry, dx, dy, a;
                nk_fill_arc(out, center.x, center.y, (bounds.w/2.0f),
                    a_min, a_max, (active_item == i) ? nk_rgb(45,100,255): nk_rgb(60,60,60));

                /* separator line */
                rx = bounds.w/2.0f; ry = 0;
                dx = rx * (float)cos(a_min) - ry * (float)sin(a_min);
                dy = rx * (float)sin(a_min) + ry * (float)cos(a_min);
                nk_stroke_line(out, center.x, center.y,
                    center.x + dx, center.y + dy, 1.0f, nk_rgb(50,50,50));

                /* button content */
                a = a_min + (a_max - a_min)/2.0f;
                rx = bounds.w/2.5f; ry = 0;
                content.w = 30; content.h = 30;
                content.x = center.x + ((rx * (float)cos(a) - ry * (float)sin(a)) - content.w/2.0f);
                content.y = center.y + (rx * (float)sin(a) + ry * (float)cos(a) - content.h/2.0f);
                nk_draw_image(out, content, &icons[i], nk_rgb(255,255,255));
                a_min = a_max; a_max += step;
            }
        }
        {
            /* inner circle */
            struct nk_rect inner;
            inner.x = bounds.x + bounds.w/2 - bounds.w/4;
            inner.y = bounds.y + bounds.h/2 - bounds.h/4;
            inner.w = bounds.w/2; inner.h = bounds.h/2;
            nk_fill_circle(out, inner, nk_rgb(45,45,45));

            /* active icon content */
            bounds.w = inner.w / 2.0f;
            bounds.h = inner.h / 2.0f;
            bounds.x = inner.x + inner.w/2 - bounds.w/2;
            bounds.y = inner.y + inner.h/2 - bounds.h/2;
            nk_draw_image(out, bounds, &icons[active_item], nk_rgb(255,255,255));
        }
        nk_layout_space_end(ctx);
        if (!nk_input_is_mouse_down(&ctx->input, NK_BUTTON_RIGHT)) {
            nk_popup_close(ctx);
            ret = active_item;
        }
    } else ret = -2;
    ctx->style.window.spacing = nk_vec2(4,4);
    ctx->style.window.padding = nk_vec2(8,8);
    nk_popup_end(ctx);

    ctx->style.window.fixed_background = background;
    ctx->style.window.border_color = border;
    return ret;
}

/* ===============================================================
 *
 *                          GRID
 *
 * ===============================================================*/
static void
grid_demo(struct nk_context *ctx, struct media *media)
{
    static char text[3][64];
    static int text_len[3];
    static const char *items[] = {"Item 0","item 1","item 2"};
    static int selected_item = 0;
    static int check = 1;

    int i;
    nk_style_set_font(ctx, &media->font_20->handle);
    if (nk_begin(ctx, "Grid Demo", nk_rect(600, 350, 275, 250),
        NK_WINDOW_TITLE|NK_WINDOW_BORDER|NK_WINDOW_MOVABLE|
        NK_WINDOW_NO_SCROLLBAR))
    {
        nk_style_set_font(ctx, &media->font_18->handle);
        nk_layout_row_dynamic(ctx, 30, 2);
        nk_label(ctx, "Floating point:", NK_TEXT_RIGHT);
        nk_edit_string(ctx, NK_EDIT_FIELD, text[0], &text_len[0], 64, nk_filter_float);
        nk_label(ctx, "Hexadecimal:", NK_TEXT_RIGHT);
        nk_edit_string(ctx, NK_EDIT_FIELD, text[1], &text_len[1], 64, nk_filter_hex);
        nk_label(ctx, "Binary:", NK_TEXT_RIGHT);
        nk_edit_string(ctx, NK_EDIT_FIELD, text[2], &text_len[2], 64, nk_filter_binary);
        nk_label(ctx, "Checkbox:", NK_TEXT_RIGHT);
        nk_checkbox_label(ctx, "Check me", &check);
        nk_label(ctx, "Combobox:", NK_TEXT_RIGHT);
        if (nk_combo_begin_label(ctx, items[selected_item], nk_vec2(nk_widget_width(ctx), 200))) {
            nk_layout_row_dynamic(ctx, 25, 1);
            for (i = 0; i < 3; ++i)
                if (nk_combo_item_label(ctx, items[i], NK_TEXT_LEFT))
                    selected_item = i;
            nk_combo_end(ctx);
        }
    }
    nk_end(ctx);
    nk_style_set_font(ctx, &media->font_14->handle);
}

/* ===============================================================
 *
 *                          BUTTON DEMO
 *
 * ===============================================================*/
static void
ui_header(struct nk_context *ctx, struct media *media, const char *title)
{
    nk_style_set_font(ctx, &media->font_18->handle);
    nk_layout_row_dynamic(ctx, 20, 1);
    nk_label(ctx, title, NK_TEXT_LEFT);
}

static void
ui_widget(struct nk_context *ctx, struct media *media, float height)
{
    static const float ratio[] = {0.15f, 0.85f};
    nk_style_set_font(ctx, &media->font_22->handle);
    nk_layout_row(ctx, NK_DYNAMIC, height, 2, ratio);
    nk_spacing(ctx, 1);
}

static void
ui_widget_centered(struct nk_context *ctx, struct media *media, float height)
{
    static const float ratio[] = {0.15f, 0.50f, 0.35f};
    nk_style_set_font(ctx, &media->font_22->handle);
    nk_layout_row(ctx, NK_DYNAMIC, height, 3, ratio);
    nk_spacing(ctx, 1);
}

static void
button_demo(struct nk_context *ctx, struct media *media)
{
    static int option = 1;
    static int toggle0 = 1;
    static int toggle1 = 0;
    static int toggle2 = 1;

    nk_style_set_font(ctx, &media->font_20->handle);
    nk_begin(ctx, "Button Demo", nk_rect(50,50,255,610),
        NK_WINDOW_BORDER|NK_WINDOW_MOVABLE|NK_WINDOW_TITLE);

    /*------------------------------------------------
     *                  MENU
     *------------------------------------------------*/
    nk_menubar_begin(ctx);
    {
        /* toolbar */
        nk_layout_row_static(ctx, 40, 40, 4);
        if (nk_menu_begin_image(ctx, "Music", media->play, nk_vec2(110,120)))
        {
            /* settings */
            nk_layout_row_dynamic(ctx, 25, 1);
            nk_menu_item_image_label(ctx, media->play, "Play", NK_TEXT_RIGHT);
            nk_menu_item_image_label(ctx, media->stop, "Stop", NK_TEXT_RIGHT);
            nk_menu_item_image_label(ctx, media->pause, "Pause", NK_TEXT_RIGHT);
            nk_menu_item_image_label(ctx, media->next, "Next", NK_TEXT_RIGHT);
            nk_menu_item_image_label(ctx, media->prev, "Prev", NK_TEXT_RIGHT);
            nk_menu_end(ctx);
        }
        nk_button_image(ctx, media->tools);
        nk_button_image(ctx, media->cloud);
        nk_button_image(ctx, media->pen);
    }
    nk_menubar_end(ctx);

    /*------------------------------------------------
     *                  BUTTON
     *------------------------------------------------*/
    ui_header(ctx, media, "Push buttons");
    ui_widget(ctx, media, 35);
    if (nk_button_label(ctx, "Push me"))
        fprintf(stdout, "pushed!\n");
    ui_widget(ctx, media, 35);
    if (nk_button_image_label(ctx, media->rocket, "Styled", NK_TEXT_CENTERED))
        fprintf(stdout, "rocket!\n");

    /*------------------------------------------------
     *                  REPEATER
     *------------------------------------------------*/
    ui_header(ctx, media, "Repeater");
    ui_widget(ctx, media, 35);
    if (nk_button_label(ctx, "Press me"))
        fprintf(stdout, "pressed!\n");

    /*------------------------------------------------
     *                  TOGGLE
     *------------------------------------------------*/
    ui_header(ctx, media, "Toggle buttons");
    ui_widget(ctx, media, 35);
    if (nk_button_image_label(ctx, (toggle0) ? media->checked: media->unchecked, "Toggle", NK_TEXT_LEFT))
        toggle0 = !toggle0;

    ui_widget(ctx, media, 35);
    if (nk_button_image_label(ctx, (toggle1) ? media->checked: media->unchecked, "Toggle", NK_TEXT_LEFT))
        toggle1 = !toggle1;

    ui_widget(ctx, media, 35);
    if (nk_button_image_label(ctx, (toggle2) ? media->checked: media->unchecked, "Toggle", NK_TEXT_LEFT))
        toggle2 = !toggle2;

    /*------------------------------------------------
     *                  RADIO
     *------------------------------------------------*/
    ui_header(ctx, media, "Radio buttons");
    ui_widget(ctx, media, 35);
    if (nk_button_symbol_label(ctx, (option == 0)?NK_SYMBOL_CIRCLE_OUTLINE:NK_SYMBOL_CIRCLE_SOLID, "Select", NK_TEXT_LEFT))
        option = 0;
    ui_widget(ctx, media, 35);
    if (nk_button_symbol_label(ctx, (option == 1)?NK_SYMBOL_CIRCLE_OUTLINE:NK_SYMBOL_CIRCLE_SOLID, "Select", NK_TEXT_LEFT))
        option = 1;
    ui_widget(ctx, media, 35);
    if (nk_button_symbol_label(ctx, (option == 2)?NK_SYMBOL_CIRCLE_OUTLINE:NK_SYMBOL_CIRCLE_SOLID, "Select", NK_TEXT_LEFT))
        option = 2;

    /*------------------------------------------------
     *                  CONTEXTUAL
     *------------------------------------------------*/
    nk_style_set_font(ctx, &media->font_18->handle);
    if (nk_contextual_begin(ctx, NK_WINDOW_NO_SCROLLBAR, nk_vec2(150, 300), nk_window_get_bounds(ctx))) {
        nk_layout_row_dynamic(ctx, 30, 1);
        if (nk_contextual_item_image_label(ctx, media->copy, "Clone", NK_TEXT_RIGHT))
            fprintf(stdout, "pressed clone!\n");
        if (nk_contextual_item_image_label(ctx, media->del, "Delete", NK_TEXT_RIGHT))
            fprintf(stdout, "pressed delete!\n");
        if (nk_contextual_item_image_label(ctx, media->convert, "Convert", NK_TEXT_RIGHT))
            fprintf(stdout, "pressed convert!\n");
        if (nk_contextual_item_image_label(ctx, media->edit, "Edit", NK_TEXT_RIGHT))
            fprintf(stdout, "pressed edit!\n");
        nk_contextual_end(ctx);
    }
    nk_style_set_font(ctx, &media->font_14->handle);
    nk_end(ctx);
}

/* ===============================================================
 *
 *                          BASIC DEMO
 *
 * ===============================================================*/
static void
basic_demo(struct nk_context *ctx, struct media *media)
{
    static int image_active;
    static int check0 = 1;
    static int check1 = 0;
    static size_t prog = 80;
    static int selected_item = 0;
    static int selected_image = 3;
    static int selected_icon = 0;
    static const char *items[] = {"Item 0","item 1","item 2"};
    static int piemenu_active = 0;
    static struct nk_vec2 piemenu_pos;

    int i = 0;
    nk_style_set_font(ctx, &media->font_20->handle);
    nk_begin(ctx, "Basic Demo", nk_rect(320, 50, 275, 610),
        NK_WINDOW_BORDER|NK_WINDOW_MOVABLE|NK_WINDOW_TITLE);

    /*------------------------------------------------
     *                  POPUP BUTTON
     *------------------------------------------------*/
    ui_header(ctx, media, "Popup & Scrollbar & Images");
    ui_widget(ctx, media, 35);
    if (nk_button_image_label(ctx, media->dir, "Images", NK_TEXT_CENTERED))
        image_active = !image_active;

    /*------------------------------------------------
     *                  SELECTED IMAGE
     *------------------------------------------------*/
    ui_header(ctx, media, "Selected Image");
    ui_widget_centered(ctx, media, 100);
    nk_image(ctx, media->images[selected_image]);

    /*------------------------------------------------
     *                  IMAGE POPUP
     *------------------------------------------------*/
    if (image_active) {
        struct nk_panel popup;
        if (nk_popup_begin(ctx, NK_POPUP_STATIC, "Image Popup", 0, nk_rect(265, 0, 320, 220))) {
            nk_layout_row_static(ctx, 82, 82, 3);
            for (i = 0; i < 9; ++i) {
                if (nk_button_image(ctx, media->images[i])) {
                    selected_image = i;
                    image_active = 0;
                    nk_popup_close(ctx);
                }
            }
            nk_popup_end(ctx);
        }
    }
    /*------------------------------------------------
     *                  COMBOBOX
     *------------------------------------------------*/
    ui_header(ctx, media, "Combo box");
    ui_widget(ctx, media, 40);
    if (nk_combo_begin_label(ctx, items[selected_item], nk_vec2(nk_widget_width(ctx), 200))) {
        nk_layout_row_dynamic(ctx, 35, 1);
        for (i = 0; i < 3; ++i)
            if (nk_combo_item_label(ctx, items[i], NK_TEXT_LEFT))
                selected_item = i;
        nk_combo_end(ctx);
    }

    ui_widget(ctx, media, 40);
    if (nk_combo_begin_image_label(ctx, items[selected_icon], media->images[selected_icon], nk_vec2(nk_widget_width(ctx), 200))) {
        nk_layout_row_dynamic(ctx, 35, 1);
        for (i = 0; i < 3; ++i)
            if (nk_combo_item_image_label(ctx, media->images[i], items[i], NK_TEXT_RIGHT))
                selected_icon = i;
        nk_combo_end(ctx);
    }

    /*------------------------------------------------
     *                  CHECKBOX
     *------------------------------------------------*/
    ui_header(ctx, media, "Checkbox");
    ui_widget(ctx, media, 30);
    nk_checkbox_label(ctx, "Flag 1", &check0);
    ui_widget(ctx, media, 30);
    nk_checkbox_label(ctx, "Flag 2", &check1);

    /*------------------------------------------------
     *                  PROGRESSBAR
     *------------------------------------------------*/
    ui_header(ctx, media, "Progressbar");
    ui_widget(ctx, media, 35);
    nk_progress(ctx, &prog, 100, nk_true);

    /*------------------------------------------------
     *                  PIEMENU
     *------------------------------------------------*/
    if (nk_input_is_mouse_click_down_in_rect(&ctx->input, NK_BUTTON_RIGHT,
        nk_window_get_bounds(ctx),nk_true)){
        piemenu_pos = ctx->input.mouse.pos;
        piemenu_active = 1;
    }

    if (piemenu_active) {
        int ret = ui_piemenu(ctx, piemenu_pos, 140, &media->menu[0], 6);
        if (ret == -2) piemenu_active = 0;
        if (ret != -1) {
            fprintf(stdout, "piemenu selected: %d\n", ret);
            piemenu_active = 0;
        }
    }
    nk_style_set_font(ctx, &media->font_14->handle);
    nk_end(ctx);
}

struct nk_font_atlas atlas;
static struct nk_context nkctx;
static struct media media;

#include <graphic/surface.h>
#include <cg.h>
#include "cat.h" //cat sample image data
	
static struct cg_ctx_t * cgctx = NULL;
struct surface_t;
extern struct surface_t *current_surface;

static void do_stretch_blit(int x, int y, int w, int h, const struct surface_t *img)
{
	struct matrix_t m;
	matrix_init_identity(&m);
	if(w != img->width || h != img->height)
	{
		matrix_translate(&m, x, y);
		matrix_scale(&m, (double)w/img->width, (double)h/img->height);
		//struct region_t r;
		//region_init(&r, x, y, w, h); //clip not needed since scaled
		//surface_blit(current_surface, /*&r*/NULL, &m, img);
		
		surface_shape_save(current_surface);
		surface_shape_set_matrix(current_surface, &m);
		surface_shape_set_source(current_surface, img, 0, 0);
		surface_shape_paint(current_surface);
		surface_shape_restore(current_surface);	
	}
	else
	{
		surface_shape_set_source(current_surface, img, x, y);
		surface_shape_paint(current_surface);
	}
}

static void do_text(int x, int y, const char *utf8, unsigned count);

static void commands_render()
{
    const struct nk_command *cmd;
    nk_foreach(cmd, &nkctx)
    {
    	const char *cmdname = "";
        switch (cmd->type) {
        case NK_COMMAND_NOP:
        	cmdname = "NK_COMMAND_NOP";
        	break;
        case NK_COMMAND_SCISSOR: {
            const struct nk_command_scissor *s =(const struct nk_command_scissor*)cmd;
        	cmdname = "NK_COMMAND_SCISSOR";
            /*
struct nk_command_scissor {
    struct nk_command header;
    short x, y;
    unsigned short w, h;
};
            */
			printf("nk_command_scissor x %d, y %d, w %d, h %d\r\n", s->x, s->y, s->w, s->h);
            if(cgctx)
            {
				cgctx->clip.x = s->x;
				cgctx->clip.y = s->y;
				cgctx->clip.w = s->w;
				cgctx->clip.h = s->h;
			}
        } break;
        case NK_COMMAND_LINE: {
            const struct nk_command_line *l = (const struct nk_command_line *)cmd;
        	cmdname = "NK_COMMAND_LINE";
            //color = nk_color_to_allegro_color(l->color);
            //al_draw_line((float)l->begin.x, (float)l->begin.y, (float)l->end.x,
            //    (float)l->end.y, color, (float)l->line_thickness);
        } break;
        case NK_COMMAND_RECT: {
            const struct nk_command_rect *r = (const struct nk_command_rect *)cmd;
        	cmdname = "NK_COMMAND_RECT";
        	/*
struct nk_command_rect {
    struct nk_command header;
    unsigned short rounding;
    unsigned short line_thickness;
    short x, y;
    unsigned short w, h;
    struct nk_color color;
};
        	*/
			printf("nk_command_rect x %d, y %d, w %d, h %d\r\n", r->x, r->y, r->w, r->h);
			cg_set_source_rgba(cgctx, r->color.r/255., r->color.g/255., r->color.b/255., r->color.a/255.);
			cg_set_line_width(cgctx, r->line_thickness);
			cg_move_to(cgctx, r->x, r->y);
			cg_rel_line_to(cgctx, r->w, 0);
			cg_rel_line_to(cgctx, 0, r->h);
			cg_rel_line_to(cgctx, -r->w, 0);
			cg_close_path(cgctx);
			cg_stroke(cgctx);
            
        } break;
        case NK_COMMAND_RECT_FILLED: {
        	/*
struct nk_command_rect_filled {
    struct nk_command header;
    unsigned short rounding;
    short x, y;
    unsigned short w, h;
    struct nk_color color;
};
        	*/
            const struct nk_command_rect_filled *r = (const struct nk_command_rect_filled *)cmd;
        	cmdname = "NK_COMMAND_RECT_FILLED";
			printf("nk_command_rect_filled x %d, y %d, w %d, h %d\r\n", r->x, r->y, r->w, r->h);
			cg_set_source_rgba(cgctx, r->color.r/255., r->color.g/255., r->color.b/255., r->color.a/255.);
			cg_move_to(cgctx, r->x, r->y);
			cg_rel_line_to(cgctx, r->w, 0);
			cg_rel_line_to(cgctx, 0, r->h);
			cg_rel_line_to(cgctx, -r->w, 0);
			cg_close_path(cgctx);
			cg_fill(cgctx);
        } break;
        case NK_COMMAND_CIRCLE: {
        /*

struct nk_command_circle {
    struct nk_command header;
    short x, y;
    unsigned short line_thickness;
    unsigned short w, h;
    struct nk_color color;
};
        */
            float xr, yr;
            const struct nk_command_circle *c = (const struct nk_command_circle *)cmd;
        	cmdname = "NK_COMMAND_CIRCLE";
            //color = nk_color_to_allegro_color(c->color);
            xr = (float)c->w/2;
            yr = (float)c->h/2;
            //al_draw_ellipse(((float)(c->x)) + xr, ((float)c->y) + yr,
            //    xr, yr, color, (float)c->line_thickness);
        } break;
        case NK_COMMAND_CIRCLE_FILLED: {
        /*
struct nk_command_circle_filled {
    struct nk_command header;
    short x, y;
    unsigned short w, h;
    struct nk_color color;
};
        */
            float xr, yr;
            const struct nk_command_circle_filled *c = (const struct nk_command_circle_filled *)cmd;
        	cmdname = "NK_COMMAND_CIRCLE_FILLED";
			printf("nk_command_circle_filled x %d, y %d, w %d, h %d\r\n", c->x, c->y, c->w, c->h);
			cg_set_source_rgba(cgctx, c->color.r/255., c->color.g/255., c->color.b/255., c->color.a/255.);
            xr = (float)c->w/2;
            yr = (float)c->h/2;
            //al_draw_filled_ellipse(((float)(c->x)) + xr, ((float)c->y) + yr,
            //    xr, yr, color);
			cg_set_line_width(cgctx, 1);
			cg_ellipse(cgctx, c->x + xr, c->y + yr, xr, yr);
			cg_fill(cgctx);

            
        } break;
        case NK_COMMAND_TRIANGLE: {
            const struct nk_command_triangle*t = (const struct nk_command_triangle*)cmd;
        	cmdname = "NK_COMMAND_TRIANGLE";
        	/*

struct nk_command_triangle {
    struct nk_command header;
    unsigned short line_thickness;
    struct nk_vec2i a;
    struct nk_vec2i b;
    struct nk_vec2i c;
    struct nk_color color;
};
        	*/
            //color = nk_color_to_allegro_color(t->color);
            //al_draw_triangle((float)t->a.x, (float)t->a.y, (float)t->b.x, (float)t->b.y,
            //    (float)t->c.x, (float)t->c.y, color, (float)t->line_thickness);
        } break;
        case NK_COMMAND_TRIANGLE_FILLED: {
            const struct nk_command_triangle_filled *t = (const struct nk_command_triangle_filled *)cmd;
        	cmdname = "NK_COMMAND_TRIANGLE_FILLED";
        	/*

struct nk_command_triangle_filled {
    struct nk_command header;
    struct nk_vec2i a;
    struct nk_vec2i b;
    struct nk_vec2i c;
    struct nk_color color;
};
        	*/
			printf("nk_command_triangle_filled x0 %d, y0 %d, x1 %d, y1 %d, x2 %d, y2 %d\r\n", t->a.x, t->a.y, t->b.x, t->b.y, t->c.x, t->c.y);
			cg_set_source_rgba(cgctx, t->color.r/255., t->color.g/255., t->color.b/255., t->color.a/255.);
			cg_move_to(cgctx, t->a.x, t->a.y);
			cg_line_to(cgctx, t->b.x, t->b.y);
			cg_line_to(cgctx, t->c.x, t->c.y);
			cg_close_path(cgctx);
			cg_fill(cgctx);
        } break;
        case NK_COMMAND_POLYGON: {
            int i;
            float *vertices;
            const struct nk_command_polygon *p = (const struct nk_command_polygon*)cmd;
        	cmdname = "NK_COMMAND_POLYGON";
            vertices = calloc(p->point_count * 2, sizeof(float));
            //color = nk_color_to_allegro_color(p->color);
            for (i = 0; i < p->point_count; i++) {
                vertices[i*2] = p->points[i].x;
                vertices[(i*2) + 1] = p->points[i].y;
            }
            //al_draw_polyline(vertices, (2 * sizeof(float)),
            //    (int)p->point_count, ALLEGRO_LINE_JOIN_ROUND, ALLEGRO_LINE_CAP_CLOSED,
            //    color, (float)p->line_thickness, 0.0);
            free(vertices);
        } break;
        case NK_COMMAND_POLYGON_FILLED: {
            int i, j = 0;
            float *vertices;
            const struct nk_command_polygon_filled *p = (const struct nk_command_polygon_filled *)cmd;
        	cmdname = "NK_COMMAND_POLYGON_FILLED";
            vertices = calloc(p->point_count * 2, sizeof(float));
            //color = nk_color_to_allegro_color(p->color);
            for (i = p->point_count - 1; i >= 0; i--) {
                vertices[j++] = p->points[i].x;
                vertices[j++] = p->points[i].y;
            }
            //al_draw_filled_polygon(vertices, (int)p->point_count, color);
            free(vertices);
        } break;
        case NK_COMMAND_POLYLINE: {
            int i;
            float *vertices;
            const struct nk_command_polyline *p = (const struct nk_command_polyline *)cmd;
        	cmdname = "NK_COMMAND_POLYLINE";
            vertices = calloc(p->point_count * 2, sizeof(float));
            //color = nk_color_to_allegro_color(p->color);
            for (i = 0; i < p->point_count; i++) {
                vertices[i*2] = p->points[i].x;
                vertices[(i*2) + 1] = p->points[i].y;
            }
            //al_draw_polyline(vertices, (2 * sizeof(float)),
            //    (int)p->point_count, ALLEGRO_LINE_JOIN_ROUND, ALLEGRO_LINE_CAP_ROUND,
            //    color, (float)p->line_thickness, 0.0);
            free(vertices);
        } break;
        case NK_COMMAND_TEXT: {
        /*

struct nk_command_text {
    struct nk_command header;
    const struct nk_user_font *font;
    struct nk_color background;
    struct nk_color foreground;
    short x, y;
    unsigned short w, h;
    float height;
    int length;
    char string[1];
};
        */
            const struct nk_command_text *t = (const struct nk_command_text*)cmd;
        	cmdname = "NK_COMMAND_TEXT";
			printf("nk_command_text x %d, y %d, w %d, h %d, text: '%.*s'\r\n", t->x, t->y, t->w, t->h, t->length, t->string);
			cg_set_source_rgba(cgctx, t->foreground.r/255., t->foreground.g/255., t->foreground.b/255., t->foreground.a/255.);
        	do_text(t->x, t->y, t->string, t->length);
        } break;
        case NK_COMMAND_CURVE: {
            float points[8];
            const struct nk_command_curve *q = (const struct nk_command_curve *)cmd;
        	cmdname = "NK_COMMAND_CURVE";
            //color = nk_color_to_allegro_color(q->color);
            points[0] = (float)q->begin.x;
            points[1] = (float)q->begin.y;
            points[2] = (float)q->ctrl[0].x;
            points[3] = (float)q->ctrl[0].y;
            points[4] = (float)q->ctrl[1].x;
            points[5] = (float)q->ctrl[1].y;
            points[6] = (float)q->end.x;
            points[7] = (float)q->end.y;
            //al_draw_spline(points, color, (float)q->line_thickness);
        } break;
        case NK_COMMAND_ARC: {
            const struct nk_command_arc *a = (const struct nk_command_arc *)cmd;
        	cmdname = "NK_COMMAND_ARC";
            //color = nk_color_to_allegro_color(a->color);
            //al_draw_pieslice((float)a->cx, (float)a->cy, (float)a->r, a->a[0],
            //    a->a[1], color, (float)a->line_thickness);
        } break;
        case NK_COMMAND_ARC_FILLED: {
            const struct nk_command_arc_filled *a = (const struct nk_command_arc_filled *)cmd;
        	cmdname = "NK_COMMAND_ARC_FILLED";
            //color = nk_color_to_allegro_color(a->color);
            //al_draw_filled_pieslice((float)a->cx, (float)a->cy, (float)a->r, a->a[0],
            //    a->a[1], color);
        } break;
        case NK_COMMAND_IMAGE: {
            const struct nk_command_image *i = (const struct nk_command_image *)cmd;
        	cmdname = "NK_COMMAND_IMAGE";
        	struct surface_t *imgsurface = (struct surface_t *) i->img.handle.ptr;
			printf("nk_command_image x %d, y %d, w %d, h %d, img ptr 0x%p, imgw %d, imgh %d\r\n", i->x, i->y, i->w, i->h, imgsurface, imgsurface ? imgsurface->width : -1, imgsurface ? imgsurface->height : -1);
        	/*
struct nk_command_image {
    struct nk_command header;
    short x, y;
    unsigned short w, h;
    struct nk_image img;
    struct nk_color col;
};
struct nk_image {nk_handle handle; nk_ushort w, h; nk_ushort region[4];};
        	*/
        	if(imgsurface)
        		do_stretch_blit(i->x, i->y, i->w, i->h, imgsurface);
        } break;
        case NK_COMMAND_RECT_MULTI_COLOR:
        	cmdname = "NK_COMMAND_RECT_MULTI_COLOR";
        	break;
        default:
        	cmdname = "UNKNOWN COMMAND";
        	break;
        }
    	printf("Nuklear draw command %d, %s\r\n", cmd->type, cmdname);
    }
    nk_clear(&nkctx);
}

float my_nk_text_width_f(nk_handle user, float h, const char*s, int len)
{
  return 10;
}

/*typedef void(*nk_query_font_glyph_f)(nk_handle handle, float font_height,
                                    struct nk_user_font_glyph *glyph,
                                    nk_rune codepoint, nk_rune next_codepoint);
*/
#include <xfs/xfs.h>
static struct xfs_context_t xfs_assets;
static void init_assets_dir(const char *path)
{
	init_list_head(&xfs_assets.mounts.list);
	spin_lock_init(&xfs_assets.lock);
	xfs_mount(&xfs_assets, path, 0);
}


static struct nk_image image_load(const char *filename)
{
	struct surface_t * img = NULL;
	{
		/*
struct surface_t
{
int width;
int height;
int stride;
int pixlen;
void * pixels;
struct render_t * r;
void * rctx;
void * priv;
};			*/
		img = surface_alloc_from_xfs(&xfs_assets, filename);
	}
	printf("loaded image %s at 0x%p\r\n", filename, img);
	return nk_image_ptr(img);
}

void nuklear_demo(void)
{
	printf("render_test2, renderer %s\r\n", current_surface->r->name);
	static int init = 0;
	if(!init)
	{
		init = 1;

		struct nk_font_config cfg = nk_font_config(0);
		cfg.oversample_h = 3; cfg.oversample_v = 2;
		/* Loading one font with different heights is only required if you want higher
		 * quality text otherwise you can just set the font height directly
		 * e.g.: nkctx->style.font.height = 20. */
		nk_font_atlas_init_default(&atlas);
		nk_font_atlas_begin(&atlas);
		media.font_14 = nk_font_atlas_add_from_file(&atlas, "/framework/assets/fonts/Roboto-Regular.ttf", 14.0f, &cfg);
		media.font_18 = nk_font_atlas_add_from_file(&atlas, "/framework/assets/fonts/Roboto-Regular.ttf", 18.0f, &cfg);
		media.font_20 = nk_font_atlas_add_from_file(&atlas, "/framework/assets/fonts/Roboto-Regular.ttf", 20.0f, &cfg);
		media.font_22 = nk_font_atlas_add_from_file(&atlas, "/framework/assets/fonts/Roboto-Regular.ttf", 22.0f, &cfg);
		//image = nk_font_atlas_bake(&atlas, &w, &h, NK_FONT_ATLAS_RGBA32);
		//device_upload_atlas(&device, image, w, h);
		struct nk_draw_null_texture null;
		nk_font_atlas_end(&atlas, nk_handle_id(-1), &null);

		media.font_14->handle.height = 14;
        media.font_14->handle.width = my_nk_text_width_f;
		media.font_18->handle.height = 18;
        media.font_18->handle.width = my_nk_text_width_f;
		media.font_20->handle.height = 20;
        media.font_20->handle.width = my_nk_text_width_f;
		media.font_22->handle.height = 22;
        media.font_22->handle.width = my_nk_text_width_f;
        
		nk_init_default(&nkctx, &media.font_14->handle);
		init_assets_dir("/application");
		media.unchecked = image_load("nuklear/icon/unchecked.png");
		media.checked = image_load("nuklear/icon/checked.png");
		media.rocket = image_load("nuklear/icon/rocket.png");
		media.cloud = image_load("nuklear/icon/cloud.png");
		media.pen = image_load("nuklear/icon/pen.png");
		media.play = image_load("nuklear/icon/play.png");
		media.pause = image_load("nuklear/icon/pause.png");
		media.stop = image_load("nuklear/icon/stop.png");
		media.next =  image_load("nuklear/icon/next.png");
		media.prev =  image_load("nuklear/icon/prev.png");
		media.tools = image_load("nuklear/icon/tools.png");
		media.dir = image_load("nuklear/icon/directory.png");
		media.copy = image_load("nuklear/icon/copy.png");
		media.convert = image_load("nuklear/icon/export.png");
		media.del = image_load("nuklear/icon/delete.png");
		media.edit = image_load("nuklear/icon/edit.png");
		media.menu[0] = image_load("nuklear/icon/home.png");
		media.menu[1] = image_load("nuklear/icon/phone.png");
		media.menu[2] = image_load("nuklear/icon/plane.png");
		media.menu[3] = image_load("nuklear/icon/wifi.png");
		media.menu[4] = image_load("nuklear/icon/settings.png");
		media.menu[5] = image_load("nuklear/icon/volume.png");
	}
	
	grid_demo(&nkctx, &media);
	button_demo(&nkctx, &media);
	basic_demo(&nkctx, &media);
	
	commands_render();
}

#include <graphic/text.h>
static struct text_t txt;


static void do_text(int x, int y, const char *utf8, unsigned count)
{
	struct matrix_t m;
	matrix_init(&m, cgctx->state->matrix.a, cgctx->state->matrix.b, cgctx->state->matrix.c, cgctx->state->matrix.d, x+cgctx->state->matrix.tx, y+cgctx->state->matrix.ty);
	text_set_text(&txt, utf8);
	surface_text(current_surface, NULL, &m, &txt);
}

void render_test2(uint32_t *pixels, unsigned w, unsigned h, unsigned stride)
{
	if(cgctx == NULL)
	{
		struct cg_surface_t * surface = cg_surface_create_for_data(w, h, pixels);
		surface->stride = stride;
		cgctx = cg_create(surface);
		//cg_matrix_translate(&cgctx->state->matrix, 800/2, 0);
		
		txt.fctx = font_context_alloc();
		static struct color_t text_color = { .r = 0xFF, .g = 0xFF, .b = 0xFF, .a = 0xFF}; //WARNING: not copied, pointer used
		text_init(&txt, "", &text_color, 0, txt.fctx, NULL, 12); //default family
	}
	nuklear_demo();
	{static int y = 0;  do_text(10, y, "Hello", 5); if(++y>=WINDOW_HEIGHT) y=0; }
}
