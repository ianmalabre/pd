/* Copyright (c) 1997-1999 Miller Puckette.
 * For information on usage and redistribution, and for a DISCLAIMER OF ALL
 * WARRANTIES, see the file, "LICENSE.txt," in this distribution. */

/* g_7_guis.c written by Thomas Musil (c) IEM KUG Graz Austria 2000-2001 */
/* thanks to Miller Puckette, Guenther Geiger and Krzystof Czaja */

#include "config.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include "m_pd.h"
#include "g_canvas.h"
#include "t_tk.h"
#include "g_all_guis.h"
#include <math.h>

#ifdef HAVE_UNISTD_H
#include <unistd.h>
#endif

#ifdef HAVE_IO_H
#include <io.h>
#endif

extern int gfxstub_haveproperties(void *key);
void toggle_draw_select(t_toggle* x, t_glist* glist);
 

/* --------------- tgl     gui-toggle ------------------------- */

t_widgetbehavior toggle_widgetbehavior;
static t_class *toggle_class;

/* widget helper functions */

void toggle_draw_update(t_gobj *xgobj, t_glist *glist)
{
    t_toggle *x = (t_toggle *)xgobj;
    if (x->x_gui.x_changed)
    {
        if(glist_isvisible(glist))
        {
            t_canvas *canvas=glist_getcanvas(glist);

            sys_vgui(".x%lx.c itemconfigure %lxX1 -stroke #%6.6x\n", canvas, x,
                     (x->x_on!=0.0)?x->x_gui.x_fcol:x->x_gui.x_bcol);
            sys_vgui(".x%lx.c itemconfigure %lxX2 -stroke #%6.6x\n", canvas, x,
                     (x->x_on!=0.0)?x->x_gui.x_fcol:x->x_gui.x_bcol);
        }
        x->x_gui.x_changed = 0;
    }
}

void toggle_draw_io(t_toggle* x, t_glist* glist, int old_snd_rcv_flags)
{
    t_canvas *canvas=glist_getcanvas(glist);
    iemgui_io_draw(&x->x_gui,canvas,old_snd_rcv_flags,"TGL");
}

void toggle_draw_new(t_toggle *x, t_glist *glist)
{
    t_canvas *canvas=glist_getcanvas(glist);
    int w=1, xx=text_xpix(&x->x_gui.x_obj, glist),
        yy=text_ypix(&x->x_gui.x_obj, glist);

    scalehandle_draw_new(x->x_gui. x_handle,canvas);
    scalehandle_draw_new(x->x_gui.x_lhandle,canvas);

        char *nlet_tag = iem_get_tag(glist, (t_iemgui *)x);

        if(x->x_gui.x_w >= 30)
            w = 2;
        if(x->x_gui.x_w >= 60)
            w = 3;
        sys_vgui(".x%lx.c create prect %d %d %d %d "
                 "-stroke $pd_colors(iemgui_border) -fill #%6.6x "
                 "-tags {%lxBASE %lxTGL %s text iemgui border}\n",
                 canvas, xx, yy, xx + x->x_gui.x_w, yy + x->x_gui.x_h,
                 x->x_gui.x_bcol, x, x, nlet_tag);
        sys_vgui(".x%lx.c create polyline %d %d %d %d "
                 "-strokewidth %d -stroke #%6.6x "
                 "-tags {%lxX1 %lxTGL %s text iemgui}\n",
                 canvas, xx+w+1, yy+w+1,
                 xx + x->x_gui.x_w-w-1, yy + x->x_gui.x_h-w-1, w,
                 (x->x_on!=0.0)?x->x_gui.x_fcol:x->x_gui.x_bcol, x, x,
                 nlet_tag);
        sys_vgui(".x%lx.c create polyline %d %d %d %d "
                 "-strokewidth %d -stroke #%6.6x "
                 "-tags {%lxX2 %lxTGL %s text iemgui}\n",
                 canvas, xx+w+1, yy + x->x_gui.x_h-w-1,
                 xx + x->x_gui.x_w-w-1, yy+w+1, w,
                 (x->x_on!=0.0)?x->x_gui.x_fcol:x->x_gui.x_bcol, x, x, nlet_tag);
        iemgui_label_draw_new(&x->x_gui,canvas,xx,yy,nlet_tag,"TGL");
    toggle_draw_io(x,glist,7);
}

