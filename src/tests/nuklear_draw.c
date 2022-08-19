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

#define NK_INCLUDE_FIXED_TYPES
#define NK_INCLUDE_STANDARD_IO
#define NK_INCLUDE_DEFAULT_ALLOCATOR
#define NK_INCLUDE_VERTEX_BUFFER_OUTPUT
#define NK_INCLUDE_FONT_BAKING
#define NK_INCLUDE_DEFAULT_FONT
#define NK_IMPLEMENTATION
#include "nuklear.h"

//#define STB_IMAGE_IMPLEMENTATION
//#include "stb_image.h"

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


struct nk_font_atlas atlas;
static struct nk_context ctx;
static struct media media;

static void commands_render()
{
    const struct nk_command *cmd;
    nk_foreach(cmd, &ctx)
    {
    	const char *cmdname = "";
        switch (cmd->type) {
        case NK_COMMAND_NOP:
        	cmdname = "NK_COMMAND_NOP";
        	break;
        case NK_COMMAND_SCISSOR: {
            const struct nk_command_scissor *s =(const struct nk_command_scissor*)cmd;
        	cmdname = "NK_COMMAND_SCISSOR";
            //al_set_clipping_rectangle((int)s->x, (int)s->y, (int)s->w, (int)s->h);
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
            //color = nk_color_to_allegro_color(r->color);
            //al_draw_rounded_rectangle((float)r->x, (float)r->y, (float)(r->x + r->w),
            //    (float)(r->y + r->h), (float)r->rounding, (float)r->rounding, color,
            //    (float)r->line_thickness);
        } break;
        case NK_COMMAND_RECT_FILLED: {
            const struct nk_command_rect_filled *r = (const struct nk_command_rect_filled *)cmd;
        	cmdname = "NK_COMMAND_RECT_FILLED";
            //color = nk_color_to_allegro_color(r->color);
            //al_draw_filled_rounded_rectangle((float)r->x, (float)r->y,
            //    (float)(r->x + r->w), (float)(r->y + r->h), (float)r->rounding,
            //    (float)r->rounding, color);
        } break;
        case NK_COMMAND_CIRCLE: {
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
            float xr, yr;
            const struct nk_command_circle_filled *c = (const struct nk_command_circle_filled *)cmd;
        	cmdname = "NK_COMMAND_CIRCLE_FILLED";
            //color = nk_color_to_allegro_color(c->color);
            xr = (float)c->w/2;
            yr = (float)c->h/2;
            //al_draw_filled_ellipse(((float)(c->x)) + xr, ((float)c->y) + yr,
            //    xr, yr, color);
        } break;
        case NK_COMMAND_TRIANGLE: {
            const struct nk_command_triangle*t = (const struct nk_command_triangle*)cmd;
        	cmdname = "NK_COMMAND_TRIANGLE";
            //color = nk_color_to_allegro_color(t->color);
            //al_draw_triangle((float)t->a.x, (float)t->a.y, (float)t->b.x, (float)t->b.y,
            //    (float)t->c.x, (float)t->c.y, color, (float)t->line_thickness);
        } break;
        case NK_COMMAND_TRIANGLE_FILLED: {
            const struct nk_command_triangle_filled *t = (const struct nk_command_triangle_filled *)cmd;
        	cmdname = "NK_COMMAND_TRIANGLE_FILLED";
            //color = nk_color_to_allegro_color(t->color);
            //al_draw_filled_triangle((float)t->a.x, (float)t->a.y, (float)t->b.x,
            //    (float)t->b.y, (float)t->c.x, (float)t->c.y, color);
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
            const struct nk_command_text *t = (const struct nk_command_text*)cmd;
        	cmdname = "NK_COMMAND_TEXT";
            //NkAllegro5Font *font;
            //color = nk_color_to_allegro_color(t->foreground);
            //font = (NkAllegro5Font*)t->font->userdata.ptr;
            //al_draw_text(font->font,
            //    color, (float)t->x, (float)t->y, 0,
            //    (const char*)t->string);
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
            //al_draw_bitmap_region(i->img.handle.ptr, 0, 0, i->w, i->h, i->x, i->y, 0);
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
    nk_clear(&ctx);
}

float my_nk_text_width_f(nk_handle user, float h, const char*s, int len)
{
  return 10;
}

typedef void(*nk_query_font_glyph_f)(nk_handle handle, float font_height,
                                    struct nk_user_font_glyph *glyph,
                                    nk_rune codepoint, nk_rune next_codepoint);

void nuklear_demo(void)
{
	printf("render_test2\r\n");
	static int init = 0;
	if(!init)
	{
		init = 1;

		const void *image; int w, h;
		struct nk_font_config cfg = nk_font_config(0);
		cfg.oversample_h = 3; cfg.oversample_v = 2;
		/* Loading one font with different heights is only required if you want higher
		 * quality text otherwise you can just set the font height directly
		 * e.g.: ctx->style.font.height = 20. */
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
        
		nk_init_default(&ctx, &media.font_14->handle);
	}
	
	grid_demo(&ctx, &media);
	button_demo(&ctx, &media);
	
	commands_render();
}

void render_test2(uint32_t *pixels, unsigned w, unsigned h, unsigned stride)
{
	nuklear_demo();
}
