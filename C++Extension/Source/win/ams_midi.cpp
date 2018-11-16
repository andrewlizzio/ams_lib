/*
 * ---------------------------------------------------------------------------------------------------------------------
 *
 * Copyright (C) 2018, Anton Synytsia
 *
 * ---------------------------------------------------------------------------------------------------------------------
 */

#include "ams_midi.h"


/*
 ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  Variables
 ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
*/

HMIDIOUT AMS::MIDI::h_midi_out = nullptr;


/*
 ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  Helper Functions
 ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
*/

MMRESULT AMS::MIDI::c_out_message(unsigned char arg0, unsigned char arg1, unsigned char arg2) {
    DWORD msg = arg2 << 16 | (arg1 << 8 | arg0);
    return midiOutShortMsg(h_midi_out, msg);
}

MMRESULT AMS::MIDI::c_edit_channel_controller(BYTE channel, BYTE cnum, BYTE cval) {
    return c_out_message(0xB0 | channel, cnum, cval);
}


/*
 ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  Ruby Functions
 ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
*/

VALUE AMS::MIDI::rbf_get_device_handle(VALUE self) {
    return RU::to_value(h_midi_out);
}

VALUE AMS::MIDI::rbf_is_device_open(VALUE self) {
    return h_midi_out != nullptr ? Qtrue : Qfalse;
}

VALUE AMS::MIDI::rbf_open_device(VALUE self) {
    if (h_midi_out != nullptr) return Qfalse;
    unsigned int num_devices = midiOutGetNumDevs();
    for (unsigned int i = 0; i < num_devices; ++i)
        if (midiOutOpen(&h_midi_out, i, NULL, NULL, CALLBACK_NULL) == MMSYSERR_NOERROR)
            return Qtrue;
    h_midi_out = nullptr;
    return Qfalse;
}

VALUE AMS::MIDI::rbf_close_device(VALUE self) {
    MMRESULT res =  midiOutClose(h_midi_out);
    h_midi_out = nullptr;
    return res == MMSYSERR_NOERROR ? Qtrue : Qfalse;
}

VALUE AMS::MIDI::rbf_out_message(int argc, VALUE* argv, VALUE self) {
    BYTE arg0, arg1, arg2;
    if (argc == 3) {
        arg0 = RU::value_to_uchar(argv[0]);
        arg1 = RU::value_to_uchar(argv[1]);
        arg2 = RU::value_to_uchar(argv[2]);
    }
    else if (argc == 2) {
        arg0 = RU::value_to_uchar(argv[0]);
        arg1 = RU::value_to_uchar(argv[1]);
        arg2 = 0;
    }
    if (argc == 1) {
        arg0 = RU::value_to_uchar(argv[0]);
        arg1 = 0;
        arg2 = 0;
    }
    else {
        rb_raise(rb_eArgError, "Wrong number of arguments! Expected 1..3 arguments.");
    }
    return RU::to_value(c_out_message(arg0, arg1, arg2));
}

VALUE AMS::MIDI::rbf_get_volume(VALUE self) {
    DWORD volume;
    if (midiOutGetVolume(h_midi_out, &volume) != MMSYSERR_NOERROR)
        return Qnil;
    VALUE v_left = RU::to_value(static_cast<double>(LOWORD(volume)) * (1.0 / 65535.0));
    VALUE v_right = RU::to_value(static_cast<double>(HIWORD(volume)) * (1.0 / 65535.0));
    return rb_ary_new3(2, v_left, v_right);
}

VALUE AMS::MIDI::rbf_set_volume(int argc, VALUE* argv, VALUE self) {
    unsigned int left, right;
    if (argc == 2) {
        left = static_cast<unsigned int>(Geom::clamp_double(RU::value_to_double(argv[0]), 0.0, 1.0) * 65535.0);
        right = static_cast<unsigned int>(Geom::clamp_double(RU::value_to_double(argv[1]), 0.0, 1.0) * 65535.0);
    }
    else if (argc == 1) {
        unsigned int vol = static_cast<unsigned int>(Geom::clamp_double(RU::value_to_double(argv[0]), 0.0, 1.0) * 65535.0);
        left = vol;
        right = vol;
    }
    else
        rb_raise(rb_eArgError, "Wrong number of arguments! Expected 1..2 arguments.");
    DWORD volume = right << 16 | left;
    return midiOutSetVolume(h_midi_out, volume) == MMSYSERR_NOERROR ? Qtrue : Qfalse;
}