void toggle_draw_move(t_toggle *x, t_glist *glist)
{
    t_canvas *canvas=glist_getcanvas(glist);
    int w=1, xx=text_xpix(&x->x_gui.x_obj, glist),
        yy=text_ypix(&x->x_gui.x_obj, glist);

    if (glist_isvisible(canvas))
    {

        char *nlet_tag = iem_get_tag(glist, (t_iemgui *)x);

        if(x->x_gui.x_w >= 30)
            w = 2;

        if(x->x_gui.x_w >= 60)
            w = 3;
        sys_vgui(".x%lx.c coords %lxBASE %d %d %d %d\n",
                 canvas, x, xx, yy, xx + x->x_gui.x_w, yy + x->x_gui.x_h);
        sys_vgui(".x%lx.c itemconfigure %lxX1 -strokewidth %d\n", canvas, x, w);
        sys_vgui(".x%lx.c coords %lxX1 %d %d %d %d\n",
                 canvas, x, xx+w+1, yy+w+1,
                 xx + x->x_gui.x_w-w-1, yy + x->x_gui.x_h-w-1);
        sys_vgui(".x%lx.c itemconfigure %lxX2 -strokewidth %d\n", canvas, x, w);
        sys_vgui(".x%lx.c coords %lxX2 %d %d %d %d\n",
                 canvas, x, xx+w+1,
                 yy + x->x_gui.x_h-w-1, xx + x->x_gui.x_w-w-1, yy+w+1);
        iemgui_label_draw_move(&x->x_gui,canvas,xx,yy);
        if(!x->x_gui.x_fsf.x_snd_able && canvas == x->x_gui.x_glist)
            sys_vgui(".x%lx.c coords %lxTGL%so%d %d %d %d %d\n",
                 canvas, x, nlet_tag, 0, xx,
                 yy + x->x_gui.x_h-1, xx + IOWIDTH, yy + x->x_gui.x_h);
        if(!x->x_gui.x_fsf.x_rcv_able && canvas == x->x_gui.x_glist)
            sys_vgui(".x%lx.c coords %lxTGL%si%d %d %d %d %d\n",
                 canvas, x, nlet_tag, 0, xx, yy, xx + IOWIDTH, yy+1);
        /* redraw scale handle rectangle if selected */
        if (x->x_gui.x_fsf.x_selected)
            toggle_draw_select(x, x->x_gui.x_glist);
    }
}

void toggle_draw_config(t_toggle* x, t_glist* glist)
{
    t_canvas *canvas=glist_getcanvas(glist);
    iemgui_label_draw_config(&x->x_gui,canvas);
    sys_vgui(".x%lx.c itemconfigure %lxBASE -fill #%6.6x\n "
             ".x%lx.c itemconfigure %lxX1 -stroke #%6.6x\n "
             ".x%lx.c itemconfigure %lxX2 -stroke #%6.6x\n",
             canvas, x, x->x_gui.x_bcol , canvas, x,
             x->x_on?x->x_gui.x_fcol:x->x_gui.x_bcol, canvas, x,
             x->x_on?x->x_gui.x_fcol:x->x_gui.x_bcol);
    /*
    sys_vgui(".x%lx.c itemconfigure %lxX1 -fill #%6.6x\n", canvas, x,
             x->x_on?x->x_gui.x_fcol:x->x_gui.x_bcol);
    sys_vgui(".x%lx.c itemconfigure %lxX2 -fill #%6.6x\n", canvas, x,
             x->x_on?x->x_gui.x_fcol:x->x_gui.x_bcol);
    */
}

void toggle_draw_select(t_toggle* x, t_glist* glist)
{
    t_canvas *canvas=glist_getcanvas(glist);
    if(x->x_gui.x_fsf.x_selected)
    {
        // check if we are drawing inside a gop abstraction
        // visible on parent canvas
        // if so, disable highlighting
        if (x->x_gui.x_glist == glist_getcanvas(glist))
        {
            sys_vgui(".x%lx.c itemconfigure %lxBASE "
                     "-stroke $pd_colors(selection)\n", canvas, x);
            scalehandle_draw_select2(&x->x_gui,glist,"TGL",
                x->x_gui.x_w-1,x->x_gui.x_h-1);
        }
    }
    else
    {
        sys_vgui(".x%lx.c itemconfigure %lxBASE -stroke %s\n",
            canvas, x, IEM_GUI_COLOR_NORMAL);
        scalehandle_draw_erase2(&x->x_gui,glist);
    }
    iemgui_label_draw_select(&x->x_gui,canvas);
    iemgui_tag_selected(&x->x_gui,canvas,"TGL");
}

