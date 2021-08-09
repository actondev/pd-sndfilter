#include <stdio.h>
#include <string.h>
// see https://github.com/pure-data/externals-howto/blob/master/example4/xfade%7E.c
#include <m_pd.h>
#include <sndfilter/reverb.h>

t_class *reverb_class;

typedef struct _t_reverb {
  t_object x_obj;
  sf_reverb_state_st state;
  t_float x_float; // I think this is to support inlet 1 handling signals?? see CLASS_MAINSIGNALIN

  t_inlet* x_in;
  t_outlet* x_outL;
  t_outlet* x_outR;
} t_reverb;

static void *reverb_new(t_symbol *s) {
  // printf("ctor: sndfilter.reverb~\n");
  t_reverb *x = (t_reverb *)pd_new(reverb_class);
  // TODO sample rate
  sf_presetreverb(&(x->state), 44100,
                  SF_REVERB_PRESET_DEFAULT);

  // inlets : 2 signals (stero)
  x->x_in = inlet_new(&x->x_obj, &x->x_obj.ob_pd, gensym("signal"), gensym("signal"));

  // outets : 2 signals (stero)
  x->x_outL = outlet_new(&x->x_obj, gensym("signal"));
  x->x_outR = outlet_new(&x->x_obj, gensym("signal"));
  return x;
}

void reverb_free(t_reverb *x)
{
  // sf_reverb_state_st state is part of t_reverb struct, no need to free something
  // printf("dtor: sndfilter.reverb~\n");
  inlet_free(x->x_in);

  outlet_free(x->x_outL);
  outlet_free(x->x_outR);
}

static t_int *reverb_perform(t_int *w) {
  t_reverb *x = (t_reverb *)(w[1]);
  t_sample *in1 = (t_sample *)(w[2]);
  t_sample *in2 = (t_sample *)(w[3]);
  t_sample *out1 = (t_sample *)(w[4]);
  t_sample *out2 = (t_sample *)(w[5]);
  int n = (int)(w[6]);

  sf_reverb_process_ext(&(x->state), n, in1, in2, out1, out2);

  return (w + 7);
}

static void dsp_add_reverb(t_reverb *x, t_sample *in1, t_sample *in2, t_sample *out1,
                           t_sample *out2, int n) {
  dsp_add(reverb_perform, 6, x, in1, in2, out1, out2, n);
}

static void reverb_dsp(t_reverb *x, t_signal **sp) {
  // this gets called when dragging the object sp[0]->s_n gives the number if samples per tick?
  // printf("dsp %d\n", sp[0]->s_n);
  dsp_add_reverb(x, sp[0]->s_vec, sp[1]->s_vec, sp[2]->s_vec, sp[3]->s_vec,
                 sp[0]->s_n);
}

static void reverb_set_preset(t_reverb *x, t_symbol *value) {
  const char *name = value->s_name;
  // TODO sample rate
  int sr = 44100;
  if (strcmp(name, "default") == 0) {
    sf_presetreverb(&(x->state), sr, SF_REVERB_PRESET_DEFAULT);
    return;
  }
  if (strcmp(name, "small-hall-1") == 0) {
    sf_presetreverb(&(x->state), sr, SF_REVERB_PRESET_SMALLHALL1);
    return;
  }
  if (strcmp(name, "small-hall-2") == 0) {
    sf_presetreverb(&(x->state), sr, SF_REVERB_PRESET_SMALLHALL2);
    return;
  }
  if (strcmp(name, "medium-hall-1") == 0) {
    sf_presetreverb(&(x->state), sr, SF_REVERB_PRESET_MEDIUMHALL1);
    return;
  }
  if (strcmp(name, "medium-hall-2") == 0) {
    return;
  }
  if (strcmp(name, "large-hall-1") == 0) {
    sf_presetreverb(&(x->state), sr, SF_REVERB_PRESET_LARGEHALL1);
    return;
  }
  if (strcmp(name, "large-hall-2") == 0) {
    sf_presetreverb(&(x->state), sr, SF_REVERB_PRESET_LARGEHALL2);
    return;
  }
  if (strcmp(name, "small-room-1") == 0) {
    sf_presetreverb(&(x->state), sr, SF_REVERB_PRESET_SMALLROOM1);
    return;
  }
  if (strcmp(name, "small-room-2") == 0) {
    sf_presetreverb(&(x->state), sr, SF_REVERB_PRESET_SMALLROOM2);
    return;
  }
  if (strcmp(name, "medium-room-1") == 0) {
    sf_presetreverb(&(x->state), sr, SF_REVERB_PRESET_MEDIUMROOM1);
    return;
  }
  if (strcmp(name, "medium-room-2") == 0) {
    sf_presetreverb(&(x->state), sr, SF_REVERB_PRESET_MEDIUMROOM2);
    return;
  }
  if (strcmp(name, "large-room-1") == 0) {
    sf_presetreverb(&(x->state), sr, SF_REVERB_PRESET_LARGEROOM1);
    return;
  }
  if (strcmp(name, "large-room-2") == 0) {
    sf_presetreverb(&(x->state), sr, SF_REVERB_PRESET_LARGEROOM2);
    return;
  }
  if (strcmp(name, "medium-er-1") == 0) {
    sf_presetreverb(&(x->state), sr, SF_REVERB_PRESET_MEDIUMER1);
    return;
  }
  if (strcmp(name, "medium-er-2") == 0) {
    sf_presetreverb(&(x->state), sr, SF_REVERB_PRESET_MEDIUMER2);
    return;
  }
  if (strcmp(name, "plate-high") == 0) {
    sf_presetreverb(&(x->state), sr, SF_REVERB_PRESET_PLATEHIGH);
    return;
  }
  if (strcmp(name, "plate-low") == 0) {
    sf_presetreverb(&(x->state), sr, SF_REVERB_PRESET_PLATELOW);
    return;
  }
  if (strcmp(name, "long-reverb-1") == 0) {
    sf_presetreverb(&(x->state), sr, SF_REVERB_PRESET_LONGREVERB1);
    return;
  }
  if (strcmp(name, "long-reverb-2") == 0) {
    sf_presetreverb(&(x->state), sr, SF_REVERB_PRESET_LONGREVERB2);
    return;
  }

  post("Could not handle preset '%s'\n", name);
}

void sndfilter_reverb_tilde_setup() {
  reverb_class = class_new(gensym("sndfilter.reverb~"),
                           (t_newmethod) reverb_new, // constructor
                           (t_method) reverb_free, // destructor
                           sizeof(t_reverb),
                           0, // 0 or CLASS_DEFAULT ?
                           A_DEFFLOAT, // ?
                           0 // ?
                           );
  CLASS_MAINSIGNALIN(reverb_class, t_reverb, x_float);
  class_addmethod(reverb_class, (t_method)reverb_dsp, gensym("dsp"), A_NULL);
  class_addmethod(reverb_class, (t_method)reverb_set_preset, gensym("preset"),
                  A_SYMBOL, A_NULL);
}


void sndfilter_setup() {
  sndfilter_reverb_tilde_setup();
}
