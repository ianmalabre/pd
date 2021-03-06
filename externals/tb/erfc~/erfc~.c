/* 
 * 
 * Complementary error function for pd
 * Copyright (C) 2006 Tim Blechmann
 * 
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; see the file COPYING.  If not, write to
 * the Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 */

#include "m_pd.h"
#include "math.h"

static t_class *erfc_class;

typedef struct _erfc
{
    t_object x_obj;
    float x_f;
} t_erfc;

static void *erfc_new(void)
{
    t_erfc *x = (t_erfc *)pd_new(erfc_class);
    outlet_new(&x->x_obj, gensym("signal"));
    x->x_f = 0;
    return (x);
}

static t_int *erfc_perfcorm(t_int *w)
{
    t_float *in = (t_float *)(w[1]);
    t_float *out = (t_float *)(w[2]);
    int n = (int)(w[3]);

    while (n--)
        *out++ = erfcf(*in++);

    return w+4;
}

static void erfc_dsp(t_erfc *x, t_signal **sp)
{
    dsp_add(erfc_perfcorm, 3, sp[0]->s_vec, sp[1]->s_vec, sp[0]->s_n);
}

void erfc_tilde_setup(void)
{
    erfc_class = class_new(gensym("erfc~"), (t_newmethod)erfc_new, 0,
        sizeof(t_erfc), 0, A_DEFFLOAT, 0);
    CLASS_MAINSIGNALIN(erfc_class, t_erfc, x_f);
    class_addmethod(erfc_class, (t_method)erfc_dsp, gensym("dsp"), 0);
}