VALUE AMS::MIDI::rbf_reset(VALUE self) {
    return midiOutReset(h_midi_out) == MMSYSERR_NOERROR ? Qtrue : Qfalse;
}

VALUE AMS::MIDI::rbf_play_note(int argc, VALUE* argv, VALUE self) {
    BYTE instrument, note, channel, volume;
    channel = 0;
    volume = 127;
    if (argc == 4) {
        channel = Geom::clamp<unsigned char>(RU::value_to_uchar(argv[2]), 0, 15);
        volume = Geom::clamp<unsigned char>(RU::value_to_uchar(argv[3]), 0, 127);
    }
    else if (argc == 3) {
        channel = Geom::clamp<unsigned char>(RU::value_to_uchar(argv[2]), 0, 15);
    }
    else if (argc != 2) {
        rb_raise(rb_eArgError, "Wrong number of arguments! Expected 2..4 arguments.");
    }
    instrument = Geom::clamp<unsigned char>(RU::value_to_uchar(argv[0]), 0, 127);
    note = Geom::clamp<unsigned char>(RU::value_to_uchar(argv[1]), 0, 127);
    // Set instrument
    DWORD msg = instrument << 8 | (0xC0 | channel);
    if (midiOutShortMsg(h_midi_out, msg) != MMSYSERR_NOERROR) return Qnil;
    // Play note
    WORD h = volume;
    WORD l = note << 8 | (0x90 | channel);
    msg = h << 16 | l;
    if (midiOutShortMsg(h_midi_out, msg) == MMSYSERR_NOERROR) {
        DWORD id = instrument << 16 | (note << 8 | channel);
        return RU::to_value(id);
    }
    // Return nil if no success
    return Qnil;
}

VALUE AMS::MIDI::rbf_stop_note(VALUE self, VALUE v_id) {
    DWORD id = RU::value_to_ulong(v_id);
    BYTE instrument = Geom::clamp<BYTE>(BYTE(id >> 16), 0, 127);
    BYTE note = Geom::clamp<BYTE>((id & 0xFFFF) >> 8, 0, 127);
    BYTE channel = Geom::clamp<BYTE>((id & 0xFFFF) & 0xFF, 0, 15);
    // Set instrument
    DWORD msg = instrument << 8 | (0xC0 | channel);
    if (midiOutShortMsg(h_midi_out, msg) != MMSYSERR_NOERROR) return Qfalse;
    // Stop note
    msg = note << 8 | (0x80 | channel);
    return midiOutShortMsg(h_midi_out, msg) == MMSYSERR_NOERROR ? Qtrue : Qfalse;
}

VALUE AMS::MIDI::rbf_change_channel_controller(VALUE self, VALUE v_channel, VALUE v_cnum, VALUE v_cval) {
    BYTE channel = Geom::clamp<unsigned char>(RU::value_to_uchar(v_channel), 0, 15);
    BYTE cnum = Geom::clamp<unsigned char>(RU::value_to_uchar(v_cnum), 0, 127);
    BYTE cval = Geom::clamp<unsigned char>(RU::value_to_uchar(v_cval), 0, 127);
    return c_edit_channel_controller(channel, cnum, cval) == MMSYSERR_NOERROR ? Qtrue : Qfalse;
}

VALUE AMS::MIDI::rbf_change_channel_volume(VALUE self, VALUE v_channel, VALUE v_volume) {
    BYTE channel = Geom::clamp<unsigned char>(RU::value_to_uchar(v_channel), 0, 15);
    BYTE volume = Geom::clamp<unsigned char>(RU::value_to_uchar(v_volume), 0, 127);
    return c_edit_channel_controller(channel, 0x07, volume) == MMSYSERR_NOERROR ? Qtrue : Qfalse;
}

VALUE AMS::MIDI::rbf_change_channel_stereo_pan(VALUE self, VALUE v_channel, VALUE v_pan) {
    BYTE channel = Geom::clamp<unsigned char>(RU::value_to_uchar(v_channel), 0, 15);
    BYTE pan = Geom::clamp<unsigned char>(RU::value_to_uchar(v_pan), 0, 127);
    return c_edit_channel_controller(channel, 0x0A, pan) == MMSYSERR_NOERROR ? Qtrue : Qfalse;
}