static void toggle__clickhook(t_scalehandle *sh, t_floatarg f,
    t_floatarg xxx, t_floatarg yyy)
{
    t_toggle *x = (t_toggle *)(sh->h_master);
    int newstate = (int)f;
    if (sh->h_dragon && newstate == 0 && sh->h_scale)
    {
        canvas_apply_setundo(x->x_gui.x_glist, (t_gobj *)x);
        if (sh->h_dragx || sh->h_dragy)
        {
            x->x_gui.x_w += sh->h_dragx;
            x->x_gui.x_h += sh->h_dragy;
            canvas_dirty(x->x_gui.x_glist, 1);
        }
        if (glist_isvisible(x->x_gui.x_glist))
        {
            toggle_draw_move(x, x->x_gui.x_glist);
            scalehandle_unclick_scale(sh);
        }
    }
    else if (!sh->h_dragon && newstate && sh->h_scale)
    {
        scalehandle_click_scale(sh);
    }
    else if (sh->h_dragon && !newstate && !sh->h_scale)
    {
        scalehandle_unclick_label(sh);
    }
    else if(!sh->h_dragon && newstate && !sh->h_scale)
    {
        scalehandle_click_label(sh);
    }
    sh->h_dragon = newstate;
}

static void toggle__motionhook(t_scalehandle *sh, t_floatarg f1, t_floatarg f2)
{
    if (sh->h_dragon && sh->h_scale)
    {
        t_toggle *x = (t_toggle *)(sh->h_master);
        int d = maxi((int)f1,(int)f2);
        d = maxi(d,IEM_GUI_MINSIZE-x->x_gui.x_w);
        sh->h_dragx = d;
        sh->h_dragy = d;
        scalehandle_drag_scale(sh);

        int properties = gfxstub_haveproperties((void *)x);
        if (properties)
        {
            int new_w = x->x_gui.x_w + sh->h_dragx;
            properties_set_field_int(properties,"dim.w_ent",new_w);
        }
    }
    scalehandle_dragon_label(sh,f1,f2);
}


void toggle_draw(t_toggle *x, t_glist *glist, int mode)
{
    if(mode == IEM_GUI_DRAW_MODE_UPDATE)
        sys_queuegui((t_gobj*)x, x->x_gui.x_glist, toggle_draw_update);
        //toggle_draw_update(x, glist);
    else if(mode == IEM_GUI_DRAW_MODE_MOVE)
        toggle_draw_move(x, glist);
    else if(mode == IEM_GUI_DRAW_MODE_NEW)
    {
        toggle_draw_new(x, glist);
        sys_vgui(".x%lx.c raise all_cords\n", glist_getcanvas(glist));
    }
    else if(mode == IEM_GUI_DRAW_MODE_SELECT)
        toggle_draw_select(x, glist);
    else if(mode == IEM_GUI_DRAW_MODE_ERASE)
        iemgui_draw_erase(&x->x_gui, glist, "TGL");
    else if(mode == IEM_GUI_DRAW_MODE_CONFIG)
        toggle_draw_config(x, glist);
    else if(mode >= IEM_GUI_DRAW_MODE_IO)
        toggle_draw_io(x, glist, mode - IEM_GUI_DRAW_MODE_IO);
}

/* ------------------------ tgl widgetbehaviour----------------------------- */

static void toggle_getrect(t_gobj *z, t_glist *glist,
    int *xp1, int *yp1, int *xp2, int *yp2)
{
    t_toggle *x = (t_toggle *)z;

    *xp1 = text_xpix(&x->x_gui.x_obj, glist);
    *yp1 = text_ypix(&x->x_gui.x_obj, glist);
    *xp2 = *xp1 + x->x_gui.x_w;
    *yp2 = *yp1 + x->x_gui.x_h;

    iemgui_label_getrect(x->x_gui, glist, xp1, yp1, xp2, yp2);
}

