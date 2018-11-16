/*
 * ---------------------------------------------------------------------------------------------------------------------
 *
 * Copyright (C) 2018, Anton Synytsia
 *
 * ---------------------------------------------------------------------------------------------------------------------
 */

#ifndef AMS_MIDI_H
#define AMS_MIDI_H

#include "ams_ext.h"

class AMS::MIDI {
private:
    // Variables
    static HMIDIOUT h_midi_out;

    // Helper Functions
    static MMRESULT c_out_message(unsigned char arg0, unsigned char arg1, unsigned char arg2);
    static MMRESULT c_edit_channel_controller(BYTE channel, BYTE cnum, BYTE cval);

public:
    // Ruby Functions
    static VALUE rbf_get_device_handle(VALUE self);
    static VALUE rbf_is_device_open(VALUE self);
    static VALUE rbf_open_device(VALUE self);
    static VALUE rbf_close_device(VALUE self);
    static VALUE rbf_out_message(int argc, VALUE* argv, VALUE self);
    static VALUE rbf_get_volume(VALUE self);
    static VALUE rbf_set_volume(int argc, VALUE* argv, VALUE self);
    static VALUE rbf_reset(VALUE self);
    static VALUE rbf_play_note(int argc, VALUE* argv, VALUE self);
    static VALUE rbf_stop_note(VALUE self, VALUE v_id);
    static VALUE rbf_change_channel_controller(VALUE self, VALUE v_channel, VALUE v_cnum, VALUE v_cval);
    static VALUE rbf_change_channel_volume(VALUE self, VALUE v_channel, VALUE v_volume);
    static VALUE rbf_change_channel_stereo_pan(VALUE self, VALUE v_channel, VALUE v_pan);
    static VALUE rbf_change_channel_expression(VALUE self, VALUE v_channel, VALUE v_expression);
    static VALUE rbf_sustain_channel_pedal(VALUE self, VALUE v_channel, VALUE v_state);
    static VALUE rbf_reset_channel_controllers(VALUE self, VALUE v_channel);
    static VALUE rbf_set_note_position(VALUE self, VALUE v_id, VALUE v_position, VALUE v_max_hearing_range);
    static VALUE rbf_change_channel_instrument(VALUE self, VALUE v_channel, VALUE v_instrument);
    static VALUE rbf_play_note2(VALUE self, VALUE v_channel, VALUE v_note, VALUE v_volume);
    static VALUE rbf_stop_note2(VALUE self, VALUE v_channel, VALUE v_note, VALUE v_volume);
    static VALUE rbf_change_channel_position(VALUE self, VALUE v_channel, VALUE v_position, VALUE v_max_hearing_range);

    // Main
    static void init_ruby(VALUE mAMS);
};

#endif  /* AMS_MIDI_H */