VALUE AMS::MIDI::rbf_change_channel_expression(VALUE self, VALUE v_channel, VALUE v_expression) {
    BYTE channel = Geom::clamp<unsigned char>(RU::value_to_uchar(v_channel), 0, 15);
    BYTE expression = Geom::clamp<unsigned char>(RU::value_to_uchar(v_expression), 0, 127);
    return c_edit_channel_controller(channel, 0x0B, expression) == MMSYSERR_NOERROR ? Qtrue : Qfalse;
}

VALUE AMS::MIDI::rbf_sustain_channel_pedal(VALUE self, VALUE v_channel, VALUE v_state) {
    BYTE channel = Geom::clamp<unsigned char>(RU::value_to_uchar(v_channel), 0, 15);
    return c_edit_channel_controller(channel, 0x40, RTEST(v_state) ? 127 : 0) == MMSYSERR_NOERROR ? Qtrue : Qfalse;
}

VALUE AMS::MIDI::rbf_reset_channel_controllers(VALUE self, VALUE v_channel) {
    BYTE channel = Geom::clamp<unsigned char>(RU::value_to_uchar(v_channel), 0, 15);
    return c_edit_channel_controller(channel, 0x79, 0) == MMSYSERR_NOERROR ? Qtrue : Qfalse;
}

VALUE AMS::MIDI::rbf_set_note_position(VALUE self, VALUE v_id, VALUE v_position, VALUE v_max_hearing_range) {
    DWORD id = RU::value_to_ulong(v_id);
    BYTE instrument = Geom::clamp<BYTE>(BYTE(id >> 16), 0, 127);
    //~ BYTE note = Geom::clamp<BYTE>((id & 0xFFFF) >> 8, 0, 127);
    BYTE channel = Geom::clamp<BYTE>((id & 0xFFFF) & 0xFF, 0, 15);
    Geom::Vector3d position, eye, xaxis, yaxis, zaxis;
    RU::value_to_vector(v_position, position);
    double max_hearing_range = Geom::max_double(RU::value_to_double(v_max_hearing_range), 1.0);
    // Set instrument
    if (midiOutShortMsg(h_midi_out, instrument << 8 | (0xC0 | channel)) != MMSYSERR_NOERROR) return Qfalse;
    // Get camera matrix
    VALUE v_model = rb_funcall(RU::SU_SKETCHUP, RU::INTERN_ACTIVE_MODEL, 0);
    VALUE v_view = rb_funcall(v_model, RU::INTERN_ACTIVE_VIEW, 0);
    VALUE v_cam = rb_funcall(v_view, RU::INTERN_CAMERA, 0);
    RU::value_to_vector(rb_funcall(v_cam, RU::INTERN_EYE, 0), eye);
    RU::value_to_vector(rb_funcall(v_cam, RU::INTERN_XAXIS, 0), xaxis);
    RU::value_to_vector(rb_funcall(v_cam, RU::INTERN_YAXIS, 0), yaxis);
    RU::value_to_vector(rb_funcall(v_cam, RU::INTERN_ZAXIS, 0), zaxis);
    Geom::Transformation matrix(zaxis, xaxis, yaxis, eye);
    matrix.m_xaxis.m_w = 0.0;
    matrix.m_yaxis.m_w = 0.0;
    matrix.m_zaxis.m_w = 0.0;
    matrix.m_origin.m_w = 1.0;
    Geom::Transformation matrix_inv(matrix.inverse());
    // Transform position relative to camera matrix
    Geom::Vector3d pt(matrix_inv.transform_vector(position));
    // Calculate volume
    double volume = (1.0 - Geom::clamp_double(pt.get_length() / max_hearing_range, 0.0, 1.0)) * 127.0;
    BYTE m_volume = static_cast<BYTE>(volume + 0.5);
    if (c_edit_channel_controller(channel, 0x07, m_volume) != MMSYSERR_NOERROR) return Qfalse;
    // Calculate stereo pan
    double dist_sq = pt.m_x * pt.m_x + pt.m_y * pt.m_y;
    double ratio;
    if (dist_sq > M_EPSILON_SQ)
        ratio = pt.m_y / dist_sq * 63.5 + 63.5;
    else
        ratio = 63.5;
    BYTE bratio = static_cast<BYTE>(ratio + 0.5);
    if (c_edit_channel_controller(channel, 0x0A, bratio) != MMSYSERR_NOERROR) return Qfalse;
    // Return success if everything goes well
    return Qtrue;
}