static void toggle_save(t_gobj *z, t_binbuf *b)
{
    t_toggle *x = (t_toggle *)z;
    int bflcol[3];
    t_symbol *srl[3];

    iemgui_save(&x->x_gui, srl, bflcol);
    binbuf_addv(b, "ssiisiisssiiiiiiiff", gensym("#X"),gensym("obj"),
                (int)x->x_gui.x_obj.te_xpix,
                (int)x->x_gui.x_obj.te_ypix,
                gensym("tgl"), x->x_gui.x_w,
                iem_symargstoint(&x->x_gui.x_isa),
                srl[0], srl[1], srl[2],
                x->x_gui.x_ldx, x->x_gui.x_ldy,
                iem_fstyletoint(&x->x_gui.x_fsf), x->x_gui.x_fontsize,
                bflcol[0], bflcol[1], bflcol[2], x->x_on, x->x_nonzero);
    binbuf_addv(b, ";");
}

static void toggle_properties(t_gobj *z, t_glist *owner)
{
    t_toggle *x = (t_toggle *)z;
    char buf[800];
    t_symbol *srl[3];

    iemgui_properties(&x->x_gui, srl);
    sprintf(buf, "pdtk_iemgui_dialog %%s |tgl| \
            ----------dimensions(pix):----------- %d %d size: 0 0 empty \
            -----------non-zero-value:----------- %g value: 0.0 empty %g \
            -1 lin log %d %d empty %d \
            {%s} {%s} \
            {%s} %d %d \
            %d %d \
            %d %d %d\n",
            x->x_gui.x_w, IEM_GUI_MINSIZE,
            x->x_nonzero, 1.0,/*non_zero-schedule*/
            x->x_gui.x_isa.x_loadinit, -1, -1,/*no multi*/
            srl[0]->s_name, srl[1]->s_name,
            srl[2]->s_name, x->x_gui.x_ldx, x->x_gui.x_ldy,
            x->x_gui.x_fsf.x_font_style, x->x_gui.x_fontsize,
            0xffffff & x->x_gui.x_bcol, 0xffffff & x->x_gui.x_fcol, 0xffffff & x->x_gui.x_lcol);
    gfxstub_new(&x->x_gui.x_obj.ob_pd, x, buf);
}

static void toggle_bang(t_toggle *x)
{
    x->x_gui.x_changed = 1;
    x->x_on = (x->x_on==0.0)?x->x_nonzero:0.0;
    (*x->x_gui.x_draw)(x, x->x_gui.x_glist, IEM_GUI_DRAW_MODE_UPDATE);
    outlet_float(x->x_gui.x_obj.ob_outlet, x->x_on);
    if(x->x_gui.x_fsf.x_snd_able && x->x_gui.x_snd->s_thing)
        pd_float(x->x_gui.x_snd->s_thing, x->x_on);
}

static void toggle_dialog(t_toggle *x, t_symbol *s, int argc, t_atom *argv)
{
    canvas_apply_setundo(x->x_gui.x_glist, (t_gobj *)x);

    t_symbol *srl[3];
    int a = (int)atom_getintarg(0, argc, argv);
    t_float nonzero = (t_float)atom_getfloatarg(2, argc, argv);
    int sr_flags;

    if(nonzero == 0.0)
        nonzero = 1.0;
    x->x_nonzero = nonzero;
    if(x->x_on != 0.0)
        x->x_on = x->x_nonzero;
    sr_flags = iemgui_dialog(&x->x_gui, srl, argc, argv);
    x->x_gui.x_w = iemgui_clip_size(a);
    x->x_gui.x_h = x->x_gui.x_w;
    (*x->x_gui.x_draw)(x, x->x_gui.x_glist, IEM_GUI_DRAW_MODE_CONFIG);
    (*x->x_gui.x_draw)(x, x->x_gui.x_glist, IEM_GUI_DRAW_MODE_IO + sr_flags);
    //(*x->x_gui.x_draw)(x, x->x_gui.x_glist, IEM_GUI_DRAW_MODE_MOVE);
    //canvas_fixlinesfor(glist_getcanvas(x->x_gui.x_glist), (t_text*)x);
    iemgui_shouldvis((void *)x, &x->x_gui, IEM_GUI_DRAW_MODE_MOVE);

    /* forcing redraw of the scale handle */
    if (x->x_gui.x_fsf.x_selected)
    {
        toggle_draw_select(x, x->x_gui.x_glist);
    }

    //ico@bukvic.net 100518 update scrollbars when
    //object potentially exceeds window size
    t_canvas *canvas=(t_canvas *)glist_getcanvas(x->x_gui.x_glist);
    sys_vgui("pdtk_canvas_getscroll .x%lx.c\n", (long unsigned int)canvas);
}