VALUE AMS::MIDI::rbf_change_channel_instrument(VALUE self, VALUE v_channel, VALUE v_instrument) {
    BYTE channel = Geom::clamp<BYTE>(BYTE(RU::value_to_int(v_channel)), 0, 15);
    BYTE instrument = Geom::clamp<BYTE>(BYTE(RU::value_to_int(v_instrument)), 0, 127);
    return (midiOutShortMsg(h_midi_out, instrument << 8 | (0xC0 | channel)) == MMSYSERR_NOERROR) ? Qtrue : Qfalse;
}

VALUE AMS::MIDI::rbf_play_note2(VALUE self, VALUE v_channel, VALUE v_note, VALUE v_volume) {
    BYTE channel = Geom::clamp<BYTE>(BYTE(RU::value_to_int(v_channel)), 0, 15);
    BYTE note = Geom::clamp<BYTE>(BYTE(RU::value_to_int(v_note)), 0, 127);
    BYTE volume = Geom::clamp<BYTE>(BYTE(RU::value_to_int(v_volume)), 0, 127);
    // Play note
    WORD h = volume;
    WORD l = note << 8 | (0x90 | channel);
    DWORD msg = h << 16 | l;
    return (midiOutShortMsg(h_midi_out, msg) == MMSYSERR_NOERROR) ? Qtrue : Qfalse;
}

VALUE AMS::MIDI::rbf_stop_note2(VALUE self, VALUE v_channel, VALUE v_note, VALUE v_volume) {
    BYTE channel = Geom::clamp<BYTE>(BYTE(RU::value_to_int(v_channel)), 0, 15);
    BYTE note = Geom::clamp<BYTE>(BYTE(RU::value_to_int(v_note)), 0, 127);
    BYTE volume = Geom::clamp<BYTE>(BYTE(RU::value_to_int(v_volume)), 0, 127);
    // Stop note
    WORD h = volume;
    WORD l = note << 8 | (0x80 | channel);
    DWORD msg = h << 16 | l;
    return midiOutShortMsg(h_midi_out, msg) == MMSYSERR_NOERROR ? Qtrue : Qfalse;
}

VALUE AMS::MIDI::rbf_change_channel_position(VALUE self, VALUE v_channel, VALUE v_position, VALUE v_max_hearing_range) {
    BYTE channel = Geom::clamp<BYTE>(BYTE(RU::value_to_int(v_channel)), 0, 15);
    Geom::Vector3d position, eye, xaxis, yaxis, zaxis;
    RU::value_to_vector(v_position, position);
    double max_hearing_range = Geom::max_double(RU::value_to_double(v_max_hearing_range), 1.0);
    // Get camera matrix
    VALUE v_model = rb_funcall(RU::SU_SKETCHUP, RU::INTERN_ACTIVE_MODEL, 0);
    VALUE v_view = rb_funcall(v_model, RU::INTERN_ACTIVE_VIEW, 0);
    VALUE v_cam = rb_funcall(v_view, RU::INTERN_CAMERA, 0);
    RU::value_to_vector( rb_funcall(v_cam, RU::INTERN_EYE, 0), eye );
    RU::value_to_vector( rb_funcall(v_cam, RU::INTERN_XAXIS, 0), xaxis );
    RU::value_to_vector( rb_funcall(v_cam, RU::INTERN_YAXIS, 0), yaxis );
    RU::value_to_vector( rb_funcall(v_cam, RU::INTERN_ZAXIS, 0), zaxis );
    Geom::Transformation matrix(zaxis, xaxis, yaxis, eye);
    matrix.m_xaxis.m_w = 0.0;
    matrix.m_yaxis.m_w = 0.0;
    matrix.m_zaxis.m_w = 0.0;
    matrix.m_origin.m_w = 1.0;
    Geom::Transformation matrix_inv(matrix.inverse());
    // Transform position relative to camera matrix
    Geom::Vector3d pt(matrix_inv.transform_vector(position));
    // Calculate volume
    double volume = (1.0 - Geom::clamp_double(pt.get_length() / max_hearing_range, 0.0, 1.0)) * 127.0;
    BYTE bvolume = static_cast<BYTE>(volume + 0.5);
    if (c_edit_channel_controller(channel, 0x07, bvolume) != MMSYSERR_NOERROR) return Qfalse;
    // Calculate stereo pan
    double dist_sq = pt.m_x * pt.m_x + pt.m_y * pt.m_y;
    double ratio;
    if (dist_sq > M_EPSILON_SQ)
        ratio = pt.m_y / dist_sq * 63.5 + 63.5;
    else
        ratio = 63.5;
    BYTE bratio = static_cast<BYTE>(ratio + 0.5);
    if (c_edit_channel_controller(channel, 0x0A, bratio) != MMSYSERR_NOERROR) return Qfalse;
    // Return success if everything goes well
    return Qtrue;
}