static void toggle_click(t_toggle *x, t_floatarg xpos, t_floatarg ypos,
    t_floatarg shift, t_floatarg ctrl, t_floatarg alt)
{toggle_bang(x);}

static int toggle_newclick(t_gobj *z, struct _glist *glist,
    int xpix, int ypix, int shift, int alt, int dbl, int doit)
{
    if(doit)
        toggle_click((t_toggle *)z, (t_floatarg)xpix, (t_floatarg)ypix,
            (t_floatarg)shift, 0, (t_floatarg)alt);
    return (1);
}

static void toggle_set(t_toggle *x, t_floatarg f)
{
    if (x->x_on != f) x->x_gui.x_changed = 1;
    x->x_on = f;
    if(f != 0.0)
        x->x_nonzero = f;
    (*x->x_gui.x_draw)(x, x->x_gui.x_glist, IEM_GUI_DRAW_MODE_UPDATE);
}

static void toggle_float(t_toggle *x, t_floatarg f)
{
    toggle_set(x, f);
    if(x->x_gui.x_fsf.x_put_in2out)
    {
        outlet_float(x->x_gui.x_obj.ob_outlet, x->x_on);
        if(x->x_gui.x_fsf.x_snd_able && x->x_gui.x_snd->s_thing)
            pd_float(x->x_gui.x_snd->s_thing, x->x_on);
    }
}

static void toggle_fout(t_toggle *x, t_floatarg f)
{
    toggle_set(x, f);
    outlet_float(x->x_gui.x_obj.ob_outlet, x->x_on);
    if(x->x_gui.x_fsf.x_snd_able && x->x_gui.x_snd->s_thing)
        pd_float(x->x_gui.x_snd->s_thing, x->x_on);
}

static void toggle_loadbang(t_toggle *x)
{
    if(!sys_noloadbang && x->x_gui.x_isa.x_loadinit)
        toggle_fout(x, (t_float)x->x_on);
}

static void toggle_size(t_toggle *x, t_symbol *s, int ac, t_atom *av)
{
    x->x_gui.x_w = iemgui_clip_size((int)atom_getintarg(0, ac, av));
    x->x_gui.x_h = x->x_gui.x_w;
    iemgui_size((void *)x, &x->x_gui);
}

static void toggle_delta(t_toggle *x, t_symbol *s, int ac, t_atom *av)
{iemgui_delta((void *)x, &x->x_gui, s, ac, av);}

static void toggle_pos(t_toggle *x, t_symbol *s, int ac, t_atom *av)
{iemgui_pos((void *)x, &x->x_gui, s, ac, av);}

static void toggle_color(t_toggle *x, t_symbol *s, int ac, t_atom *av)
{iemgui_color((void *)x, &x->x_gui, s, ac, av);}

static void toggle_send(t_toggle *x, t_symbol *s)
{iemgui_send(x, &x->x_gui, s);}

static void toggle_receive(t_toggle *x, t_symbol *s)
{iemgui_receive(x, &x->x_gui, s);}

static void toggle_label(t_toggle *x, t_symbol *s)
{iemgui_label((void *)x, &x->x_gui, s);}

static void toggle_label_font(t_toggle *x, t_symbol *s, int ac, t_atom *av)
{iemgui_label_font((void *)x, &x->x_gui, s, ac, av);}

static void toggle_label_pos(t_toggle *x, t_symbol *s, int ac, t_atom *av)
{iemgui_label_pos((void *)x, &x->x_gui, s, ac, av);}

static void toggle_init(t_toggle *x, t_floatarg f)
{
    x->x_gui.x_isa.x_loadinit = (f==0.0)?0:1;
}

static void toggle_nonzero(t_toggle *x, t_floatarg f)
{
    if(f != 0.0)
        x->x_nonzero = f;
}

static void *toggle_new(t_symbol *s, int argc, t_atom *argv)
{
    t_toggle *x = (t_toggle *)pd_new(toggle_class);
    int bflcol[]={-262144, -1, -1};
    int a=IEM_GUI_DEFAULTSIZE;
    int ldx=17, ldy=7;
    int fs=10;
    t_float on=0.0, nonzero=1.0;

    iem_inttosymargs(&x->x_gui.x_isa, 0);
    iem_inttofstyle(&x->x_gui.x_fsf, 0);

    if(((argc == 13)||(argc == 14))&&IS_A_FLOAT(argv,0)
       &&IS_A_FLOAT(argv,1)
       &&(IS_A_SYMBOL(argv,2)||IS_A_FLOAT(argv,2))
       &&(IS_A_SYMBOL(argv,3)||IS_A_FLOAT(argv,3))
       &&(IS_A_SYMBOL(argv,4)||IS_A_FLOAT(argv,4))
       &&IS_A_FLOAT(argv,5)&&IS_A_FLOAT(argv,6)
       &&IS_A_FLOAT(argv,7)&&IS_A_FLOAT(argv,8)&&IS_A_FLOAT(argv,9)
       &&IS_A_FLOAT(argv,10)&&IS_A_FLOAT(argv,11)&&IS_A_FLOAT(argv,12))
    {
        a = (int)atom_getintarg(0, argc, argv);
        iem_inttosymargs(&x->x_gui.x_isa, atom_getintarg(1, argc, argv));
        iemgui_new_getnames(&x->x_gui, 2, argv);
        ldx = (int)atom_getintarg(5, argc, argv);
        ldy = (int)atom_getintarg(6, argc, argv);
        iem_inttofstyle(&x->x_gui.x_fsf, atom_getintarg(7, argc, argv));
        fs = (int)atom_getintarg(8, argc, argv);
        bflcol[0] = (int)atom_getintarg(9, argc, argv);
        bflcol[1] = (int)atom_getintarg(10, argc, argv);
        bflcol[2] = (int)atom_getintarg(11, argc, argv);
        on = (t_float)atom_getfloatarg(12, argc, argv);
    }
    else iemgui_new_getnames(&x->x_gui, 2, 0);
    if((argc == 14)&&IS_A_FLOAT(argv,13))
        nonzero = (t_float)atom_getfloatarg(13, argc, argv);
    x->x_gui.x_draw = (t_iemfunptr)toggle_draw;

    x->x_gui.x_fsf.x_snd_able = 1;
    x->x_gui.x_fsf.x_rcv_able = 1;
    x->x_gui.x_glist = (t_glist *)canvas_getcurrent();
    if (!strcmp(x->x_gui.x_snd->s_name, "empty"))
        x->x_gui.x_fsf.x_snd_able = 0;
    if (!strcmp(x->x_gui.x_rcv->s_name, "empty"))
        x->x_gui.x_fsf.x_rcv_able = 0;
    if(x->x_gui.x_fsf.x_font_style == 1)
        strcpy(x->x_gui.x_font, "helvetica");
    else
        if(x->x_gui.x_fsf.x_font_style == 2) strcpy(x->x_gui.x_font, "times");
    else
    {
        x->x_gui.x_fsf.x_font_style = 0;
        strcpy(x->x_gui.x_font, sys_font);
    }
    x->x_nonzero = (nonzero!=0.0)?nonzero:1.0;
    if(x->x_gui.x_isa.x_loadinit)
        x->x_on = (on!=0.0)?nonzero:0.0;
    else
        x->x_on = 0.0;
    if (x->x_gui.x_fsf.x_rcv_able)
        pd_bind(&x->x_gui.x_obj.ob_pd, x->x_gui.x_rcv);
    x->x_gui.x_ldx = ldx;
    x->x_gui.x_ldy = ldy;

    if(fs < 4)
        fs = 4;
    x->x_gui.x_fontsize = fs;
    x->x_gui.x_w = iemgui_clip_size(a);
    x->x_gui.x_h = x->x_gui.x_w;
    iemgui_all_colfromload(&x->x_gui, bflcol);
    iemgui_verify_snd_ne_rcv(&x->x_gui);
    outlet_new(&x->x_gui.x_obj, &s_float);

    x->x_gui. x_handle = scalehandle_new(scalehandle_class,(t_iemgui *)x,1);
    x->x_gui.x_lhandle = scalehandle_new(scalehandle_class,(t_iemgui *)x,0);
    x->x_gui.x_obj.te_iemgui = 1;
    x->x_gui.x_changed = 1;

    return (x);
}