/*
 ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  Main
 ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
*/

void AMS::MIDI::init_ruby(VALUE mAMS) {
    VALUE mMIDI = rb_define_module_under(mAMS, "MIDI");

    rb_define_module_function(mMIDI, "get_device_handle", VALUEFUNC(AMS::MIDI::rbf_get_device_handle), 0);
    rb_define_module_function(mMIDI, "is_device_open?", VALUEFUNC(AMS::MIDI::rbf_is_device_open), 0);
    rb_define_module_function(mMIDI, "open_device", VALUEFUNC(AMS::MIDI::rbf_open_device), 0);
    rb_define_module_function(mMIDI, "close_device", VALUEFUNC(AMS::MIDI::rbf_close_device), 0);
    rb_define_module_function(mMIDI, "out_message", VALUEFUNC(AMS::MIDI::rbf_out_message), -1);
    rb_define_module_function(mMIDI, "get_volume", VALUEFUNC(AMS::MIDI::rbf_get_volume), 0);
    rb_define_module_function(mMIDI, "set_volume", VALUEFUNC(AMS::MIDI::rbf_set_volume), -1);
    rb_define_module_function(mMIDI, "reset", VALUEFUNC(AMS::MIDI::rbf_reset), 0);
    rb_define_module_function(mMIDI, "play_note", VALUEFUNC(AMS::MIDI::rbf_play_note), -1);
    rb_define_module_function(mMIDI, "stop_note", VALUEFUNC(AMS::MIDI::rbf_stop_note), 1);
    rb_define_module_function(mMIDI, "change_channel_controller", VALUEFUNC(AMS::MIDI::rbf_change_channel_controller), 3);
    rb_define_module_function(mMIDI, "change_channel_volume", VALUEFUNC(AMS::MIDI::rbf_change_channel_volume), 2);
    rb_define_module_function(mMIDI, "change_channel_stereo_pan", VALUEFUNC(AMS::MIDI::rbf_change_channel_stereo_pan), 2);
    rb_define_module_function(mMIDI, "change_channel_expression", VALUEFUNC(AMS::MIDI::rbf_change_channel_expression), 2);
    rb_define_module_function(mMIDI, "sustain_channel_pedal", VALUEFUNC(AMS::MIDI::rbf_sustain_channel_pedal), 2);
    rb_define_module_function(mMIDI, "reset_channel_controllers", VALUEFUNC(AMS::MIDI::rbf_reset_channel_controllers), 1);
    rb_define_module_function(mMIDI, "set_note_position", VALUEFUNC(AMS::MIDI::rbf_set_note_position), 3);
    rb_define_module_function(mMIDI, "change_channel_instrument", VALUEFUNC(AMS::MIDI::rbf_change_channel_instrument), 2);
    rb_define_module_function(mMIDI, "play_note2", VALUEFUNC(AMS::MIDI::rbf_play_note2), 3);
    rb_define_module_function(mMIDI, "stop_note2", VALUEFUNC(AMS::MIDI::rbf_stop_note2), 3);
    rb_define_module_function(mMIDI, "change_channel_position", VALUEFUNC(AMS::MIDI::rbf_change_channel_position), 3);
}