static void toggle_ff(t_toggle *x)
{
    if(x->x_gui.x_fsf.x_rcv_able)
        pd_unbind(&x->x_gui.x_obj.ob_pd, x->x_gui.x_rcv);
    gfxstub_deleteforkey(x);

    if (x->x_gui. x_handle) scalehandle_free(x->x_gui. x_handle);
    if (x->x_gui.x_lhandle) scalehandle_free(x->x_gui.x_lhandle);
}

void g_toggle_setup(void)
{
    toggle_class = class_new(gensym("tgl"), (t_newmethod)toggle_new,
                             (t_method)toggle_ff, sizeof(t_toggle), 0, A_GIMME, 0);
    class_addcreator((t_newmethod)toggle_new, gensym("toggle"), A_GIMME, 0);
    class_addbang(toggle_class, toggle_bang);
    class_addfloat(toggle_class, toggle_float);
    class_addmethod(toggle_class, (t_method)toggle_click, gensym("click"),
                    A_FLOAT, A_FLOAT, A_FLOAT, A_FLOAT, A_FLOAT, 0);
    class_addmethod(toggle_class, (t_method)toggle_dialog, gensym("dialog"),
                    A_GIMME, 0);
    class_addmethod(toggle_class, (t_method)toggle_loadbang, gensym("loadbang"),
        0);
    class_addmethod(toggle_class, (t_method)toggle_set, gensym("set"),
        A_FLOAT, 0);
    class_addmethod(toggle_class, (t_method)toggle_size, gensym("size"),
        A_GIMME, 0);
    class_addmethod(toggle_class, (t_method)toggle_delta, gensym("delta"),
        A_GIMME, 0);
    class_addmethod(toggle_class, (t_method)toggle_pos, gensym("pos"),
        A_GIMME, 0);
    class_addmethod(toggle_class, (t_method)toggle_color, gensym("color"),
        A_GIMME, 0);
    class_addmethod(toggle_class, (t_method)toggle_send, gensym("send"),
        A_DEFSYM, 0);
    class_addmethod(toggle_class, (t_method)toggle_receive, gensym("receive"),
        A_DEFSYM, 0);
    class_addmethod(toggle_class, (t_method)toggle_label, gensym("label"),
        A_DEFSYM, 0);
    class_addmethod(toggle_class, (t_method)toggle_label_pos,
        gensym("label_pos"), A_GIMME, 0);
    class_addmethod(toggle_class, (t_method)toggle_label_font,
        gensym("label_font"), A_GIMME, 0);
    class_addmethod(toggle_class, (t_method)toggle_init, gensym("init"),
        A_FLOAT, 0);
    class_addmethod(toggle_class, (t_method)toggle_nonzero, gensym("nonzero"),
        A_FLOAT, 0);
 
    scalehandle_class = class_new(gensym("_scalehandle"), 0, 0,
                  sizeof(t_scalehandle), CLASS_PD, 0);
    class_addmethod(scalehandle_class, (t_method)toggle__clickhook,
            gensym("_click"), A_FLOAT, A_FLOAT, A_FLOAT, 0);
    class_addmethod(scalehandle_class, (t_method)toggle__motionhook,
            gensym("_motion"), A_FLOAT, A_FLOAT, 0);

    wb_init(&toggle_widgetbehavior,toggle_getrect,toggle_newclick);
    class_setwidget(toggle_class, &toggle_widgetbehavior);
    class_sethelpsymbol(toggle_class, gensym("toggle"));
    class_setsavefn(toggle_class, toggle_save);
    class_setpropertiesfn(toggle_class, toggle_properties);
}
